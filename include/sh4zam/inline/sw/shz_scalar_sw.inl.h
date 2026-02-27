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

#include <assert.h>
#include <math.h>

SHZ_FORCE_INLINE float shz_fminf_sw(float a, float b) SHZ_NOEXCEPT {
    return __builtin_fminf(a, b);
}

SHZ_FORCE_INLINE float shz_fmaxf_sw(float a, float b) SHZ_NOEXCEPT {
    return __builtin_fmaxf(a, b);
}

SHZ_FORCE_INLINE bool shz_equalf_sw(float a, float b) SHZ_NOEXCEPT {
    return shz_equalf_abs(a, b) || shz_equalf_rel(a, b);
}

SHZ_FORCE_INLINE bool shz_equalf_abs_sw(float a, float b) SHZ_NOEXCEPT {
    return shz_fabsf(a - b) < SHZ_FLT_EPSILON;
}

SHZ_FORCE_INLINE bool shz_equalf_rel_sw(float a, float b) SHZ_NOEXCEPT {
    return shz_fabsf(a - b) < (SHZ_FLT_EPSILON * shz_fmaxf(shz_fabsf(a), shz_fabsf(b)));
}

 SHZ_FORCE_INLINE float shz_floorf_sw(float x) SHZ_NOEXCEPT {
    float result = shz_truncf(x);

    if (x < 0.0f)
        result -= 1.0f;

    return result;
}

SHZ_FORCE_INLINE float shz_ceilf_sw(float x) SHZ_NOEXCEPT {
    float result = shz_truncf(x);

    if (x > result)
        result += 1.0f;

    return result;
}

SHZ_FORCE_INLINE float shz_copysignf_sw(float x, float y) SHZ_NOEXCEPT {
    x = shz_fabsf(x);

    return (y < 0.0f)? -x : x;
}

SHZ_INLINE float shz_roundf_sw(float x) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE float shz_truncf_sw(float x) SHZ_NOEXCEPT {
    return (float)(int32_t)x;
}

SHZ_FORCE_INLINE float shz_fdimf_sw(float x, float y) SHZ_NOEXCEPT {
    return (x > y)? x - y : 0.0f;
}

SHZ_FORCE_INLINE float shz_hypotf_sw(float x, float y) SHZ_NOEXCEPT {
    return shz_sqrtf((x * x) + (y * y));
}

SHZ_FORCE_INLINE float shz_remainderf_sw(float num, float denom) SHZ_NOEXCEPT {
    return num - shz_roundf(shz_divf(num, denom)) * denom;
}

SHZ_FORCE_INLINE float shz_fmodf_sw(float num, float denom) SHZ_NOEXCEPT {
    return num - shz_truncf(shz_divf(num, denom)) * denom;
}

SHZ_FORCE_INLINE float shz_remquof_sw(float num, float denom, float* quot) SHZ_NOEXCEPT {
    *quot = shz_roundf(shz_divf(num, denom));

    return num - *quot * denom;
}

SHZ_FORCE_INLINE float shz_clampf_sw(float value, float min, float max) SHZ_NOEXCEPT {
    return shz_fminf(shz_fmaxf(value, min), max);
}

SHZ_FORCE_INLINE float shz_normalizef_sw(float current, float from, float to) SHZ_NOEXCEPT {
    float t;

    if((t = (to - from)) <= 0.0f) {
        if(t == 0.0f)
            return 1.0f;
        else
            return -shz_divf_fsrra(current - from, t);
    }

    return shz_divf_fsrra(current - from, t);
}

SHZ_FORCE_INLINE float shz_normalizef_fsrra_sw(float current, float from, float to) SHZ_NOEXCEPT {
    return shz_divf_fsrra(current - from, to - from);
}

