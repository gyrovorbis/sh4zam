#ifndef SHZ_TRIG_HPP
#define SHZ_TRIG_HPP
/*! \file
 *  \brief   C++ trigonometry API.
 *  \ingroup trig
 *
 *  This file provides an API offering fast versions of trigonometry functions
 *  for C++23.
 *
 *  \author    Falco Girgis
 *  \copyright MIT License
 */
#include <tuple>
#include <utility>

#include "shz_trig.h"

namespace shz {
    constexpr float pi_f            = SHZ_F_PI;
    constexpr float fsca_rad_factor = SHZ_FSCA_RAD_FACTOR;
    constexpr float fsca_deg_factor = SHZ_FSCA_DEG_FACTOR;

    SHZ_FORCE_INLINE constexpr float deg_to_rad(float deg) noexcept { return SHZ_DEG_TO_RAD(deg); }
    SHZ_FORCE_INLINE constexpr float rad_to_deg(float rad) noexcept { return SHZ_RAD_TO_DEG(rad); }

    struct sincos: shz_sincos_t {
        SHZ_FORCE_INLINE sincos(shz_sincos_t val) noexcept:
            shz_sincos_t(val) {}

        SHZ_FORCE_INLINE static sincos from_radians(float rad) noexcept {
            return shz_sincosf(rad);
        }

        SHZ_FORCE_INLINE static sincos from_radians(uint16_t rad) noexcept {
            return shz_sincosu16(rad);
        }

        SHZ_FORCE_INLINE static sincos from_degrees(float deg) noexcept {
            return shz_sincosf_deg(deg);
        }

        SHZ_FORCE_INLINE float sinf() const noexcept { return this->sin; }
        SHZ_FORCE_INLINE float cosf() const noexcept { return this->cos; }
        SHZ_FORCE_INLINE float tanf() const noexcept { return shz_sincos_tanf(*this); }

        SHZ_FORCE_INLINE operator std::pair<float, float>() const noexcept { 
            return std::pair(sinf(), cosf());
        }
    };

    constexpr auto sincosu16   = shz_sincosu16;
    constexpr auto sincosf     = shz_sincosf;
    constexpr auto sincosf_deg = shz_sincosf_deg;
    constexpr auto sincos_tanf = shz_sincos_tanf;
    constexpr auto sinf        = shz_sinf;
    constexpr auto sinf_deg    = shz_sinf_deg;
    constexpr auto cosf        = shz_cosf;
    constexpr auto cosf_deg    = shz_cosf_deg;
    constexpr auto tanf        = shz_tanf;
    constexpr auto tanf_deg    = shz_tanf_deg;
    constexpr auto atanf_unit  = shz_atanf_unit;
    constexpr auto atanf       = shz_atanf;
    constexpr auto asinf       = shz_asinf;
    constexpr auto acosf       = shz_acosf;
}

#endif