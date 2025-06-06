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

/*! \defgroup xmtrx
 *  \brief    Matrix Stack
 *
 */

#define shz_xmtrx_set_reg(reg, value) do {      \
        const float tmp = (value);              \
        asm volatile("frchg\n"                  \
                     "fsts fpul, fr" #reg "\n"  \
                     "frchg\n"                  \
                     :                          \
                     : "y" (tmp));              \
    } while(false)

#define shz_xmtrx_get_reg(reg) ({               \
        float val;                              \
        asm volatile("frchg\n"                  \
                     "flds fr" #reg ", fpul\n"  \
                     "frchg\n"                  \
                     : "=y" (val))              \
        val;                                    \
    })

SHZ_DECLS_BEGIN

SHZ_INLINE void shz_xmtrx_load_4x4(const shz_matrix_4x4_t *matrix) {
    asm volatile(R"(
        fschg
        fmov.d	@%[mtx], xd0
        add     #32, %[mtx]
        pref	@%[mtx]
        add     #-(32-8), %[mtx]
        fmov.d	@%[mtx]+, xd2
        fmov.d	@%[mtx]+, xd4
        fmov.d	@%[mtx]+, xd6
        fmov.d	@%[mtx]+, xd8
        fmov.d	@%[mtx]+, xd10
        fmov.d	@%[mtx]+, xd12
        fmov.d	@%[mtx]+, xd14
        fschg
    )"
    : [mtx] "+r" (matrix));
}

SHZ_INLINE void shz_xmtrx_load_4x4_rows(const shz_vec4_t *r1,
                                        const shz_vec4_t *r2,
                                        const shz_vec4_t *r3,
                                        const shz_vec4_t *r4) {
    asm volatile (R"(
        frchg

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

        fmov.s  @%3+, fr3
        fmov.s  @%3+, fr7
        fmov.s  @%3+, fr11
        fmov.s  @%3, fr15

        frchg
    )"
    : "+&r" (r1), "+&r" (r2), "+&r" (r3), "+&r" (r4));
}

SHZ_INLINE void shz_xmtrx_load_4x4_transpose(const shz_matrix_4x4_t *matrix) {
    asm volatile(R"(
        frchg

        fmov.s  @%[mtx]+, fr0

        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-(32 - 4), %[mtx]

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
    : [mtx] "+r" (matrix));
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
    :
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
    : [mat] "+r" (matrix));
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
        add     #64-8, %[mtx]
        fmov.d	xd14, @%[mtx]
        add     #-32, %[mtx]
        pref    @%[mtx]
        add     #32, %[mtx]
        fmov.d	xd12, @-%[mtx]
        fmov.d	xd10, @-%[mtx]
        fmov.d	xd8, @-%[mtx]
        fmov.d	xd6, @-%[mtx]
        fmov.d	xd4, @-%[mtx]
        fmov.d	xd2, @-%[mtx]
        fmov.d	xd0, @-%[mtx]
        fschg
    )"
    : [mtx] "+&r" (matrix), "=m" (*matrix));
}

SHZ_INLINE void shz_xmtrx_store_3x4(shz_matrix_3x4_t *matrix) {
    asm volatile(R"(
        frchg
        add     #48, %[mtx]

        fmov.s	fr14, @-%[mtx]
        fmov.s  fr13, @-%[mtx]
        fmov.s  fr12, @-%[mtx]

        fmov.s	fr10, @-%[mtx]
        fmov.s  fr9, @-%[mtx]
        fmov.s  fr8, @-%[mtx]

        fmov.s	fr6, @-%[mtx]
        fmov.s  fr5, @-%[mtx]
        fmov.s  fr4, @-%[mtx]

        fmov.s	fr2, @-%[mtx]
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

        fmov.s	fr10, @-%[mtx]
        fmov.s  fr9, @-%[mtx]
        fmov.s  fr8, @-%[mtx]

        fmov.s	fr6, @-%[mtx]
        fmov.s  fr5, @-%[mtx]
        fmov.s  fr4, @-%[mtx]

        fmov.s	fr2, @-%[mtx]
        fmov.s  fr1, @-%[mtx]
        fmov.s  fr0, @%[mtx]

        frchg
    )"
    : [mtx] "+&r" (matrix), "=m" (*matrix));
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

SHZ_INLINE void shz_xmtrx_set_identity(void) {
    asm volatile(R"(
        frchg
        fldi1	fr0
        fschg
        fldi0	fr1
        fldi0	fr2
        fldi0	fr3
        fldi0	fr4
        fldi1	fr5
        fmov	dr2, dr6
        fmov	dr2, dr8
        fmov	dr0, dr10
        fmov	dr2, dr12
        fmov	dr4, dr14
        fschg
        frchg
    )");
}

