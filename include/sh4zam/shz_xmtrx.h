/*! \file
 *  \brief   Matrix Stack API.
 *  \ingroup xmtrx
 *
 *  This file provides an API built around manipulating and performing
 *  calculations using the SH4's "current" 4x4 matrix, which is held within
 *  a secondary back-bank of 16 single-precision floating-point registers.
 *
 *  \author Falco Girgis
 *
 *  \todo
 *      - Stack operations
 *      - Fourier transforms
 *      - Arbitrarily-sized matrix routines
 *
 */
#ifndef SHZ_XMTRX_H
#define SHZ_XMTRX_H

#include "shz_matrix.h"
#include "shz_fpscr.h"

/*! \defgroup xmtrx
 *  \brief    Matrix Stack
 *
 */

SHZ_DECLS_BEGIN

typedef enum shz_xmtrx_reg {
    SHZ_XMTRX_XF0,
    SHZ_XMTRX_XF1,
    SHZ_XMTRX_XF2,
    SHZ_XMTRX_XF3,
    SHZ_XMTRX_XF4,
    SHZ_XMTRX_XF5,
    SHZ_XMTRX_XF6,
    SHZ_XMTRX_XF7,
    SHZ_XMTRX_XF8,
    SHZ_XMTRX_XF9,
    SHZ_XMTRX_XF10,
    SHZ_XMTRX_XF11,
    SHZ_XMTRX_XF12,
    SHZ_XMTRX_XF13,
    SHZ_XMTRX_XF14,
    SHZ_XMTRX_XF15
} shz_xmtrx_reg_t;

SHZ_INLINE float shz_xmtrx_read_reg(shz_xmtrx_reg_t xf) {
    float value;

    SHZ_FRCHG();
    switch(xf) {
    case SHZ_XMTRX_XF0:  asm volatile("flds fr0,  fpul"); break;
    case SHZ_XMTRX_XF1:  asm volatile("flds fr1,  fpul"); break;
    case SHZ_XMTRX_XF2:  asm volatile("flds fr2,  fpul"); break;
    case SHZ_XMTRX_XF3:  asm volatile("flds fr3,  fpul"); break;
    case SHZ_XMTRX_XF4:  asm volatile("flds fr4,  fpul"); break;
    case SHZ_XMTRX_XF5:  asm volatile("flds fr5,  fpul"); break;
    case SHZ_XMTRX_XF6:  asm volatile("flds fr6,  fpul"); break;
    case SHZ_XMTRX_XF7:  asm volatile("flds fr7,  fpul"); break;
    case SHZ_XMTRX_XF8:  asm volatile("flds fr8,  fpul"); break;
    case SHZ_XMTRX_XF9:  asm volatile("flds fr9,  fpul"); break;
    case SHZ_XMTRX_XF10: asm volatile("flds fr10, fpul"); break;
    case SHZ_XMTRX_XF11: asm volatile("flds fr11, fpul"); break;
    case SHZ_XMTRX_XF12: asm volatile("flds fr12, fpul"); break;
    case SHZ_XMTRX_XF13: asm volatile("flds fr13, fpul"); break;
    case SHZ_XMTRX_XF14: asm volatile("flds fr14, fpul"); break;
    case SHZ_XMTRX_XF15: asm volatile("flds fr15, fpul"); break;
    }
    SHZ_FRCHG();

    asm volatile("fsts fpul, %0\n" : "=f" (value) : : "fpul");
    return value;
}

SHZ_INLINE void shz_xmtrx_write_reg(shz_xmtrx_reg_t xf, float value) {
    asm volatile("flds %0, fpul\n" : : "f" (value) : "fpul");

    SHZ_FRCHG();
    switch(xf) {
    case SHZ_XMTRX_XF0:  asm volatile("fsts fpul, fr0");  break;
    case SHZ_XMTRX_XF1:  asm volatile("fsts fpul, fr1");  break;
    case SHZ_XMTRX_XF2:  asm volatile("fsts fpul, fr2");  break;
    case SHZ_XMTRX_XF3:  asm volatile("fsts fpul, fr3");  break;
    case SHZ_XMTRX_XF4:  asm volatile("fsts fpul, fr4");  break;
    case SHZ_XMTRX_XF5:  asm volatile("fsts fpul, fr5");  break;
    case SHZ_XMTRX_XF6:  asm volatile("fsts fpul, fr6");  break;
    case SHZ_XMTRX_XF7:  asm volatile("fsts fpul, fr7");  break;
    case SHZ_XMTRX_XF8:  asm volatile("fsts fpul, fr8");  break;
    case SHZ_XMTRX_XF9:  asm volatile("fsts fpul, fr9");  break;
    case SHZ_XMTRX_XF10: asm volatile("fsts fpul, fr10"); break;
    case SHZ_XMTRX_XF11: asm volatile("fsts fpul, fr11"); break;
    case SHZ_XMTRX_XF12: asm volatile("fsts fpul, fr12"); break;
    case SHZ_XMTRX_XF13: asm volatile("fsts fpul, fr13"); break;
    case SHZ_XMTRX_XF14: asm volatile("fsts fpul, fr14"); break;
    case SHZ_XMTRX_XF15: asm volatile("fsts fpul, fr15"); break;
    }
    SHZ_FRCHG();
}

