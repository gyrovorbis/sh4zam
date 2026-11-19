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
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_trig_sh4.inl.h"
#else
#   include "sw/shz_trig_sw.inl.h"
#endif

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16(uint16_t radians16) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians16))
        return shz_sincosf((float)radians16 * (SHZ_F_TAU / (float)(UINT16_MAX + 1)));
#endif

#if SHZ_BACKEND == SHZ_SH4
    return shz_sincosu16_sh4(radians16);
#else
    return shz_sincosu16_sw(radians16);
#endif
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians))
        return SHZ_INIT(shz_sincos_t, __builtin_sinf(radians), __builtin_cosf(radians));
#endif

#if SHZ_BACKEND == SHZ_SH4
    return shz_sincosf_sh4(radians);
#else
    return shz_sincosf_sw(radians);
#endif
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(degrees))
        return SHZ_INIT(shz_sincos_t, __builtin_sinf(SHZ_DEG_TO_RAD(degrees)), __builtin_cosf(SHZ_DEG_TO_RAD(degrees)));
#endif

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
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians))
        return __builtin_sinf(radians);
#endif
    return shz_sincosf(radians).sin;
}

SHZ_FORCE_INLINE float shz_sinf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).sin;
}

SHZ_FORCE_INLINE float shz_cosf(float radians) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians))
        return __builtin_cosf(radians);
#endif
    return shz_sincosf(radians).cos;
}

SHZ_FORCE_INLINE float shz_cosf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).cos;
}

SHZ_FORCE_INLINE float shz_tanf(float radians) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians))
        return __builtin_tanf(radians);
#endif
    return shz_sincos_tanf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_tanf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_tanf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_secf(float radians) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians))
        return 1.0f / __builtin_cosf(radians);
#endif
    return shz_sincos_secf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_secf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_secf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cscf(float radians) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians))
        return 1.0f / __builtin_sinf(radians);
#endif
    return shz_sincos_cscf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_cscf_deg(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_cscf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cotf(float radians) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(radians))
        return 1.0f / __builtin_tanf(radians);
#endif
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
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_atanf(x);
#endif
    if(x > 1.0f)
	    return shz_atanf_q1(x);
    else if(x < -1.0f)
        return -shz_atanf_q1(x);
    else
        return shz_atanf_unit(x);
}

SHZ_INLINE float shz_atan2f(float y, float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(y) && __builtin_constant_p(x))
        return __builtin_atan2f(y, x);
#endif
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

    /* atan(r) = r * P(r^2) over [-1, 1]. Every P is the minimax fit under
       P(1) = pi/4, which keeps the error at zero on the axes (r = +-1). The
       longer ones are evaluated in pairs so that their terms can overlap. */
    const float r2 = r * r;
#if SHZ_ATAN2F_TERMS == 2
    const float p  = shz_fmaf(r2, -0.182084382f, 0.967482567f);
#elif SHZ_ATAN2F_TERMS == 5
    const float r4 = r2 * r2;
    const float p  = shz_fmaf(r4, shz_fmaf(r4, 0.0202457625f,
                                           shz_fmaf(r2, -0.0839662105f, 0.179388285f)),
                              shz_fmaf(r2, -0.330125183f, 0.999855518f));
#elif SHZ_ATAN2F_TERMS == 6
    const float r4 = r2 * r2;
    const float p  = shz_fmaf(r4, shz_fmaf(r4, shz_fmaf(r2, -0.0114275254f, 0.0519234873f),
                                           shz_fmaf(r2, -0.11578197f, 0.193293691f)),
                              shz_fmaf(r2, -0.332585186f, 0.999975681f));
#else
    const float r4 = r2 * r2;
    const float p  = shz_fmaf(r4, shz_fmaf(r4, shz_fmaf(r4, 0.00666252105f,
                                                        shz_fmaf(r2, -0.0331594534f, 0.0791187882f)),
                                           shz_fmaf(r2, -0.132061511f, 0.198008001f)),
                              shz_fmaf(r2, -0.333166063f, 0.999995887f));
