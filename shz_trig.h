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

#include "shz_cdefs.h"

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
#define SHZ_FSCA_DEG_FACTOR     SHZ_DEG_TO_RAD(SHZ_FSCA_RAD_FACTOR)

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
typedef SHZ_ALIGNAS(8) struct shz_sincos {
    float sin;  //!< sin() approximation of the angle
    float cos;  //!< cos() approximation of the angle
} shz_sincos_t;

//! Returns sinf()/cosf() pairs for the given unsigned 16-bit angle in radians.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16(uint16_t radians16) {
    register float rsin asm("fr0");
    register float rcos asm("fr1");

    asm("fsca fpul, dr0"
        : "=f" (rsin), "=f" (rcos)
        : "y" (radians16));

    return (shz_sincos_t){ rsin, rcos };
}

//! Returns sinf()/cosf() pairs for the given floating-point angle in radians.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) {
    return shz_sincosu16(radians * SHZ_FSCA_RAD_FACTOR);
}

//! Returns sinf/cosf() pairs for the given floating-point angle in degrees.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) {
    return shz_sincosu16(degrees * SHZ_FSCA_DEG_FACTOR);
}

//! Returns tanf() from the given pre-computed \p sincos pair.
SHZ_FORCE_INLINE float shz_sincos_tanf(shz_sincos_t sincos) {
    return sincos.sin / sincos.cos;
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
    shz_sincos_tanf(shz_sincosf(radians));
}

//! One-off routine for returning only tanf() from an angle in degrees.
SHZ_FORCE_INLINE float shz_tanf_deg(float degrees) {
    shz_sincos_tanf(shz_sincosf_deg(degrees));
}

SHZ_DECLS_END

#endif
