#ifndef MINI_BLUR_AVX2_H
#define MINI_BLUR_AVX2_H
#include "mini_blur_simd.h"


namespace mini_blur::detail
{
    bool avx2Available();

    void horizontalAvx2(const std::uint16_t *, std::uint16_t *, std::size_t, std::uint16_t, const std::vector<Tap> &);

    void verticalAvx2(const std::uint16_t *const *, const std::vector<Tap> &, std::uint8_t *, std::size_t);
} // namespace mini_blur::detail

#endif // MINI_BLUR_AVX2_H
