/*! \file
 *  \brief General-purpose scalar math routines.
 *  \ingroup scalar
 *
 *  This file provides a collection of general-purpose math routines for
 *  individual scalar values.
 *
 *  \todo
 *      - ceilf()/floorf() use rounding modes?
 *      - ASM vs compiler builtins for FSRRA, FMAC.
 *
 *  \author    Falco Girgis
 *  \author    Paul Cercueil
 *  \copyright MIT License
 */
#ifndef SHZ_SCALAR_H
#define SHZ_SCALAR_H

#include <math.h>

#include "shz_cdefs.h"
#include "shz_fpscr.h"

/*! \defgroup scalar Scalar
 *  \brief           Scalar functions and utilities.
 *  \todo            FILL ME IN
 */

//! Returns the minimum of the two given values
#define SHZ_MIN(a, b)           (((a) < (b))? (a) : (b))
//! Return sthe maximum of the two given values
#define SHZ_MAX(a, b)           (((a) > (b))? (a) : (b))
//! Clamps \p v between the given \p min and \p max values
#define SHZ_CLAMP(v, min, max)  SHZ_MIN(SHZ_MAX(v, min), max)
//! Clamps \p v within \p min and \p max then normalizes it between 0.0f and 1.0f
#define SHZ_NORM(v, min, max)   ((float)(SHZ_CLAMP(v, min, max) - (min)) / (float)((max) - (min)))

SHZ_DECLS_BEGIN

/*! \name  Rounding
 *  \brief Routines for rounding floats.
 *  @{
 */

//! Returns the closest integral value to \p x, rounded down, as a float.
SHZ_FORCE_INLINE float shz_floorf(float x) SHZ_NOEXCEPT;

//! Returns the closest integral value to \p x, rounded up, as a float.
SHZ_FORCE_INLINE float shz_ceilf(float x) SHZ_NOEXCEPT;

//! @}

/*! \name  FMAC
 *  \brief Routines built around multiply + accumulate operations.
 *  @{
 */

//! Returns \p a * \p b + \p c, performing an FP multiply + accumulate operation.
SHZ_FAST_MATH SHZ_FORCE_INLINE float shz_fmacf(float a, float b, float c) SHZ_NOEXCEPT;

//! Returns a value that is linearly interpolated between \p a and \p b by the given ratio, \p t.
SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) SHZ_NOEXCEPT;

//! Returns a value that is barycentrically interpolated between \p a, \p b, and \p c using the given barycentric coordinates, \p u and \p v.
SHZ_FORCE_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) SHZ_NOEXCEPT;

//! @}

/*! \name  FSRRA
 *  \brief Routines built around fast reciprocal square root instruction.
 *  @{
 */

//! Calculates 1.0f/sqrtf( \p x ), using a fast approximation.
SHZ_FORCE_INLINE float shz_inv_sqrtf(float x) SHZ_NOEXCEPT;

//! Returns the faster approximate square root of the given value, \p x.
SHZ_FORCE_INLINE float shz_sqrtf_fsrra(float x) SHZ_NOEXCEPT;

//! Takes the inverse of \p p using a very fast approximation, returning a positive result.
SHZ_FORCE_INLINE float shz_invf_fsrra(float x) SHZ_NOEXCEPT;

//! Calculates the square root of \p x using the `FSQRT` instruction.
SHZ_FAST_MATH SHZ_FORCE_INLINE float shz_sqrtf(float x) SHZ_NOEXCEPT;

//! Takes the inverse of \p p using a slighty faster approximation than doing a full division.
SHZ_FORCE_INLINE float shz_invf(float x) SHZ_NOEXCEPT;

//! Divides \p num by \p denom using a slightly faster approximation.
SHZ_FORCE_INLINE float shz_divf(float num, float denom) SHZ_NOEXCEPT;

//! Divides \p num by \p denom using a very fast approximation, returning a positive result.
SHZ_FORCE_INLINE float shz_divf_fsrra(float num, float denom) SHZ_NOEXCEPT;

//! @}

/*! \name  FIPR
 *  \brief Routines built around fast 4D dot product.
 *  \todo  Generalize these with a macro that lets you choose vector FP regs.
 *  @{
 */

//! Takes two sets of 4D vectors as 4 floats and calculates their dot product using an approximation.
SHZ_FORCE_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) SHZ_NOEXCEPT;

//! Takes a 4D vector as 4 floats and calculates its squared magnitude using a fast approximation.
SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! @}

//! Fast approximation for raising 2 to a floating-point power.
SHZ_FORCE_INLINE float shz_pow2f(float p) SHZ_NOEXCEPT;

//! Fast approximation for C's log2f().
SHZ_FORCE_INLINE float shz_log2f(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's logf().
SHZ_FORCE_INLINE float shz_logf(float x) SHZ_NOEXCEPT;

//! Fast approximation for C's powf().
SHZ_FORCE_INLINE float shz_powf(float x, float p) SHZ_NOEXCEPT;

//! Fast approximation for C's expf().
SHZ_FORCE_INLINE float shz_expf(float p) SHZ_NOEXCEPT;

#include "shz_scalar.inl"

SHZ_DECLS_END

#endif // SHZ_INTRIN_H
