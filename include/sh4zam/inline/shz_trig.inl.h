//! \cond INTERNAL
/*! \file
 *  \brief   Trigonometry API Implementation.
 *  \ingroup trig
 *
 *  This file provides providest the inlined function implementations for the
 *  trigonometry API.
 *
 *  \author 2025 Falco Girgis
 *  \author 2025 Paul Cercueil
 *
 *  \copyright MIT License
 */

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16(uint16_t radians16) SHZ_NOEXCEPT {
    register float rsin asm("fr8");
    register float rcos asm("fr9");

    asm(R"(
            lds  %2, fpul
            fsca fpul, dr8
        )"
        : "=f" (rsin), "=f" (rcos)
        : "r" (radians16)
        : "fpul");

    return (shz_sincos_t){ rsin, rcos };
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) SHZ_NOEXCEPT {
#ifdef __FAST_MATH__
    float rsin, rcos;

    __builtin_sincosf(radians, &rsin, &rcos);

#else
    register float rsin asm("fr8");
    register float rcos asm("fr9");

    radians *= SHZ_FSCA_RAD_FACTOR;

    asm(R"(
        ftrc  %2, fpul
        fsca  fpul, dr8
    )"
    : "=f" (rsin), "=f" (rcos)
    : "f" (radians)
    : "fpul");
#endif

    return (shz_sincos_t){ rsin, rcos };
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) SHZ_NOEXCEPT {
#ifdef __FAST_MATH__
    float rsin, rcos;

    __builtin_sincosf(SHZ_DEG_TO_RAD(degrees), &rsin, &rcos);
#else
    register float rsin asm("fr8");
    register float rcos asm("fr9");

    degrees *= SHZ_FSCA_DEG_FACTOR;

    asm(R"(
        ftrc  %2, fpul
        fsca  fpul, dr8
    )"
    : "=&f" (rsin), "=&f" (rcos)
    : "f" (degrees)
    : "fpul");
#endif

    return (shz_sincos_t){ rsin, rcos };
}

SHZ_FORCE_INLINE float shz_sincos_tanf(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_divf(sincos.sin, sincos.cos);
}

SHZ_FORCE_INLINE float shz_sincos_secf(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_invf(sincos.cos);
}

SHZ_FORCE_INLINE float shz_sincos_cscf(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_invf(sincos.sin);
}

SHZ_FORCE_INLINE float shz_sincos_cotf(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_divf(sincos.cos, sincos.sin);
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

SHZ_FORCE_INLINE float shz_secf(float radians) SHZ_NOEXCEPT {
    return shz_sincos_secf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_secf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_secf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cscf(float radians) SHZ_NOEXCEPT {
    return shz_sincos_cscf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_cscf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_cscf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cotf(float radians) SHZ_NOEXCEPT {
    return shz_sincos_cotf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_cotf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_cotf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_atanf_unit(float x) SHZ_NOEXCEPT {
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;

    return shz_fmaf(n2, x * x, n1) * x;
}

SHZ_INLINE float shz_atanf_q1(float x) SHZ_NOEXCEPT {
    return SHZ_F_PI_2 - shz_atanf_unit(shz_invf_fsrra(x));
}

SHZ_INLINE float shz_atanf(float x) SHZ_NOEXCEPT {
    if(x > 1.0f)
	    return shz_atanf_q1(x);
    else if(x < -1.0f)
        return -shz_atanf_q1(x);
    else
        return shz_atanf_unit(x);
}

SHZ_INLINE float shz_atan2f(float y, float x) SHZ_NOEXCEPT {
    if(SHZ_UNLIKELY(y == 0.0f && x == 0.0f))
        return 0.0f;

    float angle = SHZ_F_PI_2;
    float r = x;
    float abs_sum = fabsf(y);

    if(x < 0.0f) {
        angle += SHZ_F_PI_4;
        r += abs_sum;
        abs_sum -= x;
    } else {
        angle -= SHZ_F_PI_4;
        r -= abs_sum;
        abs_sum += x;
    }

    r *= shz_invf_fsrra(abs_sum);
    angle += shz_fmaf(0.1963f, r * r, -0.9817f) * r;

    return shz_copysignf(angle, y);
}


SHZ_INLINE float shz_asinf(float x) SHZ_NOEXCEPT {
    return shz_atanf(x * shz_inv_sqrtf_fsrra(1.0f - (x * x)));
}

SHZ_INLINE float shz_acosf(float x) SHZ_NOEXCEPT {
    return SHZ_F_PI_2 - shz_asinf(x);
}

SHZ_INLINE float shz_asecf(float x) SHZ_NOEXCEPT {
    return shz_acosf(shz_invf(x));
}

SHZ_INLINE float shz_acscf(float x) SHZ_NOEXCEPT {
    return shz_asinf(shz_invf(x));
}

SHZ_INLINE float shz_acotf(float x) SHZ_NOEXCEPT {
    return shz_atanf(shz_invf(x));
}

 //! \endcond
