//! \cond INTERNAL
/*! \file
    \brief Software implementation of Scalar API
    \ingroup scalar

    This file contains the generic implementation routines for
    scalar math, which are platform-independent, shared,
    and may be run anywhere. They're offered as part of the
    SW back-end.

    \author 2026 Falco Girgis
    \author 2026 Aleios
    \author 2026 jnmartin64

    \copyright MIT License
*/
#ifndef SHZ_SCALAR_SW_INL_H
#define SHZ_SCALAR_SW_INL_H

SHZ_FORCE_INLINE float shz_inv_sqrtf_fsrra_sw(float x) SHZ_NOEXCEPT {
    return 1.0f / sqrtf(x);
}

SHZ_FORCE_INLINE float shz_dot6f_sw(float x1, float y1, float z1,
                                    float x2, float y2, float z2) SHZ_NOEXCEPT {
    return (x1 * x2) + (y1 * y2) + (z1 * z2);
}

SHZ_FORCE_INLINE float shz_mag_sqr3f_sw(float x, float y, float z) SHZ_NOEXCEPT {
    return (x * x) + (y * y) + (z * z);
}

SHZ_FORCE_INLINE float shz_dot8f_sw(float x1, float y1, float z1, float w1,
                                    float x2, float y2, float z2, float w2) SHZ_NOEXCEPT {
    return (x1 * x2) + (y1 * y2) + (z1 * z2) + (w1 * w2);
}

SHZ_FORCE_INLINE float shz_mag_sqr4f_sw(float x, float y, float z, float w) SHZ_NOEXCEPT {
    return (x * x) + (y * y) + (z * z) + (w * w);
}

SHZ_FORCE_INLINE float shz_cbrt_magic_sw(float x) SHZ_NOEXCEPT {
    uint32_t eax = *(shz_alias_uint32_t *)&x; // mov eax, x (as bits)
    uint32_t ecx = eax;                       // mov ecx, eax

    eax &= 0x7FFFFFFFu;                       // clear sign
    eax -= 0x3F800000u;                       // subtract 1.0f anchor
    eax = (uint32_t)((int32_t)eax >> 10);     // sar eax, 10  (arithmetic shift)
    eax = (uint32_t)((int32_t)eax * 341);     // imul eax, 341
    eax += 0x3F800000u;                       // add anchor back
    eax &= 0x7FFFFFFFu;                       // remask (keep positive magnitude)

    ecx &= 0x80000000u;                       // keep original sign bit
    eax |= ecx;                               // combine sign + new magnitude

    return *(shz_alias_float_t*)&eax; // z as float
}

//! \endcond

#endif