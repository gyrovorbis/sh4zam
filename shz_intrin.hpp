#ifndef SHZ_INTRIN_HPP
#define SHZ_INTRIN_HPP

#include "shz_intrin.h"
#include <tuple>


template<typename P>
class primitive_compatible {
protected:
    P value_;
public:
    SHZ_FORCE_INLINE primitive_compatible(P value={}) noexcept:
        value_(value) {}

    SHZ_FORCE_INLINE operator P() const noexcept { return value_; }
    SHZ_FORCE_INLINE P operator=(P rhs) noexcept { value_ = rhs; }
};

namespace shz {
    class sin_cos: public primitive_compatible<shz_sin_cos_t> {
    public:
        SHZ_FORCE_INLINE sin_cos(shz_sincos_t val):
            primitive_compatible(val)
        {}

        SHZ_FORCE_INLINE static sin_cos from_degrees(float deg) noexcept {
            return shz_sincosf_deg(deg);
        }

        SHZ_FORCE_INLINE static sin_cos from_radians(float rad) noexcept {
            return shz_sincosf(rad);
        }

        SHZ_FORCE_INLINE float sinf() const noexcept { return shz_sinf(*this); }
        SHZ_FORCE_INLINE float cosf() const noexcept { return shz_cosf(*this); }
        SHZ_FORCE_INLINE float tanf() const noexcept { return shz_tanf(*this); }

        SHZ_FORCE_INLINE operator std::pair<float, float>() const noexcept { 
            return std::pair(sinf(), cosf());
        }
    };

    constexpr auto floorf            = shz_floorf;
    constexpr auto ceilf             = shz_ceilf;
    constexpr auto fmacf             = shz_fmacf;
    constexpr auto lerpf             = shz_lerpf;
    constexpr auto barycentric_lerpf = shz_barycentric_lerpf;
    constexpr auto sqrtf             = shz_sqrtf;
    constexpr auto inverse_sqrtf     = shz_inverse_sqrtf;
    constexpr auto inverse_posf      = shz_inverse_posf;
    constexpr auto div_posf          = shz_div_posf;
    constexpr auto fast_invf         = shz_fast_invf;
    constexpr auto dot8f             = shz_dot8f;
    constexpr auto mag_sqr4f         = shz_mag_sqr4f;
}

#endif