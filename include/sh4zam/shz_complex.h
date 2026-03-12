/*! \file
    \brief Complex number API
    \ingroup complex

    This file contains a collection of routines for working with complex
    (real + imaginary) numbers. The API is mostly modeled after C99's
    <complex.h>, although it also supports being used from C++ as well.

    \author     2026 Falco Girgis
    \copyright  MIT License
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

//! @}

/*! \name  Manipulation
    \brief Extract or modify complex components.
    @{
*/

//! Returns the absolute value (or magnitude) of the complex number, \p c.
SHZ_INLINE float shz_cabsf(shz_complex_t c) SHZ_NOEXCEPT;

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

/*! \name  Trigonometry
    \brief Complex trigonometric routines.
    @{   
*/

//! Calculates and returns the complex sine of \p c.
SHZ_INLINE shz_complex_t shz_csinf(shz_complex_t c) SHZ_NOEXCEPT;
/* WIPpity McWIP
SHZ_INLINE shz_complex_t shz_ccosf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_ctanf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_casinf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_cacosf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_catanf(shz_complex_t c) SHZ_NOEXCEPT;
*/ 
// hyperbolic equivalents

//! @}

// to/from 2D matrices

//! Applies a fast fourier transform to convert the array \p s, of the given \p size, from the time to the frequency domain.
void shz_fft(shz_complex_t* s, size_t size) SHZ_NOEXCEPT;

#include "inline/shz_complex.inl.h"

SHZ_DECLS_END

#endif