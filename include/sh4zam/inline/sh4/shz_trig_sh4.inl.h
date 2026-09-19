//! \cond INTERNAL
/*! \file
    \brief SH4 implementation of the Trigonometry API
    \ingroup trig

    This file contains the Dreamcast implementation routines for
    trigonometry math, which have been hand-optimized specifically
    for the SH4 architecture.

    The main thing to note here is that when the GCC flag, -ffast-math
    is enabled, we use the compiler builtins for generating FSCA, as it
    can do a better job handling register allocation.

    When -ffast-math is NOT enabled, we fall back to inline ASM routines
    to emit the FSCA instruction explicitly.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_TRIG_SH4_INL_H
#define SHZ_TRIG_SH4_INL_H

SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16_sh4(uint16_t radians16) SHZ_NOEXCEPT {
    float rsin, rcos;

#if 0 // __FAST_MATH__
      // GCC is producing suboptimal code here... always use inline ASM fallback.

    const float radians = radians16 * (SHZ_F_TAU / (float)(UINT16_MAX + 1));
    rsin = __builtin_sinf(radians);
    rcos = __builtin_cosf(radians);
#else
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

    return SHZ_INIT(shz_sincos_t, rsin, rcos);
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_sh4(float radians) SHZ_NOEXCEPT {
    float rsin, rcos;

#ifdef __FAST_MATH__
    rsin = __builtin_sinf(radians),
    rcos = __builtin_cosf(radians);
#else
    const float r = radians * SHZ_FSCA_RAD_FACTOR;

    asm(R"(
        ftrc  %2, fpul
        fsca  fpul, dr8
        fmov  fr8, %0
        fmov  fr9, %1
    )"
    : "=&f" (rsin), "=&f" (rcos)
    : "f" (r)
    : "fpul", "fr8", "fr9");
#endif

    return SHZ_INIT(shz_sincos_t, rsin, rcos);
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg_sh4(float degrees) SHZ_NOEXCEPT {
    float rsin, rcos;

#ifdef __FAST_MATH__
    rsin = __builtin_sinf(SHZ_DEG_TO_RAD(degrees));
    rcos = __builtin_cosf(SHZ_DEG_TO_RAD(degrees));
#else
    const float d = degrees * SHZ_FSCA_DEG_FACTOR;

    asm(R"(
         ftrc  %2, fpul
         fsca  fpul, dr8
         fmov  fr8, %0
         fmov  fr9, %1
     )"
     : "=&f" (rsin), "=&f" (rcos)
     : "f" (d)
     : "fpul", "fr8", "fr9");
#endif

    return SHZ_INIT(shz_sincos_t, rsin, rcos);
}
//! \endcond

#endif
