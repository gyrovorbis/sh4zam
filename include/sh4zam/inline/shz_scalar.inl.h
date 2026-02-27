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
#include <assert.h>
#include <math.h>

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_scalar_sh4.inl.h"
#else
#   include "sw/shz_scalar_sw.inl.h"
#endif

SHZ_FORCE_INLINE float shz_fminf(float a, float b) SHZ_NOEXCEPT {
    return __builtin_fminf(a, b);
}

SHZ_FORCE_INLINE float shz_fmaxf(float a, float b) SHZ_NOEXCEPT {
    return __builtin_fmaxf(a, b);
}

SHZ_FORCE_INLINE bool shz_equalf(float a, float b) SHZ_NOEXCEPT {
    return shz_equalf_abs(a, b) || shz_equalf_rel(a, b);
}

SHZ_FORCE_INLINE bool shz_equalf_abs(float a, float b) SHZ_NOEXCEPT {
    return shz_fabsf(a - b) < SHZ_FLT_EPSILON;
}

SHZ_FORCE_INLINE bool shz_equalf_rel(float a, float b) SHZ_NOEXCEPT {
    return shz_fabsf(a - b) < (SHZ_FLT_EPSILON * shz_fmaxf(shz_fabsf(a), shz_fabsf(b)));
}

SHZ_FORCE_INLINE float shz_floorf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_floorf(x);

    float result = shz_truncf(x);

    if (x < 0.0f)
        result -= 1.0f;

    return result;
}

SHZ_FORCE_INLINE float shz_ceilf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_ceilf(x);

    float result = shz_truncf(x);

    if (x > result)
        result += 1.0f;

    return result;
}

SHZ_FORCE_INLINE float shz_copysignf(float x, float y) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y))
        return __builtin_copysignf(x, y);

    x = shz_fabsf(x);

    return (y < 0.0f)? -x : x;
}

SHZ_INLINE float shz_roundf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_roundf(x);

    if(x > 0.0f) {
        float xfloor = shz_truncf(x);

        if(x - xfloor >= 0.5f)
            xfloor += 1.0f;

        return xfloor;
    } else if(x < 0.0f)
        return -shz_roundf(-x);
    else
        return x;
}

SHZ_FORCE_INLINE float shz_truncf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_truncf(x);

    return (float)(int32_t)x;
}

SHZ_FORCE_INLINE float shz_fdimf(float x, float y) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y))
        return (x > y)? x - y : 0.0f;

    return (x > y)? x - y : 0.0f;
}

SHZ_FORCE_INLINE float shz_hypotf(float x, float y) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y))
        return __builtin_hypotf(x, y);

    return shz_sqrtf((x * x) + (y * y));
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

#if SHZ_BACKEND == SHZ_SH4
    float z = shz_cbrt_magic_sh4(x);
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

    return num - shz_roundf(shz_divf(num, denom)) * denom;
}

SHZ_FORCE_INLINE float shz_fmodf(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(num) && __builtin_constant_p(denom))
        return __builtin_fmodf(num, denom);

    return num - shz_truncf(shz_divf(num, denom)) * denom;
}

SHZ_FORCE_INLINE float shz_remquof(float num, float denom, float* quot) SHZ_NOEXCEPT {
    if(__builtin_constant_p(num) && __builtin_constant_p(denom)) {
        int int_quot;
        float remainder = __builtin_remquof(num, denom, &int_quot);

        *quot = int_quot;

        return remainder;
    }

    *quot = shz_roundf(shz_divf(num, denom));

    return num - *quot * denom;
}

SHZ_FORCE_INLINE float shz_clampf(float value, float min, float max) SHZ_NOEXCEPT {
    return shz_fminf(shz_fmaxf(value, min), max);
}

SHZ_FORCE_INLINE float shz_normalizef(float current, float from, float to) SHZ_NOEXCEPT {
    float t;

    if((t = (to - from)) <= 0.0f) {
        if(t == 0.0f)
            return 1.0f;
        else
            return -shz_divf_fsrra(current - from, t);
    }

    return shz_divf_fsrra(current - from, t);
}

SHZ_FORCE_INLINE float shz_normalizef_fsrra(float current, float from, float to) SHZ_NOEXCEPT {
    return shz_divf_fsrra(current - from, to - from);
}

