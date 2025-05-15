#ifndef SHZ_SCALAR_HPP
#define SHZ_SCALAR_HPP

#include "shz_scalar.h"

namespace shz {
    constexpr auto floorf            = shz_floorf;
    constexpr auto ceilf             = shz_ceilf;
    constexpr auto fmacf             = shz_fmacf;
    constexpr auto lerpf             = shz_lerpf;
    constexpr auto barycentric_lerpf = shz_barycentric_lerpf;
    constexpr auto inverse_sqrtf     = shz_inverse_sqrtf;
    constexpr auto inverse_posf      = shz_inverse_posf;
    constexpr auto div_posf          = shz_div_posf;
    constexpr auto fast_invf         = shz_fast_invf;
    constexpr auto dot8f             = shz_dot8f;
    constexpr auto mag_sqr4f         = shz_mag_sqr4f;
}

#endif