#endif

    return shz_copysignf(shz_fmaf(-r, p, angle), y);
}

/* acos(t) = sqrt(1 - t) * Q(t) over [0, 1]: Q is smooth there, since the
   square root carries the infinite slope at 1. Q is a quartic, the minimax
   fit under Q(0) = pi/2, which keeps the error at zero at t = 0. Returns
   (1 - t) * Q(t), to be multiplied by the reciprocal square root of (1 - t). */
SHZ_FORCE_INLINE float shz_acosf_unit_poly(float t, float one_minus_t) SHZ_NOEXCEPT {
    const float t2   = t * t;
    const float low  = shz_fmaf(t, -0.214366547f, SHZ_F_PI_2);
    const float high = shz_fmaf(t2, 0.0100441489f, shz_fmaf(t, -0.0382241586f, 0.0860212715f));

    return one_minus_t * shz_fmaf(t2, high, low);
}

SHZ_FORCE_INLINE float shz_asinf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_asinf(shz_clampf(x, -1.0f, 1.0f));
#endif
    const float t           = shz_fabsf(x);
    const float one_minus_t = 1.0f - t;

    // asin(t) = pi/2 - acos(t); fabsf() keeps a last-bit excess near t = 0 from flipping the sign.
    float r = shz_fabsf(shz_fmaf(-shz_inv_sqrtf_fsrra(one_minus_t),
                                 shz_acosf_unit_poly(t, one_minus_t), SHZ_F_PI_2));

    // Also discards the reciprocal root of a non-positive value.
    if(SHZ_UNLIKELY(t >= 1.0f))
        r = SHZ_F_PI_2;

    return (x < 0.0f)? -r : r;
}

SHZ_INLINE float shz_acosf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_acosf(shz_clampf(x, -1.0f, 1.0f));
#endif
    const float t           = shz_fabsf(x);
    const float one_minus_t = 1.0f - t;

    float r = shz_inv_sqrtf_fsrra(one_minus_t) * shz_acosf_unit_poly(t, one_minus_t);

    // Also discards the reciprocal root of a non-positive value.
    if(SHZ_UNLIKELY(t >= 1.0f))
        r = 0.0f;

    return (x < 0.0f)? SHZ_F_PI - r : r;
}

SHZ_INLINE float shz_asecf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_acosf(1.0f / x);
#endif
    return shz_acosf(shz_invf(x));
}

SHZ_INLINE float shz_acscf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_asinf(1.0f / x);
#endif
    return shz_asinf(shz_invf(x));
}

SHZ_INLINE float shz_acotf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_atanf(1.0f / x);
#endif
    return shz_atanf(shz_invf(x));
}

SHZ_FORCE_INLINE float shz_sinhf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_sinhf(x);
#endif
    return (shz_expf(x) - shz_expf(-x)) * 0.5f;
}

SHZ_FORCE_INLINE float shz_coshf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_coshf(x);
#endif
    return (shz_expf(x) + shz_expf(-x)) * 0.5f;
}

SHZ_FORCE_INLINE float shz_tanhf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_tanhf(x);
#endif
    float ex = shz_expf(x);     // e^x
    float enx = shz_expf(-x);   // e^-x
    return shz_divf_fsrra(ex - enx, ex + enx);
}

SHZ_FORCE_INLINE float shz_cschf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return 1.0f / __builtin_sinhf(x);
#endif
    return shz_divf(2.0f, shz_expf(x) - shz_expf(-x));
}

SHZ_FORCE_INLINE float shz_sechf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return 1.0f / __builtin_coshf(x);
#endif
    return shz_divf_fsrra(2.0f, shz_expf(x) + shz_expf(-x));
}

