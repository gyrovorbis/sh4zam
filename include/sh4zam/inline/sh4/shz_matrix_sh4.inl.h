//! \cond INTERNAL
/*! \file
 *  \brief   SH4 Matrix API Implementation.
 *  \ingroup matrix
 *
 *  This file contains the low-level SH4 implementation of the Matrix API.
 *
 *  \author 2026 Falco Girgis
 *
 *  \copyright MIT License
 */

#ifndef SHZ_MATRIX_SH4_INL_H
#define SHZ_MATRIX_SH4_INL_H

SHZ_FORCE_INLINE shz_vec3_t shz_mat4x4_transform_vec3_sh4(const shz_mat4x4_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
    register float vx asm("fr4") = v.x;
    register float vy asm("fr5") = v.y;
    register float vz asm("fr6") = v.z;
    register float ox asm("fr8");
    register float oy asm("fr11");
    register float oz asm("fr3");

    asm(R"(
        fmov.s    @%[m]+, fr0
        add       #28, %[m]
        pref      @%[m]
        add       #-28, %[m]
        fldi0     fr7
        fmov.s    @%[m]+, fr8
        add       #8, %[m]
        fmov.s    @%[m]+, fr1
        fmov.s    @%[m]+, fr9
        add       #8, %[m]
        fldi0     fr3
        fmov.s    @%[m]+, fr2
        fmov.s    @%[m]+, fr10
        fldi0     fr11
        add       #-32, %[m]
        fipr      fv4, fv0
        fmov.s    @%[m], fr0
        add       #16, %[m]
        fmov.s    @%[m], fr1
        fipr      fv4, fv8
        add       #16, %[m]
        fmov.s    @%[m], fr2
        fmov      fr3, fr8
        fldi0     fr3
        fipr      fv4, fv0
    )"
    : [m] "+&r" (mat), "=&f" (ox), "=&f" (oy), "=&f" (oz)
    : "f" (vx), "f" (vy), "f" (vz), "m" (*mat)
    : "fr0", "fr1", "fr2", "fr7", "fr9", "fr10");

    return shz_vec3_init(ox, oy, oz);
}

SHZ_FORCE_INLINE shz_vec4_t shz_mat4x4_transform_vec4_sh4(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT {
    register float vx asm("fr4") = in.x;
    register float vy asm("fr5") = in.y;
    register float vz asm("fr6") = in.z;
    register float vw asm("fr7") = in.w;
    register float ox asm("fr12");
    register float oy asm("fr13");
    register float oz asm("fr3");
    register float ow asm("fr11");

    asm(R"(
        fmov.s    @%[m]+, fr0
        add       #28, %[m]
        pref      @%[m]
        add       #-28, %[m]
        fmov.s    @%[m]+, fr8
        add       #8, %[m]
        fmov.s    @%[m]+, fr1
        fmov.s    @%[m]+, fr9
        add       #8, %[m]
        fmov.s    @%[m]+, fr2
        fmov.s    @%[m]+, fr10
        add       #8, %[m]
        fmov.s    @%[m]+, fr3
        fmov.s    @%[m]+, fr11
        add       #-48, %[m]
        fipr      fv4, fv0
        fmov.s    @%[m]+, fr0
        fipr      fv4, fv8
        fmov.s    @%[m]+, fr8
        add       #8, %[m]
        fmov.s    @%[m]+, fr1
        fmov.s    @%[m]+, fr9
        add       #8, %[m]
        fmov.s    @%[m]+, fr2
        fmov.s    @%[m]+, fr10
        fmov      fr3, fr12
        add       #8, %[m]
        fmov      fr11, fr13
        fmov.s    @%[m]+, fr3
        fmov.s    @%[m]+, fr11
        fipr      fv4, fv0
        add       #-64, %[m]
        fipr      fv4, fv8
    )"
    : "=&f" (ox), "=&f" (oy), "=&f" (oz), "=&f" (ow)
    : "m" (*mat), [m] "r" (mat), "f" (vx), "f" (vy), "f" (vz), "f" (vw)
    : "fr0", "fr1", "fr2", "fr8", "fr9", "fr10");

    return shz_vec4_init(ox, oy, oz, ow);
}

