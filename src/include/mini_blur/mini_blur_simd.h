// Internal SSE2 kernels; scalar fallback on other targets.
#ifndef MINI_BLUR_SIMD_H
#define MINI_BLUR_SIMD_H
#include <cstdint>
#include <cstring>
#include <vector>
#if !defined(MINI_BLUR_DISABLE_SIMD) &&                                                            \
    (defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2))
#define MINI_BLUR_SSE2 1
#include <emmintrin.h>
#else
#define MINI_BLUR_SSE2 0
#endif

namespace mini_blur::detail
{
    struct Tap
    {
        int offset;
        std::uint16_t weight;
    };

    inline void expand(const std::uint8_t *src, std::uint16_t *dst, std::size_t n)
    {
        std::size_t i = 0;
#if MINI_BLUR_SSE2
        for (; i + 16 <= n; i += 16)
        {
            const auto a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(src + i));
            _mm_storeu_si128(reinterpret_cast<__m128i *>(dst + i),
                             _mm_unpacklo_epi8(a, _mm_setzero_si128()));
            _mm_storeu_si128(reinterpret_cast<__m128i *>(dst + i + 8),
                             _mm_unpackhi_epi8(a, _mm_setzero_si128()));
        }
#endif
        for (; i < n; ++i)
            dst[i] = src[i];
    }

    inline void horizontal(
        const std::uint16_t *center,
        std::uint16_t *dst,
        std::size_t n,
        std::uint16_t middle,
        const std::vector<Tap> &pairs)
    {
        std::size_t i = 0;
#if MINI_BLUR_SSE2
        for (; i + 8 <= n; i += 8)
        {
            auto sum = _mm_mullo_epi16(_mm_loadu_si128(reinterpret_cast<const __m128i *>(center + i)),
                                       _mm_set1_epi16(middle));
            for (const auto &t: pairs)
            {
                const auto l =
                        _mm_loadu_si128(reinterpret_cast<const __m128i *>(center + i - t.offset));
                const auto r =
                        _mm_loadu_si128(reinterpret_cast<const __m128i *>(center + i + t.offset));
                sum =
                        _mm_add_epi16(sum, _mm_mullo_epi16(_mm_add_epi16(l, r), _mm_set1_epi16(t.weight)));
            }
            _mm_storeu_si128(reinterpret_cast<__m128i *>(dst + i), sum);
        }
#endif
        for (; i < n; ++i)
        {
            std::uint32_t sum = center[i] * middle;
            for (const auto &t: pairs)
                sum += ((center + i)[-t.offset] + (center + i)[t.offset]) * t.weight;
            dst[i] = static_cast<std::uint16_t>(sum);
        }
    }

    inline void vertical(const std::uint16_t *const *rows, const std::vector<Tap> &taps, std::uint8_t *dst, std::size_t n)
    {
        std::size_t i = 0;
#if MINI_BLUR_SSE2
        for (; i + 8 <= n; i += 8)
        {
            auto lo = _mm_set1_epi32(32768), hi = lo;
            for (std::size_t j = 0; j < taps.size(); ++j)
            {
                const auto a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(rows[j] + i));
                const auto w = _mm_set1_epi16(taps[j].weight);
                const auto l = _mm_mullo_epi16(a, w), h = _mm_mulhi_epu16(a, w);
                lo = _mm_add_epi32(lo, _mm_unpacklo_epi16(l, h));
                hi = _mm_add_epi32(hi, _mm_unpackhi_epi16(l, h));
            }
            const auto words = _mm_packs_epi32(_mm_srli_epi32(lo, 16), _mm_srli_epi32(hi, 16));
            _mm_storel_epi64(reinterpret_cast<__m128i *>(dst + i), _mm_packus_epi16(words, words));
        }
