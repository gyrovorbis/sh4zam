/*! \file
    \brief Complex number API
    \ingroup complex

    This file contains a collection of routines for working with complex
    (real + imaginary) numbers. The API is mostly modeled after C99's
    <complex.h>, although it also supports being used from C++ as well.

    \author     2026 Falco Girgis
    \copyright  MIT License

    \todo
        - to/from shz_mat2x2_t and XMTRX
        - FFT utilities
        - inverse FFT
*/

#ifndef SHZ_COMPLEX_H
#define SHZ_COMPLEX_H

#include "shz_cdefs.h"
#include <stddef.h>

/*! \defgroup complex Complex Numbers
    \brief    API for representing and operating on complex numbers.

    The complex number API serves as a replacement for C and C++'s <complex.h>,
    offering fast, accelerated approximations for each routine. The API also
    features shz_fft(), an SH4 optimized fast-fourier transform. 
*/

/*! \name  Preprocessor
    \brief Macros and preprocessor constants.
    @{
*/

#define SHZ_CMPLXF(x, y)    shz_cinitf(x, y)         //!< Initializes a complex number.
#define SHZ_CMPLXF32(x, y)  SHZ_CMPLXF(x, y)         //!< Initializes a complex number.
#define SHZ_I               SHZ_CMPLXF32(0.0f, 1.0f) //!< Unit imaginary constant, 'I'.

//! @}

SHZ_DECLS_BEGIN

//! Represents a floating-point complex number real/imaginary pair.
typedef struct shz_complex {
    float real;     //!< The real portion of the complex number.
    float imag;     //!< The imaginary portion of the complex number.
} shz_complex_t;

//! shz_complex_t alias for those who don't like POSIX-style.
typedef shz_complex_t shz_complex;

/*! \name  Basic
    \brief Basic operations on complex numbers.
    @{
*/

//! Returns a complex number which has been initialized to contain the given component values.
SHZ_INLINE shz_complex_t shz_cinitf(float real, float imag) SHZ_NOEXCEPT;

//! Returns a complex number with a magnitude of \p r, and a phase angle of \p theta (in radians).
SHZ_INLINE shz_complex_t shz_cpolarf(float r, float theta) SHZ_NOEXCEPT;

//! Checks for equality between two complex numbers with either an absolute or relative tolerance.
SHZ_INLINE bool shz_cequalf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

//! Returns the real component of a complex number.
SHZ_INLINE float shz_crealf(shz_complex_t c) SHZ_NOEXCEPT;

//! Returns the imaginary component of a complex number.
SHZ_INLINE float shz_cimagf(shz_complex_t c) SHZ_NOEXCEPT;

//! @}

/*! \name  Arithmetic
    \brief Arithmetic operations on complex numbers.
    @{
*/

//! Adds two complex numbers together and returns the result.
SHZ_INLINE shz_complex_t shz_caddf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

//! Subtracts \p rhs from \p lhs and returns the complex result.
SHZ_INLINE shz_complex_t shz_csubf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

//! Multiplies two complex numbers together, returning the complex result.
SHZ_INLINE shz_complex_t shz_cmulf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

//! Divides \p lhs by \p rhs, returning the complex result.
SHZ_INLINE shz_complex_t shz_cdivf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

//! Scales both componets of the complex number, \p c, by the value, \p v, returning the result.
SHZ_INLINE shz_complex_t shz_cscalef(shz_complex_t c, float v) SHZ_NOEXCEPT;

//! Returns the multiplicative inverse or reciprocal of the complex number, \p c.
SHZ_INLINE shz_complex_t shz_crecipf(shz_complex_t c) SHZ_NOEXCEPT;

//! @}

/*! \name  Manipulation
    \brief Extract or modify complex components.
    @{
*/

//! Returns the absolute value (or magnitude) of the complex number, \p c.
SHZ_INLINE float shz_cabsf(shz_complex_t c) SHZ_NOEXCEPT;

//! Returns the inverse of the absolute value (or inverse of the magnitude) of the complex number, \p c.
SHZ_INLINE float shz_inv_cabsf(shz_complex_t c) SHZ_NOEXCEPT;

//! Returns the squared magnitude of the complex number, \p c.
SHZ_INLINE float shz_cnormf(shz_complex_t c) SHZ_NOEXCEPT;

//! Returns the phase angle of the complex number, \p c.
SHZ_INLINE float shz_cargf(shz_complex_t c) SHZ_NOEXCEPT;

//! Returns the complex conjugate of \p c.
SHZ_INLINE shz_complex_t shz_conjf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates the projection of the complex number, \p c, onto the Riemann sphere.
SHZ_INLINE shz_complex_t shz_cprojf(shz_complex_t c) SHZ_NOEXCEPT;

