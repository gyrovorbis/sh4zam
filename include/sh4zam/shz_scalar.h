/*! \file
 *  \brief General-purpose scalar math routines.
 *  \ingroup scalar
 *
 *  This file provides a collection of general-purpose math routines for
 *  operating on floating-point scalar values.
 *
 *  \todo
 *      - Use FP rounding modes for rounding functionality.
 *
 *  \author 2025, 2026 Falco Girgis
 *  \author 2025, 2026 Paul Cercueil
 *  \author       2026 Aleios
 *  \author       2026 jnmartin64
 *
 *  \copyright MIT License
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#ifndef SHZ_SCALAR_H
#define SHZ_SCALAR_H

#include <math.h>
#include <stdbool.h>

#include "shz_cdefs.h"

/*! \defgroup scalar Scalar
    \brief    Scalar functions and utilities.

    This API is designed around performing various scalar operations, providing
    alternatives for routines typically found within `<math.h>`.

    \warning
    Unlike the standard C floating-point routines, these routines are often saving
    cycles by not handling NaN and INF values as well as by not reporting rounding
    and domain errors back to the user.
*/

//! Floating-point epsilon used with inexact FP-based comparisons.
#define SHZ_FLT_EPSILON 0.01f

SHZ_DECLS_BEGIN

/*! \name  Comparisons
    \brief Routines for comparing and classifying floating-point values.
    @{
*/

//! Returns the minimum value of two given floats.
SHZ_INLINE float shz_fminf(float a, float b) SHZ_NOEXCEPT;

//! Returns the maximum value of two given floats.
SHZ_INLINE float shz_fmaxf(float a, float b) SHZ_NOEXCEPT;

//! Checks for equality based on EITHER the absolute tolerance or relative tolerance, using SHZ_FLT_EPSILON.
SHZ_INLINE bool shz_equalf(float a, float b) SHZ_NOEXCEPT;

//! Checks for equality based on the absolute tolerance using SHZ_FLT_EPSILON.
SHZ_INLINE bool shz_equalf_abs(float a, float b) SHZ_NOEXCEPT;

//! Checks for equality based on the relative tolerance using SHZ_FLT_EPSILON.
SHZ_INLINE bool shz_equalf_rel(float a, float b) SHZ_NOEXCEPT;

//! @}

/*! \name  Rounding
    \brief Routines for rounding and manipulating floats.
    @{
 */

/*! Replacement for the <math.h> routine, floorf().

    Returns the closest integral value to \p x, rounded down, as a float.

    \sa ceilf()
*/
SHZ_INLINE float shz_floorf(float x) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, ceilf().

    Returns the closest integral value to \p x, rounded up, as a float.

    \sa floorf()
*/
SHZ_INLINE float shz_ceilf(float x) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, roundf().

    Returns the value of \p x rounded to the nearest integer, as a float.
*/
SHZ_INLINE float shz_roundf(float x) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, truncf().

    Returns the value of \p with its fractional component discarded.
*/
SHZ_INLINE float shz_truncf(float x) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, remainderf().

    Returns the floating-point remainder of \p num divided by \p denom,
    rounded to the nearest integer (as a float).

    \sa shz_fmodf(), shz_remquof(), shz_truncf().
*/
SHZ_INLINE float shz_remainderf(float num, float denom) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, fmodf().

    Returns the floating-point remainder of \p num divided by \p denom,
    rounded towards zero.

    \warning
    This routine does not gracefully handle dividing by zero.

    \sa shz_remainderf()
*/
SHZ_INLINE float shz_fmodf(float num, float denom) SHZ_NOEXCEPT;

/*! (Sorta) Replacement for the <math.h> routine, remquof().

    Returns the floating-point remainder of \p num divided by \p denom,
    rounded to the nearest integer (as a float). \p quot is set equal to
    the quotient which is used as part of the calculation.

    \note
    \p quot is returning as a `float` rather than an `int` as with standard C.
    Simply cast to an `int` manually afterwards if that is the desired behavior.

    \warning
    This routine does not gracefully handle dividing by zero.

    \sa shz_remainderf()
*/
SHZ_INLINE float shz_remquof(float num, float denom, float* quot) SHZ_NOEXCEPT;

