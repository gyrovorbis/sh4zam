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
SHZ_FORCE_INLINE float shz_floorf(float x) SHZ_NOEXCEPT {
    float result = (float)(int)x;

    if (x < 0.0f)
        result -= 1.0f;

    return result;
}

//! Returns the closest integral value to \p x, rounded up, as a float.
SHZ_FORCE_INLINE float shz_ceilf(float x) SHZ_NOEXCEPT {
    float result = (float)(int)x;

    if (x > result)
        result += 1.0f;

    return result;
}
//! @}

/*! \name  FMAC
 *  \brief Routines built around multiply + accumulate operations.
 *  @{
 */
//! Returns \p a * \p b + \p c, performing an FP multiply + accumulate operation.
SHZ_FAST_MATH SHZ_FORCE_INLINE float shz_fmacf(float a, float b, float c) SHZ_NOEXCEPT {
#if 1 // Trust the compiler?
    return a * b + c;
#else // FUCK NO?
    asm volatile ("fmac fr0, %[b], %[c]\n"
    : [c] "+&f" (c)
    : "w" (a), [b] "f" (b));
  return c;
#endif
}

//! Returns a value that is linearly interpolated between \p a and \p b by the given ratio, \p t.
SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) SHZ_NOEXCEPT {
    return shz_fmacf(t, (b - a), a);
}

//! Returns a value that is barycentrically interpolated between \p a, \p b, and \p c using the given barycentric coordinates, \p u and \p v.
SHZ_FORCE_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) SHZ_NOEXCEPT {
    return shz_fmacf(u, (b - a), shz_fmacf(v, (c - a), a));
}
//! @}

/*! \name  FSRRA
 *  \brief Routines built around fast reciprocal square root instruction.
 *  @{
 */
//! Calculates 1.0f/sqrtf( \p x ), using a fast approximation.
SHZ_FORCE_INLINE float shz_inv_sqrtf(float x) SHZ_NOEXCEPT {
    asm volatile("fsrra %0" : "+f" (x));
    return x;
}

//! Returns the faster approximate square root of the given value, \p x.
SHZ_FORCE_INLINE float shz_sqrtf_fsrra(float x) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(x) * x;
}

//! Takes the inverse of \p p using a very fast approximation, returning a positive result.
SHZ_FORCE_INLINE float shz_invf_fsrra(float x) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(x * x);
}

//! Calculates the square root of \p x using the `FSQRT` instruction.
SHZ_FAST_MATH SHZ_FORCE_INLINE float shz_sqrtf(float x) SHZ_NOEXCEPT {
    // Call the compiler built-in explicitly in case -fno-builtin is used.
    return __builtin_sqrtf(x);
}

//! Takes the inverse of \p p using a slighty faster approximation than doing a full division.
SHZ_FORCE_INLINE float shz_invf(float x) SHZ_NOEXCEPT {
    float inv;

    if (__builtin_constant_p(x))
        return 1.0f / x;

    inv = shz_invf_fsrra(x);

    if (x < 0.0f)
        inv = -inv;

    return inv;
}

//! Divides \p num by \p denom using a slightly faster approximation.
SHZ_FORCE_INLINE float shz_divf(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return num * shz_invf(denom);
}

//! Divides \p num by \p denom using a very fast approximation, returning a positive result.
SHZ_FORCE_INLINE float shz_divf_fsrra(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return num * shz_invf_fsrra(denom);
}
//! @}

/*! \name  FIPR
 *  \brief Routines built around fast 4D dot product.
 *  \todo  Generalize these with a macro that lets you choose vector FP regs.
 *  @{
 */
//! Takes two sets of 4D vectors as 4 floats and calculates their dot product using an approximation.
SHZ_FORCE_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) SHZ_NOEXCEPT {
    register float rx1 asm("fr8")  = x1;
    register float ry1 asm("fr9")  = y1;
    register float rz1 asm("fr10") = z1;
    register float rw1 asm("fr11") = w1;
    register float rx2 asm("fr12") = x2;
    register float ry2 asm("fr13") = y2;
    register float rz2 asm("fr14") = z2;
    register float rw2 asm("fr15") = w2;

    // Undefined behavior when in another mode with FIPR
    SHZ_SINGLE_PRECISION_GUARD();

    asm("fipr fv8, fv12"
        : "+f" (rw2)
        : "f" (rx1), "f" (ry1), "f" (rz1), "f" (rw1),
          "f" (rx2), "f" (ry2), "f" (rz2));

    return rw2;
}

//! Takes a 4D vector as 4 floats and calculates its squared magnitude using a fast approximation.
SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) SHZ_NOEXCEPT {
    register float rx asm("fr8")  = x;
    register float ry asm("fr9")  = y;
    register float rz asm("fr10") = z;
    register float rw asm("fr11") = w;

     // Undefined behavior when in another mode with FIPR
    SHZ_SINGLE_PRECISION_GUARD();

    asm("fipr fv8, fv8"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}
//! @}

// https://github.com/appleseedhq/appleseed/blob/master/src/appleseed/foundation/math/fastmath.h
//! Fast approximation for raising 2 to a floating-point power.
SHZ_FORCE_INLINE float shz_pow2f(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return powf(2.0f, p);

    // Underflow of exponential is common practice in numerical routines, so handle it here.
    const float clipp = p < -126.0f ? -126.0f : p;
    const union { uint32_t i; float f; } v = {
        (uint32_t)((1 << 23) * (clipp + 126.94269504f))
    };

    return v.f;
}

//! Fast approximation for C's log2f().
SHZ_FORCE_INLINE float shz_log2f(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return log2f(x);

    assert(x >= 0.0f);

    const union { float f; uint32_t i; } vx = { x };
    const float y = (float) (vx.i) * 1.1920928955078125e-7f;

    return y - 126.94269504f;
}

//! Fast approximation for C's logf().
SHZ_FORCE_INLINE float shz_logf(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return logf(x);

    return 0.69314718f * shz_log2f(x);
}

//! Fast approximation for C's powf().
SHZ_FORCE_INLINE float shz_powf(float x, float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x) && __builtin_constant_p(p))
        return powf(x, p);

    return shz_pow2f(p * shz_log2f(x));
}

//! Fast approximation for C's expf().
SHZ_FORCE_INLINE float shz_expf(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return expf(p);

    return shz_pow2f(1.442695040f * p);
}

SHZ_DECLS_END

#endif // SHZ_INTRIN_H
