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

    \important Do not copy this file or its routines into other projects. Use the
               KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
*/
#ifndef SHZ_TRIG_SH4_INL_H
#define SHZ_TRIG_SH4_INL_H

SHZ_FORCE_INLINE void shz_sincosf_ftrc_sh4(float units, float* s, float* c) SHZ_NOEXCEPT {
    asm(R"(
        ftrc    %2, fpul
        fsca    fpul, dr8
        fmov    fr8, %0
        fmov    fr9, %1
    )"
    : "=&f" (*s), "=&f" (*c)
    : "f" (units)
    : "fpul", "fr8", "fr9");
}

/* GCC's -ffast-math path currently has a bug in it that can
   misoptimize sinf/cosf() patterns on uint16_t and emit full float
   conversion code rather than just lds + fsca. Do it ourselves in
   inline ASM to be safe. */
SHZ_FORCE_INLINE shz_sincos_t shz_sincosu16_sh4(uint16_t radians16) SHZ_NOEXCEPT {
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

    return SHZ_INIT(shz_sincos_t, rsin, rcos);
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_sh4(float radians) SHZ_NOEXCEPT {
    float rsin, rcos;

#ifdef __FAST_MATH__
    rsin = __builtin_sinf(radians);
    rcos = __builtin_cosf(radians);
#else
    shz_sincosf_ftrc_sh4(radians * SHZ_FSCA_RAD_FACTOR, &rsin, &rcos);
#endif

    return SHZ_INIT(shz_sincos_t, rsin, rcos);
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg_sh4(float degrees) SHZ_NOEXCEPT {
    float rsin, rcos;

#ifdef __FAST_MATH__
    rsin = __builtin_sinf(SHZ_DEG_TO_RAD(degrees));
    rcos = __builtin_cosf(SHZ_DEG_TO_RAD(degrees));
#else
    shz_sincosf_ftrc_sh4(degrees * SHZ_FSCA_DEG_FACTOR, &rsin, &rcos);
#endif

    return SHZ_INIT(shz_sincos_t, rsin, rcos);
}

//! \endcond

#endif
