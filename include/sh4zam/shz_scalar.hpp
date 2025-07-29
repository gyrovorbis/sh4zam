/*! \file
 *  \brief   C++ scalar math API.
 *  \ingroup scalar
 *
 *  This file provides a collection of general-purpose math routines for
 *  individual scalar values in C++23.
 *
 *  \author Falco Girgis
 */
#ifndef SHZ_SCALAR_HPP
#define SHZ_SCALAR_HPP

#include "shz_scalar.h"

namespace shz {
    constexpr float fipr_max_error = 0.1f;

    /*! \name  Min/Max
     *  \brief Routines for minimum, maximum, clamping, and normalization.
     *  @{
     */
    //! Returns the minimum of the two given values.
    SHZ_FORCE_INLINE constexpr auto min(auto &&a, auto &&b) noexcept {
        return SHZ_MIN(std::forward<decltype(a)>(a),
                       std::forward<decltype(b)>(b));
    }

    //! Returns the maximum of the two given values.
    SHZ_FORCE_INLINE constexpr auto max(auto &&a, auto &&b) noexcept {
        return SHZ_MAX(std::forward<decltype(a)>(a),
                       std::forward<decltype(b)>(b));
    }

    //! Clamps \p v between \p min and \p max.
    SHZ_FORCE_INLINE constexpr auto clamp(auto &&v, auto &&min, auto &&max) noexcept {
        return SHZ_CLAMP(std::forward<decltype(v)>(v),
                         std::forward<decltype(min)>(min),
                         std::forward<decltype(max)>(max));
    }

    //! Clamps \p v within \p min and \p max then normalizes it between 0.0f and 1.0f
    SHZ_FORCE_INLINE constexpr auto norm(auto &&v, auto &&min, auto &&max) noexcept {
        return SHZ_NORM(std::forward<decltype(v)>(v),
                        std::forward<decltype(min)>(min),
                        std::forward<decltype(max)>(max));
    }
    //! @}

    /*! \name  Rounding
     *  \brief Routines for rounding floats.
     *  @{
     */
    //! C++ alias for shz_floorf().
    constexpr auto floorf = shz_floorf;
    //! C++ alias for shz_ceilf().
    constexpr auto ceilf  = shz_ceilf;
    //! @}

    /*! \name  FMAC
     *  \brief Routines built around multiply + accumulate operations.
     *  @{
     */
    //! C++ alias for shz_fmacf().
    constexpr auto fmacf             = shz_fmacf;
    //! C++ alias for shz_lerpf().
    constexpr auto lerpf             = shz_lerpf;
    //! C++ alias for shz_barycentric_lerpf().
    constexpr auto barycentric_lerpf = shz_barycentric_lerpf;
    //! @}

    /*! \name  FSRRA
     *  \brief Routines built around fast reciprocal square root.
     *  @{
     */
    //! C++ alias for shz_inverse_sqrtf().
    constexpr auto inverse_sqrtf = shz_inverse_sqrtf;
    //! C++ alias for shz_posf().
    constexpr auto inverse_posf  = shz_inverse_posf;
    //! C++ alias for shz_div_posf().
    constexpr auto div_posf      = shz_div_posf;
    //! C++ alias for shz_fast_invf().
    constexpr auto fast_invf     = shz_fast_invf;
    //! @}

    /*! \name  FIPR
     *  \brief Routines built around fast 4D dot product.
     *  @{
     */
    //! C++ alias for shz_dot8f().
    constexpr auto dot8f     = shz_dot8f;
    //! C++ alias for shz_mag_sqr4f().
    constexpr auto mag_sqr4f = shz_mag_sqr4f;
    //! @}
}

#endif