//! @}

/*! \name  Mapping
    \brief Routines for mapping a number to another range.
    @{
*/

//! Clamps a floating-point value by the given \p min and \p max values.
SHZ_INLINE float shz_clampf(float value, float min, float max) SHZ_NOEXCEPT;

//! Maps a value within the given range \p from to \p to, to be within the range of `0.0f += 1.0f`.
SHZ_INLINE float shz_normalizef(float current, float from, float to) SHZ_NOEXCEPT;

//! Maps a value within the given range \p from to \p to, to be within the range of `0.0f + 1.0f` more quickly, provided \p to - \p from is a positive difference.
SHZ_INLINE float shz_normalizef_fsrra(float current, float from, float to) SHZ_NOEXCEPT;

//! Maps a value within the given range \p inputStart to \p inputEnd, to be within the range of \p outputStart to \p outputEnd.
SHZ_INLINE float shz_remapf(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT;

//! Maps a value within the given range \p inputStart to \p inputEnd, to be within the range of \p outputStart to \p outputEnd more quickly, provided the \p outputEnd - \p outputStart is a positive difference.
SHZ_INLINE float shz_remapf_fsrra(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT;

//! Wraps the given \p value back to be within the range of \p min to \p max.
SHZ_INLINE float shz_wrapf(float value, float min, float max) SHZ_NOEXCEPT;

//! Wraps the given \p value back to be within the range of \p min to \p max more quickly, provided \p max - \p min is a positive difference.
SHZ_INLINE float shz_wrapf_fsrra(float value, float min, float max) SHZ_NOEXCEPT;

//! Returns the fractional part of \p x, equivalent to GLSL `fract()`.
SHZ_INLINE float shz_fractf(float x) SHZ_NOEXCEPT;

//! Returns -1.0f if \p x < 0, 0.0f if \p x == 0, or 1.0f if \p x > 0.
SHZ_INLINE float shz_signf(float x) SHZ_NOEXCEPT;

//! Clamps \p x to the range [0.0f, 1.0f].
SHZ_INLINE float shz_saturatef(float x) SHZ_NOEXCEPT;

//! @}

/*! \name  Miscellaneous
 *  \brief Assorted routines implementing other fp operations.
 *  @{
 */

/*! Replacement for the <math.h> routine, fabsf().

    Returns the absolute value of \p x.
*/
SHZ_INLINE float shz_fabsf(float x) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, copysignf().

    Returns the value of \p x with the sign of \p y.

    \warning When `y == -0.0f`, a positive value is returned.
*/
SHZ_INLINE float shz_copysignf(float x, float y) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, fmaf().

    Returns \p a * \p b + \p c, performing an FP multiply + accumulate operation.
*/
SHZ_INLINE float shz_fmaf(float a, float b, float c) SHZ_NOEXCEPT;

/*! Returns \p value unchanged, hidden from the optimizer.

    Emits no instructions, but prevents the compiler from reassociating
    across \p value or substituting it with a known equivalent, which can
    be used to influence instruction scheduling.
*/
SHZ_INLINE float shz_opaquef(float value) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, fdimf(),

    Returns the positive difference between \p x and \p y or zero if
    y >= x.

    \warning
    Unlike fdimf(), this routine does not handle INF and NAN values.
*/
SHZ_INLINE float shz_fdimf(float x, float y) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, hypotf().

    Returns the hypoteneuse of the right triangle with the given legs.

    \warning
    Unlike hypotf(), this routine has no error or overflow handling.
*/
SHZ_INLINE float shz_hypotf(float x, float y) SHZ_NOEXCEPT;

/*! Replacement for the <math.h> routine, cbrtf().

    Returns the cubed root of \p x, using a fast approximation.
*/
SHZ_INLINE float shz_cbrtf(float x) SHZ_NOEXCEPT;

//! Returns a value that is linearly interpolated between \p a and \p b by the given ratio, \p t.
SHZ_INLINE float shz_lerpf(float a, float b, float t) SHZ_NOEXCEPT;

//! Returns a value that is barycentrically interpolated between \p a, \p b, and \p c using the given barycentric coordinates, \p u and \p v.
SHZ_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) SHZ_NOEXCEPT;

//! Uses the quadratic formula with the given coefficients to solve for the two roots, returning `true` if any real roots exist, and `false` if the roots are only imaginary.
SHZ_INLINE bool shz_quadratic_roots(float a, float b, float c, float* root1, float* root2) SHZ_NOEXCEPT;

//! Returns a random floating-point number between `0.0f` and `1.0f`, using and updating the given seed.
SHZ_INLINE float shz_randf(int* seed) SHZ_NOEXCEPT;

//! Returns a random floating-point number between \p min and \p max, using and updating the given seed.
SHZ_INLINE float shz_randf_range(int* seed, float min, float max) SHZ_NOEXCEPT;

//! returns 0.0f if x < edge, otherwise 1.0f
SHZ_INLINE float shz_stepf(float x, float edge) SHZ_NOEXCEPT;

//! Returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between. edge0 must be less than edge1 or result is undefined.
SHZ_INLINE float shz_smoothstepf(float x, float edge0, float edge1) SHZ_NOEXCEPT;

//! Returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between. Accepts inverse edges.
SHZ_INLINE float shz_smoothstepf_safe(float x, float edge0, float edge1) SHZ_NOEXCEPT;

//! @}

/*! \name  FSRRA
 *  \brief Routines built around fast reciprocal square root instruction.
 *  @{
 */

//! Calculates 1.0f/sqrtf( \p x), using a fast approximation.
SHZ_INLINE float shz_inv_sqrtf_fsrra(float x) SHZ_NOEXCEPT;

//! Calculates 1.0f/sqrtf( \p x ), using a fast approximation, while safely protecting against division-by-zero.
SHZ_INLINE float shz_inv_sqrtf(float x) SHZ_NOEXCEPT;

//! Returns the fast approximate square root of the given value, \p x.
SHZ_INLINE float shz_sqrtf_fsrra(float x) SHZ_NOEXCEPT;

//! Returns the fast approximate square root of the given value, \p x, safely returning `0.0f` is \p x == `0.0f`.
SHZ_INLINE float shz_sqrtf(float x) SHZ_NOEXCEPT;

/*! Approximate inverse of a signed number.

    This routine takes approximately the same number of cycles as `1.0f / x` on the SH4
    and is slightly less accurate; however, it can be paired with a full inversion to
    perform the two in parallel.

    \note The vector forms of this routine automatically handle optimal parallelization
          of this operation across multiple lanes.

    \param  x   The signed number to reciprocate.
    \returns    The approximated value `1.0f / x`.

    \sa shz_invf_fsrra(), shz_vec2_invf(), shz_vec3_invf(), shz_vec4_invf()
*/
SHZ_INLINE float shz_invf(float x) SHZ_NOEXCEPT;

/*! Fast unsigned approximate inverse of a number.

    This routine returns a fast unsigned reciprocal, which is approximately equal
    to `1.0f / fabsf(x)`. It can also be paired with a real floating-point reciprocal
    in the same form with both computing in parallel.

    \note The vector forms of this routine automatically handle optimal parallelization
          of this operation across multiple lanes.

    \warning The result value is always positive, regardless of the sign of \p x.

    \param  x   The unsigned number to reciprocate.
    \returns    The approximated value `1.0f / fabsf(x)`.

    \sa shz_invf_fsrra(), shz_vec2_invf(), shz_vec3_invf(), shz_vec4_invf()
*/
SHZ_INLINE float shz_invf_fsrra(float x) SHZ_NOEXCEPT;

/*! Approximate division by a signed denominator.

    This routine takes approximately the same number of cycles as a full float division
    on the SH4; however, it can be paired with a real division to perform two in parallel.

    \note The vector forms of this routine automatically handle optimal parallelization
          of this operation across multiple lanes.

    \param  num     Numerator to divide into.
    \param  denom   Signed denominoator to divide by.
    \returns        The approximated quotient `num / denom`.

    \sa shz_divf_fsrra(), shz_vec2_divf(), shz_vec3_divf(), shz_vec4_divf()
*/
SHZ_INLINE float shz_divf(float num, float denom) SHZ_NOEXCEPT;

/*! Fast approximate division by an unsigned denominator.

    This routine performs a fast division approximation that takes fewer clock cycles to
    perform than using a regular division operator on two floats.

    \note The vector forms of this routine automatically handle optimal parallelization
          of this operation across multiple lanes.

    \warning The denominator is treated as though it were an unsigned value.

    \param  num     Numerator to divide into.
    \param  denom   Unsigned denominoator to divide by.

    \returns        The approximated quotient `num / fabsf(denom)`.

    \sa shz_divf(), shz_vec2_divf_fsrra(), shz_vec3_divf_fsrra(), shz_vec4_divf_fsrra()
*/
SHZ_INLINE float shz_divf_fsrra(float num, float denom) SHZ_NOEXCEPT;

//! @}

/*! \name  FIPR
 *  \brief Routines built around fast 4D dot product.
 *  \todo  Generalize these with a macro that lets you choose vector FP regs.
 *  @{
 */

//! Takes two sets of 3D vectors as 3 floats and calculates their dot product using an approximation.
SHZ_INLINE float shz_dot6f(float x1, float y1, float z1,
                           float x2, float y2, float z2) SHZ_NOEXCEPT;

//! Takes two sets of 4D vectors as 4 floats and calculates their dot product using an approximation.
SHZ_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                           float x2, float y2, float z2, float w2) SHZ_NOEXCEPT;