SHZ_INLINE shz_vec4_t shz_mat4x4_transform_vec4_transpose_sh4(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT {
    SHZ_PREFETCH(mat);

    shz_vec4_t* v = &in;
    const shz_vec4_t* c[4] = {
        &mat->col[0], &mat->col[1], &mat->col[2], &mat->col[3]
    };

    asm(R"(
        ! Load input vector into FV12
        fmov.s  @%[v]+, fr12
        fmov.s  @%[v]+, fr13
        fmov.s  @%[v]+, fr14
        fmov.s  @%[v]+, fr15

        ! Prefetch the second half of the matrix
        pref    @%[c2]

        ! Load first column into FV0
        fmov.s  @%[c0]+, fr0
        fmov.s  @%[c0]+, fr1
        fmov.s  @%[c0]+, fr2
        fmov.s  @%[c0]+, fr3
        ! Start loading next column
        fmov.s  @%[c1]+, fr4   ! Vector instructions need 3 cycles between
        fmov.s  @%[c1]+, fr5   ! loading arguments and using them.

        ! Calculate output vector's X component
        fipr    fv12, fv0

        ! Finish loading second column vector
        fmov.s  @%[c1]+, fr6
        fmov.s  @%[c1]+, fr7
        ! Begin loading third column vector
        fmov.s  @%[c2]+, fr8
        fmov.s  @%[c2]+, fr9

        ! Calculate output vector's Y componennt
        fipr    fv12, fv4

        ! Finish loading third column vector
        fmov.s  @%[c2]+, fr10
        add     #-16, %[v]      ! Point v back to the beginning of the input vector
        fmov.s  @%[c2]+, fr11
        fmov.s  fr3, @%[v]      ! Store output vector X component
        ! Start loading fourth column vector
        fmov.s  @%[c3]+, fr0

        ! Calculate output vector's Z component
        fipr    fv12, fv8

        ! Finish loading the fourth column vector
        fmov.s  @%[c3]+, fr1
        fmov.s  @%[c3]+, fr2
        fmov.s  @%[c3]+, fr3
        add     #4, %[v]        ! Advance output vector pointer
        fmov.s  fr7, @%[v]      ! Store output vector Y component

        ! Calculate output vector's W component
        fipr    fv12, fv0       ! FUCKING STALL - 4th column vector is still loading (3 cycle delay)

        ! Store output vector's Z component
        add     #4, %[v]        ! Advance output vector pointer
        fmov.s  fr11, @%[v]

        ! Store output vector's W component
        add     #4, %[v]        ! Advance output vector pointer
        fmov.s  fr3, @%[v]      ! FUCKING STALL - previous FIPR still in pipeline!
    )"
    : [v] "+r" (v), "=m" (in),
      [c0] "+r" (c[0]), [c1] "+r" (c[1]), [c2] "+r" (c[2]), [c3] "+r" (c[3])
    : "m" (in), "m" (*c[0]), "m" (*c[1]), "m" (*c[2]), "m" (*c[3])
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");

    return in;
}

SHZ_INLINE shz_vec3_t shz_mat4x4_transform_vec3_transpose_sh4(const shz_mat4x4_t* m, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec3_t out;

    register float fr0 asm("fr0") = v.x;
    register float fr1 asm("fr1") = v.y;
    register float fr2 asm("fr2") = v.z;
    register float fr3 asm("fr3") = 0.0f;

    register float fr4 asm("fr4") = m->elem2D[0][0];
    register float fr5 asm("fr5") = m->elem2D[0][1];
    register float fr6 asm("fr6") = m->elem2D[0][2];
    register float fr7 asm("fr7");

    register float fr8  asm("fr8");
    register float fr9  asm("fr9");
    register float fr10 asm("fr10");
    register float fr11 asm("fr11");

    asm("fipr fv0, fv4"
        : "=f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    SHZ_MEMORY_BARRIER_SOFT();

    fr8  = m->elem2D[1][0];
    fr9  = m->elem2D[1][1];
    fr10 = m->elem2D[1][2];

    asm("fipr fv0, fv8"
        : "=f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));

    SHZ_MEMORY_BARRIER_SOFT();

    out.x = fr7;

    SHZ_MEMORY_BARRIER_SOFT();

    fr4 = m->elem2D[2][0];
    fr5 = m->elem2D[2][1];
    fr6 = m->elem2D[2][2];

    asm("fipr fv0, fv4"
        : "=f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    SHZ_MEMORY_BARRIER_SOFT();

    out.y = fr11;
    out.z = fr7;

    return out;
}

SHZ_INLINE void shz_mat4x4_copy_sh4(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
    asm volatile(R"(
        fschg

        fmov.d  @%[src]+, dr0
        pref    @%[dst]
        fmov.d  @%[src]+, dr2
        fmov.d  @%[src]+, dr4
        add     #32, %[dst]
        fmov.d  @%[src]+, dr6

        fmov.d  dr6, @-%[dst]
        fmov.d  dr4, @-%[dst]
        fmov.d  dr2, @-%[dst]
        fmov.d  dr0, @-%[dst]

        add     #32, %[dst]
        fmov.d  @%[src]+, dr0
        pref    @%[dst]
        fmov.d  @%[src]+, dr2
        fmov.d  @%[src]+, dr4
        fmov.d  @%[src]+, dr6

        add     #32, %[dst]
        fmov.d  dr6, @-%[dst]
        fmov.d  dr4, @-%[dst]
        fmov.d  dr2, @-%[dst]
        fmov.d  dr0, @-%[dst]

        fschg
    )"
    : [dst] "+&r" (dst), [src] "+&r" (src), "=m" (*dst)
    : "m" (*src)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7");
}

SHZ_INLINE void shz_mat4x4_swap_sh4(shz_mat4x4_t* matA, shz_mat4x4_t* matB) SHZ_NOEXCEPT {
    asm volatile(R"(
        fschg

        fmov.d  @%[m1]+, xd0
        pref    @%[m2]
        fmov.d  @%[m1]+, xd2
        fmov.d  @%[m1]+, xd4
        fmov.d  @%[m1]+, xd6

        fmov.d  @%[m2]+, xd8
        pref    @%[m1]
        fmov.d  @%[m2]+, xd10
        fmov.d  @%[m2]+, xd12
        fmov.d  @%[m2]+, xd14

        fmov.d  xd14, @-%[m1]
        fmov.d  xd12, @-%[m1]
        fmov.d  xd10, @-%[m1]
        fmov.d  xd8, @-%[m1]
        add     #32, %[m1]

        fmov.d  xd6, @-%[m2]
        fmov.d  xd4, @-%[m2]
        fmov.d  xd2, @-%[m2]
        fmov.d  xd0, @-%[m2]
        add     #32, %[m2]

        fmov.d  @%[m1]+, xd0
        pref    @%[m2]
        fmov.d  @%[m1]+, xd2
        fmov.d  @%[m1]+, xd4
        fmov.d  @%[m1]+, xd6

        fmov.d  @%[m2]+, xd8
        fmov.d  @%[m2]+, xd10
        fmov.d  @%[m2]+, xd12
        fmov.d  @%[m2]+, xd14

        fmov.d  xd14, @-%[m1]
        fmov.d  xd12, @-%[m1]
        fmov.d  xd10, @-%[m1]
        fmov.d  xd8, @-%[m1]

        fmov.d  xd6, @-%[m2]
        fmov.d  xd4, @-%[m2]
        fmov.d  xd2, @-%[m2]
        fmov.d  xd0, @-%[m2]

        fschg
    )"
    : [m1] "+&r" (matA), [m2] "+&r" (matB),
      "+m" (*matA), "+m" (*matB));
}

SHZ_INLINE shz_vec3_t shz_mat3x3_transform_vec3_sh4(const shz_mat3x3_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec3_t out;

    register float fr0 asm("fr0") = v.x;
    register float fr1 asm("fr1") = v.y;
    register float fr2 asm("fr2") = v.z;
    register float fr3 asm("fr3") = 0.0f;

    register float fr4 asm("fr4") = mat->elem2D[0][0];
    register float fr5 asm("fr5") = mat->elem2D[1][0];
    register float fr6 asm("fr6") = mat->elem2D[2][0];
    register float fr7 asm("fr7");

    register float fr8  asm("fr8");
    register float fr9  asm("fr9");
    register float fr10 asm("fr10");
    register float fr11 asm("fr11");

    asm("fipr fv0, fv4"
        : "=f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    SHZ_MEMORY_BARRIER_SOFT();

    fr8  = mat->elem2D[0][1];
    fr9  = mat->elem2D[1][1];
    fr10 = mat->elem2D[2][1];

    asm("fipr fv0, fv8"
        : "=f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));

    SHZ_MEMORY_BARRIER_SOFT();

    out.x = fr7;

    SHZ_MEMORY_BARRIER_SOFT();

    fr4 = mat->elem2D[0][2];
    fr5 = mat->elem2D[1][2];
    fr6 = mat->elem2D[2][2];

    asm("fipr fv0, fv4"
        : "=f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    SHZ_MEMORY_BARRIER_SOFT();

    out.y = fr11;
    out.z = fr7;

    return out;
}

SHZ_INLINE shz_vec3_t shz_mat3x3_transform_vec3_transpose_sh4(const shz_mat3x3_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec3_t out;

    register float fr0 asm("fr0") = v.x;
    register float fr1 asm("fr1") = v.y;
    register float fr2 asm("fr2") = v.z;
    register float fr3 asm("fr3") = 0.0f;

    register float fr4 asm("fr4") = mat->elem2D[0][0];
    register float fr5 asm("fr5") = mat->elem2D[0][1];
    register float fr6 asm("fr6") = mat->elem2D[0][2];
    register float fr7 asm("fr7");

    register float fr8  asm("fr8");
    register float fr9  asm("fr9");
    register float fr10 asm("fr10");
    register float fr11 asm("fr11");

    asm("fipr fv0, fv4"
        : "=f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    SHZ_MEMORY_BARRIER_SOFT();

    fr8  = mat->elem2D[1][0];
    fr9  = mat->elem2D[1][1];
    fr10 = mat->elem2D[1][2];

    asm("fipr fv0, fv8"
        : "=f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));

    SHZ_MEMORY_BARRIER_SOFT();

    out.x = fr7;

    SHZ_MEMORY_BARRIER_SOFT();

    fr4 = mat->elem2D[2][0];
    fr5 = mat->elem2D[2][1];
    fr6 = mat->elem2D[2][2];

    asm("fipr fv0, fv4"
        : "=f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    SHZ_MEMORY_BARRIER_SOFT();

    out.y = fr11;
    out.z = fr7;

    return out;
}

/* Same scheme as shz_xmtrx_get_scale_sh4(): branchless x * fsrra(x + 2^-119)
   for X and Y (exact 0 for a zero column), FSQRT for Z in parallel. */
SHZ_INLINE shz_vec3_t shz_mat4x4_get_scale_sh4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    register float fr3  asm("fr3");
    register float fr6  asm("fr6");
    register float fr10 asm("fr10");
    uintptr_t zero, bias;
    uintptr_t pref = (uintptr_t)mat + 32;

    asm(R"(
        pref      @%[p]
        fschg
        mov       #0, %[z]
        fmov.d    @%[m]+, dr8
        lds       %[z], fpul
        fmov.d    @%[m]+, dr10
        mov       #4, %[b]
        fmov.d    @%[m]+, dr4
        float     fpul, fr11
        fmov.d    @%[m]+, dr6
        shll16    %[b]
        fmov.d    @%[m]+, dr0
        float     fpul, fr7
        fmov.d    @%[m]+, dr2
        float     fpul, fr3
        fipr      fv8, fv8
        shll8     %[b]
        fipr      fv4, fv4
        lds       %[b], fpul
        fipr      fv0, fv0
        fsts      fpul, fr10
        fsts      fpul, fr6
        fadd      fr11, fr10
        fadd      fr7, fr6
        fsqrt     fr3
        fsrra     fr10
        fsrra     fr6
        fmul      fr11, fr10
        fmul      fr7, fr6
        fschg
    )"
    : "=f" (fr3), "=f" (fr6), "=f" (fr10),
      [z] "=&r" (zero), [b] "=&r" (bias), [m] "+&r" (mat)
    : "m" (*mat), [p] "r" (pref)
    : "fpul", "fr0", "fr1", "fr2", "fr4",
      "fr5", "fr7", "fr8", "fr9", "fr11");

    return shz_vec3_init(fr10, fr6, fr3);
}

#endif
