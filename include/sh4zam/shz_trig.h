/*! \file
 *  \brief   Trigonometry functions and macros.
 *  \ingroup trig
 *
 *  This file provides an API offering fast versions of trigonometry functions.
 *
 *  \author 2025 Falco Girgis
 *  \author 2025 Paul Cercueil
 *
 *  \copyright MIT License
 */

#ifndef SHZ_TRIG_H
#define SHZ_TRIG_H

#include "shz_scalar.h"

/*! \defgroup trig Trigonometry
    \brief    Trig functions and utilities.

    The following API provides a series of routines implementing trigonometric
    functions. While some of these are specialized, many are meant to be
    drop-in replacements for the equivalent functions provided by <math.h>.
*/

//! Single-precision floating-point PI approximation (do not use M_PI!)
#define SHZ_F_PI                3.1415926f
//! Single-precision FP PI approximation divided by 2.
#define SHZ_F_PI_2              (SHZ_F_PI * 0.5f)
//! Single-precision FP PI approximation divided by 4.
#define SHZ_F_PI_4              (SHZ_F_PI * 0.25f)
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

//! Alternate C shz_sincos_t typedef for those who hate POSIX style.
typedef shz_sincos_t shz_sincos;

/*! \name  Sin/Cos Pairs
    \brief Routines involving precomputed sine + cosine pairs.
    @{
*/

//! Returns sinf()/cosf() pairs for the given unsigned 16-bit angle in radians.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16(uint16_t radians16) SHZ_NOEXCEPT;

//! Returns sinf()/cosf() pairs for the given floating-point angle in radians.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) SHZ_NOEXCEPT;

//! Returns sinf/cosf() pairs for the given floating-point angle in degrees.
SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) SHZ_NOEXCEPT;

//! Returns tanf() from the given pre-computed \p sincos pair.
SHZ_FORCE_INLINE float shz_sincos_tanf(shz_sincos_t sincos) SHZ_NOEXCEPT;

//! Returns the floating-point secant (1.0f / cosf(x)) from the given pre-computed \p sincos pair.
SHZ_FORCE_INLINE float shz_sincos_secf(shz_sincos_t sincos) SHZ_NOEXCEPT;

//! Returns the floating-point cosecant (1.0f / sinf(x)) from the given pre-computed \p sincos pair.
SHZ_FORCE_INLINE float shz_sincos_cscf(shz_sincos_t sincos) SHZ_NOEXCEPT;

//! Returns the floating-point cotangent (1.0f / tanf(x)) from the given pre-computed \p sincos pair.
SHZ_FORCE_INLINE float shz_sincos_cotf(shz_sincos_t sincos) SHZ_NOEXCEPT;

//! @}

/*! \name  Independent Functions
    \brief Routines providing single trigonometric functions.
    @{
*/

//! One-off routine for returning only sinf() from an angle in radians.
SHZ_FORCE_INLINE float shz_sinf(float radians) SHZ_NOEXCEPT;

//! One-off routine for returning only sinf() from an angle in degrees.
SHZ_FORCE_INLINE float shz_sinf_deg(float degrees) SHZ_NOEXCEPT;

//! One-off routine for returning only cosf() from an angle in radians.
SHZ_FORCE_INLINE float shz_cosf(float radians) SHZ_NOEXCEPT;

//! One-off routine for returning only cosf() from an angle in degrees.
SHZ_FORCE_INLINE float shz_cosf_deg(float degrees) SHZ_NOEXCEPT;

//! One-off routine for returning only tanf() from an angle in radians.
SHZ_FORCE_INLINE float shz_tanf(float radians) SHZ_NOEXCEPT;

//! One-off routine for returning only tanf() from an angle in degrees.
SHZ_FORCE_INLINE float shz_tanf_deg(float degrees) SHZ_NOEXCEPT;

//! One-off routine for returning only secant (1 / cos(x)) from an angle in radians.
SHZ_FORCE_INLINE float shz_secf(float radians) SHZ_NOEXCEPT;