SHZ_INLINE void shz_xmtrx_load_4x4(const shz_matrix_4x4_t *matrix) {
    asm volatile(R"(
        fschg
        fmov.d  @%[mtx], xd0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-(32-8), %[mtx]
        fmov.d  @%[mtx]+, xd2
        fmov.d  @%[mtx]+, xd4
        fmov.d  @%[mtx]+, xd6
        fmov.d  @%[mtx]+, xd8
        fmov.d  @%[mtx]+, xd10
        fmov.d  @%[mtx]+, xd12
        fmov.d  @%[mtx]+, xd14
        fschg
    )"
    : [mtx] "+r" (matrix)
    : "m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_load_4x4_unaligned(const float matrix[16]) {
    asm volatile(R"(
        frchg
        fmov.s  @%[mtx]+, fr0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-32, %[mtx]
        fmov.s  @%[mtx]+, fr1
        fmov.s  @%[mtx]+, fr2
        fmov.s  @%[mtx]+, fr3
        fmov.s  @%[mtx]+, fr4
        fmov.s  @%[mtx]+, fr5
        fmov.s  @%[mtx]+, fr6
        fmov.s  @%[mtx]+, fr7
        fmov.s  @%[mtx]+, fr8
        fmov.s  @%[mtx]+, fr9
        fmov.s  @%[mtx]+, fr10
        fmov.s  @%[mtx]+, fr11
        fmov.s  @%[mtx]+, fr12
        fmov.s  @%[mtx]+, fr13
        fmov.s  @%[mtx]+, fr14
        fmov.s  @%[mtx]+, fr15
        frchg
    )"
    : [mtx] "+r" (matrix)
    :  "m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_load_4x4_cols(const shz_vec4_t *c1,
                                        const shz_vec4_t *c2,
                                        const shz_vec4_t *c3,
                                        const shz_vec4_t *c4) {
    asm volatile (R"(
        frchg

        pref    @%1
        fmov.s  @%0+, fr0
        fmov.s  @%0+, fr1
        fmov.s  @%0+, fr2
        fmov.s  @%0,  fr3

        pref    @%2
        fmov.s  @%1+, fr4
        fmov.s  @%1+, fr5
        fmov.s  @%1+, fr6
        fmov.s  @%1,  fr7

        pref    @%3
        fmov.s  @%2+, fr8
        fmov.s  @%2+, fr9
        fmov.s  @%2+, fr10
        fmov.s  @%2,  fr11

        fmov.s  @%3+, fr12
        fmov.s  @%3+, fr13
        fmov.s  @%3+, fr14
        fmov.s  @%3,  fr15

        frchg
    )"
    : "+&r" (c1), "+&r" (c2), "+&r" (c3), "+&r" (c4)
    : "m" (*c1), "m" (*c2), "m" (*c3), "m" (*c4));
}

