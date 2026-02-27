//! \cond INTERNAL
/*! \file
 *  \brief General-purpose scalar math routines.
 *  \ingroup scalar
 *
 *  This file provides the inlined implementation of the scalar API,
 *  including handling of any constant expression evaluation, followed
 *  by forwarding the call on to a specific back-end implementation of
 *  the scalar routines.
 *
 *  \author 2025, 2026 Falco Girgis
 *  \author 2025, 2026 Paul Cercueil
 *  \author       2026 Aleios
 *  \author       2026 jnmartin64
 *
 *  \copyright MIT License
 */

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_scalar_sh4.inl.h"
#endif

#include "sw/shz_scalar_sw.inl.h"

SHZ_FORCE_INLINE float shz_fminf(float a, float b) SHZ_NOEXCEPT {
    return shz_fminf_sw(a, b);
}

SHZ_FORCE_INLINE float shz_fmaxf(float a, float b) SHZ_NOEXCEPT {
    return shz_fmaxf_sw(a, b);
}

SHZ_FORCE_INLINE bool shz_equalf(float a, float b) SHZ_NOEXCEPT {
    return shz_equalf_sw(a, b);
}

SHZ_FORCE_INLINE bool shz_equalf_abs(float a, float b) SHZ_NOEXCEPT {
    return shz_equalf_abs_sw(a, b);
}

SHZ_FORCE_INLINE bool shz_equalf_rel(float a, float b) SHZ_NOEXCEPT {
    return shz_equalf_rel_sw(a, b);
}

SHZ_FORCE_INLINE float shz_floorf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_floorf(x);

    return shz_floorf_sw(x);
}

SHZ_FORCE_INLINE float shz_ceilf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_ceilf(x);

    return shz_ceilf_sw(x);
}

SHZ_FORCE_INLINE float shz_copysignf(float x, float y) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y))
        return __builtin_copysignf(x, y);

    return shz_copysignf_sw(x, y);
}

SHZ_FORCE_INLINE float shz_roundf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_roundf(x);

    return shz_roundf_sw(x);
}

SHZ_FORCE_INLINE float shz_truncf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_truncf(x);

    return shz_truncf_sw(x);
}

SHZ_FORCE_INLINE float shz_fdimf(float x, float y) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y))
        return (x > y)? x - y : 0.0f;

    return shz_fdimf_sw(x, y);
}

SHZ_FORCE_INLINE float shz_hypotf(float x, float y) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y))
        return __builtin_hypotf(x, y);

    return shz_hypotf_sw(x,y);
}

// https://www.musicdsp.org/en/latest/Other/206-fast-cube-root-square-root-and-reciprocal-for-x86-sse-cpus.html#id2
// cuberoot_sse_16bits( float x ) -> fast_cbrt_2step(float x)
SHZ_FORCE_INLINE float shz_cbrt_newton1(float x, float z) SHZ_NOEXCEPT {
    float z2 = z * z;
    float z3 = z2 * z;
    float num = z3 - x;
    float rden = shz_inv_sqrtf_fsrra(3.0f * z2);
    return z - (num * rden * rden);
}

SHZ_FORCE_INLINE float shz_cbrtf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_cbrtf(x);

    if(x == 0.0f)
        return 0.0f;

#if SHZ_BACKEND == SHZ_DREAMCAST
    float z = shz_cbrt_magic_dc(x);
#else
    float z = shz_cbrt_magic_sw(x);
#endif

    z = shz_cbrt_newton1(x, z);
    z = shz_cbrt_newton1(x, z);

    return z;
}

SHZ_FORCE_INLINE float shz_remainderf(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(num) && __builtin_constant_p(denom))
        return __builtin_remainderf(num, denom);

    return shz_remainderf_sw(num, denom);
}

SHZ_FORCE_INLINE float shz_fmodf(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(num) && __builtin_constant_p(denom))
        return __builtin_fmodf(num, denom);

    return shz_fmodf_sw(num, denom);
}

