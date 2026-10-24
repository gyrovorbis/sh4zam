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

    \important Do not copy this file or its routines into other projects. Use the
               KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
*/
#ifndef SHZ_TRIG_SW_INL_H
#define SHZ_TRIG_SW_INL_H

#include <math.h>

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16_sw(uint16_t radians16) SHZ_NOEXCEPT {
    const float radians = ((float)radians16 / (float)((float)UINT16_MAX + 1.0f)) * SHZ_F_TAU;

    return SHZ_INIT(shz_sincos_t, sinf(radians), cosf(radians));
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_sw(float radians) SHZ_NOEXCEPT {
    return SHZ_INIT(shz_sincos_t, sinf(radians), cosf(radians));
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg_sw(float degrees) SHZ_NOEXCEPT {
    return SHZ_INIT(shz_sincos_t, sinf(SHZ_DEG_TO_RAD(degrees)), cosf(SHZ_DEG_TO_RAD(degrees)));
}

SHZ_INLINE float shz_asinf_sw(float x) SHZ_NOEXCEPT {
    /* Protect against passing a non-positive value to shz_inv_sqrtf_fsrra()
       which will produce incorrect results on x86 with -ffast-math. */
    if(shz_fabsf(x) >= 1.0f)
        return shz_copysignf(SHZ_F_PI_2, x);

    return shz_atanf(x * shz_inv_sqrtf_fsrra(1.0f - (x * x)));
}

//! \endcond

#endif
