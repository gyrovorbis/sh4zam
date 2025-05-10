#ifndef SH4_INTRIN_H
#define SH4_INTRIN_H

#include "sh4_cdefs.h"

#define SH4_F_PI                3.1415926f
#define SH4_FSCA_RAD_FACTOR     10430.37835f

#define SH4_DEG_TO_RAD(deg)     ((deg) * SH4_F_PI / 180.0f)

#define SH4_MIN(a, b)           (((a) < (b))? (a) : (b))
#define SH4_MAX(a, b)           (((a) > (b))? (a) : (b))
#define SH4_CLAMP(v, min, max)  SH4_MIN(SH4_MAX(v, min), max)
#define SH4_NORM(v, min, max)   ((SH4_CLAMP(v, min, max) - (min)) / ((max) - (min)))

SH4_BEGIN_DECLS

SH4_FORCE_INLINE float sh4_floorf(float x) {
    float result, neg1;

    asm volatile(R"(
            fldi1   %[neg1]
            fneg    %[neg1]
            fcmp/gt %[neg1], %[in]
            ftrc    %[in], fpul
            bt.s    1f
            float   fpul, %[out]
            fadd    %[neg1], %[out]
        1:
    )"
    : [neg1] "=&f" (neg1), [out] "=f" (result)
    : [in] "f" (x)
    : "fpul", "t");

    return result;
}

SH4_FORCE_INLINE float sh4_ceilf(float x) {
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

SH4_FORCE_INLINE float sh4_fmacf(float a, float b, float c) {
    register float ra asm("fr0") = a;

    asm volatile("fmac fr0, %[b], %[c]"
                 : [c] "+f" (c)
                 : "f" (ra), [b] "f" (b));

    return c;
}

SH4_FORCE_INLINE float sh4_lerpf(float a, float b, float t) {
    return sh4_fmacf(t, (b - a), a);
}

SH4_FORCE_INLINE float sh4_sinf(float radians) {
    register float rsin asm("fr10");

    radians *= SH4_FSCA_RAD_FACTOR;

    asm volatile(R"(
        ftrc    %[radians], fpul
        fsca    fpul, dr10
    )"
    : "=f" (rsin)
    : [radians] "f" (radians)
    : "fr11", "fpul");

    return rsin;
}

SH4_FORCE_INLINE float sh4_cosf(float radians) {
    register float rcos asm("fr11");

    radians *= SH4_FSCA_RAD_FACTOR;

    asm volatile(R"(
        ftrc    %[radians], fpul
        fsca    fpul, dr10
    )"
    : "=f" (rcos)
    : [radians] "f" (radians)
    : "fr10", "fpul");

    return rcos;
}

SH4_FORCE_INLINE void sh4_sin_cosf(float radians, float *sin, float *cos) {
    register float rsin asm("fr10");
    register float rcos asm("fr11");

    radians *= SH4_FSCA_RAD_FACTOR;

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

SH4_FORCE_INLINE float sh4_tanf(float radians) {
    float s, c;
    sh4_sin_cosf(radians, &s, &c);
    return s / c;
}

SH4_FORCE_INLINE float sh4_sqrtf(float x) {
    asm volatile("fsqrt %0" : "+f" (x));
    return x;
}

SH4_FORCE_INLINE float sh4_inverse_sqrtf(float x) {
    asm volatile("fsrra %0" : "+f" (x));
    return x;
}

SH4_FORCE_INLINE float sh4_inverse_posf(float x) {
    return sh4_inverse_sqrtf(x * x);
}

SH4_FORCE_INLINE float sh4_div_posf(float num, float denom) {
    return num * sh4_inverse_posf(denom);
}

SH4_FORCE_INLINE float sh4_dot8f(float x1, float y1, float z1, float w1,
                                 float x2, float y2, float z2, float w2) {
    register float rx1 asm("fr4")  = x1;
    register float ry1 asm("fr5")  = y1;
    register float rz1 asm("fr6")  = z1;
    register float rw1 asm("fr7")  = w1;
    register float rx2 asm("fr8")  = x2;
    register float ry2 asm("fr9")  = y2;
    register float rz2 asm("fr10") = z2;
    register float rw2 asm("fr11") = w2;

    asm volatile("fipr	fv8, fv4"
                 : "+f" (rw1)
                 : "f" (rx1), "f" (ry1), "f" (rz1),
                   "f" (rx2), "f" (ry2), "f" (rz2), "f" (rw2));

    return rw1;
}

SH4_FORCE_INLINE float sh4_mag_sqr4f(float x, float y, float z, float w) {
    register float rx asm("fr4") = x;
    register float ry asm("fr5") = y;
    register float rz asm("fr6") = z;
    register float rw asm("fr7") = w;

    asm volatile("fipr fv4, fv4"
                 : "+f" (rw)
                 : "f" (rx), "f" (ry), "f" (rz));

    return rw;
}

SH4_END_DECLS

#endif // SH4_INTRIN_H