SHZ_FORCE_INLINE float shz_remquof(float num, float denom, float* quot) SHZ_NOEXCEPT {
    if(__builtin_constant_p(num) && __builtin_constant_p(denom)) {
        int int_quot;
        float remainder = __builtin_remquof(num, denom, &int_quot);

        *quot = int_quot;

        return remainder;
    }

    return shz_remquof_sw(num, denom, quot);
}

SHZ_FORCE_INLINE float shz_clampf(float value, float min, float max) SHZ_NOEXCEPT {
    return shz_clampf_sw(value, min, max);
}

SHZ_FORCE_INLINE float shz_normalizef(float current, float from, float to) SHZ_NOEXCEPT {
    return shz_normalizef_sw(current, from, to);
}

SHZ_FORCE_INLINE float shz_normalizef_fsrra(float current, float from, float to) SHZ_NOEXCEPT {
    return shz_normalizef_fsrra_sw(current, from, to);
}

SHZ_FORCE_INLINE float shz_remapf(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT {
    return shz_remapf_sw(value, inputStart, inputEnd, outputStart, outputEnd);
}

SHZ_FORCE_INLINE float shz_remapf_fsrra(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT {
    return shz_remapf_fsrra_sw(value, inputStart, inputEnd, outputStart, outputEnd);
}

SHZ_FORCE_INLINE float shz_wrapf(float value, float min, float max) SHZ_NOEXCEPT {
    return shz_wrapf_sw(value, min, max);
}

SHZ_FORCE_INLINE float shz_wrapf_fsrra(float value, float min, float max) SHZ_NOEXCEPT {
    return shz_wrapf_fsrra_sw(value, min, max);
}

/* Only here in case we ever find a platform where the compiler is so dumb
   fabsf() isn't optimal... but in the meantime, it is.
*/
SHZ_FORCE_INLINE float shz_fabsf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_fabsf(x);

    return shz_fabsf_sw(x);
}

/* Compiler is smart enough to do the right thing regardless of flags for SH4.
   Smarter than even using the builtin (seriously). */
SHZ_FORCE_INLINE float shz_fmaf(float a, float b, float c) SHZ_NOEXCEPT {
    return shz_fmaf_sw(a, b, c);
}

SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) SHZ_NOEXCEPT {
    return shz_lerpf_sw(a, b, t);
}

SHZ_FORCE_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) SHZ_NOEXCEPT {
    return shz_barycentric_lerpf_sw(a, b, c, u, v);
}

SHZ_FORCE_INLINE bool shz_quadratic_roots(float a, float b, float c,
                                          float* root1, float* root2) SHZ_NOEXCEPT {
    return shz_quadratic_roots_sw(a, b, c, root1, root2);
}

SHZ_FORCE_INLINE float shz_inv_sqrtf_fsrra(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / __builtin_sqrtf(x);

#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_inv_sqrtf_fsrra_dc(x);
#else
    return shz_inv_sqrtf_fsrra_sw(x);
#endif
}

SHZ_FORCE_INLINE float shz_inv_sqrtf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / __builtin_sqrtf(x);

    return shz_inv_sqrtf_sw(x);
}

SHZ_FORCE_INLINE float shz_sqrtf_fsrra(float x) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra_sw(x);
}

SHZ_FORCE_INLINE float shz_sqrtf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_sqrtf(x);

    return shz_sqrtf_sw(x);
}

SHZ_FORCE_INLINE float shz_invf_fsrra(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / __builtin_fabsf(x);

    return shz_invf_fsrra_sw(x);
}

SHZ_FORCE_INLINE float shz_invf(float x) SHZ_NOEXCEPT {
    if (__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / x;

    return shz_invf_sw(x);
}

SHZ_FORCE_INLINE float shz_divf_fsrra(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return shz_divf_fsrra_sw(num, denom);
}

SHZ_FORCE_INLINE float shz_divf(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return shz_divf_sw(num, denom);
}

SHZ_FORCE_INLINE float shz_dot6f(float x1, float y1, float z1,
                                 float x2, float y2, float z2) SHZ_NOEXCEPT {

    if(__builtin_constant_p(x1) && __builtin_constant_p(y1) && __builtin_constant_p(z1) &&
       __builtin_constant_p(x2) && __builtin_constant_p(y2) && __builtin_constant_p(z2))
        return (x1 * x2) + (y1 * y2) + (z1 * z2);

#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_dot6f_dc(x1, y1, z1, x2, y2, z2);
#else
    return shz_dot6f_sw(x1, y1, z1, x2, y2, z2);
#endif
}

SHZ_FORCE_INLINE float shz_mag_sqr3f(float x, float y, float z) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y) && __builtin_constant_p(z))
        return (x * x) + (y * y) + (z * z);

