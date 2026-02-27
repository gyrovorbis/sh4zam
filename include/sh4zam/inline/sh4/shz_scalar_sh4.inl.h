//! \cond INTERNAL
/*! \file
    \brief SH4 implementation of Scalar API
    \ingroup scalar

    This file contains the Dreamcast implementation routines for
    scalar math, which have been hand-optimized specifically
    for the SH4 architecture.

    \author 2026 Falco Girgis
    \author 2026 Paul Cercueil

    \copyright MIT License
*/
#ifndef SHZ_SCALAR_SH4_INL_H
#define SHZ_SCALAR_SH4_INL_H

SHZ_FORCE_INLINE float shz_inv_sqrtf_fsrra_dc(float x) SHZ_NOEXCEPT {
    asm("fsrra %0" : "+f" (x));

    return x;
}

SHZ_FORCE_INLINE float shz_dot6f_dc(float x1, float y1, float z1,
                                    float x2, float y2, float z2) SHZ_NOEXCEPT {
    register float rx1 asm("fr8")  = x1;
    register float ry1 asm("fr9")  = y1;
    register float rz1 asm("fr10") = z1;
    register float rw1 asm("fr11") = 0.0f;
    register float rx2 asm("fr12") = x2;
    register float ry2 asm("fr13") = y2;
    register float rz2 asm("fr14") = z2;
    register float rw2 asm("fr15");

    asm("fipr fv8, fv12"
        : "=f" (rw2)
        : "f" (rx1), "f" (ry1), "f" (rz1), "f" (rw1),
          "f" (rx2), "f" (ry2), "f" (rz2));

    return rw2;
}

SHZ_FORCE_INLINE float shz_mag_sqr3f_dc(float x, float y, float z) SHZ_NOEXCEPT {
    register float rx asm("fr8")  = x;
    register float ry asm("fr9")  = y;
    register float rz asm("fr10") = z;
    register float rw asm("fr11") = 0.0f;

    asm("fipr fv8, fv8"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

SHZ_FORCE_INLINE float shz_dot8f_dc(float x1, float y1, float z1, float w1,
                                    float x2, float y2, float z2, float w2) SHZ_NOEXCEPT {
    register float rx1 asm("fr8")  = x1;
    register float ry1 asm("fr9")  = y1;
    register float rz1 asm("fr10") = z1;
    register float rw1 asm("fr11") = w1;
    register float rx2 asm("fr12") = x2;
    register float ry2 asm("fr13") = y2;
    register float rz2 asm("fr14") = z2;
    register float rw2 asm("fr15") = w2;

    asm("fipr fv8, fv12"
        : "+f" (rw2)
        : "f" (rx1), "f" (ry1), "f" (rz1), "f" (rw1),
          "f" (rx2), "f" (ry2), "f" (rz2));

    return rw2;
}

SHZ_FORCE_INLINE float shz_mag_sqr4f_dc(float x, float y, float z, float w) SHZ_NOEXCEPT {
    register float rx asm("fr8")  = x;
    register float ry asm("fr9")  = y;
    register float rz asm("fr10") = z;
    register float rw asm("fr11") = w;

    asm("fipr fv8, fv8"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

SHZ_FORCE_INLINE float shz_cbrt_magic_dc(float x) SHZ_NOEXCEPT {
    int32_t eax = *(shz_alias_int32_t*)&x; // mov eax, x (as bits)

    asm inline("shll %0" :"+r"(eax) :: "t");

    eax = ((eax - 0x7f000000) >> 10) * 341 + 0x7f000000;

    asm inline("rotcr %0" :"+r"(eax) :: "t");

    return *(shz_alias_float_t*)&eax; // z as float
}

//! \endcond

#endif
