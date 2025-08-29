/*! \file
 *  \brief   Trigonometry functions and macros.
 *  \ingroup trig
 *
 *  This file provides an API offering fast versions of trigonometry functions.
 * 
 *  \author Falco Girgis
 *  \author Paul Cercueil
 */
#ifndef SHZ_TRIG_H
#define SHZ_TRIG_H

#include "shz_scalar.h"

/** \defgroup trig Trigonometry
 *  \brief         Trig functions and utilities.
 *  \todo          FILL ME IN
 *  \todo          Find fast approximations of arc trig functions
 */

//! Single-precision floating-point PI approximation (do not use M_PI!)
#define SHZ_F_PI                3.1415926f
//! Multiplicative factor for passing the FSCA instruction angles in radians
#define SHZ_FSCA_RAD_FACTOR     10430.37835f
//! Multiplicative factor for passing the FSCA instrution angles in degrees
#define SHZ_FSCA_DEG_FACTOR     182.04444443f
//! Maximum FP error for FSCA instruction
#define SHZ_FSCA_ERROR_MAX      4.76837158e-7

//! Converts the given angle in degrees to radians
#define SHZ_DEG_TO_RAD(deg)     ((deg) * SHZ_F_PI / 180.0f)
//! Converts the given angle in radians to degrees
#define SHZ_RAD_TO_DEG(rad)     ((rad) * 180.0f / SHZ_F_PI)

SHZ_DECLS_BEGIN

/*! Pair representing the sine and cosine of a given angle. 
 *  
 *  The SH4 provides a single instruction, `FSCA`, for retrieving fast
 *  approximations of \b both the sine and cosine of a given angle. As such,
 *  when you need both values, it's most efficient to grab them both at once as
 *  a pair using this structure than it is to retrieve them separately. 
 * 
 *  \sa shz_sincosf(), shz_sincosf_deg()
 */
typedef struct shz_sincos {
    float sin;  //!< sinf() approximation for the angle
    float cos;  //!< cosf() approximation for the angle
} shz_sincos_t;

//! Returns sinf()/cosf() pairs for the given unsigned 16-bit angle in radians.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16(uint16_t radians16) {
    register float rsin asm("fr8");
    register float rcos asm("fr9");

    asm volatile(R"(
            lds  %2, fpul
            fsca fpul, dr8
        )"
        : "=f" (rsin), "=f" (rcos)
        : "r" (radians16)
        : "fpul");

    return (shz_sincos_t){ rsin, rcos };
}

//! Returns sinf()/cosf() pairs for the given floating-point angle in radians.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) {
    register float rsin asm("fr8");
    register float rcos asm("fr9");

    radians *= SHZ_FSCA_RAD_FACTOR;

    asm volatile(R"(
            ftrc  %2, fpul
            fsca  fpul, dr8
        )"
        : "=f" (rsin), "=f" (rcos)
        : "f" (radians)
        : "fpul");

    return (shz_sincos_t){ rsin, rcos };
}

//! Returns sinf/cosf() pairs for the given floating-point angle in degrees.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) {
    register float rsin asm("fr8");
    register float rcos asm("fr9");

    degrees *= SHZ_FSCA_DEG_FACTOR;

    asm volatile(R"(
        ftrc  %2, fpul
        fsca  fpul, dr8
    )"
    : "=&f" (rsin), "=&f" (rcos)
    : "f" (degrees)
    : "fpul");

    return (shz_sincos_t){ rsin, rcos };
}

//! Returns tanf() from the given pre-computed \p sincos pair.
SHZ_FORCE_INLINE float shz_sincos_tanf(shz_sincos_t sincos) {
    return shz_divf(sincos.sin, sincos.cos);
}

//! One-off routine for returning only sinf() from an angle in radians.
SHZ_FORCE_INLINE float shz_sinf(float radians) {
    return shz_sincosf(radians).sin;
}

//! One-off routine for returning only sinf() from an angle in degrees.
SHZ_FORCE_INLINE float shz_sinf_deg(float degrees) {
    return shz_sincosf_deg(degrees).sin;
}

//! One-off routine for returning only cosf() from an angle in radians.
SHZ_FORCE_INLINE float shz_cosf(float radians) {
    return shz_sincosf(radians).cos;
}

//! One-off routine for returning only cosf() from an angle in degrees.
SHZ_FORCE_INLINE float shz_cosf_deg(float degrees) {
    return shz_sincosf_deg(degrees).cos;
}

//! One-off routine for returning only tanf() from an angle in radians.
SHZ_FORCE_INLINE float shz_tanf(float radians) {
    return shz_sincos_tanf(shz_sincosf(radians));
}

//! One-off routine for returning only tanf() from an angle in degrees.
SHZ_FORCE_INLINE float shz_tanf_deg(float degrees) {
    return shz_sincos_tanf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_atanf_unit(float x) SHZ_NOEXCEPT {
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;

    return shz_fmacf(n2, x * x, n1) * x;
}

SHZ_INLINE float shz_atanf_q1(float x) SHZ_NOEXCEPT {
    return (SHZ_F_PI * 0.5f) - shz_atanf_unit(shz_invf_fsrra(x));
}

SHZ_INLINE float shz_atanf(float x) SHZ_NOEXCEPT {
    if(x > 1.0f)
	    return shz_atanf_q1(x);
    else if(x < -1.0f)
        return -shz_atanf_q1(x);
    else
        return shz_atanf_unit(x);
}

SHZ_INLINE float shz_asinf(float x) SHZ_NOEXCEPT {
    return shz_atanf(x * shz_inv_sqrtf(1.0f - (x * x)));
}

SHZ_INLINE float shz_acosf(float x) SHZ_NOEXCEPT {
    return (SHZ_F_PI * 0.5f) - shz_asinf(x);
}

SHZ_DECLS_END

#endif