SHZ_FORCE_INLINE float shz_cothf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return 1.0f / __builtin_tanhf(x);
#endif
    float ex = shz_expf(x);
    float e_neg_x = shz_expf(-x);

    float sinh_val = ex - e_neg_x;
    float cosh_val = ex + e_neg_x;

    return shz_divf(cosh_val, sinh_val);
}

SHZ_FORCE_INLINE float shz_asinhf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_asinhf(x);
#endif
    return shz_logf(x + shz_sqrtf_fsrra(x * x + 1.0f));
}

SHZ_FORCE_INLINE float shz_acoshf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_acoshf(x);
#endif
    return shz_logf(x + shz_sqrtf(x * x - 1.0f));
}

SHZ_FORCE_INLINE float shz_atanhf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_atanhf(x);
#endif
    return 0.5f * shz_logf(shz_divf(1.0f + x, 1.0f - x));
}

SHZ_FORCE_INLINE float shz_acschf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_asinhf(1.0f / x);
#endif
    float inv_x = shz_invf(x);
    return shz_logf(inv_x + shz_sqrtf_fsrra(inv_x * inv_x + 1.0f));
}

SHZ_FORCE_INLINE float shz_asechf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_acoshf(1.0f / x);
#endif
    return shz_logf(shz_divf(1.0f + shz_sqrtf(1.0f - (x * x)), x));
}

SHZ_FORCE_INLINE float shz_acothf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_atanhf(1.0f / x);
#endif
    return 0.5f * shz_logf(shz_divf(x + 1.0f, x - 1.0f));
}

SHZ_FORCE_INLINE float shz_lgammaf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_lgammaf(x);
#endif
    float neg_offset = 0.0f;
    bool is_neg = (x < 0.0f);

    if(is_neg) {
        const float LN_PI = 1.14472989f;

        float sin_val = shz_fabsf(shz_sinf(SHZ_F_PI * x));
        /* Do we even need to protect against this degenerate case?
        if (sin_val == 0.0f) {
            return INFINITY;
        }
        */
        neg_offset = LN_PI - shz_logf(sin_val);
        x = 1.0f - x; // x is now guaranteed > 1.0f
    }

    // Argument shift (runs at most 5 times if x was reflected, 6 times if x >= 0)
    float prod = 1.0f;
    while(x < 6.0f) {
        prod *= x;
        x += 1.0f;
    }

    // Stirling's Approximation
    const float HALF_LN_2PI = 0.91893853f;
    const float c1 =  1.0f / 12.0f;
    const float c2 = -1.0f / 360.0f;
    const float c3 =  1.0f / 1260.0f;

    float inv_x  = shz_invf_fsrra(x);
    float inv_x2 = inv_x * inv_x;
    float inv_x3 = inv_x * inv_x2;
    float inv_x5 = inv_x3 * inv_x2;
    float series = shz_dot8f(inv_x, inv_x3, inv_x5, 0.0f, c1, c2, c3, 0.0f);

    float pos_res = (x - 0.5f) * shz_logf(x) - x + HALF_LN_2PI + series - shz_logf(prod);

    // Return either pos_res or (neg_offset - pos_res) without recursion
    return is_neg ? (neg_offset - pos_res) : pos_res;
}

SHZ_FORCE_INLINE float shz_tgammaf(float x) SHZ_NOEXCEPT {
#ifdef SHZ_GNUC
    if(__builtin_constant_p(x))
        return __builtin_tgammaf(x);
#endif
    // For positive x, Gamma(x) is always positive
    if(x >= 0.0f) {
        return shz_expf(shz_lgammaf(x));
    }

    // For negative x, sign alternates on every integer interval:
    // (-1, 0) -> negative, (-2, -1) -> positive, (-3, -2) -> negative, etc.
    int n = (int)shz_floorf(x);
    float sign = (n & 0x1) ? -1.0f : 1.0f;
    return sign * shz_expf(shz_lgammaf(x));
}

 //! \endcond
