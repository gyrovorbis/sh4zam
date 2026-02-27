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
//! \endcond

#endif