SHZ_INLINE void shz_xmtrx_set_diagonal(float x, float y, float z, float w) {
    asm volatile(R"(
        frchg
        fldi0	fr1
        fschg
        fldi0	fr2
        fldi0	fr3
        fldi0	fr4
        fmov	dr2, dr6
        fmov	dr2, dr8
        fldi0	fr11
        fmov	dr2, dr12
        fldi0	fr14
        fschg
        fmov.s	@%[x], fr0
        fmov.s	@%[y], fr5
        fmov.s	@%[z], fr10
        fmov.s  @%[w], fr15
        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z), [w] "r" (&w));
}

SHZ_FORCE_INLINE void shz_xmtrx_set_scale(float x, float y, float z) {
    shz_xmtrx_set_diagonal(x, y, z, 1.0f);
}

SHZ_INLINE void shz_xmtrx_set_rotation_x(float x) {
    x *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        frchg
        fldi0   fr1
        fldi0   fr2
        fldi0   fr3
        fldi0   fr7
        fldi0   fr8
        fldi0   fr12
        fldi0   fr13
        fsca    fpul, dr0
        fldi0   fr4
        fldi0   fr11
        fldi0   fr14
        fldi1   fr15
        fmov    fr1, fr5
        fmov    fr1, fr10
        fmov    fr0, fr9
        fmov    fr0, fr6
        fneg    fr6
        fldi1   fr0
        fldi0   fr1
        frchg
    )"
    :
    : "y" (x));
}

SHZ_INLINE void shz_xmtrx_set_rotation_y(float y) {
    y *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        frchg
        fldi0	fr3
        fldi1	fr5
        fldi0	fr6
        fldi0	fr7
        fldi0	fr12
        fldi0	fr13
        fsca	fpul, dr0
        fldi0	fr4
        fldi0	fr9
        fldi0	fr11
        fldi0	fr14
        fldi1	fr15
        fmov	fr1, fr10
        fmov	fr0, fr8
        fneg	fr8
        fmov	fr0, fr2
        fmov	fr1, fr0
        fldi0	fr1
        frchg
    )"
    :
    : "y" (y));
}

SHZ_INLINE void shz_xmtrx_set_rotation_z(float z) {
    z *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        frchg
        fldi0	fr2
        fldi0	fr3
        fldi1	fr10
        fldi0	fr11
        fsca	fpul, dr4
        fschg
        fmov	dr2, dr6
        fmov	dr2, dr8
        fmov	dr2, dr12
        fldi0	fr14
        fldi1	fr15
        fschg
        fmov	fr5, fr0
        fmov	fr4, fr1
        fneg	fr1
        frchg
    )"
    :
    : "y" (z));
}

SHZ_INLINE void shz_xmtrx_set_translation(float x, float y, float z) {
    asm volatile(R"(
        frchg
        fldi1	fr0
        fschg
        fldi0	fr1
        fldi0	fr2
        fldi0	fr3
        fldi0	fr4
        fldi1	fr5
        fmov	dr2,dr6
        fmov	dr2,dr8
        fmov	dr0,dr10
        fschg
        fmov.s  @%[x], fr12
        fmov.s  @%[y], fr13
        fmov.s  @%[z], fr14
        fldi1   fr15
        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z));
}

SHZ_INLINE void shz_xmtrx_set_symmetric_skew(float x, float y, float z) {
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
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z));
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
    :
    : "r0", "r7", "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6",
      "fr7", "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
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