#endif
        for (; i < n; ++i)
        {
            std::uint32_t sum = 32768;
            for (std::size_t j = 0; j < taps.size(); ++j)
                sum += rows[j][i] * taps[j].weight;
            dst[i] = static_cast<std::uint8_t>(sum >> 16);
        }
    }

    inline void updateSum(std::uint32_t *sums, const std::uint32_t *entering, const std::uint32_t *leaving, std::size_t n)
    {
        std::size_t i = 0;
#if MINI_BLUR_SSE2
        for (; i + 4 <= n; i += 4)
        {
            auto s = _mm_loadu_si128(reinterpret_cast<const __m128i *>(sums + i));
            if (leaving)
                s = _mm_sub_epi32(s, _mm_loadu_si128(reinterpret_cast<const __m128i *>(leaving + i)));
            if (entering)
                s = _mm_add_epi32(s, _mm_loadu_si128(reinterpret_cast<const __m128i *>(entering + i)));
            _mm_storeu_si128(reinterpret_cast<__m128i *>(sums + i), s);
        }
#endif
        for (; i < n; ++i)
        {
            if (leaving)
                sums[i] -= leaving[i];
            if (entering)
                sums[i] += entering[i];
        }
    }
#if MINI_BLUR_SSE2
    inline __m128i loadPixel4(const std::uint8_t *in, int x)
    {
        if (x < 0)
            return _mm_setzero_si128();
        std::uint32_t bytes;
        std::memcpy(&bytes, in + static_cast<std::size_t>(x) * 4, 4);
        const auto words =
                _mm_unpacklo_epi8(_mm_cvtsi32_si128(static_cast<int>(bytes)), _mm_setzero_si128());
        return _mm_unpacklo_epi16(words, _mm_setzero_si128());
    }

    inline void boxHorizontal4(const std::uint8_t *in, std::uint32_t *out, int width, int kw, const int *map)
    {
        auto sum = _mm_setzero_si128();
        for (int i = 0; i < kw; ++i)
            sum = _mm_add_epi32(sum, loadPixel4(in, map[i]));
        for (int x = 0; x < width; ++x)
        {
            _mm_storeu_si128(reinterpret_cast<__m128i *>(out + std::size_t(x) * 4), sum);
            sum = _mm_sub_epi32(sum, loadPixel4(in, map[x]));
            sum = _mm_add_epi32(sum, loadPixel4(in, map[std::size_t(x) + kw]));
        }
    }
#endif
    inline void average(const std::uint32_t *sums, std::uint8_t *dst, std::size_t n, std::uint32_t area)
    {
        std::size_t i = 0;
#if MINI_BLUR_SSE2
        const auto scale = _mm_set1_pd(1.0 / area);
        // Correct reciprocal error at exact half ties. For kernels <=4095,
        // the next non-tie is more than 2.9e-8 away.
        const auto round = _mm_set1_pd(0.5 + 1e-10), highScale = _mm_set1_pd(2147483648.0);
        for (; i + 4 <= n; i += 4)
        {
            const auto s = _mm_loadu_si128(reinterpret_cast<const __m128i *>(sums + i));
            const auto low = _mm_and_si128(s, _mm_set1_epi32(0x7fffffff)), high = _mm_srli_epi32(s, 31);
            const auto a = _mm_add_pd(_mm_cvtepi32_pd(low), _mm_mul_pd(_mm_cvtepi32_pd(high), highScale));
            const auto b = _mm_add_pd(_mm_cvtepi32_pd(_mm_srli_si128(low, 8)),
                                      _mm_mul_pd(_mm_cvtepi32_pd(_mm_srli_si128(high, 8)), highScale));
            const auto q0 = _mm_cvttpd_epi32(_mm_add_pd(_mm_mul_pd(a, scale), round));
            const auto q1 = _mm_cvttpd_epi32(_mm_add_pd(_mm_mul_pd(b, scale), round));
            const auto q = _mm_unpacklo_epi64(q0, q1), w = _mm_packs_epi32(q, q);
            const auto bytes = static_cast<std::uint32_t>(_mm_cvtsi128_si32(_mm_packus_epi16(w, w)));
            std::memcpy(dst + i, &bytes, 4);
        }
#endif
        for (; i < n; ++i)
            dst[i] = static_cast<std::uint8_t>((std::uint64_t(sums[i]) + area / 2) / area);
    }
} // namespace mini_blur::detail

#endif // MINI_BLUR_SIMD_H
