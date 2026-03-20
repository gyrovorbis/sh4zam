/*! \file
    \brief Complex number C++ API
    \ingroup complex

    This file contains the C++ layer for wrapping the Complex
    Number C API. The major thing that it adds in C++ is convenient
    overloaded operators for performing arithmetic on complex numbers
    using the regular arithmetic operators, rather than having to make
    function calls which implement them, as in C.

    \author     2026 Falco Girgis
    \copyright  MIT License

    \todo
        - custom literals
        - array index accessors?
        - iterator support?
*/

#ifndef SHZ_COMPLEX_HPP
#define SHZ_COMPLEX_HPP

#include "shz_complex.h"

namespace shz {

    //! C++ wrapper around a floating-point complex number, real/imaginary pair.
    struct complex: public shz_complex_t {

        /*! \name  Constructors
            \brief Members for initializing and constructing.
            @{
        */

        // Default constructor.
        complex() noexcept = default;

        //! Value constructor, sets the imaginary component to `0` if none is supplied.
        SHZ_FORCE_INLINE complex(float re, float im = 0.0f) noexcept:
            shz_complex_t({ re, im }) {}

        //! Converting constructor for initializing from the C base type.
        SHZ_FORCE_INLINE complex(shz_complex_t cplx) noexcept:
            complex(cplx.real, cplx.imag) {}

        //! @}

        /*! \name  Member Operators
            \brief Overloaded operators defined as member functions.
            @{
        */

        //! Overloaded assignment operator for assining to the C base type.
        SHZ_FORCE_INLINE complex& operator=(shz_complex_t rhs) noexcept {
            real = rhs.real;
            imag = rhs.imag;
            return *this;
        }

        //! Adds and accumulates \p rhs onto the given complex number.
        SHZ_FORCE_INLINE complex& operator+=(shz_complex_t rhs) noexcept {
            return (*this = shz_caddf(*this, rhs));
        }

        //! Subtracts \p rhs from the given complex number, assigning it to the result.
        SHZ_FORCE_INLINE complex& operator-=(shz_complex_t rhs) noexcept {
            return (*this = shz_csubf(*this, rhs));
        }

        //! Multiplies and accumulates \p rhs by the given complex number.
        SHZ_FORCE_INLINE complex& operator*=(shz_complex_t rhs) noexcept {
            return (*this = shz_cmulf(*this, rhs));
        }

        //! Multiplies and accumulates the given complex number by \p scalar.
        SHZ_FORCE_INLINE complex& operator*=(float scale) noexcept {
            return (*this = shz_cscalef(*this, scale));
        }

        //! Divides the given complex number by \p rhs, assigning it to the result.
        SHZ_FORCE_INLINE complex& operator/=(shz_complex_t rhs) noexcept {
            return (*this = shz_cdivf(*this, rhs));
        }

        //! @}
    };

    //! POSIX-style C++ alias, for those who dig that kind of type name.
    using complex_t = complex;

    /*! \name  Global Operators
        \brief Globally-defined overloaded operators.
        @{
    */

    //! Adds the two complex numbers, \p lhs and \p rhs, returning the result.
    SHZ_FORCE_INLINE complex operator+(complex lhs, complex rhs) noexcept {
        return shz_caddf(lhs, rhs);
    }

    //! Subtracts \p rhs from \p lhs, returning the complex result.
    SHZ_FORCE_INLINE complex operator-(complex lhs, complex rhs) noexcept {
        return shz_csubf(lhs, rhs);
    }

    //! Multiplies \p lhs by \p rhs, returning the complex result.
    SHZ_FORCE_INLINE complex operator*(complex lhs, complex rhs) noexcept {
        return shz_cmulf(lhs, rhs);
    }

    //! Multiplies \p lhs by a complex number with the real component given as \p rhs and no imaginary component.
    SHZ_FORCE_INLINE complex operator*(complex lhs, float rhs) noexcept {
        return shz_cscalef(lhs, rhs);
    }

    //! Divides \p lhs by \p rhs, returning the complex result.
    SHZ_FORCE_INLINE complex operator/(complex lhs, complex rhs) noexcept {
        return shz_cdivf(lhs, rhs);
    }

    //! Divides \p lhs by a complex number with the real compoonent given as \p rhs and no imaginary component.
    SHZ_FORCE_INLINE complex operator/(complex lhs, float rhs) noexcept {
        return shz_cscalef(lhs, shz_invf(rhs));
    }

    //! Returns true if the two complex numbers are approximately equal.
    SHZ_FORCE_INLINE bool operator==(complex lhs, complex rhs) noexcept {
        return shz_cequalf(lhs, rhs);
    }

    //! Returns true if the two complex numbers are approximately inequal.
    SHZ_FORCE_INLINE bool operator!=(complex lhs, complex rhs) noexcept {
        return !(lhs == rhs);
    }