SHZ_INLINE void shz_xmtrx_load_4x4_transpose(const shz_matrix_4x4_t *matrix) {
    asm volatile(R"(
        frchg

        fmov.s  @%[mtx]+, fr0

        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-32, %[mtx]

        fmov.s  @%[mtx]+, fr4
        fmov.s  @%[mtx]+, fr8
        fmov.s  @%[mtx]+, fr12

        fmov.s  @%[mtx]+, fr1
        fmov.s  @%[mtx]+, fr5
        fmov.s  @%[mtx]+, fr9
        fmov.s  @%[mtx]+, fr13

        fmov.s  @%[mtx]+, fr2
        fmov.s  @%[mtx]+, fr6
        fmov.s  @%[mtx]+, fr10
        fmov.s  @%[mtx]+, fr14

        fmov.s  @%[mtx]+, fr3
        fmov.s  @%[mtx]+, fr7
        fmov.s  @%[mtx]+, fr11
        fmov.s  @%[mtx]+, fr15

        frchg
    )"
    : [mtx] "+r" (matrix)
    : "m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_load_4x4_apply(const shz_matrix_4x4_t *matrix1,
                                         const shz_matrix_4x4_t *matrix2)
{
    unsigned int prefetch_scratch;

    asm volatile (R"(
        mov     %[m1], %[prefscr]
        add     #32, %[prefscr]
        fschg
        pref    @%[prefscr]

        fmov.d  @%[m1]+, xd0
        fmov.d  @%[m1]+, xd2
        fmov.d  @%[m1]+, xd4
        fmov.d  @%[m1]+, xd6
        pref    @%[m1]
        fmov.d  @%[m1]+, xd8
        fmov.d  @%[m1]+, xd10
        fmov.d  @%[m1]+, xd12
        mov     %[m2], %[prefscr]
        add     #32, %[prefscr]
        fmov.d  @%[m1], xd14
        pref    @%[prefscr]

        fmov.d  @%[m2]+, dr0
        fmov.d  @%[m2]+, dr2
        fmov.d  @%[m2]+, dr4
        ftrv    xmtrx, fv0

        fmov.d  @%[m2]+, dr6
        fmov.d  @%[m2]+, dr8
        ftrv    xmtrx, fv4

        fmov.d  @%[m2]+, dr10
        fmov.d  @%[m2]+, dr12
        ftrv    xmtrx, fv8

        fmov.d  @%[m2], dr14
        fschg
        ftrv    xmtrx, fv12
        frchg
    )"
    : [m1] "+&r" (matrix1), [m2] "+r" (matrix2),
      [prefscr] "=&r" (prefetch_scratch)
    : "m" (*matrix1), "m" (*matrix2)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_load_4x4_apply_store(shz_matrix_4x4_t *out,
                                               const shz_matrix_4x4_t *matrix1,
                                               const shz_matrix_4x4_t *matrix2)
{
    unsigned int prefetch_scratch;

    asm volatile (R"(
        mov     %[m1], %[prefscr]
        add     #32, %[prefscr]
        fschg
        pref    @%[prefscr]

        fmov.d  @%[m1]+, xd0
        fmov.d  @%[m1]+, xd2
        fmov.d  @%[m1]+, xd4
        fmov.d  @%[m1]+, xd6
        pref    @%[m1]
        fmov.d  @%[m1]+, xd8
        fmov.d  @%[m1]+, xd10
        fmov.d  @%[m1]+, xd12
        mov     %[m2], %[prefscr]
        add     #32, %[prefscr]
        fmov.d  @%[m1], xd14
        pref    @%[prefscr]

        fmov.d  @%[m2]+, dr0
        fmov.d  @%[m2]+, dr2
        fmov.d  @%[m2]+, dr4
        ftrv    xmtrx, fv0

        fmov.d  @%[m2]+, dr6
        fmov.d  @%[m2]+, dr8
        ftrv    xmtrx, fv4

        fmov.d  @%[m2]+, dr10
        fmov.d  @%[m2]+, dr12
        ftrv    xmtrx, fv8

        add     #16, %[out]
        fmov.d  dr2, @-%[out]
        fmov.d  dr0,  @-%[out]

        fmov.d  @%[m2], dr14
        ftrv    xmtrx, fv12

        add     #32, %[out]
        fmov.d  dr6, @-%[out]
        fmov.d  dr4, @-%[out]

        add     #32, %[out]
        fmov.d  dr10, @-%[out]
        fmov.d  dr8, @-%[out]

        add     #32, %[out]
        fmov.d  dr14, @-%[out]
        fmov.d  dr12, @-%[out]

        fschg
    )"
    : [m1] "+&r" (matrix1), [m2] "+r" (matrix2), "=m" (*out),
      [prefscr] "=&r" (prefetch_scratch)
    : [out] "r" (out), "m" (*matrix1), "m" (*matrix2)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_load_3x4(const shz_matrix_3x4_t *matrix) {
    asm volatile(R"(
        frchg

        fmov.s  @%[mat]+, fr0
        fmov.s  @%[mat]+, fr1
        fmov.s  @%[mat]+, fr2
        fldi0   fr3

        fmov.s  @%[mat]+, fr4
        fmov.s  @%[mat]+, fr5
        fmov.s  @%[mat]+, fr6
        fldi0   fr7

        fmov.s  @%[mat]+, fr8
        fmov.s  @%[mat]+, fr9
        fmov.s  @%[mat]+, fr10
        fldi0   fr11

        fmov.s  @%[mat]+, fr12
        fmov.s  @%[mat]+, fr13
        fmov.s  @%[mat], fr14
        fldi1   fr15

        frchg
    )"
    : [mat] "+r" (matrix));
}

inline static void shz_xmtrx_load_3x4_cols(const shz_vec4_t* c1,
                                           const shz_vec4_t* c2,
                                           const shz_vec4_t* c3) {
    asm volatile(R"(
        pref    @%0
        frchg

        fldi0   fr12
        fldi0   fr13
        fldi0   fr14
        fldi1   fr15

        pref    @%1
        fmov.s  @%0+, fr0
        fmov.s  @%0+, fr1
        fmov.s  @%0+, fr2
        fmov.s  @%0,  fr3

        pref    @%2
        fmov.s  @%1+, fr4
        fmov.s  @%1+, fr5
        fmov.s  @%1+, fr6
        fmov.s  @%1,  fr7

        fmov.s  @%2+, fr8
        fmov.s  @%2+, fr9
        fmov.s  @%2+, fr10
        fmov.s  @%2,  fr11

        frchg
    )"
    : "+&r"(c1), "+&r"(c2), "+&r"(c3)
    : "m" (*c1), "m" (*c2), "m" (*c3));
}

SHZ_INLINE void shz_xmtrx_load_3x4_rows(const shz_vec4_t *r1,
                                        const shz_vec4_t *r2,
                                        const shz_vec4_t *r3) {
    asm volatile(R"(
        pref    @%0
        frchg

        fldi0   fr3
        fldi0   fr7
        fldi0   fr11
        fldi1   fr15

        pref    @%1
        fmov.s  @%0+, fr0
        fmov.s  @%0+, fr4
        fmov.s  @%0+, fr8
        fmov.s  @%0, fr12

        pref    @%2
        fmov.s  @%1+, fr1
        fmov.s  @%1+, fr5
        fmov.s  @%1+, fr9
        fmov.s  @%1, fr13

        pref    @%3
        fmov.s  @%2+, fr2
        fmov.s  @%2+, fr6
        fmov.s  @%2+, fr10
        fmov.s  @%2, fr14

        frchg
    )"
    : "+&r" (r1), "+&r" (r2), "+&r" (r3)
    : "m" (*r1), "m" (*r2), "m" (*r3));
}

