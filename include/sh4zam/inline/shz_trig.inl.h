//! \cond INTERNAL
/*! \file
 *  \brief   Trigonometry API Implementation.
 *  \ingroup trig
 *
 *  This file provides providest the inlined function implementations for the
 *  trigonometry API, providing constant expression evaluation then forwarding
 *  on the function call to the routines provided by a particular back-end.
 *
 *  \author 2025, 2026 Falco Girgis
 *  \author 2025 Paul Cercueil
 *
 *  \copyright MIT License
 */

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_trig_sh4.inl.h"
#else
#   include "sw/shz_trig_sw.inl.h"
#endif

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16(uint16_t radians16) SHZ_NOEXCEPT {
    if(__builtin_constant_p(radians16))
        return shz_sincosf((float)radians16 * (2.0f * SHZ_F_PI / (float)UINT16_MAX));

#if SHZ_BACKEND == SHZ_SH4
    return shz_sincosu16_sh4(radians16);
#else
    return shz_sincosu16_sw(radians16);
#endif
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) SHZ_NOEXCEPT {
    if(__builtin_constant_p(radians))
        return (shz_sincos_t) { __builtin_sinf(radians), __builtin_cosf(radians) };

#if SHZ_BACKEND == SHZ_SH4
    return shz_sincosf_sh4(radians);
#else
    return shz_sincosf_sw(radians);
#endif
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) SHZ_NOEXCEPT {
    if(__builtin_constant_p(degrees))
        return (shz_sincos_t) { __builtin_sinf(SHZ_DEG_TO_RAD(degrees)), __builtin_cosf(SHZ_DEG_TO_RAD(degrees)) };

#if SHZ_BACKEND == SHZ_SH4
    return shz_sincosf_deg_sh4(degrees);
#else
    return shz_sincosf_deg_sw(degrees);
#endif
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
    if(__builtin_constant_p(radians))
        return __builtin_sinf(radians);

    return shz_sincosf(radians).sin;
}

SHZ_FORCE_INLINE float shz_sinf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).sin;
}

SHZ_FORCE_INLINE float shz_cosf(float radians) SHZ_NOEXCEPT {
    if(__builtin_constant_p(radians))
        return __builtin_cosf(radians);

    return shz_sincosf(radians).cos;
}

SHZ_FORCE_INLINE float shz_cosf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).cos;
}

SHZ_FORCE_INLINE float shz_tanf(float radians) SHZ_NOEXCEPT {
    if(__builtin_constant_p(radians))
        return __builtin_tanf(radians);

    return shz_sincos_tanf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_tanf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_tanf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_secf(float radians) SHZ_NOEXCEPT {
    if(__builtin_constant_p(radians))
        return 1.0f / __builtin_cosf(radians);

    return shz_sincos_secf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_secf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_secf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cscf(float radians) SHZ_NOEXCEPT {
    if(__builtin_constant_p(radians))
        return 1.0f / __builtin_sinf(radians);

    return shz_sincos_cscf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_cscf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_cscf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cotf(float radians) SHZ_NOEXCEPT {
    if(__builtin_constant_p(radians))
        return 1.0f / __builtin_tanf(radians);

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
    if(__builtin_constant_p(y) && __builtin_constant_p(x))
        return __builtin_atan2f(y, x);

    float angle = SHZ_F_PI_2;
    float r = x;
    float abs_sum = shz_fabsf(y);

    if(x <= 0.0f) {
        if(SHZ_UNLIKELY(x == 0.0f && y == 0.0f))
            return 0.0f;

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
    if(__builtin_constant_p(x))
        return __builtin_asinf(x);

return shz_atanf(x * shz_inv_sqrtf_fsrra(1.0f - (x * x)));
}

SHZ_INLINE float shz_acosf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_acosf(x);

    return SHZ_F_PI_2 - shz_asinf(x);
}

SHZ_INLINE float shz_asecf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_acosf(1.0f / x);

    return shz_acosf(shz_invf(x));
}

SHZ_INLINE float shz_acscf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_asinf(1.0f / x);

    return shz_asinf(shz_invf(x));
}

SHZ_INLINE float shz_acotf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_atanf(1.0f / x);

    return shz_atanf(shz_invf(x));
}

SHZ_FORCE_INLINE float shz_sinhf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_sinhf(x);

    return (shz_expf(x) - shz_expf(-x)) * 0.5f;
}

SHZ_FORCE_INLINE float shz_coshf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_coshf(x);

    return (shz_expf(x) + shz_expf(-x)) * 0.5f;
}

SHZ_FORCE_INLINE float shz_tanhf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_tanhf(x);

    float ex = shz_expf(x);     // e^x
    float enx = shz_expf(-x);   // e^-x
    return shz_divf_fsrra(ex - enx, ex + enx);
}

SHZ_FORCE_INLINE float shz_cschf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return 1.0f / __builtin_sinhf(x);

    return shz_divf(2.0f, shz_expf(x) - shz_expf(-x));
}

SHZ_FORCE_INLINE float shz_sechf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return 1.0f / __builtin_coshf(x);

    return shz_divf_fsrra(2.0f, shz_expf(x) + shz_expf(-x));
}

SHZ_FORCE_INLINE float shz_cothf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return 1.0f / __builtin_tanhf(x);

    float ex = shz_expf(x);
    float e_neg_x = shz_expf(-x);

    float sinh_val = ex - e_neg_x;
    float cosh_val = ex + e_neg_x;

    return shz_divf(cosh_val, sinh_val);
}

SHZ_FORCE_INLINE float shz_asinhf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_asinhf(x);

    return shz_logf(x + shz_sqrtf_fsrra(x * x + 1.0f));
}

SHZ_FORCE_INLINE float shz_acoshf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_acoshf(x);

    return shz_logf(x + shz_sqrtf(x * x - 1.0f));
}

SHZ_FORCE_INLINE float shz_atanhf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_atanhf(x);

    return 0.5f * shz_logf(shz_divf(1.0f + x, 1.0f - x));
}

SHZ_FORCE_INLINE float shz_acschf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_asinhf(1.0f / x);

    float inv_x = shz_invf(x);
    return shz_logf(inv_x + shz_sqrtf_fsrra(inv_x * inv_x + 1.0f));
}

SHZ_FORCE_INLINE float shz_asechf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_acoshf(1.0f / x);

    return shz_logf(shz_divf(1.0f + shz_sqrtf(1.0f - (x * x)), x));
}

SHZ_FORCE_INLINE float shz_acothf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_atanhf(1.0f / x);

    return 0.5f * shz_logf(shz_divf(x + 1.0f, x - 1.0f));
}

 //! \endcond
