/*! \file
 *  \brief   C++ trigonometry API.
 *  \ingroup trig
 *
 *  This file provides an API offering fast versions of trigonometry functions
 *  for C++23.
 *
 *  \author    2025, 2026 Falco Girgis
 *  \copyright MIT License
 */

#ifndef SHZ_TRIG_HPP
#define SHZ_TRIG_HPP

#include <tuple>
#include <utility>

#include "shz_trig.h"

namespace shz {
    //! Floating-point constant approximation for Pi.
    constexpr float pi_f            = SHZ_F_PI;
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
        SHZ_FORCE_INLINE sincos(shz_sincos_t val) noexcept:
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
    constexpr auto sincosu16   = shz_sincosu16;
    //! C++ wrapper around shz_sincosf().
    constexpr auto sincosf     = shz_sincosf;
    //! C++ wrapper around shz_sincosf_deg().
    constexpr auto sincosf_deg = shz_sincosf_deg;
    //! C++ wrapper around shz_sincos_tanf().
    constexpr auto sincos_tanf = shz_sincos_tanf;
    //! C++ wrapper around shz_sincos_secf().
    constexpr auto sincos_secf = shz_sincos_secf;
    //! C++ wrapper around shz_sincos_cscf().
    constexpr auto sincos_cscf = shz_sincos_cscf;
    //! C++ wrapper around shz_sincos_cotf().
    constexpr auto sincos_cotf = shz_sincos_cotf;

    //! @}

    /*! \name  One-off Trig Functions
        \brief Routines for calculating results of single trig functions.
        @{
    */

    //! C++ wrapper around shz_sinf().
    constexpr auto sinf     = shz_sinf;
    //! C++ wrapper around shz_sinf_deg().
    constexpr auto sinf_deg = shz_sinf_deg;
    //! C++ wrapper around shz_cosf().
    constexpr auto cosf     = shz_cosf;
    //! C++ wrapper around shz_cosf_deg().
    constexpr auto cosf_deg = shz_cosf_deg;
    //! C++ wrapper around shz_tanf().
    constexpr auto tanf     = shz_tanf;
    //! C++ wrapper around shz_tanf_deg().
    constexpr auto tanf_deg = shz_tanf_deg;
    //! C++ wrapper around shz_secf().
    constexpr auto secf     = shz_secf;
    //! C++ wrapper around shz_secf_deg().
    constexpr auto secf_deg = shz_secf_deg;
    //! C++ wrapper around shz_cscf().
    constexpr auto cscf     = shz_cscf;
    //! C++ wrapper around shz_cscf_deg().
    constexpr auto cscf_deg = shz_cscf_deg;
    //! C++ wrapper around shz_cotf().
    constexpr auto cotf     = shz_cotf;
    //! C++ wrapper around shz_cotf_deg().
    constexpr auto cotf_deg = shz_cotf_deg;

    //! @}

    /*! \name  Inverse Trig Functions
        \brief Routines for calculating results of inverse trig functions.
        @{
    */

    //! C++ wrapper around shz_atanf_unit().
    constexpr auto atanf_unit = shz_atanf_unit;
    //! C++ wrapper around shz_atanf_q1().
    constexpr auto atanf_q1   = shz_atanf_q1;
    //! C++ wrapper around shz_atanf().
    constexpr auto atanf      = shz_atanf;
    //! C++ wrapper around shz_atan2f().
    constexpr auto atan2f     = shz_atan2f;
    //! C++ wrapper around shz_asinf().
    constexpr auto asinf      = shz_asinf;
    //! C++ wrapper around shz_acosf().
    constexpr auto acosf      = shz_acosf;
    //! C++ wrapper around shz_asecf().
    constexpr auto asecf      = shz_asecf;
    //! C++ wrapper around shz_acscf().
    constexpr auto acscf      = shz_acscf;
    //! C++ wrapper around shz_acotf().
    constexpr auto acotf      = shz_acotf;

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
}

#endif