SHZ_INLINE void shz_xmtrx_load_3x3(const shz_matrix_3x3_t *matrix) {
    asm volatile(R"(
        frchg

        fmov.s  @%[mat]+, fr0
        fldi0   fr3
        fmov.s  @%[mat]+, fr1
        fldi0   fr12
        fmov.s  @%[mat]+, fr2

        fmov.s  @%[mat]+, fr4
        fldi0   fr7
        fmov.s  @%[mat]+, fr5
        fldi0   fr13
        fmov.s  @%[mat]+, fr6

        fmov.s  @%[mat]+, fr8
        fldi0   fr11
        fmov.s  @%[mat]+, fr9
        fldi0   fr14
        fmov.s  @%[mat], fr10
        fldi1   fr15

        frchg
    )"
    : [mat] "+r" (matrix)
    : "m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_load_3x3_transpose(const float *matrix) {
    asm volatile(R"(
        frchg

        fmov.s  @%[mat]+, fr0
        fldi0   fr3
        fmov.s  @%[mat]+, fr4
        fldi0   fr12
        fmov.s  @%[mat]+, fr8

        fmov.s  @%[mat]+, fr1
        fldi0   fr7
        fmov.s  @%[mat]+, fr5
        fldi0   fr13
        fmov.s  @%[mat]+, fr9

        fmov.s  @%[mat]+, fr2
        fldi0   fr11
        fmov.s  @%[mat]+, fr6
        fldi0   fr14
        fmov.s  @%[mat], fr10
        fldi1   fr15

        frchg
    )"
    : [mat] "+r" (matrix)
    : "m" (matrix[0]), "m" (matrix[1]), "m" (matrix[2]),
      "m" (matrix[3]), "m" (matrix[4]), "m" (matrix[5]),
      "m" (matrix[6]), "m" (matrix[7]), "m" (matrix[8]));
}

SHZ_INLINE void shz_xmtrx_load_2x2(const shz_matrix_2x2_t *matrix) {
    asm volatile(R"(
        pref    @%[mat]
        frchg

        fldi0   fr2
        fldi1   fr10
        fmov    fr2, fr3
        fldi1   fr15

        fschg
        fmov    dr2, dr6
        fmov    dr2, dr8
        fmov    dr2, dr12

        fldi0   fr11
        fmov.d  @%[mat]+, dr0

        fldi0   fr14
        fmov.d  @%[mat], dr4

        fschg
        frchg
    )"
    : [mat] "+r" (matrix));
}

SHZ_INLINE void shz_xmtrx_store_4x4(shz_matrix_4x4_t *matrix) {
    asm volatile(R"(
        fschg
        add     #64, %[mtx]
        fmov.d  xd14, @-%[mtx]
        add     #-32, %[mtx]
        pref    @%[mtx]
        add     #32, %[mtx]
        fmov.d  xd12, @-%[mtx]
        fmov.d  xd10, @-%[mtx]
        fmov.d  xd8, @-%[mtx]
        fmov.d  xd6, @-%[mtx]
        fmov.d  xd4, @-%[mtx]
        fmov.d  xd2, @-%[mtx]
        fmov.d  xd0, @-%[mtx]
        fschg
    )"
    : [mtx] "+&r" (matrix), "=m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_store_4x4_unaligned(float matrix[16]) {
    asm volatile(R"(
        frchg
        add     #64, %[mtx]
        fmov.s  fr15, @-%[mtx]
        add     #-32, %[mtx]
        pref    @%[mtx]
        add     #32, %[mtx]
        fmov.s  fr14, @-%[mtx]
        fmov.s  fr13, @-%[mtx]
        fmov.s  fr12, @-%[mtx]
        fmov.s  fr11, @-%[mtx]
        fmov.s  fr10, @-%[mtx]
        fmov.s  fr9, @-%[mtx]
        fmov.s  fr8, @-%[mtx]
        fmov.s  fr7, @-%[mtx]
        fmov.s  fr6, @-%[mtx]
        fmov.s  fr5, @-%[mtx]
        fmov.s  fr4, @-%[mtx]
        fmov.s  fr3, @-%[mtx]
        fmov.s  fr2, @-%[mtx]
        fmov.s  fr1, @-%[mtx]
        fmov.s  fr0, @-%[mtx]
        frchg
    )"
    : "=m" (*matrix)
    : [mtx] "r" (matrix));
}

