/*! \file
 *  \brief   C++ trigonometry API.
 *  \ingroup trig
 *
 *  This file provides an API offering fast versions of trigonometry functions
 *  for C++23.
 *
 *  \author    2025, 2026 Falco Girgis
 *  \copyright MIT License
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#ifndef SHZ_TRIG_HPP
#define SHZ_TRIG_HPP

#include <tuple>
#include <utility>

#include "shz_trig.h"

namespace shz {
    //! Floating-point constant approximation for Pi.
    constexpr float pi_f            = SHZ_F_PI;
    //! Floating-point constant approximation for Pi*2.
    constexpr float tau_f           = SHZ_F_TAU;
    //! Floating-point constant approximation for Pi/2.
    constexpr float pi_f_2          = SHZ_F_PI_2;
    //! Floating-point constant approximation for Pi/4.
    constexpr float pi_f_4          = SHZ_F_PI_4;
    //! Scaling factor used to scale the input to FSCA from radians.
    constexpr float fsca_rad_factor = SHZ_FSCA_RAD_FACTOR;
    //! Scaling factor used to scale the input to FSCA from degrees.
    constexpr float fsca_deg_factor = SHZ_FSCA_DEG_FACTOR;

    //! Converts degrees to radians.
    SHZ_FORCE_INLINE constexpr float deg_to_rad(float deg) noexcept { return SHZ_DEG_TO_RAD(deg); }
    //! Converts radians to degrees.
    SHZ_FORCE_INLINE constexpr float rad_to_deg(float rad) noexcept { return SHZ_RAD_TO_DEG(rad); }

    /*! C++ sine/cosine approximation pairs.

        This structure contains the precomputed values for both sine
        and cosine of an angle.
    */
    struct sincos: shz_sincos_t {
        //! Converting constructor from C struct.
        SHZ_FORCE_INLINE sincos(const shz_sincos_t& val) noexcept:
            shz_sincos_t(val) {}

        //! Returns a new sin/cos pair from the given angle in radians.
        SHZ_FORCE_INLINE static sincos from_radians(float rad) noexcept {
            return shz_sincosf(rad);
        }

        //! Returns a new sin/cos pair from the given 16-bit integer angle in radians.
        SHZ_FORCE_INLINE static sincos from_radians(uint16_t rad) noexcept {
            return shz_sincosu16(rad);
        }

        //! Returns a new sin/cos pair from the given angle in degrees.
        SHZ_FORCE_INLINE static sincos from_degrees(float deg) noexcept {
            return shz_sincosf_deg(deg);
        }

        //! Returns the sine component from the pair.
        SHZ_FORCE_INLINE float sinf() const noexcept { return this->sin; }
        //! Returns the cosine component from the pair.
        SHZ_FORCE_INLINE float cosf() const noexcept { return this->cos; }
        //! Calculates the tangent from the given pair.
        SHZ_FORCE_INLINE float tanf() const noexcept { return shz_sincos_tanf(*this); }
        //! Calculates the secant from the given pair.
        SHZ_FORCE_INLINE float secf() const noexcept { return shz_sincos_secf(*this); }
        //! Calculates the cosecant from the given pair.
        SHZ_FORCE_INLINE float cscf() const noexcept { return shz_sincos_cscf(*this); }
        //! Calculates the cotangent from the given pair.
        SHZ_FORCE_INLINE float cotf() const noexcept { return shz_sincos_cotf(*this); }

        // Conversion operator to a std::pair<> of two floats, for sine and cosine values.
        SHZ_FORCE_INLINE operator std::pair<float, float>() const noexcept { 
            return std::pair(sinf(), cosf());
        }
    };

    //! C++ alias for sincos for those who like POSIX-style typenames.
    using sincos_t = sincos;

    /*! \name  Sine/Cosine pairs
        \brief Routines operating on pairs of sine and cosine values.
        @{
    */

    //! C++ wrapper around shz_sincosu16().
    SHZ_FORCE_INLINE shz_sincos_t sincosu16(uint16_t radians16) noexcept { return shz_sincosu16(radians16); }
    //! C++ wrapper around shz_sincosf().
    SHZ_FORCE_INLINE shz_sincos_t sincosf(float radians) noexcept { return shz_sincosf(radians); }
    //! C++ wrapper around shz_sincosf_deg().
    SHZ_FORCE_INLINE shz_sincos_t sincosf_deg(float degrees) noexcept { return shz_sincosf_deg(degrees); }
    //! C++ wrapper around shz_sincos_tanf().
    SHZ_FORCE_INLINE float sincos_tanf(const shz_sincos_t& sincos) noexcept { return shz_sincos_tanf(sincos); }
    //! C++ wrapper around shz_sincos_secf().
    SHZ_FORCE_INLINE float sincos_secf(const shz_sincos_t& sincos) noexcept { return shz_sincos_secf(sincos); }
    //! C++ wrapper around shz_sincos_cscf().
    SHZ_FORCE_INLINE float sincos_cscf(const shz_sincos_t& sincos) noexcept { return shz_sincos_cscf(sincos); }
    //! C++ wrapper around shz_sincos_cotf().
    SHZ_FORCE_INLINE float sincos_cotf(const shz_sincos_t& sincos) noexcept { return shz_sincos_cotf(sincos); }

    //! @}

    /*! \name  One-off Trig Functions
        \brief Routines for calculating results of single trig functions.
        @{
    */

    //! C++ wrapper around shz_sinf().
    SHZ_FORCE_INLINE float sinf(float radians) noexcept { return shz_sinf(radians); }
    //! C++ wrapper around shz_sinf_deg().
    SHZ_FORCE_INLINE float sinf_deg(float degrees) noexcept { return shz_sinf_deg(degrees); }
    //! C++ wrapper around shz_cosf().
    SHZ_FORCE_INLINE float cosf(float radians) noexcept { return shz_cosf(radians); }
    //! C++ wrapper around shz_cosf_deg().
    SHZ_FORCE_INLINE float cosf_deg(float degrees) noexcept { return shz_cosf_deg(degrees); }
    //! C++ wrapper around shz_tanf().
    SHZ_FORCE_INLINE float tanf(float radians) noexcept { return shz_tanf(radians); }
    //! C++ wrapper around shz_tanf_deg().
    SHZ_FORCE_INLINE float tanf_deg(float degrees) noexcept { return shz_tanf_deg(degrees); }
    //! C++ wrapper around shz_secf().
    SHZ_FORCE_INLINE float secf(float radians) noexcept { return shz_secf(radians); }
    //! C++ wrapper around shz_secf_deg().
    SHZ_FORCE_INLINE float secf_deg(float degrees) noexcept { return shz_secf_deg(degrees); }
    //! C++ wrapper around shz_cscf().
    SHZ_FORCE_INLINE float cscf(float radians) noexcept { return shz_cscf(radians); }
    //! C++ wrapper around shz_cscf_deg().
    SHZ_FORCE_INLINE float cscf_deg(float degrees) noexcept { return shz_cscf_deg(degrees); }
    //! C++ wrapper around shz_cotf().
    SHZ_FORCE_INLINE float cotf(float radians) noexcept { return shz_cotf(radians); }
    //! C++ wrapper around shz_cotf_deg().
    SHZ_FORCE_INLINE float cotf_deg(float degrees) noexcept { return shz_cotf_deg(degrees); }

    //! @}

    /*! \name  Inverse Trig Functions
        \brief Routines for calculating results of inverse trig functions.
        @{
    */

    //! C++ wrapper around shz_atanf_unit().
    SHZ_FORCE_INLINE float atanf_unit(float x) noexcept { return shz_atanf_unit(x); }
    //! C++ wrapper around shz_atanf_q1().
    SHZ_FORCE_INLINE float atanf_q1(float x) noexcept { return shz_atanf_q1(x); }
    //! C++ wrapper around shz_atanf().
    SHZ_FORCE_INLINE float atanf(float x) noexcept { return shz_atanf(x); }
    //! C++ wrapper around shz_atan2f().
    SHZ_FORCE_INLINE float atan2f(float y, float x) noexcept { return shz_atan2f(y, x); }
    //! C++ wrapper around shz_asinf().
    SHZ_FORCE_INLINE float asinf(float x) noexcept { return shz_asinf(x); }
    //! C++ wrapper around shz_acosf().
    SHZ_FORCE_INLINE float acosf(float x) noexcept { return shz_acosf(x); }
    //! C++ wrapper around shz_asecf().
    SHZ_FORCE_INLINE float asecf(float x) noexcept { return shz_asecf(x); }
    //! C++ wrapper around shz_acscf().
    SHZ_FORCE_INLINE float acscf(float x) noexcept { return shz_acscf(x); }
    //! C++ wrapper around shz_acotf().
    SHZ_FORCE_INLINE float acotf(float x) noexcept { return shz_acotf(x); }

    //! @}


    /*! \name  Hyperbolic Functions
        \brief Trigonometric functions for hyperbolas
        @{
    */

    //! C++ wrapper around shz_sinhf().
    SHZ_FORCE_INLINE float sinhf(float x) noexcept { return shz_sinhf(x); }
    //! C++ wrapper around shz_coshf().
    SHZ_FORCE_INLINE float coshf(float x) noexcept { return shz_coshf(x); }
    //! C++ wrapper around shz_tanhf().
    SHZ_FORCE_INLINE float tanhf(float x) noexcept { return shz_tanhf(x); }
    //! C++ wrapper around shz_cschf().
    SHZ_FORCE_INLINE float cschf(float x) noexcept { return shz_cschf(x); }
    //! C++ wrapper around shz_sechf().
    SHZ_FORCE_INLINE float sechf(float x) noexcept { return shz_sechf(x); }
    //! C++ wrapper around shz_cothf().
    SHZ_FORCE_INLINE float cothf(float x) noexcept { return shz_cothf(x); }

    //! @}

    /*! \name  Inverse Hyperbolic Functions
        \brief Inverse trigonometric functions for hyperbolas
        @{
    */

    //! C++ wrapper around shz_asinhf().
    SHZ_FORCE_INLINE float asinhf(float x) noexcept { return shz_asinhf(x); }
    //! C++ wrapper around shz_acoshf().
    SHZ_FORCE_INLINE float acoshf(float x) noexcept { return shz_acoshf(x); }
    //! C++ wrapper around shz_atanhf().
    SHZ_FORCE_INLINE float atanhf(float x) noexcept { return shz_atanhf(x); }
    //! C++ wrapper around shz_acscf().
    SHZ_FORCE_INLINE float acschf(float x) noexcept { return shz_acschf(x); }
    //! C++ wrapper around shz_asechf().
    SHZ_FORCE_INLINE float asechf(float x) noexcept { return shz_asechf(x); }
    //! C++ wrapper around shz_acothf().
    SHZ_FORCE_INLINE float acothf(float x) noexcept { return shz_acothf(x); }

    //! @}

    /*! \name  Gamma
        \brief Fast approximations for the gamma function.
        @{
    */
    //! C++ wrapper around shz_lgammaf().
    SHZ_FORCE_INLINE float lgammaf(float x) noexcept { return shz_lgammaf(x); }
    //! C++ wrapper around shz_tgammaf().
    SHZ_FORCE_INLINE float tgammaf(float x) noexcept { return shz_tgammaf(x); }
    //! @}
}

#endif