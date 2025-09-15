//! \cond INTERNAL
/*! \file
 *  \brief General-purpose scalar math routines.
 *  \ingroup scalar
 *
 *  This file provides the inlined implementation of the scalar API.
 *
 *  \author    Falco Girgis
 *  \author    Paul Cercueil
 *  \copyright MIT License
 */

 SHZ_FORCE_INLINE float shz_floorf(float x) SHZ_NOEXCEPT {
    float result = (float)(int)x;

    if (x < 0.0f)
        result -= 1.0f;

    return result;
}

SHZ_FORCE_INLINE float shz_ceilf(float x) SHZ_NOEXCEPT {
    float result = (float)(int)x;

    if (x > result)
        result += 1.0f;

    return result;
}

SHZ_FAST_MATH SHZ_FORCE_INLINE float shz_fmacf(float a, float b, float c) SHZ_NOEXCEPT {
#if 1 // Trust the compiler?
    return a * b + c;
#else // FUCK NO?
    asm volatile ("fmac fr0, %[b], %[c]\n"
    : [c] "+&f" (c)
    : "w" (a), [b] "f" (b));
  return c;
#endif
}

SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) SHZ_NOEXCEPT {
    return shz_fmacf(t, (b - a), a);
}

SHZ_FORCE_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) SHZ_NOEXCEPT {
    return shz_fmacf(u, (b - a), shz_fmacf(v, (c - a), a));
}

SHZ_FORCE_INLINE float shz_inv_sqrtf(float x) SHZ_NOEXCEPT {
    asm volatile("fsrra %0" : "+f" (x));
    return x;
}

SHZ_FORCE_INLINE float shz_sqrtf_fsrra(float x) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(x) * x;
}

SHZ_FORCE_INLINE float shz_invf_fsrra(float x) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(x * x);
}

SHZ_FAST_MATH SHZ_FORCE_INLINE float shz_sqrtf(float x) SHZ_NOEXCEPT {
    // Call the compiler built-in explicitly in case -fno-builtin is used.
    return __builtin_sqrtf(x);
}

SHZ_FORCE_INLINE float shz_invf(float x) SHZ_NOEXCEPT {
    float inv;

    if (__builtin_constant_p(x))
        return 1.0f / x;

    inv = shz_invf_fsrra(x);

    if (x < 0.0f)
        inv = -inv;

    return inv;
}

SHZ_FORCE_INLINE float shz_divf(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return num * shz_invf(denom);
}

SHZ_FORCE_INLINE float shz_divf_fsrra(float num, float denom) SHZ_NOEXCEPT {
    if(__builtin_constant_p(denom))
        return num / denom;
    else
        return num * shz_invf_fsrra(denom);
}

SHZ_FORCE_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) SHZ_NOEXCEPT {
    register float rx1 asm("fr8")  = x1;
    register float ry1 asm("fr9")  = y1;
    register float rz1 asm("fr10") = z1;
    register float rw1 asm("fr11") = w1;
    register float rx2 asm("fr12") = x2;
    register float ry2 asm("fr13") = y2;
    register float rz2 asm("fr14") = z2;
    register float rw2 asm("fr15") = w2;

    // Undefined behavior when in another mode with FIPR
    SHZ_SINGLE_PRECISION_GUARD();

    asm("fipr fv8, fv12"
        : "+f" (rw2)
        : "f" (rx1), "f" (ry1), "f" (rz1), "f" (rw1),
          "f" (rx2), "f" (ry2), "f" (rz2));

    return rw2;
}

SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) SHZ_NOEXCEPT {
    register float rx asm("fr8")  = x;
    register float ry asm("fr9")  = y;
    register float rz asm("fr10") = z;
    register float rw asm("fr11") = w;

     // Undefined behavior when in another mode with FIPR
    SHZ_SINGLE_PRECISION_GUARD();

    asm("fipr fv8, fv8"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

// https://github.com/appleseedhq/appleseed/blob/master/src/appleseed/foundation/math/fastmath.h
SHZ_FORCE_INLINE float shz_pow2f(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return powf(2.0f, p);

    // Underflow of exponential is common practice in numerical routines, so handle it here.
    const float clipp = p < -126.0f ? -126.0f : p;
    const union { uint32_t i; float f; } v = {
        (uint32_t)((1 << 23) * (clipp + 126.94269504f))
    };

    return v.f;
}

SHZ_FORCE_INLINE float shz_log2f(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return log2f(x);

    assert(x >= 0.0f);

    const union { float f; uint32_t i; } vx = { x };
    const float y = (float) (vx.i) * 1.1920928955078125e-7f;

    return y - 126.94269504f;
}

SHZ_FORCE_INLINE float shz_logf(float x) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x))
        return logf(x);

    return 0.69314718f * shz_log2f(x);
}

SHZ_FORCE_INLINE float shz_powf(float x, float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(x) && __builtin_constant_p(p))
        return powf(x, p);

    return shz_pow2f(p * shz_log2f(x));
}

SHZ_FORCE_INLINE float shz_expf(float p) SHZ_NOEXCEPT {
    // Let GCC compute statically if compile-time constant.
    if(__builtin_constant_p(p))
        return expf(p);

    return shz_pow2f(1.442695040f * p);
}

//! \endcond