SHZ_INLINE void shz_xmtrx_store_4x4_transpose(shz_matrix_4x4_t *matrix) {
    asm volatile(R"(
        frchg
        add     #64-8, %[mtx]
        fmov.s  fr15, @%[mtx]
        add     #-32, %[mtx]
        pref    @%[mtx]
        add     #32, %[mtx]
        fmov.s  fr11, @-%[mtx]
        fmov.s  fr7, @-%[mtx]
        fmov.s  fr3, @-%[mtx]
        fmov.s  fr14, @-%[mtx]
        fmov.s  fr10, @-%[mtx]
        fmov.s  fr6, @-%[mtx]
        fmov.s  fr2, @-%[mtx]
        fmov.s  fr13, @-%[mtx]
        fmov.s  fr9, @-%[mtx]
        fmov.s  fr5, @-%[mtx]
        fmov.s  fr1, @-%[mtx]
        fmov.s  fr12, @-%[mtx]
        fmov.s  fr8, @-%[mtx]
        fmov.s  fr4, @-%[mtx]
        fmov.s  fr0, @-%[mtx]
        frchg
    )"
    : [mtx] "+&r" (matrix), "=m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_store_3x4(shz_matrix_3x4_t *matrix) {
    asm volatile(R"(
        frchg
        add     #48, %[mtx]

        fmov.s  fr14, @-%[mtx]
        fmov.s  fr13, @-%[mtx]
        fmov.s  fr12, @-%[mtx]

        fmov.s  fr10, @-%[mtx]
        fmov.s  fr9, @-%[mtx]
        fmov.s  fr8, @-%[mtx]

        fmov.s  fr6, @-%[mtx]
        fmov.s  fr5, @-%[mtx]
        fmov.s  fr4, @-%[mtx]

        fmov.s  fr2, @-%[mtx]
        fmov.s  fr1, @-%[mtx]
        fmov.s  fr0, @%[mtx]

        frchg
    )"
    : [mtx] "+&r" (matrix), "=m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_store_3x3(shz_matrix_3x3_t *matrix) {
    asm volatile(R"(
        frchg
        add     #36, %[mtx]

        fmov.s  fr10, @-%[mtx]
        fmov.s  fr9, @-%[mtx]
        fmov.s  fr8, @-%[mtx]

        fmov.s  fr6, @-%[mtx]
        fmov.s  fr5, @-%[mtx]
        fmov.s  fr4, @-%[mtx]

        fmov.s  fr2, @-%[mtx]
        fmov.s  fr1, @-%[mtx]
        fmov.s  fr0, @-%[mtx]

        frchg
    )"
    : "=m" (*matrix)
    : [mtx] "r" (matrix));
}

SHZ_INLINE void shz_xmtrx_store_3x3_transpose(shz_matrix_3x3_t *matrix) {
    asm volatile(R"(
        frchg
        add     #36, %[mtx]

        fmov.s  fr10, @-%[mtx]
        fmov.s  fr6, @-%[mtx]
        fmov.s  fr2, @-%[mtx]

        fmov.s  fr9, @-%[mtx]
        fmov.s  fr5, @-%[mtx]
        fmov.s  fr1, @-%[mtx]

        fmov.s  fr8, @-%[mtx]
        fmov.s  fr4, @-%[mtx]
        fmov.s  fr0, @-%[mtx]

        frchg
    )"
    : "=m" (*matrix)
    : [mtx] "r" (matrix));
}

SHZ_INLINE void shz_xmtrx_store_2x2(shz_matrix_2x2_t *matrix) {
    asm volatile(R"(
        pref    @%[mtx]
        frchg
        fschg

        add     #16, %[mtx]
        fmov.d  dr4, @-%[mtx]
        fmov.d  dr0, @-%[mtx]

        fschg
        frchg
    )"
    : [mtx] "+&r" (matrix), "=m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_init_identity(void) {
    asm volatile(R"(
        frchg
        fldi1   fr0
        fschg
        fldi0   fr1
        fldi0   fr2
        fldi0   fr3
        fldi0   fr4
        fldi1   fr5
        fmov    dr2, dr6
        fmov    dr2, dr8
        fldi1   fr10
        fldi0   fr11
        fmov    dr2, dr12
        fmov    dr4, dr14
        fschg
        frchg
    )");
}

SHZ_INLINE void shz_xmtrx_init_diagonal(float x, float y, float z, float w) {
    asm volatile(R"(
        frchg
        fldi0   fr1
        fschg
        fldi0   fr2
        fldi0   fr3
        fldi0   fr4
        fmov    dr2, dr6
        fmov    dr2, dr8
        fldi0   fr11
        fmov    dr2, dr12
        fldi0   fr14
        fschg
        fmov.s  @%[x], fr0
        fmov.s  @%[y], fr5
        fmov.s  @%[z], fr10
        fmov.s  @%[w], fr15
        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z), [w] "r" (&w),
      "m" (x), "m" (y), "m" (z), "m" (w));
}

SHZ_FORCE_INLINE void shz_xmtrx_init_scale(float x, float y, float z) {
    shz_xmtrx_init_diagonal(x, y, z, 1.0f);
}

SHZ_INLINE void shz_xmtrx_init_rotation_x(float x) {
    x *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        ftrc    %0, fpul
        frchg
        fsca    fpul, dr0
        fldi0   fr2
        fldi0   fr3
        fldi0   fr4
        fmov    fr1, fr5
        fmov    fr0, fr6
        fldi0   fr7
        fldi0   fr8
        fmov    fr0, fr9
        fneg    fr9
        fmov    fr1, fr10
        fldi0   fr11
        fldi0   fr12
        fldi0   fr13
        fldi0   fr14
        fldi1   fr15
        fldi1   fr0
        fldi0   fr1
        frchg
    )"
    :
    : "f" (x)
    : "fpul");
}

