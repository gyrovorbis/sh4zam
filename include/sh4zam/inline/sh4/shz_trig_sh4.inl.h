//! \cond INTERNAL
/*! \file
    \brief SH4 implementation of the Trigonometry API
    \ingroup trig

    This file contains the Dreamcast implementation routines for
    trigonometry math, which have been hand-optimized specifically
    for the SH4 architecture.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_TRIG_SH4_INL_H
#define SHZ_TRIG_SH4_INL_H

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16_sh4(uint16_t radians16) SHZ_NOEXCEPT {
// \todo Ask Oleg Endo wtf?
#if 0 //__FAST_MATH__

#else
    float rsin, rcos;

    asm(R"(
            lds  %2, fpul
            fsca fpul, dr8
            fmov fr8, %0
            fmov fr9, %1
        )"
        : "=&f" (rsin), "=&f" (rcos)
        : "r" (radians16)
        : "fpul", "fr8", "fr9");
#endif

    return (shz_sincos_t){ rsin, rcos };
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_sh4(float radians) SHZ_NOEXCEPT {
#ifdef __FAST_MATH__
    return (shz_sincos_t) { __builtin_sinf(radians), __builtin_cosf(radians) };
#else
    float rsin, rcos;
    float r = radians * SHZ_FSCA_RAD_FACTOR;

    asm(R"(
        ftrc  %2, fpul
        fsca  fpul, dr8
        fmov  fr8, %0
        fmov  fr9, %1
    )"
    : "=&f" (rsin), "=&f" (rcos)
    : "f" (r)
    : "fpul", "fr8", "fr9");

    return (shz_sincos_t){ rsin, rcos };
#endif
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg_sh4(float degrees) SHZ_NOEXCEPT {
#ifdef __FAST_MATH__
    return (shz_sincos_t) { __builtin_sinf(SHZ_DEG_TO_RAD(degrees)), __builtin_cosf(SHZ_DEG_TO_RAD(degrees)) };
#else
    float rsin, rcos;
    float d = degrees * SHZ_FSCA_DEG_FACTOR;

    asm(R"(
         ftrc  %2, fpul
         fsca  fpul, dr8
         fmov  fr8, %0
         fmov  fr9, %1
     )"
     : "=&f" (rsin), "=&f" (rcos)
     : "f" (d)
     : "fpul", "fr8", "fr9");

    return (shz_sincos_t){ rsin, rcos };
#endif
}
//! \endcond

#endif