    //! @}

    /*! \name  Basic
        \brief Basic operations on complex numbers.
        @{
    */

    //! Returns a new complex number with the given components.
    SHZ_FORCE_INLINE complex cinitf(float real, float imag) noexcept {
        return shz_cinitf(real, imag);
    }

    //! Converts the given polar coordinates into a complex number.
    SHZ_FORCE_INLINE complex cpolarf(float r, float theta) noexcept {
        return shz_cpolarf(r, theta);
    }

    //! Checks for relative equality between \p lhs and \p rhs.
    SHZ_FORCE_INLINE bool cequalf(complex lhs, complex rhs) noexcept {
        return shz_cequalf(lhs, rhs);
    }

    //! Returns the real component of the given complex number.
    SHZ_FORCE_INLINE float crealf(complex c) noexcept {
        return shz_crealf(c);
    }

    //! Returns the imaginary component of the given complex number.
    SHZ_FORCE_INLINE float cimagf(complex c) noexcept {
        return shz_cimagf(c);
    }

    //! @}

    /*! \name  Arithmetic
        \brief Arithmetic operations on complex numbers.
        @{
    */

    //! Adds the two complex numbers together, returning the complex result.
    SHZ_FORCE_INLINE complex caddf(complex lhs, complex rhs) noexcept {
        return shz_caddf(lhs, rhs);
    }

    //! Subtracts \p rhs from \p lhs, returning the complex result.
    SHZ_FORCE_INLINE complex csubf(complex lhs, complex rhs) noexcept {
        return shz_csubf(lhs, rhs);
    }

    //! Multiplies the two complex numbers together, returning the complex result.
    SHZ_FORCE_INLINE complex cmulf(complex lhs, complex rhs) noexcept {
        return shz_cmulf(lhs, rhs);
    }

    //! Divides \p lhs by \p rhs, returning the complex result.
    SHZ_FORCE_INLINE complex cdivf(complex lhs, complex rhs) noexcept {
        return shz_cdivf(lhs, rhs);
    }

    //! Multiplies \p lhs by the complex number created with \p v as its real component's value and no imaginary component value.
    SHZ_FORCE_INLINE complex cscalef(complex lhs, float v) noexcept {
        return shz_cscalef(lhs, v);
    }

    //! Returns the multiplicative reciprocal of the given complex number.
    SHZ_FORCE_INLINE complex crecipf(complex c) noexcept {
        return shz_crecipf(c);
    }

    //! @}

    /*! \name  Manipulation
        \brief Extract or modify complex components.
        @{
    */

    //! Returns the absolute value or magnitude of the given complex number.
    SHZ_FORCE_INLINE float cabsf(complex c) noexcept {
        return shz_cabsf(c);
    }

    //! Returns the inverse absolute value or magnitude of the given complex number.
    SHZ_FORCE_INLINE float inv_cabsf(complex c) noexcept {
        return shz_inv_cabsf(c);
    }

    //! Returns the squared magnitude of the given complex number.
    SHZ_FORCE_INLINE float cnormf(complex c) noexcept {
        return shz_cnormf(c);
    }

    //! Returns the phase angle of the given complex number.
    SHZ_FORCE_INLINE float cargf(complex c) noexcept {
        return shz_cargf(c);
    }

    //! Returns the complex conjugate of the given complex number.
    SHZ_FORCE_INLINE complex conjf(complex c) noexcept {
        return shz_conjf(c);
    }

    //! Returns the projection of the complex number onto the Riemann sphere.
    SHZ_FORCE_INLINE complex cprojf(complex c) noexcept {
        return shz_cprojf(c);
    }

    //! @}

    /*! \name  Transcendental
        \brief Complex transcendental functions.
        @{
    */

    //! Returns the complex square root of the given complex number.
    SHZ_FORCE_INLINE complex csqrtf(complex c) noexcept {
        return shz_csqrtf(c);
    }

    //! Raises a complex \p base to a complex power given by \p exp, returning a complex result.
    SHZ_FORCE_INLINE complex cpowf(complex base, complex exp) noexcept {
        return shz_cpowf(base, exp);
    }

    //! Returns the complex base `e` exponential of the given complex number.
    SHZ_FORCE_INLINE complex cexpf(complex c) noexcept {
        return shz_cexpf(c);
    }

    //! returns the complex natural logarithm of the given complex number.
    SHZ_FORCE_INLINE complex clogf(complex c) noexcept {
        return shz_clogf(c);
    }

    //! Returns the complex base 10 logarithm of the given complex number.
    SHZ_FORCE_INLINE complex clog10f(complex c) noexcept {
        return shz_clog10f(c);
    }

    //! @}

