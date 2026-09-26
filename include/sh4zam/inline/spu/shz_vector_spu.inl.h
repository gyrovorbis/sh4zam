//! \cond INTERNAL
/*! \file
    \brief SPU implementation of the Vector API
    \ingroup trig

    This file contains the Cell B/E implementation routines for
    vector math, which have been hand-optimized specifically for
    the SPU co-processor architecture.

    \author 2026 Fancy2209

    \copyright MIT License
*/
#ifndef SHZ_VECTOR_SH4_INL_H
#define SHZ_VECTOR_SH4_INL_H

#include <spu_intrinsics.h>

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_fill_spu(float v) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = spu_splats(v);
    return r;
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add_spu(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = spu_add(vec1.e, vec2.e);
    return r;
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub_spu(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = spu_sub(vec1.e, vec2.e);
    return r;
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul_spu(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = spu_mul(vec1.e, vec2.e);
    return r;
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div_spu(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = spu_mul(vec1.e, spu_re(vec2.e));
    return r;
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_minv_spu(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = (spu_sel(a.e, b.e, spu_cmpgt(a.e, b.e))); // if(b > a) return a else return b
    return r;
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_maxv_spu(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = (spu_sel(b.e, a.e, spu_cmpgt(a.e, b.e))); // if(b > a) return b else return a
    return r;
}

SHZ_INLINE shz_vec4_t shz_vec4_inv_sqrtf_spu(shz_vec4_t vec) SHZ_NOEXCEPT {
    shz_vec4_t r;
    r.e = spu_rsqrte(vec.e);
    return r;
}

#endif