//! Takes a 3D vector as 3 floats and calculates its squared magnitude using a fast approximation.
SHZ_INLINE float shz_mag_sqr3f(float x, float y, float z) SHZ_NOEXCEPT;

//! Takes a 4D vector as 4 floats and calculates its squared magnitude using a fast approximation.
SHZ_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! @}

/*! \name  Transcendental
    \brief Fast approximations for non-trig transcendental functions.
    @{
*/

//! Fast approximation for raising 2 to a floating-point power.
SHZ_INLINE float shz_pow2f(float p) SHZ_NOEXCEPT;

//! Fast approximation for C's powf().
SHZ_INLINE float shz_powf(float x, float p) SHZ_NOEXCEPT;

/*! Raises \p x to the integer power \p n, using exponentiation by squaring.

    Unlike shz_powf(), this is not an approximation: the result is computed by repeated
    multiplication, taking popcount(|n|) + floor(log2(|n|)) multiplies, plus a single
    division by \p x when \p n is negative.

    When \p n is a compile-time constant, this expands to GCC's optimal multiplication chain
    for that exponent instead.

    \note shz_powif(x, 0) returns 1.0f for every \p x, including 0.0f.
    \note Rounding may differ slightly from multiplying \p x by itself \p n times, or from powf().

    \sa shz_powf()
*/
SHZ_INLINE float shz_powif(float x, int n) SHZ_NOEXCEPT;

//! Fast approximation of POSIX's pow10f().
SHZ_INLINE float shz_pow10f(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's log2f().
SHZ_INLINE float shz_log2f(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's logf().
SHZ_INLINE float shz_logf(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's log10f().
SHZ_INLINE float shz_log10f(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's expf().
SHZ_INLINE float shz_expf(float p) SHZ_NOEXCEPT;

//! Fast approximation for C's expm1f().
SHZ_INLINE float shz_expm1f(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's log1pf().
SHZ_INLINE float shz_log1pf(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's logbf().
SHZ_INLINE float shz_logbf(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's ldexpf(), taking a floating-point exponent.
SHZ_INLINE float shz_ldexpf(float x, float exp) SHZ_NOEXCEPT;

//! Fast approximation for C's scalbnf(), taking a floating-point exponent.
SHZ_INLINE float shz_scalbnf(float x, float exp) SHZ_NOEXCEPT;

//! Fast approximation for C's erff().
SHZ_INLINE float shz_erff(float x) SHZ_NOEXCEPT;

//! @}

#include "inline/shz_scalar.inl.h"

SHZ_DECLS_END

#endif