#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_mag_sqr3f_dc(x, y, z);
#else
    return shz_mag_sqr3f_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) SHZ_NOEXCEPT {

    if(__builtin_constant_p(x1) && __builtin_constant_p(y1) && __builtin_constant_p(z1) && __builtin_constant_p(w1) &&
       __builtin_constant_p(x2) && __builtin_constant_p(y2) && __builtin_constant_p(z2) && __builtin_constant_p(w2))
        return (x1 * x2) + (y1 * y2) + (z1 * z2) + (w1 * w2);

#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_dot8f_dc(x1, y1, z1, w1, x2, y2, z2, w2);
#else
    return shz_dot8f_sw(x1, y1, z1, w1, x2, y2, z2, w2);
#endif
}

SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y) && __builtin_constant_p(z) && __builtin_constant_p(w))
        return (x * x) + (y * y) + (z * z) + (w * w);

#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_mag_sqr4f_dc(x, y, z, w);
#else
    return shz_mag_sqr4f_sw(x, y, z, w);
#endif
}


SHZ_FORCE_INLINE float shz_log2f(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return __builtin_log2f(x);

    return shz_log2f_sw(x);
}

SHZ_FORCE_INLINE float shz_log10f(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_log10f(x);

    return shz_log10f_sw(x);
}

SHZ_FORCE_INLINE float shz_logf(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return __builtin_logf(x);

    return shz_logf_sw(x);
}

// https://github.com/appleseedhq/appleseed/blob/master/src/appleseed/foundation/math/fastmath.h
SHZ_FORCE_INLINE float shz_pow2f(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return __builtin_powf(2.0f, p);

    return shz_pow2f_sw(p);
}

SHZ_FORCE_INLINE float shz_pow10f(float p) SHZ_NOEXCEPT {
    if(__builtin_constant_p(p))
        return __builtin_pow10f(p);

    return shz_pow10f_sw(p);
}

SHZ_FORCE_INLINE float shz_powf(float x, float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x) && __builtin_constant_p(p))
        return __builtin_powf(x, p);

    return shz_powf_sw(x, p);
}

SHZ_FORCE_INLINE float shz_expf(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return __builtin_expf(p);

    return shz_expf_sw(p);
}

SHZ_FORCE_INLINE float shz_randf(int* seed) SHZ_NOEXCEPT {
    return shz_randf_sw(seed);
}

SHZ_FORCE_INLINE float shz_randf_range(int* seed, float min, float max) SHZ_NOEXCEPT {
    return shz_randf_range_sw(seed, min, max);
}

SHZ_FORCE_INLINE float shz_fractf(float x) SHZ_NOEXCEPT {
    return x - shz_floorf(x);
}

SHZ_FORCE_INLINE float shz_signf(float x) SHZ_NOEXCEPT {
    if(x > 0.0f) return 1.0f;
    else if(x < 0.0f) return -1.0f;
    else return 0.0f;
}

SHZ_FORCE_INLINE float shz_saturatef(float x) SHZ_NOEXCEPT {
    return shz_clampf(x, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE float shz_stepf(float x, float edge) SHZ_NOEXCEPT {
    return shz_stepf_sw(x, edge);
}

SHZ_FORCE_INLINE float shz_smoothstepf(float x, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_smoothstepf_sw(x, edge0, edge1);
}

SHZ_FORCE_INLINE float shz_smoothstepf_safe(float x, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_smoothstepf_safe_sw(x, edge0, edge1);
}

//! \endcond