SHZ_FORCE_INLINE float shz_remapf(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT {
    return shz_divf(value - inputStart, inputEnd - inputStart)*(outputEnd - outputStart) + outputStart;
}

SHZ_FORCE_INLINE float shz_remapf_fsrra(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT {
     return shz_divf_fsrra(value - inputStart, inputEnd - inputStart) * (outputEnd - outputStart) + outputStart;
}

SHZ_FORCE_INLINE float shz_wrapf(float value, float min, float max) SHZ_NOEXCEPT {
    float diff = max - min;

    return value - (diff * shz_floorf(shz_divf(value - min, diff)));
}

SHZ_FORCE_INLINE float shz_wrapf_fsrra(float value, float min, float max) SHZ_NOEXCEPT {
    float diff = max - min;

    return value - (diff * shz_floorf(shz_divf_fsrra(value - min, diff)));
}

/* Only here in case we ever find a platform where the compiler is so dumb
   fabsf() isn't optimal... but in the meantime, it is.
*/
SHZ_FORCE_INLINE float shz_fabsf(float x) SHZ_NOEXCEPT {
    return __builtin_fabsf(x);
}

/* Compiler is smart enough to do the right thing regardless of flags for SH4.
   Smarter than even using the builtin (seriously). */
SHZ_FORCE_INLINE float shz_fmaf(float a, float b, float c) SHZ_NOEXCEPT {
    return a * b + c;
}

SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) SHZ_NOEXCEPT {
    return shz_fmaf(t, (b - a), a);
}

SHZ_FORCE_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) SHZ_NOEXCEPT {
    // Calculate the third barycentric coordinate
    float w = 1.0f - u - v;

    // Optional: Clamp u, v, w to the [0, 1] range if the point must be inside the triangle
    // This is where fmax can be used
    // u = fmax(0.0f, u);
    // v = fmax(0.0f, v);
    // w = fmax(0.0f, w);
    // Note: Naive clamping like this without re-normalizing can distort the interpolation
    // if the original u, v were outside the valid range.
    // The standard formula below assumes u+v+w = 1.

    // Perform the weighted average (interpolation)
    return shz_dot6f(a, b, c, w, u, v);
}

SHZ_FORCE_INLINE bool shz_quadratic_roots(float a, float b, float c,
                                          float* root1, float* root2) SHZ_NOEXCEPT {
    const float discriminant = (b * b) - (4.0f * a * c);

    // One real root
    if(discriminant == 0.0f) {
        *root1 = *root2 = shz_divf(-b, 2.0f * a);
        return true;

    // Two real roots
    } else if(discriminant > 0.0f) {
        const float denom = shz_invf(2.0f * a);

        *root1 = (-b + shz_sqrtf_fsrra(discriminant)) * denom;
        *root2 = (-b - shz_sqrtf_fsrra(discriminant)) * denom;
        return true;

    // Two imaginary roots
    } else {
        *root1 = *root2 = 0.0f;
        return false;
    }
}

SHZ_FORCE_INLINE float shz_inv_sqrtf_fsrra(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / __builtin_sqrtf(x);

#if SHZ_BACKEND == SHZ_SH4
    return shz_inv_sqrtf_fsrra_sh4(x);
#else
    return shz_inv_sqrtf_fsrra_sw(x);
#endif
}

SHZ_FORCE_INLINE float shz_inv_sqrtf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / __builtin_sqrtf(x);

    return (x == 0.0f)? 0.0f : shz_inv_sqrtf_fsrra(x);
}

SHZ_FORCE_INLINE float shz_sqrtf_fsrra(float x) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(x) * x;
}

SHZ_FORCE_INLINE float shz_sqrtf(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_sqrtf(x);

    return (x == 0.0f)? 0.0f : shz_sqrtf_fsrra(x);
}

SHZ_FORCE_INLINE float shz_invf_fsrra(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / __builtin_fabsf(x);

    return shz_inv_sqrtf_fsrra(x * x);
}

SHZ_FORCE_INLINE float shz_invf(float x) SHZ_NOEXCEPT {
    if (__builtin_constant_p(x))
        return (x == 0.0f)? 0.0f : 1.0f / x;

    float inv = shz_invf_fsrra(x);

    if (x < 0.0f)
        inv = -inv;

    return inv;
}

SHZ_FORCE_INLINE float shz_divf_fsrra(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return num * shz_invf_fsrra(denom);
}

SHZ_FORCE_INLINE float shz_divf(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return num * shz_invf(denom);
}

SHZ_FORCE_INLINE float shz_dot6f(float x1, float y1, float z1,
                                 float x2, float y2, float z2) SHZ_NOEXCEPT {

    if(__builtin_constant_p(x1) && __builtin_constant_p(y1) && __builtin_constant_p(z1) &&
       __builtin_constant_p(x2) && __builtin_constant_p(y2) && __builtin_constant_p(z2))
        return (x1 * x2) + (y1 * y2) + (z1 * z2);

#if SHZ_BACKEND == SHZ_SH4
    return shz_dot6f_sh4(x1, y1, z1, x2, y2, z2);
#else
    return shz_dot6f_sw(x1, y1, z1, x2, y2, z2);
#endif
}