SHZ_INLINE void shz_xmtrx_init_rotation_y(float y) {
    y *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        ftrc    %0, fpul
        frchg
        fsca    fpul, dr0
        fmov    fr0, fr2
        fneg    fr2
        fldi0   fr3
        fldi0   fr4
        fldi1   fr5
        fldi0   fr6
        fldi0   fr7
        fmov    fr0, fr8
        fldi0   fr9
        fmov    fr1, fr10
        fldi0   fr11
        fldi0   fr12
        fldi0   fr13
        fldi0   fr14
        fldi1   fr15
        fmov    fr1, fr0
        fldi0   fr1
        frchg
    )"
    :
    : "f" (y)
    : "fpul");
}

SHZ_INLINE void shz_xmtrx_init_rotation_z(float z) {
    z *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        ftrc    %0, fpul
        frchg
        fsca    fpul, dr4
        fldi0   fr2
        fldi0   fr3
        fmov    fr5, fr0
        fmov    fr4, fr1
        fneg    fr4
        fschg
        fmov    dr2, dr6
        fmov    dr2, dr8
        fldi1   fr10
        fldi0   fr11
        fmov    dr2, dr12
        fldi0   fr14
        fldi1   fr15
        fschg
        frchg
    )"
    :
    : "f" (z)
    : "fpul");
}

SHZ_INLINE void shz_xmtrx_init_translation(float x, float y, float z) {
    asm volatile(R"(
        frchg
        fldi1   fr0
        fschg
        fldi0   fr1
        fldi0   fr2
        fldi0   fr3
        fldi0   fr4
        fldi1   fr5
        fmov    dr2, dr6
        fmov    dr2, dr8
        fschg
        fldi1   fr10
        fldi0   fr11
        fmov.s  @%[x], fr12
        fmov.s  @%[y], fr13
        fmov.s  @%[z], fr14
        fldi1   fr15
        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z));
}

SHZ_INLINE void shz_xmtrx_init_symmetric_skew(float x, float y, float z) {
    asm volatile(R"(
        frchg

        fldi0   fr0
        fmov.s  @%[z], fr1
        fmov.s  @%[y], fr2
        fldi0   fr3

        fmov    fr1, fr4
        fldi0   fr5
        fmov.s  @%[x], fr6
        fldi0   fr7

        fmov    fr2, fr8
        fmov    fr6, fr9
        fldi0   fr10
        fldi0   fr11

        fldi0   fr12
        fneg    fr1
        fldi0   fr13
        fneg    fr6
        fldi0   fr14
        fneg    fr8
        fldi1   fr15

        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z));
}

void shz_xmtrx_set_frustum(float left, float right, float bottom, float top, float near, float far);
void shz_xmtrx_set_orthographic(float left, float right, float bottom, float top);
void shz_xmtrx_set_perspective(float fovy, float aspect, float znear, float zfar);

SHZ_INLINE void shz_xmtrx_apply_4x4(const shz_matrix_4x4_t *matrix) {
    asm volatile(R"(
        mov     r15, r0
        pref    @%[mtx]
        or      #0x0f, r0
        xor     #0x0f, r0
        mov     r15, r7
        fschg
        mov     r0, r15

        fmov.d  dr14, @-r15
        fmov.d  dr12, @-r15

        fmov.d  @%[mtx], dr0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-(32-8), %[mtx]
        fmov.d  @%[mtx]+, dr2
        fmov.d  @%[mtx]+, dr4
        fmov.d  @%[mtx]+, dr6

        ftrv    xmtrx, fv0

        fmov.d  @%[mtx]+, dr8
        fmov.d  @%[mtx]+, dr10

        ftrv    xmtrx, fv4

        fmov.d  @%[mtx]+, dr12
        fmov.d  @%[mtx], dr14

        ftrv    xmtrx, fv8
        ftrv    xmtrx, fv12

        frchg
        fmov.d  @r15+, dr12
        fmov.d  @r15, dr14

        mov     r7, r15
        fschg
    )"
    : [mtx] "+r" (matrix)
    : "m" (*matrix)
    : "r0", "r7", "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6",
      "fr7", "fr8", "fr9", "fr10", "fr11", "fr12");
}

