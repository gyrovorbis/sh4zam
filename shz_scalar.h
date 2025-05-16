/*! \file
 *  \brief General-purpose scalar math routines.
 *  \ingroup scalar
 *
 *  This file provides a collection of general-purpose math routines for
 *  individual scalar values.
 * 
 *  \author Falco Girgis
 *  \author Paul Cercueil
 *
 *  \todo
 *      - set/check FP precision mode
 *      - ceilf()/floorf() use rounding modes
 */
#ifndef SHZ_SCALAR_H
#define SHZ_SCALAR_H

#include "shz_cdefs.h"

/** \defgroup scalar Scalar
 *  \brief           scalar functions and utilities.
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
SHZ_FORCE_INLINE float shz_floorf(float x) {
    float result = (float)(int)x;

    if (x < 0.0f)
        result -= 1.0f;

    return result;
}

//! Returns the closest integral value to \p x, rounded up, as a float.
SHZ_FORCE_INLINE float shz_ceilf(float x) {
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
SHZ_FORCE_INLINE float shz_fmacf(float a, float b, float c) {
    return a * b + c;
}

//! Returns a value that is linearly interpolated between \p a and \p b by the given ratio, \p t. 
SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) {
    return shz_fmacf(t, (b - a), a);
}

//! 
SHZ_FORCE_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) {
    return shz_fmacf(u, (b - a), shz_fmacf(v, (c - a), a));
}
//! @}

/*! \name  FSRRA
 *  \brief Routines built around fast reciprocal square root.
 *  @{
 */
//! Returns the square root of the given value, \p x.
SHZ_FORCE_INLINE float shz_sqrtf(float x) {
    return __builtin_sqrtf(x);
}

//! Calculates 1.0f/sqrtf( \p x ), using a fast approximation.
SHZ_FORCE_INLINE float shz_inverse_sqrtf(float x) {
    asm("fsrra %0" : "+f" (x));
    return x;
}

//! Takes the inverse of \p p using a very fast approximation, returning a positive result.
SHZ_FORCE_INLINE float shz_inverse_posf(float x) {
    return shz_inverse_sqrtf(x * x);
}

//! Divides \p num by \p denom using a very fast approximation, returning a positive result.
SHZ_FORCE_INLINE float shz_div_posf(float num, float denom) {
    return num * shz_inverse_posf(denom);
}

//! Takes the inverse of \p p using a faster approximation than doing a full division.
SHZ_FORCE_INLINE float shz_fast_invf(float x) {
    float inv;

    if (__builtin_constant_p(x))
        return 1.0f / x;

    inv = shz_inverse_posf(x);

    if (x < 0.0f)
        inv = -inv;

    return inv;
}
//! @}

/*! \name  FIPR
 *  \brief Routines built around fast 4D dot product. 
 *  \todo  Generalize these with a macro that lets you choose vector FP regs.
 *  @{
 */
//! Takes two sets of 4D vectors as 4 floats and calculates their dot product using an approximation.
SHZ_FORCE_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) {
    register float rx1 asm("fr0") = x1;
    register float ry1 asm("fr1") = y1;
    register float rz1 asm("fr2") = z1;
    register float rw1 asm("fr3") = w1;
    register float rx2 asm("fr4") = x2;
    register float ry2 asm("fr5") = y2;
    register float rz2 asm("fr6") = z2;
    register float rw2 asm("fr7") = w2;

    asm("fipr fv0, fv4"
        : "+f" (rw2)
        : "f" (rx1), "f" (ry1), "f" (rz1), "f" (rw1),
          "f" (rx2), "f" (ry2), "f" (rz2));

    return rw2;
}

//! Takes a 4D vector as 4 floats and calculates its squared magnitude using a fast approximation.
SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) {
    register float rx asm("fr0") = x;
    register float ry asm("fr1") = y;
    register float rz asm("fr2") = z;
    register float rw asm("fr3") = w;

    asm("fipr fv0, fv0"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}
//! @}

SHZ_DECLS_END

#endif // SHZ_INTRIN_H
