/*! \file
 *  \brief   C++ scalar math API.
 *  \ingroup scalar
 *
 *  This file provides a collection of general-purpose math routines for
 *  individual scalar values in C++23.
 *
 *  \author    Falco Girgis
 *  \copyright MIT License
 */

#ifndef SHZ_SCALAR_HPP
#define SHZ_SCALAR_HPP

#include "shz_scalar.h"

namespace shz {
    constexpr float fipr_max_error = 0.1f; // Not accurate yet. lol.

    /*! \name  Min/Max
     *  \brief Routines for minimum, maximum, clamping, and normalization.
     *  @{
     */
    //! Returns the minimum of the two given values.
    SHZ_FORCE_INLINE constexpr auto min(auto&& a, auto&& b) noexcept {
        return SHZ_MIN(std::forward<decltype(a)>(a),
                       std::forward<decltype(b)>(b));
    }

    //! Returns the maximum of the two given values.
    SHZ_FORCE_INLINE constexpr auto max(auto&& a, auto&& b) noexcept {
        return SHZ_MAX(std::forward<decltype(a)>(a),
                       std::forward<decltype(b)>(b));
    }

    //! Clamps \p v between \p min and \p max.
    SHZ_FORCE_INLINE constexpr auto clamp(auto&& v, auto&& min, auto&& max) noexcept {
        return SHZ_CLAMP(std::forward<decltype(v)>(v),
                         std::forward<decltype(min)>(min),
                         std::forward<decltype(max)>(max));
    }

    //! Clamps \p v within \p min and \p max then normalizes it between 0.0f and 1.0f
    SHZ_FORCE_INLINE constexpr auto norm(auto&& v, auto&& min, auto&& max) noexcept {
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
    constexpr auto floorf     = shz_floorf;
    //! C++ alias for shz_ceilf().
    constexpr auto ceilf      = shz_ceilf;
    //! C++ alias for shz_roundf().
    constexpr auto roundf     = shz_roundf;
    //! C++ alias for shz_remainderf().
    constexpr auto remainderf = shz_remainderf;
    //! C++ alias for shz_fmodf().
    constexpr auto fmodf      = shz_fmodf;
    //! C++ alias for shz_remquof().
    constexpr auto remquof    = shz_remquof;
    //! @}

    /*! \name  Miscellaneous
     *  \brief Fast versions of miscellaneous FP routines.
     *  @{
     */
    //! C++ alias for shz_copysignf().
    constexpr auto copysignf         = shz_copysignf;
    //! C++ alias for shz_fmacf().
    constexpr auto fmaf              = shz_fmaf;
    //! C++ alias for shz_fdimf().
    constexpr auto fdimf             = shz_fdimf;
    //! C++ alias for shz_hypotf().
    constexpr auto hypotf            = shz_hypotf;
    //! C++ alias for shz_lerpf().
    constexpr auto lerpf             = shz_lerpf;
    //! C++ alias for shz_barycentric_lerpf().
    constexpr auto barycentric_lerpf = shz_barycentric_lerpf;
    //! C++ alias for shz_randf().
    constexpr auto randf             = shz_randf;
    //! C++ alias for shz_randf_range()
    constexpr auto randf_range       = shz_randf_range;
    //! @}

    /*! \name  FSRRA
     *  \brief Routines built around fast reciprocal square root.
     *  @{
     */
    //! C++ alias for shz_sqrtf().
    constexpr auto sqrtf       = shz_sqrtf;
    //! C++ alias for shz_sqrtf_fsrra().
    constexpr auto sqrtf_fsrra = shz_sqrtf_fsrra;
    //! C++ alias for shz_inv_sqrtf().
    constexpr auto inv_sqrtf   = shz_inv_sqrtf;
    //! C++ alias for shz_invf().
    constexpr auto invf        = shz_invf;
    //! C++ alias for shz_invf_fsrra().
    constexpr auto invf_fsrra  = shz_invf_fsrra;
    //! C++ alias for shz_divf().
    constexpr auto divf        = shz_divf;
    //! C++ alias for shz_divf_fsrra().
    constexpr auto divf_fsrra  = shz_divf_fsrra;
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

    /*! \name  Transcendental
     *  \brief Routines for accelerating transcendental functions.
     *  @{
    */
    //! C++ alias for shz_pow2f().
    constexpr auto pow2f = shz_pow2f;
    //! C++ alias for shz_log2f().
    constexpr auto log2f = shz_log2f;
    //! C++ alias for shz_logf().
    constexpr auto logf  = shz_logf;
    //! C++ alias for shz_powf().
    constexpr auto powf  = shz_powf;
    // C++ alias for shz_expf()
    constexpr auto expf  = shz_expf;
    //! @}
}

#endif