SHZ_INLINE void shz_xmtrx_apply_3x4(const shz_matrix_3x4_t *matrix) {
    asm volatile(R"(
        pref    @%[mtx]
        fldi0   fr3
        fldi0   fr7
        fldi0   fr11
        fldi1   fr15

        fmov.s  @%[mtx], fr0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-(32-4), %[mtx]
        fmov.s  @%[mtx]+, fr1
        fmov.s  @%[mtx]+, fr2

        fmov.s  @%[mtx]+, fr4
        fmov.s  @%[mtx]+, fr5
        fmov.s  @%[mtx]+, fr6

        ftrv    xmtrx, fv0

        fmov.s  @%[mtx]+, fr8
        fmov.s  @%[mtx]+, fr9
        fmov.s  @%[mtx]+, fr10

        ftrv    xmtrx, fv4

        fmov.s  @%[mtx]+, fr12
        fmov.s  @%[mtx]+, fr13
        fmov.s  @%[mtx], fr14

        ftrv    xmtrx, fv8
        ftrv    xmtrx, fv12

        frchg
    )"
    : [mtx] "+r" (matrix)
    :
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_apply_3x3(const shz_matrix_3x3_t *matrix) {
    asm volatile(R"(
        fmov.s  @%[mtx], fr0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-(32-4), %[mtx]
        fmov.s  @%[mtx]+, fr1
        fmov.s  @%[mtx]+, fr2
        fldi0   fr3

        fmov.s  @%[mtx]+, fr4
        fmov.s  @%[mtx]+, fr5
        fmov.s  @%[mtx]+, fr6
        fldi0   fr7

        ftrv    xmtrx, fv0

        fmov.s  @%[mtx]+, fr8
        fmov.s  @%[mtx]+, fr9
        fmov.s  @%[mtx]+, fr10
        fldi0   fr11

        ftrv    xmtrx, fv4

        fschg
        fmov    xd12, dr12
        fmov    xd14, dr14
        fschg

        ftrv    xmtrx, fv8

        frchg
    )"
    : [mtx] "+r" (matrix)
    : "m" (*matrix)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_apply_4x4_unaligned(const float matrix[16]) {
    asm volatile(R"(
        mov     r15, r0
        pref    @%[mtx]
        or      #0x0f, r0
        xor     #0x0f, r0
        mov     r15, r7

        fschg
        mov     r0, r15
        fmov.d  dr14, @-r15
        fmov.d  dr12, @-r15
        fschg

        fmov.s  @%[mtx]+, fr0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-32, %[mtx]
        fmov.s  @%[mtx]+, fr1
        fmov.s  @%[mtx]+, fr2
        fmov.s  @%[mtx]+, fr3

        ftrv    xmtrx, fv0

        fmov.s  @%[mtx]+, fr4
        fmov.s  @%[mtx]+, fr5
        fmov.s  @%[mtx]+, fr6
        fmov.s  @%[mtx]+, fr7

        ftrv    xmtrx, fv4

        fmov.s  @%[mtx]+, fr8
        fmov.s  @%[mtx]+, fr9
        fmov.s  @%[mtx]+, fr10
        fmov.s  @%[mtx]+, fr11

        ftrv    xmtrx, fv8

        fmov.s  @%[mtx]+, fr12
        fmov.s  @%[mtx]+, fr13
        fmov.s  @%[mtx]+, fr14
        fmov.s  @%[mtx]+, fr15

        ftrv    xmtrx, fv12

        frchg

        fschg
        fmov.d  @r15+, dr12
        fmov.d  @r15, dr14
        mov     r7, r15
        fschg
    )"
    : [mtx] "+r" (matrix)
    :  "m" (*matrix)
    : "r0", "r7", "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6",
      "fr7", "fr8", "fr9", "fr10", "fr11", "fr12");
}

void shz_xmtrx_apply_2x2(const shz_matrix_2x2_t *matrix) {
    asm volatile(R"(
        pref    @%[mtx]
        fschg

        fldi0   fr2
        fldi0   fr3
        fmov    dr2, dr6

        fmov.d  @%[mtx]+, dr0
        fmov.d  @%[mtx]+, dr4

        ftrv    xmtrx, fv0
        ftrv    xmtrx, fv4

        fmov    dr0, xd0
        fmov    dr2, xd2
        fmov    dr4, xd4
        fmov    dr6, xd6

        fschg
    )"
    : [mtx] "+r" (matrix)
    :
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7");
}

SHZ_FORCE_INLINE void shz_xmtrx_set_translation(float x, float y, float z) {
    asm volatile(R"(
        frchg

        fmov.s  @%[x], fr12
        fmov.s  @%[y], fr13
        fmov.s  @%[z], fr14

        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z)
    );
}

SHZ_INLINE void shz_xmtrx_apply_translation(float x, float y, float z) {
    asm volatile(R"(
        fschg
        fmov    xd0, dr4
        fmov    xd2, dr6
        fschg

        frchg
        fmov.s  @%[x], fr0
        fmov.s  @%[y], fr1
        fmov.s  @%[z], fr2

        fadd    fr0, fr12
        fadd    fr1, fr13
        fadd    fr2, fr14
        frchg

        fschg
        fmov    dr4, xd0
        fmov    dr6, xd2
        fschg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z)
    : "fr4", "fr5", "fr6", "fr7");
}

SHZ_INLINE void shz_xmtrx_apply_scale(float x, float y, float z) {
        asm volatile(R"(
        fschg
        fmov    xd12, dr4
        fmov    xd14, dr6
        fschg

        frchg
        fmov.s  @%[x], fr12
        fmov.s  @%[y], fr13
        fmov.s  @%[z], fr14

        fmul    fr12, fr0
        fmul    fr12, fr1
        fmul    fr12, fr2

        fmul    fr13, fr4
        fmul    fr13, fr5
        fmul    fr13, fr6

        fmul    fr14, fr8
        fmul    fr14, fr9
        fmul    fr14, fr10
        frchg

        fschg
        fmov    dr4, xd12
        fmov    dr6, xd14
        fschg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z)
    : "fr4", "fr5", "fr6", "fr7");
}

