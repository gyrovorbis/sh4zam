/*! \file
    \brief Complex number API
    \ingroup complex

    This file contains a collection of routines for working
    with complex (real + imaginary) numbers. The API is mostly
    modeled after C99's <complex.h>, although it also supports
    being used from C++ as well.

    \author 2026 Falco Girgis

    \copyright MIT License
*/

#ifndef SHZ_COMPLEX_H
#define SHZ_COMPLEX_H

#include "shz_cdefs.h"

#define SHZ_CMPLXF(x, y) 	shz_cinitf(x, y)
#define SHZ_CMPLXF32(x, y) 	SHZ_CMPLXF(x, y)
#define SHZ_I		        SHZ_CMPLXF32(0.0f, 1.0f)

SHZ_DECLS_BEGIN

typedef struct shz_complex {
    float real;
    float imag;
} shz_complex_t;

typedef shz_complex_t shz_complex;

SHZ_INLINE shz_complex_t shz_cinitf(float real, float imag) SHZ_NOEXCEPT;

SHZ_INLINE bool shz_cequalf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

SHZ_INLINE shz_complex_t shz_caddf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

SHZ_INLINE shz_complex_t shz_csubf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

SHZ_INLINE shz_complex_t shz_cmulf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

SHZ_INLINE shz_complex_t shz_cdivf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT;

SHZ_INLINE float shz_crealf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE float shz_cimagf(shz_complex_t c) SHZ_NOEXCEPT;

SHZ_INLINE float shz_cabsf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE float shz_cargf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_conjf(shz_complex_t c) SHZ_NOEXCEPT;

SHZ_INLINE shz_complex_t shz_csqrtf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_cpowf(shz_complex_t base, shz_complex_t exp) SHZ_NOEXCEPT;

SHZ_INLINE shz_complex_t shz_clogf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_cexpf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_clog10f(shz_complex_t c) SHZ_NOEXCEPT;

// shz_sincos_t compat
SHZ_INLINE shz_complex_t shz_csinf(shz_complex_t c) SHZ_NOEXCEPT;
/* WIPpity McWIP
SHZ_INLINE shz_complex_t shz_ccosf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_ctanf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_casinf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_cacosf(shz_complex_t c) SHZ_NOEXCEPT;
SHZ_INLINE shz_complex_t shz_catanf(shz_complex_t c) SHZ_NOEXCEPT;
*/ 
// hyperbolic equivalents

// to/from 2D matrices

// FFT-needed shit
void shz_fft(shz_complex_t* s, size_t size) SHZ_NOEXCEPT;

#include "inline/shz_complex.inl.h"

SHZ_DECLS_END

#endif