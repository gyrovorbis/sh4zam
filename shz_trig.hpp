#ifndef SHZ_TRIG_HPP
#define SHZ_TRIG_HPP

#include <tuple>
#include <utility>

#include "shz_trig.h"

namespace shz {
    class sincos: public primitive_compatible<shz_sin_cos_t> {
    public:
        SHZ_FORCE_INLINE sin_cos(shz_sincos_t val) noexcept:
            primitive_compatible(val)
        {}

        SHZ_FORCE_INLINE static sin_cos from_radians(float rad) noexcept {
            return shz_sincosf(rad);
        }

        SHZ_FORCE_INLINE static sin_cos from_radians(uint16_t rad) noexcept {
            return shz_sincosu16(rad);
        }

        SHZ_FORCE_INLINE static sin_cos from_degrees(float deg) noexcept {
            return shz_sincosf_deg(deg);
        }

        SHZ_FORCE_INLINE float sinf() const noexcept { return this->sin; }
        SHZ_FORCE_INLINE float cosf() const noexcept { return this->cos; }
        SHZ_FORCE_INLINE float tanf() const noexcept { return shz_sincos_tanf(*this); }

        SHZ_FORCE_INLINE operator std::pair<float, float>() const noexcept { 
            return std::pair(sinf(), cosf());
        }
    };

    constexpr auto floorf            = shz_floorf;
    constexpr auto ceilf             = shz_ceilf;
    constexpr auto fmacf             = shz_fmacf;
    constexpr auto lerpf             = shz_lerpf;
    constexpr auto barycentric_lerpf = shz_barycentric_lerpf;
    constexpr auto sincosu16         = shz_sincosu16;
    constexpr auto sincosf           = shz_sincosf;
    constexpr auto sincosf_deg       = shz_sincosf_deg;
    constexpr auto sincos_tanf       = shz_sincos_tanf;
    constexpr auto sinf              = shz_sinf;
    constexpr auto sinf_deg          = shz_sinf_deg;
    constexpr auto cosf              = shz_cosf;
    constexpr auto cosf_deg          = shz_cosf_deg;
    constexpr auto tanf              = shz_tanf;
    constexpr auto tanf_deg          = shz_tanf_deg;
    constexpr auto sqrtf             = shz_sqrtf;
    constexpr auto inverse_sqrtf     = shz_inverse_sqrtf;
    constexpr auto inverse_posf      = shz_inverse_posf;
    constexpr auto div_posf          = shz_div_posf;
    constexpr auto fast_invf         = shz_fast_invf;
    constexpr auto dot8f             = shz_dot8f;
    constexpr auto mag_sqr4f         = shz_mag_sqr4f;
}

#endif