//! One-off routine for returning only secant (1 / cos(x)) from an angle in degrees.
SHZ_FORCE_INLINE float shz_secf_deg(float degrees) SHZ_NOEXCEPT;

//! One-off routine for returning only cosecant (1 / sin(x)) from an angle in radians.
SHZ_FORCE_INLINE float shz_cscf(float radians) SHZ_NOEXCEPT;

//! One-off routine for returning only cosecant (1 / sin(x)) from an angle in degrees.
SHZ_FORCE_INLINE float shz_cscf_deg(float degrees) SHZ_NOEXCEPT;

//! One-off routine for returning only cotangent (1 / tan(x)) from an angle in radians.
SHZ_FORCE_INLINE float shz_cotf(float radians) SHZ_NOEXCEPT;

//! One-off routine for returning only cotangent (1 / cot(x)) from an angle in degrees.
SHZ_FORCE_INLINE float shz_cotf_deg(float degrees) SHZ_NOEXCEPT;

//! @}

/*! \name  Inverse Functions
    \brief Routines providing inverse trigonometric functions.
    @{
*/

//! Fast arcsine approximation; equivalent to C's asinf().
SHZ_INLINE float shz_asinf(float x) SHZ_NOEXCEPT;

//! Fast arccosine approximation; equivalent to C's acosf().
SHZ_INLINE float shz_acosf(float x) SHZ_NOEXCEPT;

//! Fast arctangent approximation for unit values between `0.0f` and `1.0f`.
SHZ_FORCE_INLINE float shz_atanf_unit(float x) SHZ_NOEXCEPT;

//! Fast arctangent approximation for values lying within the first quadrant (`>= 0.0f`).
SHZ_INLINE float shz_atanf_q1(float x) SHZ_NOEXCEPT;

//! Fast arctangent approximation; equvalent to C's atanf().
SHZ_INLINE float shz_atanf(float x) SHZ_NOEXCEPT;

//! Computes arctangent of `y / x`, using the signs of arguments to determine correct quadtrant. Equivalent to C's atan2f().
SHZ_INLINE float shz_atan2f(float y, float x) SHZ_NOEXCEPT;

//! Fast arcsecant/inverse secant approximation, taking units in radians.
SHZ_INLINE float shz_asecf(float x) SHZ_NOEXCEPT;

//! Fast arccosecant/inverse cosecant approximation; taking units in radians.
SHZ_INLINE float shz_acscf(float x) SHZ_NOEXCEPT;

//! Fast arccotangent/inverse cotangent approximation; taking units in radians.
SHZ_INLINE float shz_acotf(float x) SHZ_NOEXCEPT;

//! @}

/*! \name  Hyperbolic Functions
    \brief Trigonometric functions for hyperbolas
    @{
*/

//! Fast hyperbolic sine function.
SHZ_INLINE float shz_sinhf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic cosine function.
SHZ_INLINE float shz_coshf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic tangent function.
SHZ_INLINE float shz_tanhf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic cosecant function.
SHZ_INLINE float shz_cschf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic secant function.
SHZ_INLINE float shz_sechf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic cotangent function.
SHZ_INLINE float shz_cothf(float x) SHZ_NOEXCEPT;

//! @}

/*! \name  Inverse Hyperbolic Functions
    \brief Inverse trigonometric functions for hyperbolas
    @{
*/

//! Fast hyperbolic arcsine function.
SHZ_INLINE float shz_asinhf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic arccosine function.
SHZ_INLINE float shz_acoshf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic arctangent function.
SHZ_INLINE float shz_atanhf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic arccosecant function.
SHZ_INLINE float shz_acschf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic arcsecant function.
SHZ_INLINE float shz_asechf(float x) SHZ_NOEXCEPT;

//! Fast hyperbolic arccotangent function.
SHZ_INLINE float shz_acothf(float x) SHZ_NOEXCEPT;

//! @}

#include "inline/shz_trig.inl.h"

SHZ_DECLS_END

#endif