SHZ_FORCE_INLINE float shz_mag_sqr3f(float x, float y, float z) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y) && __builtin_constant_p(z))
        return (x * x) + (y * y) + (z * z);

#if SHZ_BACKEND == SHZ_SH4
    return shz_mag_sqr3f_sh4(x, y, z);
#else
    return shz_mag_sqr3f_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) SHZ_NOEXCEPT {

    if(__builtin_constant_p(x1) && __builtin_constant_p(y1) && __builtin_constant_p(z1) && __builtin_constant_p(w1) &&
       __builtin_constant_p(x2) && __builtin_constant_p(y2) && __builtin_constant_p(z2) && __builtin_constant_p(w2))
        return (x1 * x2) + (y1 * y2) + (z1 * z2) + (w1 * w2);

#if SHZ_BACKEND == SHZ_SH4
    return shz_dot8f_sh4(x1, y1, z1, w1, x2, y2, z2, w2);
#else
    return shz_dot8f_sw(x1, y1, z1, w1, x2, y2, z2, w2);
#endif
}

SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x) && __builtin_constant_p(y) && __builtin_constant_p(z) && __builtin_constant_p(w))
        return (x * x) + (y * y) + (z * z) + (w * w);

#if SHZ_BACKEND == SHZ_SH4
    return shz_mag_sqr4f_sh4(x, y, z, w);
#else
    return shz_mag_sqr4f_sw(x, y, z, w);
#endif
}

SHZ_FORCE_INLINE float shz_log2f(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return __builtin_log2f(x);

    const union {
        float    f;
        uint32_t i;
    } vx = { x };

    const float y = (float)(vx.i) * 1.1920928955078125e-7f;

    return y - 126.94269504f;
}

SHZ_FORCE_INLINE float shz_log10f(float x) SHZ_NOEXCEPT {
    if(__builtin_constant_p(x))
        return __builtin_log10f(x);

    return shz_log2f(x) * 0.3010299956639812f;
}

SHZ_FORCE_INLINE float shz_logf(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return __builtin_logf(x);

    return 0.69314718f * shz_log2f(x);
}

// https://github.com/appleseedhq/appleseed/blob/master/src/appleseed/foundation/math/fastmath.h
SHZ_FORCE_INLINE float shz_pow2f(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return __builtin_powf(2.0f, p);

    // Underflow of exponential is common practice in numerical routines, so handle it here.
    const float clipp = p < -126.0f ? -126.0f : p;

    const union {
        uint32_t i;
        float    f;
    } v = {
        (uint32_t)((1 << 23) * (clipp + 126.94269504f))
    };

    return v.f;
}

SHZ_FORCE_INLINE float shz_pow10f(float p) SHZ_NOEXCEPT {
    if(__builtin_constant_p(p))
        return __builtin_pow10f(p);

    return shz_expf(2.302585092994046f * p);
}

SHZ_FORCE_INLINE float shz_powf(float x, float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x) && __builtin_constant_p(p))
        return __builtin_powf(x, p);

    return shz_pow2f(p * shz_log2f(x));
}

SHZ_FORCE_INLINE float shz_expf(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return __builtin_expf(p);

    return shz_pow2f(1.442695040f * p);
}

SHZ_FORCE_INLINE float shz_randf(int* seed) SHZ_NOEXCEPT {
    union {
        float    f32;
        unsigned u32;
    } res;

    *seed = 0x00269ec3 + (*seed) * 0x000343fd;
    res.u32 = ((((unsigned) * seed) >> 9) | 0x3f800000);

    return res.f32;
}

SHZ_FORCE_INLINE float shz_randf_range(int* seed, float min, float max) SHZ_NOEXCEPT {
    return shz_fmaf(shz_randf(seed), max - min, min);
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
    return (x < edge) ? 0.0f : 1.0f;
}

SHZ_FORCE_INLINE float shz_smoothstepf(float x, float edge0, float edge1) SHZ_NOEXCEPT {
    if (x >= edge1) return 1.0f;
    if (x <= edge0) return 0.0f;

    float diff = edge1 - edge0;

    float inv_diff = shz_inv_sqrtf_fsrra(diff);

    float t = (x - edge0) * inv_diff;
    t *= inv_diff;
    return t * t * shz_fmaf(t, -2.0f, 3.0f);
}

SHZ_FORCE_INLINE float shz_smoothstepf_safe(float x, float edge0, float edge1) SHZ_NOEXCEPT {
    if(edge0 == edge1) {
        return shz_stepf(x, edge0);
    }

    float t = (x - edge0) / (edge1 - edge0);
    t = shz_clampf(t, 0.0f, 1.0f);
    return t * t * shz_fmaf(t, -2.0f, 3.0f);
}

//! \endcond
