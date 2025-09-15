//! \cond INTERNAL
/*! \file
 *  \brief   Trigonometry API Implementation.
 *  \ingroup trig
 *
 *  This file provides providest the inlined function implementations for the
 *  trigonometry API.
 *
 *  \author Falco Girgis
 *  \author Paul Cercueil
 *
 *  \copyright MIT License
 */

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16(uint16_t radians16) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE float shz_sincos_tanf(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_divf(sincos.sin, sincos.cos);
}

SHZ_FORCE_INLINE float shz_sinf(float radians) SHZ_NOEXCEPT {
    return shz_sincosf(radians).sin;
}

SHZ_FORCE_INLINE float shz_sinf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).sin;
}

SHZ_FORCE_INLINE float shz_cosf(float radians) SHZ_NOEXCEPT {
    return shz_sincosf(radians).cos;
}

SHZ_FORCE_INLINE float shz_cosf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).cos;
}

SHZ_FORCE_INLINE float shz_tanf(float radians) SHZ_NOEXCEPT {
    return shz_sincos_tanf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_tanf_deg(float degrees) SHZ_NOEXCEPT {
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

 //! \endcond