    /*! \name  Spherical Trigonometry
        \brief Complex spherical trigonometric functions.
        @{
    */

    //! Returns the sine of the given complex number.
    SHZ_FORCE_INLINE complex csinf(complex c) noexcept {
        return shz_csinf(c);
    }

    //! Returns the cosine of the given complex number.
    SHZ_FORCE_INLINE complex ccosf(complex c) noexcept {
        return shz_ccosf(c);
    }

    //! Returns the tangent of the given complex number.
    SHZ_FORCE_INLINE complex ctanf(complex c) noexcept {
        return shz_ctanf(c);
    }

    //! Returns the cosecant of the given complex number.
    SHZ_FORCE_INLINE complex ccscf(complex c) noexcept {
        return shz_ccscf(c);
    }

    //! Returns the secant of the given complex number.
    SHZ_FORCE_INLINE complex csecf(complex c) noexcept {
        return shz_csecf(c);
    }

    //! Returns the cotangent of the given complex number.
    SHZ_FORCE_INLINE complex ccotf(complex c) noexcept {
        return shz_ccotf(c);
    }

    //! Returns the arcsine of the given complex number.
    SHZ_FORCE_INLINE complex casinf(complex c) noexcept {
        return shz_casinf(c);
    }

    //! Returns the arccosine of the given complex number.
    SHZ_FORCE_INLINE complex cacosf(complex c) noexcept {
        return shz_cacosf(c);
    }

    //! Returns the arctangent of the given complex number.
    SHZ_FORCE_INLINE complex catanf(complex c) noexcept {
        return shz_catanf(c);
    }

    //! Returns the arccosecant of the given complex number.
    SHZ_FORCE_INLINE complex cacscf(complex c) noexcept {
        return shz_cacscf(c);
    }

    //! Returns the arcsecant of the given complex number.
    SHZ_FORCE_INLINE complex casecf(complex c) noexcept {
        return shz_casecf(c);
    }

    //! Returns the arccotangent of the given complex number.
    SHZ_FORCE_INLINE complex cacotf(complex c) noexcept {
        return shz_cacotf(c);
    }

    //! @}

    /*! \name  Hyperbolic Trigonometry
        \brief Complex hyperbolic trigonometric functions.
        @{
    */

    //! Returns the hyperbolic sine of the given complex number.
    SHZ_FORCE_INLINE complex csinhf(complex c) noexcept {
        return shz_csinhf(c);
    }

    //! Returns the hyperbolic cosine of the given complex number.
    SHZ_FORCE_INLINE complex ccoshf(complex c) noexcept {
        return shz_ccoshf(c);
    }

    //! Returns the hyperbolic tangent of the given complex number.
    SHZ_FORCE_INLINE complex ctanhf(complex c) noexcept {
        return shz_ctanhf(c);
    }

    //! Returns the hyperbolic cosecant of the given complex number.
    SHZ_FORCE_INLINE complex ccschf(complex c) noexcept {
        return shz_ccschf(c);
    }

    //! Returns the hyperbolic secant of the given complex number.
    SHZ_FORCE_INLINE complex csechf(complex c) noexcept {
        return shz_csechf(c);
    }

    //! Returns the hyperbolic cotangent of the given complex number.
    SHZ_FORCE_INLINE complex ccothf(complex c) noexcept {
        return shz_ccothf(c);
    }

    //! Returns the hyperbolic arcsine of the given complex number.
    SHZ_FORCE_INLINE complex casinhf(complex c) noexcept {
        return shz_casinhf(c);
    }

    //! Returns the hyperbolic arccosine of the given complex number.
    SHZ_FORCE_INLINE complex cacoshf(complex c) noexcept {
        return shz_cacoshf(c);
    }

    //! Returns the hyperbolic arctangent of the given complex number.
    SHZ_FORCE_INLINE complex catanhf(complex c) noexcept {
        return shz_catanhf(c);
    }

    //! Returns the hyperbolic arccosecant of the given complex number.
    SHZ_FORCE_INLINE complex cacschf(complex c) noexcept {
        return shz_cacschf(c);
    }

    //! Returns the hyperbolic arcsecant of the given complex number.
    SHZ_FORCE_INLINE complex casechf(complex c) noexcept {
        return shz_casechf(c);
    }

    //! Returns the hyperbolic arccotangent of the given complex number.
    SHZ_FORCE_INLINE complex cacothf(complex c) noexcept {
        return shz_cacothf(c);
    }

    //! @}

    /*! \name  Signal Processing
        \brief Functions for processing complex signals.
        @{
    */

    //! C++ wrapper around shz_fft(), which computes the FFT of the given buffer. \sa shz_fft()
    SHZ_FORCE_INLINE void fft(shz_complex_t* s, size_t size) noexcept {
        return shz_fft(s, size);
    }

    //! @}
}

#endif
