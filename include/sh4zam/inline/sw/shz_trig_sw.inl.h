//! \cond INTERNAL
/*! \file
    \brief Software implementation of Trigonometry API
    \ingroup trig

    This file contains the generic implementation routines for
    trigonometry math, which are platform-independent, shared,
    and may be run anywhere. They're offered as part of the
    SW back-end.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_TRIG_SW_INL_H
#define SHZ_TRIG_SW_INL_H

#include <math.h>

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16_sw(uint16_t radians16) SHZ_NOEXCEPT {
    float radians = ((float)radians16 / 65535.0f) * 2.0f * SHZ_F_PI;
 
    return (shz_sincos_t){ sinf(radians), cosf(radians) };
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_sw(float radians) SHZ_NOEXCEPT {
    return (shz_sincos_t) { sinf(radians), cosf(radians) };
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return (shz_sincos_t) { sinf(SHZ_DEG_TO_RAD(degrees)), cosf(SHZ_DEG_TO_RAD(degrees)) };
}

SHZ_FORCE_INLINE float shz_sincos_tanf_sw(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_divf(sincos.sin, sincos.cos);
}

SHZ_FORCE_INLINE float shz_sincos_secf_sw(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_invf(sincos.cos);
}

SHZ_FORCE_INLINE float shz_sincos_cscf_sw(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_invf(sincos.sin);
}

SHZ_FORCE_INLINE float shz_sincos_cotf_sw(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_divf(sincos.cos, sincos.sin);
}

SHZ_FORCE_INLINE float shz_sinf_sw(float radians) SHZ_NOEXCEPT {
    return shz_sincosf(radians).sin;
}

SHZ_FORCE_INLINE float shz_sinf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).sin;
}

SHZ_FORCE_INLINE float shz_cosf_sw(float radians) SHZ_NOEXCEPT {
    return shz_sincosf(radians).cos;
}

SHZ_FORCE_INLINE float shz_cosf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return shz_sincosf_deg(degrees).cos;
}

SHZ_FORCE_INLINE float shz_tanf_sw(float radians) SHZ_NOEXCEPT {
    return shz_sincos_tanf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_tanf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_tanf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_secf_sw(float radians) SHZ_NOEXCEPT {
    return shz_sincos_secf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_secf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_secf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cscf_sw(float radians) SHZ_NOEXCEPT {
    return shz_sincos_cscf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_cscf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_cscf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_cotf_sw(float radians) SHZ_NOEXCEPT {
    return shz_sincos_cotf(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_cotf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return shz_sincos_cotf(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE float shz_atanf_unit_sw(float x) SHZ_NOEXCEPT {
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;

    return shz_fmaf(n2, x * x, n1) * x;
}

SHZ_INLINE float shz_atanf_q1_sw(float x) SHZ_NOEXCEPT {
    return SHZ_F_PI_2 - shz_atanf_unit(shz_invf_fsrra(x));
}

SHZ_INLINE float shz_atanf_sw(float x) SHZ_NOEXCEPT {
    if(x > 1.0f)
	    return shz_atanf_q1(x);
    else if(x < -1.0f)
        return -shz_atanf_q1(x);
    else
        return shz_atanf_unit(x);
}

SHZ_INLINE float shz_atan2f_sw(float y, float x) SHZ_NOEXCEPT {
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


SHZ_INLINE float shz_asinf_sw(float x) SHZ_NOEXCEPT {
    return shz_atanf(x * shz_inv_sqrtf_fsrra(1.0f - (x * x)));
}

SHZ_INLINE float shz_acosf_sw(float x) SHZ_NOEXCEPT {
    return SHZ_F_PI_2 - shz_asinf(x);
}

SHZ_INLINE float shz_asecf_sw(float x) SHZ_NOEXCEPT {
    return shz_acosf(shz_invf(x));
}

SHZ_INLINE float shz_acscf_sw(float x) SHZ_NOEXCEPT {
    return shz_asinf(shz_invf(x));
}

SHZ_INLINE float shz_acotf_sw(float x) SHZ_NOEXCEPT {
    return shz_atanf(shz_invf(x));
}
//! \endcond

#endif
