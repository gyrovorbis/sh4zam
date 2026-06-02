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

SHZ_FORCE_INLINE float shz_inv_sqrtf_fsrra_sh4(float x) SHZ_NOEXCEPT {
    asm("fsrra %0" : "+f" (x));

    return x;
}

SHZ_FORCE_INLINE float shz_dot6f_sh4(float x1, float y1, float z1,
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

SHZ_FORCE_INLINE float shz_mag_sqr3f_sh4(float x, float y, float z) SHZ_NOEXCEPT {
    register float rx asm("fr8")  = x;
    register float ry asm("fr9")  = y;
    register float rz asm("fr10") = z;
    register float rw asm("fr11") = 0.0f;

    asm("fipr fv8, fv8"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

SHZ_FORCE_INLINE float shz_dot8f_sh4(float x1, float y1, float z1, float w1,
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

SHZ_FORCE_INLINE float shz_mag_sqr4f_sh4(float x, float y, float z, float w) SHZ_NOEXCEPT {
    register float rx asm("fr8")  = x;
    register float ry asm("fr9")  = y;
    register float rz asm("fr10") = z;
    register float rw asm("fr11") = w;

    asm("fipr fv8, fv8"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

#if 1
SHZ_FORCE_INLINE float shz_cbrt_magic_sh4(float x) SHZ_NOEXCEPT {
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
#else
SHZ_FORCE_INLINE float shz_cbrt_magic_sh4(float x) SHZ_NOEXCEPT {
    shz_alias_int32_t bits = *(shz_alias_int32_t*)&x;
    shz_alias_int32_t s1, s2;

    asm volatile(
        "shll   %[x]        \n\t" /* T = sign bit, x = abs(bits) << 1      */
        "mov    #0x7f, %[s1]\n\t" /* build anchor 0x7f000000                */
        "shll8  %[s1]       \n\t"
        "shll16 %[s1]       \n\t"
        "sub    %[s1], %[x] \n\t" /* x -= 0x7f000000                        */
        "mov    #-10, %[s2] \n\t" /* shift amount (negative = right in SHAD) */
        "shad   %[s2], %[x] \n\t" /* x >>= 10 arithmetic; T unmodified       */
        "mov    #1,  %[s2]  \n\t" /* build 341 = 256 + 85                    */
        "shll8  %[s2]       \n\t"
        "mov    #85, %[s1]  \n\t"
        "add    %[s2], %[s1]\n\t"
        "mul.l  %[s1], %[x] \n\t" /* MACL = x * 341; T unmodified            */
        "mov    #0x7f, %[s1]\n\t" /* rebuild anchor (fills mul.l latency)    */
        "shll8  %[s1]       \n\t"
        "shll16 %[s1]       \n\t"
        "sts    macl, %[x]  \n\t" /* x = MACL (>= 2 cycles after mul.l)     */
        "add    %[s1], %[x] \n\t" /* x += 0x7f000000                        */
        "rotcr  %[x]        \n\t" /* restore sign from T                     */
        : [x] "+r" (bits), [s1] "=&r" (s1), [s2] "=&r" (s2)
        :
        : "macl", "t"
    );

    return *(shz_alias_float_t*)&bits;
}


/* Broken: shll saves sign in T flag, but >> 10 compiles to SHAR which
   overwrites T on every shift, so rotcr restores the wrong bit. */
SHZ_FORCE_INLINE float shz_cbrt_magic_sh4_broken(float x) SHZ_NOEXCEPT {
    shz_alias_int32_t eax = *(shz_alias_int32_t*)&x;

    asm("shll %0" :"+r"(eax) :: "t");

    eax = ((eax - 0x7f000000) >> 10) * 341 + 0x7f000000;

    asm("rotcr %0" :"+r"(eax) :: "t");

    return *(shz_alias_float_t*)&eax;
}
#endif

//! \endcond

#endif