void shz_xmtrx_apply_3x3(const shz_matrix_3x3_t *matrix) {
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
    :
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

void shz_xmtrx_apply_2x2(const shz_matrix_2x2_t *matrix) {
    asm volatile(R"(
        pref    @%[mtx]
        fschg

        fldi0   fr2
        fldi0   fr3
        fmov    dr2, dr6

        fmov.d  @%[mtx]+, dr0
        fmov.d  @[mtx]+, dr4

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

SHZ_INLINE void shz_xmtrx_apply_translation(float x, float y, float z) {
    asm volatile(R"(
        fschg
        fmov	xd12, dr4
        fmov	xd14, dr6
        fschg

        fmov.s  @%[x], fr0
        fmov.s  @%[y], fr1
        fmov.s  @%[z], fr2

        fadd	fr0, fr4
        fadd	fr1, fr5
        fadd	fr2, fr6

        fschg
        fmov	dr4, xd12
        fmov	dr6, xd14
        fschg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z)
    : "fr0", "fr1", "fr2", "fr4", "fr5", "fr6", "fr7");
}

SHZ_INLINE void shz_xmtrx_apply_scale(float x, float y, float z) {
        asm volatile(R"(
        fschg
        fmov	xd0, dr4
        fmov	xd2, dr6
        fschg

        frchg
        fmov.s  @%[x], fr0
        fmov.s  @%[y], fr1
        fmov.s  @%[z], fr2

        fmul	fr0, fr4
        fmul	fr0, fr8
        fmul	fr0, fr12
        fmul	fr1, fr5
        fmul	fr1, fr9
        fmul	fr1, fr13
        fmul	fr2, fr6
        fmul	fr2, fr10
        fmul	fr2, fr14

        fschg
        fmov	dr4, xd0
        fmul	fr3, fr7
        fmov	dr6, xd2
        fmul	fr3, fr11
        fmov	xd4, dr4
        fmul	fr3, fr15
        fmov	xd6, dr6
        fschg

        fmul	fr4, fr0

        fmul	fr5, fr1
        fmul	fr6, fr2
        fmul	fr7, fr3

        fschg
        fmov	xd0, dr4
        fmov	xd2, dr6
        fschg

        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z)
    : "fr0", "fr1", "fr2");
}

SHZ_INLINE void shz_xmtrx_apply_rotation_x(float x) {
    x *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        fsca 	fpul, dr4
        fldi0	fr8
        fldi0	fr11
        fmov	fr5, fr10
        fmov	fr4, fr9
        fneg	fr9
        ftrv	xmtrx, fv8
        fmov	fr4, fr6
        fldi0	fr7
        fldi0	fr4
        ftrv	xmtrx, fv4
        fschg
        fmov	dr8, xd8
        fmov	dr10, xd10
        fmov	dr4, xd4
        fmov	dr6, xd6
        fschg
    )"
    :
    : "y" (x)
    : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11");
}

SHZ_INLINE void shz_xmtrx_apply_rotation_y(float y) {
    y *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        fsca    fpul, dr6
        fldi0	fr9
        fldi0	fr11
        fmov	fr6, fr8
        fmov	fr7, fr10
        ftrv	xmtrx, fv8
        fmov	fr7, fr4
        fldi0	fr5
        fneg	fr6
        fldi0	fr7
        ftrv	xmtrx, fv4
        fschg
        fmov	dr8, xd8
        fmov	dr10, xd10
        fmov	dr4, xd0
        fmov	dr6, xd2
        fschg
    )"
    :
    : "y" (y)
    : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11");
}

SHZ_INLINE void shz_xmtrx_apply_rotation_z(float z) {
    z *= SHZ_FSCA_RAD_FACTOR;
    asm volatile(R"(
        fsca    fpul, dr8
        fldi0	fr10
        fldi0	fr11
        fmov	fr8, fr5
        fneg	fr8
        ftrv	xmtrx, fv8
        fmov	fr9, fr4
        fschg
        fmov	dr10, dr6
        ftrv	xmtrx, fv4
        fmov	dr8, xd4
        fmov	dr10, xd6
        fmov	dr4, xd0
        fmov	dr6, xd2
        fschg
    )"
    :
    : "y" (z)
    : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11");
}

SHZ_INLINE void shz_xmtrx_set_rotation(float roll, float pitch, float yaw) {
    shz_xmtrx_set_rotation_x(roll);
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

        flds    fr11, fpUL
        fmov    fr14, fr11
        fsts    fpul, fr14

        frchg
    )"
    :
    :
    : "fpul");
}

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_trans_vec4(shz_vec4_t vec) {
    register float rx asm("fr0") = vec.x;
    register float ry asm("fr1") = vec.y;
    register float rz asm("fr2") = vec.z;
    register float rw asm("fr3") = vec.w;

    asm volatile("ftrv xmtrx, fv0"
                 : "+f" (rx), "+f" (ry), "+f" (rz), "+f" (rw));

    return (shz_vec4_t) { rx, ry, rz, rw };
}

SHZ_FORCE_INLINE shz_vec3_t shz_xmtrx_trans_vec3(shz_vec3_t vec) {
    return shz_xmtrx_trans_vec4((shz_vec4_t) { .vec3 = vec }).vec3;
}

SHZ_FORCE_INLINE shz_vec2_t shz_xmtrx_trans_vec2(shz_vec2_t vec) {
    return shz_xmtrx_trans_vec3((shz_vec3_t) { .vec2 = vec }).vec2;
}

SHZ_DECLS_END

#endif // SHZ_XMTRX_H
