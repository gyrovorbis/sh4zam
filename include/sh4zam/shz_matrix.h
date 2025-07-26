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

SHZ_INLINE shz_vec3_t shz_matrix3x3_trans_vec3(const shz_matrix_3x3_t *m, shz_vec3_t v) {
    shz_vec3_t out;

    register float fr0 asm("fr0") = v.x;
    register float fr1 asm("fr1") = v.y;
    register float fr2 asm("fr2") = v.z;
    register float fr3 asm("fr3") = 0.0f;

    register float fr4 asm("fr4") = m->elem2D[0][0];
    register float fr5 asm("fr5") = m->elem2D[0][1];
    register float fr6 asm("fr6") = m->elem2D[0][2];
    register float fr7 asm("fr7") = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    register float fr8  asm("fr8")  = m->elem2D[1][0];
    register float fr9  asm("fr9")  = m->elem2D[1][1];
    register float fr10 asm("fr10") = m->elem2D[1][2];
    register float fr11 asm("fr11") = 0.0f;

    asm volatile("fipr fv0, fv8"
        : "+f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));


    __atomic_thread_fence(1);

    out.x = fr7;

    __atomic_thread_fence(1);

    fr4 = m->elem2D[2][0];
    fr5 = m->elem2D[2][1];
    fr6 = m->elem2D[2][2];
    fr7 = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    out.y = fr11;
    out.z = fr7;

    return out;
}

SHZ_INLINE void shz_matrix_4x4_copy(shz_matrix_4x4_t *dst, const shz_matrix_4x4_t *src) {
    asm volatile(R"(
        fschg

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

        fschg
    )"
    : [dst] "+&r" (dst), [src] "+&r" (src), "=m" (*dst)
    : "m" (*src));
}

SHZ_INLINE shz_vec3_t shz_matrix4x4_trans_vec3(const shz_matrix_4x4_t *m, shz_vec3_t v) {
    shz_vec3_t out;

    register float fr0 asm("fr0") = v.x;
    register float fr1 asm("fr1") = v.y;
    register float fr2 asm("fr2") = v.z;
    register float fr3 asm("fr3") = 0.0f;

    register float fr4 asm("fr4") = m->elem2D[0][0];
    register float fr5 asm("fr5") = m->elem2D[0][1];
    register float fr6 asm("fr6") = m->elem2D[0][2];
    register float fr7 asm("fr7") = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    register float fr8  asm("fr8")  = m->elem2D[1][0];
    register float fr9  asm("fr9")  = m->elem2D[1][1];
    register float fr10 asm("fr10") = m->elem2D[1][2];
    register float fr11 asm("fr11") = 0.0f;

    asm volatile("fipr fv0, fv8"
        : "+f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));

    __atomic_thread_fence(1);

    out.x = fr7;

    __atomic_thread_fence(1);

    fr4 = m->elem2D[2][0];
    fr5 = m->elem2D[2][1];
    fr6 = m->elem2D[2][2];
    fr7 = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    out.y = fr11;
    out.z = fr7;

    return out;
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
