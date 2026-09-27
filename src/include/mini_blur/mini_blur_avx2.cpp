// Isolated AVX2 translation unit. Do not enable /arch:AVX2 or -mavx2 globally:
// the portable entry points must remain executable on older processors.
#include "mini_blur_avx2.h"
#if MINI_BLUR_SSE2 && (defined(__GNUC__) || defined(_MSC_VER))
#define MINI_BLUR_HAVE_AVX2 1
#include <immintrin.h>
#ifdef _MSC_VER
#include <intrin.h>
#endif
#else
#define MINI_BLUR_HAVE_AVX2 0
#endif

namespace mini_blur::detail
{
    bool avx2Available()
    {
        static const bool available = [] {
#if MINI_BLUR_HAVE_AVX2 && defined(__GNUC__)
            __builtin_cpu_init();
            return bool(__builtin_cpu_supports("avx2"));
#elif MINI_BLUR_HAVE_AVX2 && defined(_MSC_VER)
            int info[4];
            __cpuid(info, 0);
            if (info[0] < 7)
                return false;
            __cpuidex(info, 1, 0);
            if ((info[2] & (1 << 27)) == 0 || (info[2] & (1 << 28)) == 0)
                return false;
            if ((_xgetbv(0) & 6) != 6)
                return false;
            __cpuidex(info, 7, 0);
            return (info[1] & (1 << 5)) != 0;
#else
            return false;
#endif
        }();
        return available;
    }
#if MINI_BLUR_HAVE_AVX2 && defined(__GNUC__)
#define MINI_BLUR_AVX2_TARGET __attribute__((target("avx2")))
#else
#define MINI_BLUR_AVX2_TARGET
#endif
    MINI_BLUR_AVX2_TARGET void horizontalAvx2(
        const std::uint16_t *center,
        std::uint16_t *dst,
        std::size_t n,
        std::uint16_t middle,
        const std::vector<Tap> &pairs)
    {
        std::size_t i = 0;
#if MINI_BLUR_HAVE_AVX2
        for (; i + 16 <= n; i += 16)
        {
            auto sum = _mm256_mullo_epi16(_mm256_loadu_si256(reinterpret_cast<const __m256i *>(center + i)),
                                          _mm256_set1_epi16(middle));
            for (const auto &t: pairs)
            {
                const auto l = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(center + i - t.offset));
                const auto r = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(center + i + t.offset));
                sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(_mm256_add_epi16(l, r), _mm256_set1_epi16(t.weight)));
            }
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(dst + i), sum);
        }
        _mm256_zeroupper();
#endif
        horizontal(center + i, dst + i, n - i, middle, pairs);
    }

    MINI_BLUR_AVX2_TARGET void verticalAvx2(
        const std::uint16_t *const *rows,
        const std::vector<Tap> &taps,
        std::uint8_t *dst,
        std::size_t n)
    {
        std::size_t i = 0;
#if MINI_BLUR_HAVE_AVX2
        for (; i + 16 <= n; i += 16)
        {
            auto lo = _mm256_set1_epi32(32768), hi = lo;
            for (std::size_t j = 0; j < taps.size(); ++j)
            {
                const auto a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(rows[j] + i));
                const auto w = _mm256_set1_epi16(taps[j].weight);
                const auto l = _mm256_mullo_epi16(a, w), h = _mm256_mulhi_epu16(a, w);
                lo = _mm256_add_epi32(lo, _mm256_unpacklo_epi16(l, h));
                hi = _mm256_add_epi32(hi, _mm256_unpackhi_epi16(l, h));
            }
            const auto words = _mm256_packs_epi32(_mm256_srli_epi32(lo, 16), _mm256_srli_epi32(hi, 16));
            const auto bytes = _mm_packus_epi16(_mm256_castsi256_si128(words), _mm256_extracti128_si256(words, 1));
            _mm_storeu_si128(reinterpret_cast<__m128i *>(dst + i), bytes);
        }
        _mm256_zeroupper();
#endif
        for (; i < n; ++i)
        {
            std::uint32_t sum = 32768;
            for (std::size_t j = 0; j < taps.size(); ++j)
                sum += rows[j][i] * taps[j].weight;
            dst[i] = static_cast<std::uint8_t>(sum >> 16);
        }
    }
#undef MINI_BLUR_AVX2_TARGET
} // namespace mini_blur::detail
