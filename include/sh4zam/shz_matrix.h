#ifndef SHZ_MATRIX_H
#define SHZ_MATRIX_H

#include "shz_vector.h"

SHZ_DECLS_BEGIN

typedef SHZ_ALIGNAS(8) union shz_matrix_2x2 {
    float       elem[4];
    float       elem2D[2][2];
    shz_vec2_t  col[2];
} shz_matrix_2x2_t;

typedef union shz_matrix_3x3 {
    float      elem[9];
    float      elem2D[3][3];
    shz_vec3_t col[3];
    struct {
        shz_vec3_t left;
        shz_vec3_t up;
        shz_vec3_t forward;
    };
} shz_matrix_3x3_t;

typedef union shz_matrix_3x4 {
    float      elem[12];
    float      elem2D[3][4];
    shz_vec3_t col[4];
    struct {
        shz_vec3_t left;
        shz_vec3_t up;
        shz_vec3_t forward;
        shz_vec3_t pos;
    };
} shz_matrix_3x4_t;

typedef SHZ_ALIGNAS(8) union shz_matrix_4x4 {
    float      elem[16];
    float      elem2D[4][4];
    shz_vec4_t col[4];
    struct {
        shz_vec4_t left;
        shz_vec4_t up;
        shz_vec4_t forward;
        shz_vec4_t pos;
    };
} shz_matrix_4x4_t;

SHZ_INLINE void shz_matrix_4x4_copy(shz_matrix_4x4 *dst, const shz_matrix_4x4 *src) {
    asm volatile(R"(
        fschg
        frchg

        pref    @%[dst]
        fmov.d  @%[src]+, xd0
        fmov.d  @%[src]+, xd2
        fmov.d  @%[src]+, xd4
        fmov.d  @%[src]+, xd6

        pref    @%[src]
        add     #32, %[dst]

        fmov.d  xd6, @-%[dst]
        fmov.d  xd4, @-%[dst]
        fmov.d  xd2, @-%[dst]
        fmov.d  xd0, @-%[dst]

        add     #32, %[dst]
        pref    @%[dst]

        fmov.d  @%[src]+, xd0
        fmov.d  @%[src]+, xd2
        fmov.d  @%[src]+, xd4
        fmov.d  @%[src]+, xd6

        add     #32, %[dst]
        fmov.d  xd6, @-%[dst]
        fmov.d  xd4, @-%[dst]
        fmov.d  xd2, @-%[dst]
        fmov.d  xd0, @-%[dst]

        frchg
        fschg
    )"
    : [dst] "+&r" (dst), [src] "+&r" (src), "=m" (*dst));
}

SHZ_INLINE shz_vec4_t shz_matrix_4x4_trans_vec4(const shz_matrix_4x4_t *mat, shz_vec4_t in) {
    SHZ_PREFETCH(mat);

    const shz_vec4_t *c[4] = {
        &mat->col[0], &mat->col[1], &mat->col[2], &mat->col[3]
    };

    asm volatile(R"(
        frchg

        .irp    reg, 12, 13, 14, 15
            fmov.s  @%[v]+, fr\reg
        .endr

        pref    @%[c2]

        .irp    reg, 0, 1, 2, 3
            fmov.s  @%[c\reg]+, fr\reg
        .endr

        fipr    fv12, fv0

        fmov.s  @%[c0]+, fr4
        fmov.s  @%[c1]+, fr5
        fmov.s  @%[c2]+, fr6
        fmov.s  @%[c2]+, fr7

        fipr    fv12, fv4

        fmov.s  @%[c0]+, fr8
        fmov.s  @%[c1]+, fr9
        fmov.s  @%[c2]+, fr10
        fmov.s  @%[c2]+, fr11

        fipr    fv12, fv8
        fmov.s  fr3, @%[v]

        .irp    reg, 0, 1, 2, 3
            fmov.s  @%[c\reg]+, fr\reg
        .endr

        fipr    fv12, fv0
        add     #4, @%[v]
        fmov.s  fr7, @%[v]
        add     #4, %[v]
        fmov.s  fr11, @%[v]
        add     #4, %[v]
        fmov.s  fr3, @%[v]

        frchg
    )"
    : [v] "+r" (&in),
      [c0] "+r" (c[0]), [c1] "+r" (c[1]), [c2] "+r" (c[2]), [c3], "+r" (c[3]));
}

SHZ_DECLS_END

#endif // SHZ_MATRIX_H
