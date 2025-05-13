#ifndef SHZ_INTRIN_H
#define SHZ_INTRIN_H

#include "shz_cdefs.h"

#define SHZ_F_PI                3.1415926f
#define SHZ_FSCA_RAD_FACTOR     10430.37835f
#define SHZ_FSCA_DEG_FACTOR     SHZ_DEG_TO_RAD(SHZ_FSCA_RAD_FACTOR)

#define SHZ_DEG_TO_RAD(deg)     ((deg) * SHZ_F_PI / 180.0f)
#define SHZ_RAD_TO_DEG(rad)     ((rad) * 180.0f / SHZ_F_PI)

#define SHZ_MIN(a, b)           (((a) < (b))? (a) : (b))
#define SHZ_MAX(a, b)           (((a) > (b))? (a) : (b))
#define SHZ_CLAMP(v, min, max)  SHZ_MIN(SHZ_MAX(v, min), max)
#define SHZ_NORM(v, min, max)   ((SHZ_CLAMP(v, min, max) - (min)) / ((max) - (min)))

SHZ_BEGIN_DECLS

typedef struct shz_sincos {
    _Complex float f;
} shz_sincos_t;

SHZ_FORCE_INLINE float shz_floorf(float x) {
    float result = (float)(int)x;

    if (x < 0.0f)
        result -= 1.0f;

    return result;
}

SHZ_FORCE_INLINE float shz_ceilf(float x) {
    float result = (float)(int)x;

    if (x > result)
        result += 1.0f;

    return result;
}

SHZ_FORCE_INLINE float shz_fmacf(float a, float b, float c) {
    return a * b + c;
}

SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) {
    return shz_fmacf(t, (b - a), a);
}

SHZ_FORCE_INLINE float shz_barycentric_lerpf(float a, float b, float c, float u, float v) {
    return shz_fmacf(u, (b - a), shz_fmacf(v, (c - a), a));
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincos16(uint16_t radians16) {
    register _Complex float rsin;

    asm("fsca fpul, %0" : "=f" (rsin) : "y" (radians16));

    return (shz_sincos_t){ rsin };
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf(float radians) {
    return shz_sincos16(radians * SHZ_FSCA_RAD_FACTOR);
}

SHZ_FORCE_INLINE shz_sincos_t shz_sincosf_deg(float degrees) {
    return shz_sincos16(radians * SHZ_FSCA_DEG_FACTOR);
}

SHZ_FORCE_INLINE float shz_sinf(shz_sincos_t sincos) {
    return __real__ sincos.f;
}

SHZ_FORCE_INLINE float shz_cosf(shz_sincos_t sincos) {
    return __imag__ sincos.f;
}

SHZ_FORCE_INLINE float shz_sqrtf(float x) {
    return __builtin_sqrtf(x);
}

SHZ_FORCE_INLINE float shz_inverse_sqrtf(float x) {
    asm("fsrra %0" : "+f" (x));
    return x;
}

SHZ_FORCE_INLINE float shz_inverse_posf(float x) {
    return shz_inverse_sqrtf(x * x);
}

SHZ_FORCE_INLINE float shz_div_posf(float num, float denom) {
    return num * shz_inverse_posf(denom);
}

SHZ_FORCE_INLINE float shz_fast_invf(float x) {
    float inv;

    if (__builtin_constant_p(x))
        return 1.0f / x;

    inv = shz_inverse_posf(x);

    if (x < 0.0f)
        inv = -inv;

    return inv;
}

SHZ_FORCE_INLINE float shz_tanf(shz_sincos_t sincos) {
    return shz_div_posf(shz_sinf(sincos), shz_cosf(sincos));
}

SHZ_FORCE_INLINE float shz_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) {
    register float rx1 asm("fr0") = x1;
    register float ry1 asm("fr1") = y1;
    register float rz1 asm("fr2") = z1;
    register float rw1 asm("fr3") = w1;
    register float rx2 asm("fr4") = x2;
    register float ry2 asm("fr5") = y2;
    register float rz2 asm("fr6") = z2;
    register float rw2 asm("fr7") = w2;

    asm("fipr fv0, fv4"
        : "+f" (rw2)
        : "f" (rx1), "f" (ry1), "f" (rz1), "f" (rw1)
          "f" (rx2), "f" (ry2), "f" (rz2));

    return rw2;
}

SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) {
    register float rx asm("fr0") = x;
    register float ry asm("fr1") = y;
    register float rz asm("fr2") = z;
    register float rw asm("fr3") = w;

    asm("fipr fv0, fv0"
        : "+f" (rw)
        : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

SHZ_END_DECLS

#endif // SHZ_INTRIN_H
