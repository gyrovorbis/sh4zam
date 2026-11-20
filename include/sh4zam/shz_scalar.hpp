/*! \file
 *  \brief   C++ scalar math API.
 *  \ingroup scalar
 *
 *  This file provides a collection of general-purpose math routines for
 *  individual scalar values in C++23.
 *
 *  \author    2025, 2026 Falco Girgis
 *  \author          2026 Aleios
 *  \copyright MIT License
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#ifndef SHZ_SCALAR_HPP
#define SHZ_SCALAR_HPP

#include "shz_cdefs.hpp"
#include "shz_scalar.h"

namespace shz {
    constexpr float fipr_max_error = 0.1f; // Not accurate yet. lol.

    /*! \name  Comparisons
        \brief Routines for comparing and classifying floating-point values.
        @{
    */
    //! C++ wrapper around shz_fminf().
    SHZ_FORCE_INLINE float fminf(float a, float b) noexcept { return shz_fminf(a, b); }
    //! C++ wrapper around shz_fmaxf().
    SHZ_FORCE_INLINE float fmaxf(float a, float b) noexcept { return shz_fmaxf(a, b); }
    //! C++ wrapper around shz_equalf().
    SHZ_FORCE_INLINE bool equalf(float a, float b) noexcept { return shz_equalf(a, b); }
    //! C++ wrapper around shz_equalf_abs().
    SHZ_FORCE_INLINE bool equalf_abs(float a, float b) noexcept { return shz_equalf_abs(a, b); }
    //! C++ wrapper around shz_equalf_rel().
    SHZ_FORCE_INLINE bool equalf_rel(float a, float b) noexcept { return shz_equalf_rel(a, b); }
    //! @}

    /*! \name  Rounding
        \brief Routines for rounding floats.
        @{
    */
    //! C++ wrapper around shz_floorf().
    SHZ_FORCE_INLINE float floorf(float x) noexcept { return shz_floorf(x); }
    //! C++ wrapper around shz_ceilf().
    SHZ_FORCE_INLINE float ceilf(float x) noexcept { return shz_ceilf(x); }
    //! C++ wrapper around shz_roundf().
    SHZ_FORCE_INLINE float roundf(float x) noexcept { return shz_roundf(x); }
    //! C++ wrapper around shz_truncf().
    SHZ_FORCE_INLINE float truncf(float x) noexcept { return shz_truncf(x); }
    //! C++ wrapper around shz_remainderf().
    SHZ_FORCE_INLINE float remainderf(float num, float denom) noexcept { return shz_remainderf(num, denom); }
    //! C++ wrapper around shz_fmodf().
    SHZ_FORCE_INLINE float fmodf(float num, float denom) noexcept { return shz_fmodf(num, denom); }
    //! C++ wrapper around shz_remquof().
    SHZ_FORCE_INLINE float remquof(float num, float denom, float* quot) noexcept { return shz_remquof(num, denom, quot); }
    //! @}

    /*! \name  Mapping
        \brief Routines for mapping a number to another range.
        @{
    */
    //! C++ wrapper around shz_clampf().
    SHZ_FORCE_INLINE float clampf(float value, float min, float max) noexcept { return shz_clampf(value, min, max); }
    //! C++ wrapper around shz_normalizef().
    SHZ_FORCE_INLINE float normalizef(float current, float from, float to) noexcept { return shz_normalizef(current, from, to); }
    //! C++ wrapper around shz_normalizef_fsrra().
    SHZ_FORCE_INLINE float normalizef_fsrra(float current, float from, float to) noexcept { return shz_normalizef_fsrra(current, from, to); }
    //! C++ wrapper around shz_remapf().
    SHZ_FORCE_INLINE float remapf(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) noexcept { return shz_remapf(value, inputStart, inputEnd, outputStart, outputEnd); }
    //! C++ wrapper around shz_remapf_fsrra().
    SHZ_FORCE_INLINE float remapf_fsrra(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) noexcept { return shz_remapf_fsrra(value, inputStart, inputEnd, outputStart, outputEnd); }
    //! C++ wrapper around shz_wrapf().
    SHZ_FORCE_INLINE float wrapf(float value, float min, float max) noexcept { return shz_wrapf(value, min, max); }
    //! C++ wrapper around shz_wrapf_fsrra().
    SHZ_FORCE_INLINE float wrapf_fsrra(float value, float min, float max) noexcept { return shz_wrapf_fsrra(value, min, max); }
    //! C++ wrapper around shz_fractf().
    SHZ_FORCE_INLINE float fractf(float x) noexcept { return shz_fractf(x); }
    //! C++ wrapper around shz_signf().
    SHZ_FORCE_INLINE float signf(float x) noexcept { return shz_signf(x); }
    //! C++ wrapper around shz_saturatef().
    SHZ_FORCE_INLINE float saturatef(float x) noexcept { return shz_saturatef(x); }
    //! @}

    /*! \name  Miscellaneous
        \brief Fast versions of miscellaneous FP routines.
        @{
    */
    //! C++ wrapper around shz_fabsf().
    SHZ_FORCE_INLINE float fabsf(float x) noexcept { return shz_fabsf(x); }
    //! C++ wrapper around shz_copysignf().
    SHZ_FORCE_INLINE float copysignf(float x, float y) noexcept { return shz_copysignf(x, y); }
    //! C++ wrapper around shz_fmaf().
    SHZ_FORCE_INLINE float fmaf(float a, float b, float c) noexcept { return shz_fmaf(a, b, c); }
    //! C++ wrapper around shz_opaquef().
    SHZ_FORCE_INLINE float opaquef(float value) noexcept { return shz_opaquef(value); }
    //! C++ wrapper around shz_fdimf().
    SHZ_FORCE_INLINE float fdimf(float x, float y) noexcept { return shz_fdimf(x, y); }
    //! C++ wrapper around shz_hypotf().
    SHZ_FORCE_INLINE float hypotf(float x, float y) noexcept { return shz_hypotf(x, y); }
    //! C++ wrapper around shz_cbrtf().
    SHZ_FORCE_INLINE float cbrtf(float x) noexcept { return shz_cbrtf(x); }
    //! C++ wrapper around shz_lerpf().
    SHZ_FORCE_INLINE float lerpf(float a, float b, float t) noexcept { return shz_lerpf(a, b, t); }
    //! C++ wrapper around shz_barycentric_lerpf().
    SHZ_FORCE_INLINE float barycentric_lerpf(float a, float b, float c, float u, float v) noexcept { return shz_barycentric_lerpf(a, b, c, u, v); }
    //! C++ wrapper around shz_quadratic_roots().
    SHZ_FORCE_INLINE bool quadratic_roots(float a, float b, float c, float* root1, float* root2) noexcept { return shz_quadratic_roots(a, b, c, root1, root2); }
    //! C++ wrapper around shz_randf().
    SHZ_FORCE_INLINE float randf(int* seed) noexcept { return shz_randf(seed); }
    //! C++ wrapper around shz_randf_range().
    SHZ_FORCE_INLINE float randf_range(int* seed, float min, float max) noexcept { return shz_randf_range(seed, min, max); }
    //! C++ wrapper around shz_stepf().
    SHZ_FORCE_INLINE float stepf(float x, float edge) noexcept { return shz_stepf(x, edge); }
    //! C++ wrapper around shz_smoothstepf().
    SHZ_FORCE_INLINE float smoothstepf(float x, float edge0, float edge1) noexcept { return shz_smoothstepf(x, edge0, edge1); }
    //! C++ wrapper around shz_smoothstepf_safe().
    SHZ_FORCE_INLINE float smoothstepf_safe(float x, float edge0, float edge1) noexcept { return shz_smoothstepf_safe(x, edge0, edge1); }

    //! @}

    /*! \name  FSRRA
        \brief Routines built around fast reciprocal square root.
        @{
    */
    //! C++ wrapper around shz_inv_sqrtf_fsrra().
    SHZ_FORCE_INLINE float inv_sqrtf_fsrra(float x) noexcept { return shz_inv_sqrtf_fsrra(x); }
    //! C++ wrapper around shz_inv_sqrtf().
    SHZ_FORCE_INLINE float inv_sqrtf(float x) noexcept { return shz_inv_sqrtf(x); }
    //! C++ wrapper around shz_sqrtf_fsrra().
    SHZ_FORCE_INLINE float sqrtf_fsrra(float x) noexcept { return shz_sqrtf_fsrra(x); }
    //! C++ wrapper around shz_sqrtf().
    SHZ_FORCE_INLINE float sqrtf(float x) noexcept { return shz_sqrtf(x); }
    //! C++ wrapper around shz_invf_fsrra().
    SHZ_FORCE_INLINE float invf_fsrra(float x) noexcept { return shz_invf_fsrra(x); }
    //! C++ wrapper around shz_invf().
    SHZ_FORCE_INLINE float invf(float x) noexcept { return shz_invf(x); }
    //! C++ wrapper around shz_divf_fsrra().
    SHZ_FORCE_INLINE float divf_fsrra(float num, float denom) noexcept { return shz_divf_fsrra(num, denom); }
    //! C++ wrapper around shz_divf().
    SHZ_FORCE_INLINE float divf(float num, float denom) noexcept { return shz_divf(num, denom); }
    //! @}

    /*! \name  FIPR
        \brief Routines built around fast 4D dot product.
        @{
    */
    //! C++ wrapper around shz_dot6f().
    SHZ_FORCE_INLINE float dot6f(float x1, float y1, float z1, float x2, float y2, float z2) noexcept { return shz_dot6f(x1, y1, z1, x2, y2, z2); }
    //! C++ wrapper around shz_dot8f().
    SHZ_FORCE_INLINE float dot8f(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) noexcept { return shz_dot8f(x1, y1, z1, w1, x2, y2, z2, w2); }
    //! C++ wrapper around shz_mag_sqr3f().
    SHZ_FORCE_INLINE float mag_sqr3f(float x, float y, float z) noexcept { return shz_mag_sqr3f(x, y, z); }
    //! C++ wrapper around shz_mag_sqr4f().
    SHZ_FORCE_INLINE float mag_sqr4f(float x, float y, float z, float w) noexcept { return shz_mag_sqr4f(x, y, z, w); }
    //! @}

    /*! \name  Transcendental
        \brief Routines for accelerating transcendental functions.
        @{
    */
    //! C++ wrapper around shz_pow2f().
    SHZ_FORCE_INLINE float pow2f(float p) noexcept { return shz_pow2f(p); }
    //! C++ wrapper around shz_powf().
    SHZ_FORCE_INLINE float powf(float x, float p) noexcept { return shz_powf(x, p); }
    //! C++ wrapper around shz_powif().
    SHZ_FORCE_INLINE float powif(float x, int n) noexcept { return shz_powif(x, n); }
    //! C++ wrapper around shz_pow10f().
    SHZ_FORCE_INLINE float pow10f(float x) noexcept { return shz_pow10f(x); }
    //! C++ wrapper around shz_log2f().
    SHZ_FORCE_INLINE float log2f(float x) noexcept { return shz_log2f(x); }
    //! C++ wrapper around shz_logf().
    SHZ_FORCE_INLINE float logf(float x) noexcept { return shz_logf(x); }
    //! C++ wrapper around shz_log10f().
    SHZ_FORCE_INLINE float log10f(float x) noexcept { return shz_log10f(x); }
    //! C++ wrapper around shz_expf().
    SHZ_FORCE_INLINE float expf(float p) noexcept { return shz_expf(p); }
    //! C++ wrapper around shz_expm1f().
    SHZ_FORCE_INLINE float expm1f(float x) noexcept { return shz_expm1f(x); }
    //! C++ wrapper around shz_log1pf().
    SHZ_FORCE_INLINE float log1pf(float x) noexcept { return shz_log1pf(x); }
    //! C++ wrapper around shz_logbf().
    SHZ_FORCE_INLINE float logbf(float x) noexcept { return shz_logbf(x); }
    //! C++ wrapper around shz_ldexpf().
    SHZ_FORCE_INLINE float ldexpf(float x, float exp) noexcept { return shz_ldexpf(x, exp); }
    //! C++ wrapper around shz_scalbnf().
    SHZ_FORCE_INLINE float scalbnf(float x, float exp) noexcept { return shz_scalbnf(x, exp); }
    //! C++ wrapper around shz_erff().
    SHZ_FORCE_INLINE float erff(float x) noexcept { return shz_erff(x); }
    //! @}
}

#endif
