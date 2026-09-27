// Gaussian tables, 8-bit error diffusion and border semantics adapted from
// OpenCV 5.0d. SIMD and scheduling are standalone implementations.
// See THIRD_PARTY_NOTICES.md and THIRD_PARTY_SOURCE_HEADERS.txt.
#include "mini_blur.h"
#include "mini_blur_avx2.h"
#include "mini_blur_simd.h"
#include "mini_blur_workers.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace mini_blur
{
    namespace
    {
        unsigned threadNumber(unsigned n)
        {
            if (n > 64)
                throw std::invalid_argument("mini_blur: thread count must be <=64");
            return n ? n : std::max(1u, std::min(4u, std::thread::hardware_concurrency()));
        }

        void validate(ImageView s, MutableImageView d, int kw, int kh, Border b)
        {
            if (!s.data || !d.data || s.width <= 0 || s.height <= 0 || s.channels < 1 || s.channels > 4 ||
                d.width != s.width || d.height != s.height || d.channels != s.channels)
                throw std::invalid_argument("mini_blur: invalid image geometry");
            if (kw < 1 || kh < 1 || kw > 4095 || kh > 4095)
                throw std::invalid_argument("mini_blur: kernel dimensions must be 1..4095");
            if (b != Border::Reflect101 && b != Border::Reflect && b != Border::Replicate &&
                b != Border::ConstantZero)
                throw std::invalid_argument("mini_blur: unsupported border");
            const auto row = std::size_t(s.width) * s.channels,
                    max = std::numeric_limits<std::size_t>::max();
            if (row / s.channels != std::size_t(s.width) || s.stride < row || d.stride < row ||
                std::size_t(s.height) > max / row / sizeof(std::uint32_t) ||
                (s.height > 1 &&
                 (s.stride > (max - row) / (s.height - 1) || d.stride > (max - row) / (s.height - 1))))
                throw std::invalid_argument("mini_blur: invalid stride or image too large");
        }

        int borderIndex(long long p, int n, Border b)
        {
            if (p >= 0 && p < n)
                return int(p);
            if (b == Border::ConstantZero)
                return -1;
            if (b == Border::Replicate || n == 1)
                return p < 0 ? 0 : n - 1;
            const long long period = b == Border::Reflect101 ? 2LL * (n - 1) : 2LL * n;
            p %= period;
            if (p < 0)
                p += period;
            return int(p < n ? p : period - p - (b == Border::Reflect));
        }

        int roundEven(double x)
        {
            const int lo = int(std::floor(x));
            const double f = x - lo;
            return lo + (f > 0.5 || (f == 0.5 && (lo & 1)));
        }

        std::vector<std::uint16_t> kernel(int n, double sigma)
        {
            std::vector<double> k(n);
            if (n == 1)
                k = {1};
            else if (sigma == 0 && n == 3)
                k = {0.25, 0.5, 0.25};
            else if (sigma == 0 && n == 5)
                k = {1. / 16, 4. / 16, 6. / 16, 4. / 16, 1. / 16};
            else if (sigma == 0 && n == 7)
                k = {4. / 128, 14. / 128, 28. / 128, 36. / 128, 28. / 128, 14. / 128, 4. / 128};
            else if (sigma == 0 && n == 9)
                k = {
                    4. / 256,
                    13. / 256,
                    30. / 256,
                    51. / 256,
                    60. / 256,
                    51. / 256,
                    30. / 256,
                    13. / 256,
                    4. / 256
                };
            else
            {
                if (sigma == 0)
                    sigma = n * 0.15 + 0.35;
                double sum = 0;
                for (int i = 0; i < n; ++i)
                {
                    const double x = double(i - n / 2) / sigma;
                    k[i] = std::exp(-0.5 * x * x);
                    sum += k[i];
                }
                for (auto &v: k)
                    v /= sum;
            }
            // Adapted from getGaussianKernelFixedPoint_ED, fractionBits=8.
            // Preserve symmetry and sum exactly 256, including quantization error.
            std::vector<std::uint16_t> fixed(n);
            double error = 0;
            int sum = 0;
            for (int i = 0; i < n / 2; ++i)
            {
                const double adjusted = k[i] * 256 + error;
                const int value = roundEven(adjusted);
                error = adjusted - value;
                fixed[i] = fixed[n - 1 - i] = std::uint16_t(value);
                sum += value;
            }
            fixed[n / 2] = std::uint16_t(256 - 2 * sum);
            return fixed;
        }
    } // namespace
    struct Context::Impl
    {
        bool avx2 = detail::avx2Available();
        detail::Workers pool;
        std::vector<std::uint16_t> gauss, zero, kx, ky;
        std::vector<std::uint32_t> box;
        std::vector<std::vector<std::uint16_t> > padded;
        std::vector<std::vector<std::uint32_t> > sums;
        std::vector<std::vector<const std::uint16_t *> > rows;
        std::vector<detail::Tap> pairs, taps;
        std::vector<int> mapX, mapY;
        int cachedKx = 0, cachedKy = 0;
        double cachedSx = -1, cachedSy = -1;
        explicit Impl(unsigned n) : pool(n), padded(n), sums(n), rows(n) {}
    };

    Context::Context(unsigned n) : impl_(new Impl(threadNumber(n))) {}

    Context::~Context() = default;

    unsigned Context::threadCount() const { return impl_->pool.count(); }

    const char *Context::backend()
    {
        return detail::avx2Available() ? "AVX2" : MINI_BLUR_SSE2 ? "SSE2" : "scalar";
    }

    void Context::gaussianBlur(
        ImageView src,
        MutableImageView dst,
        int kw,
        int kh,
        double sx,
        double sy,
        Border border)
    {
        validate(src, dst, kw, kh, border);
        if (!(kw & 1) || !(kh & 1) || !std::isfinite(sx) || !std::isfinite(sy) || sx < 0 || sy < 0)
            throw std::invalid_argument(
                "mini_blur: Gaussian sizes must be odd and sigmas finite/nonnegative");
        if (sy == 0)
            sy = sx;
        auto &p = *impl_;
        if (kw != p.cachedKx || sx != p.cachedSx)
        {
            p.kx = kernel(kw, sx);
            p.cachedKx = kw;
            p.cachedSx = sx;
        }
        if (kh != p.cachedKy || sy != p.cachedSy)
        {
            p.ky = kernel(kh, sy);
            p.cachedKy = kh;
            p.cachedSy = sy;
        }
        const std::size_t row = std::size_t(src.width) * src.channels;
        const int rx = kw / 2, ry = kh / 2;
        const std::size_t padding = std::size_t(rx) * src.channels;
        p.gauss.resize(row * src.height);
        p.zero.assign(row, 0);
        p.pairs.clear();
        p.taps.clear();
        for (int i = 1; i <= rx; ++i)
            if (p.kx[rx + i])
                p.pairs.push_back({i * src.channels, p.kx[rx + i]});
        for (int i = 0; i < kh; ++i)
            if (p.ky[i])
                p.taps.push_back({i - ry, p.ky[i]});
        p.mapX.resize(std::size_t(src.width) + 2 * rx);
        for (std::size_t i = 0; i < p.mapX.size(); ++i)
            p.mapX[i] = borderIndex(static_cast<long long>(i) - rx, src.width, border);
        p.mapY.resize(std::size_t(src.height) + 2 * ry);
        for (std::size_t i = 0; i < p.mapY.size(); ++i)
            p.mapY[i] = borderIndex(static_cast<long long>(i) - ry, src.height, border);
        for (unsigned t = 0; t < threadCount(); ++t)
        {
            p.padded[t].resize(row + 2 * padding);
            p.rows[t].resize(p.taps.size());
        }
        const unsigned nt = threadCount();
        p.pool.run([&](unsigned t) {
            const int first = int(std::int64_t(src.height) * t / nt),
                    last = int(std::int64_t(src.height) * (t + 1) / nt);
            auto *pad = p.padded[t].data();
            for (int y = first; y < last; ++y)
            {
                const auto *in = src.data + std::size_t(y) * src.stride;
                detail::expand(in, pad + padding, row);
                for (int x = 0; x < rx; ++x)
                {
                    const int left = p.mapX[x], right = p.mapX[std::size_t(rx) + src.width + x];
                    for (int c = 0; c < src.channels; ++c)
                    {
                        pad[std::size_t(x) * src.channels + c] =
                                left < 0 ? 0 : in[std::size_t(left) * src.channels + c];
                        pad[padding + row + std::size_t(x) * src.channels + c] =
                                right < 0 ? 0 : in[std::size_t(right) * src.channels + c];
                    }
                }
                if (p.avx2)
                    detail::horizontalAvx2(pad + padding, p.gauss.data() + std::size_t(y) * row, row,
                                           p.kx[rx], p.pairs);
                else
                    detail::horizontal(pad + padding, p.gauss.data() + std::size_t(y) * row, row,
                                       p.kx[rx], p.pairs);
            }
        });
        // Completing horizontal reads before any output write permits in-place use.
        p.pool.run([&](unsigned t) {
            const int first = int(std::int64_t(src.height) * t / nt),
                    last = int(std::int64_t(src.height) * (t + 1) / nt);
            for (int y = first; y < last; ++y)
            {
                for (std::size_t j = 0; j < p.taps.size(); ++j)
                {
                    const int iy = p.mapY[std::size_t(y) + std::size_t(ry + p.taps[j].offset)];
                    p.rows[t][j] = iy < 0 ? p.zero.data() : p.gauss.data() + std::size_t(iy) * row;
                }
                if (p.avx2)
                    detail::verticalAvx2(p.rows[t].data(), p.taps,
                                         dst.data + std::size_t(y) * dst.stride, row);
                else
                    detail::vertical(p.rows[t].data(), p.taps, dst.data + std::size_t(y) * dst.stride,
                                     row);
            }
        });
    }

    void Context::boxBlur(ImageView src, MutableImageView dst, int kw, int kh, Border border)
    {
        validate(src, dst, kw, kh, border);
        auto &p = *impl_;
        const std::size_t row = std::size_t(src.width) * src.channels;
        p.box.resize(row * src.height);
        const int ax = kw / 2, ay = kh / 2;
        p.mapX.resize(std::size_t(src.width) + kw);
        for (std::size_t i = 0; i < p.mapX.size(); ++i)
            p.mapX[i] = borderIndex(static_cast<long long>(i) - ax, src.width, border);
        p.mapY.resize(std::size_t(src.height) + kh);
        for (std::size_t i = 0; i < p.mapY.size(); ++i)
            p.mapY[i] = borderIndex(static_cast<long long>(i) - ay, src.height, border);
        const unsigned nt = threadCount();
        for (unsigned t = 0; t < nt; ++t)
            p.sums[t].resize(row * (t + 1) / nt - row * t / nt);
        p.pool.run([&](unsigned t) {
            const int first = int(std::int64_t(src.height) * t / nt),
                    last = int(std::int64_t(src.height) * (t + 1) / nt);
            for (int y = first; y < last; ++y)
            {
                const auto *in = src.data + std::size_t(y) * src.stride;
                auto *out = p.box.data() + std::size_t(y) * row;
#if MINI_BLUR_SSE2
                if (src.channels == 4)
                {
                    detail::boxHorizontal4(in, out, src.width, kw, p.mapX.data());
                    continue;
                }
#endif
                std::uint32_t sum[4] = {};
                for (int k = 0; k < kw; ++k)
                {
                    const int ix = p.mapX[k];
                    if (ix >= 0)
                        for (int c = 0; c < src.channels; ++c)
                            sum[c] += in[std::size_t(ix) * src.channels + c];
                }
                for (int x = 0; x < src.width; ++x)
                {
                    const int leave = p.mapX[x], enter = p.mapX[std::size_t(x) + kw];
                    for (int c = 0; c < src.channels; ++c)
                    {
                        out[std::size_t(x) * src.channels + c] = sum[c];
                        if (leave >= 0)
                            sum[c] -= in[std::size_t(leave) * src.channels + c];
                        if (enter >= 0)
                            sum[c] += in[std::size_t(enter) * src.channels + c];
                    }
                }
            }
        });
        p.pool.run([&](unsigned t) {
            const auto begin = row * t / nt, count = row * (t + 1) / nt - begin;
            auto *sum = p.sums[t].data();
            std::fill(p.sums[t].begin(), p.sums[t].end(), 0);
            for (int k = 0; k < kh; ++k)
            {
                const int iy = p.mapY[k];
                if (iy >= 0)
                    detail::updateSum(sum, p.box.data() + std::size_t(iy) * row + begin, nullptr,
                                      count);
            }
            for (int y = 0; y < src.height; ++y)
            {
                detail::average(sum, dst.data + std::size_t(y) * dst.stride + begin, count,
                                std::uint32_t(kw) * kh);
                const int leave = p.mapY[y], enter = p.mapY[std::size_t(y) + kh];
                detail::updateSum(
                    sum, enter < 0 ? nullptr : p.box.data() + std::size_t(enter) * row + begin,
                    leave < 0 ? nullptr : p.box.data() + std::size_t(leave) * row + begin, count);
            }
        });
    }

    void gaussianBlur(
        ImageView src,
        MutableImageView dst,
        int kw,
        int kh,
        double sx,
        double sy,
        Border b)
    {
        thread_local Context context(1);
        context.gaussianBlur(src, dst, kw, kh, sx, sy, b);
    }

    void boxBlur(ImageView src, MutableImageView dst, int kw, int kh, Border b)
    {
        thread_local Context context(1);
        context.boxBlur(src, dst, kw, kh, b);
    }
} // namespace mini_blur
