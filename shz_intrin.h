#ifndef SHZ_INTRIN_H
#define SHZ_INTRIN_H

#include "shz_cdefs.h"

#define SHZ_F_PI                3.1415926f
#define SHZ_FSCA_RAD_FACTOR     10430.37835f

#define SHZ_DEG_TO_RAD(deg)     ((deg) * SHZ_F_PI / 180.0f)
#define SHZ_RAD_TO_DEG(rad)     ((rad) * 180.0f / SHZ_F_PI)

#define SHZ_MIN(a, b)           (((a) < (b))? (a) : (b))
#define SHZ_MAX(a, b)           (((a) > (b))? (a) : (b))
#define SHZ_CLAMP(v, min, max)  SHZ_MIN(SHZ_MAX(v, min), max)
#define SHZ_NORM(v, min, max)   ((SHZ_CLAMP(v, min, max) - (min)) / ((max) - (min)))

SHZ_BEGIN_DECLS

SHZ_FORCE_INLINE float shz_floorf(float x) {
    float result, tmp;

    asm volatile(R"(
            fldi0   %[tmp]
            fcmp/gt %[tmp], %[in]
            ftrc    %[in], fpul
            bt.s    1f
            float   fpul, %[out]
            fldi1   %[tmp]
            fneg    %[tmp]
            fadd    %[tmp], %[out]
        1:
    )"
    : [tmp] "=&f" (tmp), [out] "=f" (result)
    : [in] "f" (x)
    : "fpul", "t");

    return result;
}

#if 0
float shz_floorf2(float x) {
    float result = (float)(int)x;

    if (x < 0.0f)
        result -= 1.0f;

    return result;
}
#endif

SHZ_FORCE_INLINE float shz_ceilf(float x) {
    float result, zero_one;

    asm volatile(R"(
            fldi0   %[zero_one]
            fcmp/gt %[zero_one], %[in]
            ftrc    %[in], fpul
            bf.s    1f
            float   fpul, %[out]
            fldi1   %[zero_one]
            fadd    %[zero_one], %[out]
        1:
    )"
    : [zero_one] "=&f" (zero_one), [out] "=f" (result)
    : [in] "f" (x)
    : "fpul", "t");

    return result;
}

SHZ_FORCE_INLINE float shz_fmacf(float a, float b, float c) {
    register float ra asm("fr0") = a;

    asm volatile("fmac fr0, %[b], %[c]"
                 : [c] "+f" (c)
                 : "f" (ra), [b] "f" (b));

    return c;
}

SHZ_FORCE_INLINE float shz_lerpf(float a, float b, float t) {
    return shz_fmacf(t, (b - a), a);
}

SHZ_FORCE_INLINE float shz_sinf(float radians) {
    register float rsin asm("fr10");

    radians *= SHZ_FSCA_RAD_FACTOR;

    asm volatile(R"(
        ftrc    %[radians], fpul
        fsca    fpul, dr10
    )"
    : "=f" (rsin)
    : [radians] "f" (radians)
    : "fr11", "fpul");

    return rsin;
}

SHZ_FORCE_INLINE float shz_cosf(float radians) {
    register float rcos asm("fr11");

    radians *= SHZ_FSCA_RAD_FACTOR;

    asm volatile(R"(
        ftrc    %[radians], fpul
        fsca    fpul, dr10
    )"
    : "=f" (rcos)
    : [radians] "f" (radians)
    : "fr10", "fpul");

    return rcos;
}

SHZ_FORCE_INLINE void shz_sin_cosf(float radians, float *sin, float *cos) {
    register float rsin asm("fr10");
    register float rcos asm("fr11");

    radians *= SHZ_FSCA_RAD_FACTOR;

    asm volatile(R"(
        ftrc    %[radians], fpul
        fsca    fpul, dr10
    )"
    : "=f" (rsin), "=f" (rcos)
    : [radians] "f" (radians)
    : "fpul");

    *sin = rsin;
    *cos = rcos;
}

SHZ_FORCE_INLINE float shz_tanf(float radians) {
    float s, c;
    shz_sin_cosf(radians, &s, &c);
    return s / c;
}

SHZ_FORCE_INLINE float shz_sqrtf(float x) {
    asm volatile("fsqrt %0" : "+f" (x));
    return x;
}

SHZ_FORCE_INLINE float shz_inverse_sqrtf(float x) {
    asm volatile("fsrra %0" : "+f" (x));
    return x;
}

SHZ_FORCE_INLINE float shz_inverse_posf(float x) {
    return shz_inverse_sqrtf(x * x);
}

SHZ_FORCE_INLINE float shz_div_posf(float num, float denom) {
    return num * shz_inverse_posf(denom);
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

    asm volatile("fipr	fv0, fv4"
                 : "+f" (rw2)
                 : "f" (rx1), "f" (ry1), "f" (rz1), "f" (rw1)
                   "f" (rx2), "f" (ry2), "f" (rz2));

    return rw1;
}

SHZ_FORCE_INLINE float shz_mag_sqr4f(float x, float y, float z, float w) {
    register float rx asm("fr0") = x;
    register float ry asm("fr1") = y;
    register float rz asm("fr2") = z;
    register float rw asm("fr3") = w;

    asm volatile("fipr fv0, fv0"
                 : "+f" (rw)
                 : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

SHZ_END_DECLS

#endif // SHZ_INTRIN_H