SHZ_INLINE void shz_xmtrx_apply_rotation_x(float x) {
    x *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        ftrc    %0, fpul

        fldi0   fr8
        fldi0   fr11
        fsca    fpul, dr4
        fmov    fr4, fr9
        fneg    fr9
        fmov    fr5, fr10
        ftrv    xmtrx, fv8

        fmov    fr4, fr6
        fldi0   fr7
        fldi0   fr4
        ftrv    xmtrx, fv4

        fschg
        fmov    dr8, xd8
        fmov    dr10, xd10
        fmov    dr4, xd4
        fmov    dr6, xd6
        fschg
    )"
    :
    : "f" (x)
    : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11", "fpul");
}

SHZ_INLINE void shz_xmtrx_apply_rotation_y(float y) {
    y *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        ftrc    %0, fpul

        fldi0   fr9
        fsca    fpul, dr6
        fldi0   fr11
        fmov    fr6, fr8
        fmov    fr7, fr10
        ftrv    xmtrx, fv8

        fmov    fr7, fr4
        fldi0   fr5
        fneg    fr6
        fldi0   fr7
        ftrv    xmtrx, fv4

        fschg
        fmov    dr8, xd8
        fmov    dr10, xd10
        fmov    dr4, xd0
        fmov    dr6, xd2
        fschg
    )"
    :
    : "f" (y)
    : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11", "fpul");
}

SHZ_INLINE void shz_xmtrx_apply_rotation_z(float z) {
    z *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        ftrc    %0, fpul

        fldi0   fr11
        fsca    fpul, dr8
        fldi0   fr10
        fmov    fr8, fr5
        fneg    fr8
        ftrv    xmtrx, fv8

        fmov    fr9, fr4
        fschg
        fmov    dr10, dr6
        ftrv    xmtrx, fv4

        fmov    dr8, xd4
        fmov    dr10, xd6
        fmov    dr4, xd0
        fmov    dr6, xd2
        fschg
    )"
    :
    : "f" (z)
    : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11", "fpul");
}

SHZ_INLINE void shz_xmtrx_init_rotation(float roll, float pitch, float yaw) {
    shz_xmtrx_init_rotation_x(roll);
    shz_xmtrx_apply_rotation_y(pitch);
    shz_xmtrx_apply_rotation_z(yaw);
}

SHZ_INLINE void shz_xmtrx_apply_rotation(float roll, float pitch, float yaw) {
    shz_xmtrx_apply_rotation_x(roll);
    shz_xmtrx_apply_rotation_y(pitch);
    shz_xmtrx_apply_rotation_z(yaw);
}

void shz_xmtrx_outer_product_2x2(shz_vec2_t col, shz_vec2_t row);
void shz_xmtrx_outer_product_3x3(shz_vec3_t col, shz_vec3_t row);
void shz_xmtrx_outer_product_3x4(shz_vec4_t col, shz_vec3_t row);
void shz_xmtrx_outer_product_4x4(shz_vec4_t col, shz_vec4_t row);

void shz_xmtrx_invert(void);
void shz_xmtrx_invert_full(void);
float shz_xmtrx_determinant(void);

SHZ_INLINE void shz_xmtrx_transpose(void) {
    asm volatile (R"(
        frchg

        flds    fr1, fpul
        fmov    fr4, fr1
        fsts    fpul, fr4

        flds    fr2, fpul
        fmov    fr8, fr2
        fsts    fpul, fr8

        flds    fr3, fpul
        fmov    fr12, fr3
        fsts    fpul, fr12

        flds    fr6, fpul
        fmov    fr9, fr6
        fsts    fpul, fr9

        flds    fr7, fpul
        fmov    fr13, fr7
        fsts    fpul, fr13

        flds    fr11, fpul
        fmov    fr14, fr11
        fsts    fpul, fr14

        frchg
    )"
    :
    :
    : "fpul");
}

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_trans_vec4(shz_vec4_t vec) {
    register float rx asm("fr8")  = vec.x;
    register float ry asm("fr9")  = vec.y;
    register float rz asm("fr10") = vec.z;
    register float rw asm("fr11") = vec.w;

    asm volatile("ftrv xmtrx, fv8"
                 : "+f" (rx), "+f" (ry), "+f" (rz), "+f" (rw));

    return (shz_vec4_t) { .x = rx, .y = ry, .z = rz, .w = rw };
}

SHZ_FORCE_INLINE shz_vec3_t shz_xmtrx_trans_vec3(shz_vec3_t vec) {
    return shz_xmtrx_trans_vec4((shz_vec4_t) { .vec3 = vec }).vec3;
}

SHZ_FORCE_INLINE shz_vec2_t shz_xmtrx_trans_vec2(shz_vec2_t vec) {
    return shz_xmtrx_trans_vec3((shz_vec3_t) { .vec2 = vec }).vec2;
}

SHZ_DECLS_END

#endif // SHZ_XMTRX_H