SHZ_FORCE_INLINE float shz_remapf_sw(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT {
    return shz_divf(value - inputStart, inputEnd - inputStart)*(outputEnd - outputStart) + outputStart;
}

SHZ_FORCE_INLINE float shz_remapf_fsrra_sw(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) SHZ_NOEXCEPT {
    return shz_divf_fsrra(value - inputStart, inputEnd - inputStart) * (outputEnd - outputStart) + outputStart;
}

SHZ_FORCE_INLINE float shz_wrapf_sw(float value, float min, float max) SHZ_NOEXCEPT {
    float diff = max - min;

    return value - (diff * shz_floorf(shz_divf(value - min, diff)));
}

SHZ_FORCE_INLINE float shz_wrapf_fsrra_sw(float value, float min, float max) SHZ_NOEXCEPT {
    float diff = max - min;

    return value - (diff * shz_floorf(shz_divf_fsrra(value - min, diff)));
}

SHZ_FORCE_INLINE float shz_fabsf_sw(float x) SHZ_NOEXCEPT {
    return __builtin_fabsf(x);
}

/* Compiler is smart enough to do the right thing regardless of flags for SH4.
   Smarter than even using the builtin (seriously). */
SHZ_FORCE_INLINE float shz_fmaf_sw(float a, float b, float c) SHZ_NOEXCEPT {
    return a * b + c;
}

SHZ_FORCE_INLINE float shz_lerpf_sw(float a, float b, float t) SHZ_NOEXCEPT {
    return shz_fmaf(t, (b - a), a);
}

SHZ_FORCE_INLINE float shz_barycentric_lerpf_sw(float a, float b, float c, float u, float v) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE bool shz_quadratic_roots_sw(float a, float b, float c,
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

SHZ_FORCE_INLINE float shz_inv_sqrtf_fsrra_sw(float x) SHZ_NOEXCEPT {
    return 1.0f / sqrtf(x);
}

SHZ_FORCE_INLINE float shz_inv_sqrtf_sw(float x) SHZ_NOEXCEPT {
    return (x == 0.0f)? 0.0f : shz_inv_sqrtf_fsrra(x);
}

SHZ_FORCE_INLINE float shz_sqrtf_fsrra_sw(float x) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(x) * x;
}

SHZ_FORCE_INLINE float shz_sqrtf_sw(float x) SHZ_NOEXCEPT {
    return (x == 0.0f)? 0.0f : shz_sqrtf_fsrra(x);
}

SHZ_FORCE_INLINE float shz_invf_fsrra_sw(float x) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(x * x);
}

SHZ_FORCE_INLINE float shz_invf_sw(float x) SHZ_NOEXCEPT {
    float inv = shz_invf_fsrra(x);

    if (x < 0.0f)
        inv = -inv;

    return inv;
}

SHZ_FORCE_INLINE float shz_divf_fsrra_sw(float num, float denom) SHZ_NOEXCEPT {
    return num * shz_invf_fsrra(denom);
}

SHZ_FORCE_INLINE float shz_divf_sw(float num, float denom) SHZ_NOEXCEPT {
    return num * shz_invf(denom);
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

// https://github.com/appleseedhq/appleseed/blob/master/src/appleseed/foundation/math/fastmath.h
SHZ_FORCE_INLINE float shz_pow2f_sw(float p) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE float shz_log2f_sw(float x) SHZ_NOEXCEPT {
    const union {
        float    f;
        uint32_t i;
    } vx = { x };

    const float y = (float)(vx.i) * 1.1920928955078125e-7f;

    return y - 126.94269504f;
}

SHZ_FORCE_INLINE float shz_logf_sw(float x) SHZ_NOEXCEPT {
    return 0.69314718f * shz_log2f(x);
}

SHZ_FORCE_INLINE float shz_log10f_sw(float x) SHZ_NOEXCEPT {
    return shz_log2f(x) * 0.3010299956639812f;
}

SHZ_FORCE_INLINE float shz_powf_sw(float x, float p) SHZ_NOEXCEPT {
    return shz_pow2f(p * shz_log2f(x));
}

SHZ_FORCE_INLINE float shz_pow10f_sw(float x) SHZ_NOEXCEPT {
    return shz_expf(2.302585092994046f * x);
}

SHZ_FORCE_INLINE float shz_expf_sw(float p) SHZ_NOEXCEPT {
    return shz_pow2f(1.442695040f * p);
}

// https://iquilezles.org/articles/sfrand/
// \todo I can hand-write the ASM better than GCC here. --Falco
SHZ_FORCE_INLINE float shz_randf_sw(int* seed) SHZ_NOEXCEPT {
    union {
        float    f32;
        unsigned u32;
    } res;

    *seed = 0x00269ec3 + (*seed) * 0x000343fd;
    res.u32 = ((((unsigned) * seed) >> 9) | 0x3f800000);

    return res.f32;
}

SHZ_FORCE_INLINE float shz_randf_range_sw(int* seed, float min, float max) SHZ_NOEXCEPT {
    return shz_fmaf(shz_randf(seed), max - min, min);
}

SHZ_FORCE_INLINE float shz_stepf_sw(float x, float edge) SHZ_NOEXCEPT {
    return (x < edge) ? 0.0f : 1.0f;
}

SHZ_FORCE_INLINE float shz_smoothstepf_sw(float x, float edge0, float edge1) SHZ_NOEXCEPT {
    if (x >= edge1) return 1.0f;
    if (x <= edge0) return 0.0f;

    float diff = edge1 - edge0;

    float inv_diff = shz_inv_sqrtf_fsrra(diff);

    float t = (x - edge0) * inv_diff;
    t *= inv_diff;
    return t * t * shz_fmaf(t, -2.0f, 3.0f);
}

SHZ_FORCE_INLINE float shz_smoothstepf_safe_sw(float x, float edge0, float edge1) SHZ_NOEXCEPT {
    if(edge0 == edge1) {
        return shz_stepf(x, edge0);
    }

    float t = (x - edge0) / (edge1 - edge0);
    t = shz_clampf(t, 0.0f, 1.0f);
    return t * t * shz_fmaf(t, -2.0f, 3.0f);
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