//! @}

/*! \name  Transcendental
    \brief Complex transcendental functions.
    @{
*/

//! Computes and returns the complex square root of \p c.
SHZ_INLINE shz_complex_t shz_csqrtf(shz_complex_t c) SHZ_NOEXCEPT;

//! Raises the complex \p base to the complex \p exp power, returning a complex result.
SHZ_INLINE shz_complex_t shz_cpowf(shz_complex_t base, shz_complex_t exp) SHZ_NOEXCEPT;

//! Returns the complex base `e` exponential of \p c.
SHZ_INLINE shz_complex_t shz_cexpf(shz_complex_t c) SHZ_NOEXCEPT;

//! Computes and returns the complex natural logarithm of \p c.
SHZ_INLINE shz_complex_t shz_clogf(shz_complex_t c) SHZ_NOEXCEPT;

//! Computes and returns the complex base 10 logarithm of \p c.
SHZ_INLINE shz_complex_t shz_clog10f(shz_complex_t c) SHZ_NOEXCEPT;

//! @}

/*! \name  Spherical Trigonometry
    \brief Complex spherical trigonometric functions.
    @{   
*/

//! Calculates and returns the complex sine of \p c.
SHZ_INLINE shz_complex_t shz_csinf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex cosine of \p c.
SHZ_INLINE shz_complex_t shz_ccosf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex tangent of \p c.
SHZ_INLINE shz_complex_t shz_ctanf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex cosecant of \p c.
SHZ_INLINE shz_complex_t shz_ccscf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex secant of \p c.
SHZ_INLINE shz_complex_t shz_csecf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex cotangent of \p c.
SHZ_INLINE shz_complex_t shz_ccotf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex arcsine of \p c.
SHZ_INLINE shz_complex_t shz_casinf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex arccosine of \p c.
SHZ_INLINE shz_complex_t shz_cacosf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex arctangent of \p c.
SHZ_INLINE shz_complex_t shz_catanf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex arccosecant of \p c.
SHZ_INLINE shz_complex_t shz_cacscf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex arcsecant of \p c.
SHZ_INLINE shz_complex_t shz_casecf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex arccotangent of \p c.
SHZ_INLINE shz_complex_t shz_cacotf(shz_complex_t c) SHZ_NOEXCEPT;

//! @}

/*! \name  Hyperbolic Trigonometry
    \brief Complex hyperbolic trigonometric functions.
    @{
*/

//! Calculates and returns the complex hyperbolic sine of \p c.
SHZ_INLINE shz_complex_t shz_csinhf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic cosine of \p c.
SHZ_INLINE shz_complex_t shz_ccoshf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic tangent of \p c.
SHZ_INLINE shz_complex_t shz_ctanhf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic cosecant of \p c.
SHZ_INLINE shz_complex_t shz_ccschf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic secant of \p c.
SHZ_INLINE shz_complex_t shz_csechf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic cotangent of \p c.
SHZ_INLINE shz_complex_t shz_ccothf(shz_complex_t C) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic arcsine of \p c.
SHZ_INLINE shz_complex_t shz_casinhf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic arccosine of \p c.
SHZ_INLINE shz_complex_t shz_cacoshf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic arctangent of \p c.
SHZ_INLINE shz_complex_t shz_catanhf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic arccosecant of \p c.
SHZ_INLINE shz_complex_t shz_cacschf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic arcsecant of \p c.
SHZ_INLINE shz_complex_t shz_casechf(shz_complex_t c) SHZ_NOEXCEPT;

//! Calculates and returns the complex hyperbolic arccotangent of \p c.
SHZ_INLINE shz_complex_t shz_cacothf(shz_complex_t C) SHZ_NOEXCEPT;

//! @}

/*! \name  Signal Processing
    \brief Functions for processing complex signals.
    @{
*/

/*! Fast Fourier Transform

    Applies a fast fourier transform to convert the complex array
    of samples, \p s, of the given \p size, from the time to the
    frequency domain. The conversion is done in-place.

    \note
    The underlying implementation uses the Radix2 variant of the
    Cooley-Tukey FFT algorithm, using an XMTRX-accelerated 2 point
    butterfly DIF.

    \warning \p size must be a power-of-two!
    \warning This routine clobbers `XMTRX`!
*/
void shz_fft(shz_complex_t* s, size_t size) SHZ_NOEXCEPT;

//! @}

#include "inline/shz_complex.inl.h"

SHZ_DECLS_END

#endif