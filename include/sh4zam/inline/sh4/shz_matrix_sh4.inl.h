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
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#ifndef SHZ_MATRIX_SH4_INL_H
#define SHZ_MATRIX_SH4_INL_H

SHZ_FORCE_INLINE shz_vec3_t shz_mat4x4_transform_vec3_sh4(const shz_mat4x4_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
    register float vx asm("fr4") = v.x;
    register float vy asm("fr5") = v.y;
    register float vz asm("fr6") = v.z;

    register float ox asm("fr8");
    register float oy asm("fr9");
    register float oz asm("fr10");

    asm(R"(
        fmov.s    @%[m]+, fr8
        add       #28, %[m]
        pref      @%[m]
        add       #-28, %[m]
        fmov      fr4, fr0
        fmov.s    @%[m]+, fr9
        fmov.s    @%[m], fr10
        add       #8, %[m]
        fmul      fr0, fr8
        fmov.s    @%[m]+, fr1
        fmul      fr0, fr9
        fmov.s    @%[m]+, fr2
        fmul      fr0, fr10
        fmov.s    @%[m], fr3
        fmov      fr5, fr0
        add       #8, %[m]
        fmac      fr0, fr1, fr8
        fmov.s    @%[m]+, fr1
        fmac      fr0, fr2, fr9
        fmov.s    @%[m]+, fr2
        fmac      fr0, fr3, fr10
        fmov.s    @%[m], fr3
        fmov      fr6, fr0
        fmac      fr0, fr1, fr8
        fmac      fr0, fr2, fr9
        fmac      fr0, fr3, fr10
    )"
    : [m] "+&r" (mat), "=&f" (ox), "=&f" (oy), "=&f" (oz)
    : "f" (vx), "f" (vy), "f" (vz), "m" (*mat)
    : "fr0", "fr1", "fr2", "fr3");

    return shz_vec3_init(ox, oy, oz);
}

SHZ_FORCE_INLINE shz_vec4_t shz_mat4x4_transform_vec4_sh4(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT {
    register float vx asm("fr4") = in.x;
    register float vy asm("fr5") = in.y;
    register float vz asm("fr6") = in.z;
    register float vw asm("fr7") = in.w;

    register float ox asm("fr10");
    register float oy asm("fr11");
    register float oz asm("fr15");
    register float ow asm("fr3");

    asm(R"(
        fmov.s    @%[m]+, fr0
        add       #28, %[m]
        pref      @%[m]
        add       #-28, %[m]
        fmov.s    @%[m]+, fr8
        fmov.s    @%[m]+, fr12
        add       #4, %[m]
        fmov.s    @%[m]+, fr1
        fmov.s    @%[m]+, fr9
        fmov.s    @%[m]+, fr13
        add       #4, %[m]
        fmov.s    @%[m]+, fr2
        fmov.s    @%[m]+, fr10
        fmov.s    @%[m]+, fr14
        add       #4, %[m]
        fmov.s    @%[m]+, fr3
        fmov.s    @%[m]+, fr11
        fmov.s    @%[m]+, fr15
        add       #-48, %[m]
        fipr      fv4, fv0
        fmov.s    @%[m]+, fr0
        add       #12, %[m]
        fipr      fv4, fv8
        fmov.s    @%[m]+, fr1
        add       #12, %[m]
        fmov.s    @%[m]+, fr2
        add       #12, %[m]
        fmov      fr3, fr10
        fmov.s    @%[m], fr3
        fipr      fv4, fv12
        add       #-60, %[m]
        fipr      fv4, fv0
    )"
    : "=&f" (ox), "=&f" (oy), "=&f" (oz), "=&f" (ow)
    : [m] "u" (mat), "m" (*mat),
      "f" (vx), "f" (vy), "f" (vz), "f" (vw)
    : "fr0", "fr1", "fr2", "fr8", "fr9", "fr12", "fr13", "fr14");

    return shz_vec4_init(ox, oy, oz, ow);
}

SHZ_FORCE_INLINE shz_vec4_t shz_mat4x4_transform_vec4_transpose_sh4(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT {
    register float vx asm("fr4") = in.x;
    register float vy asm("fr5") = in.y;
    register float vz asm("fr6") = in.z;
    register float vw asm("fr7") = in.w;

    register float ox asm("fr10");
    register float oy asm("fr11");
    register float oz asm("fr15");
    register float ow asm("fr3");


    asm(R"(
        fmov.s    @%[m]+, fr0
        add       #28, %[m]
        pref      @%[m]
        add       #-28, %[m]
        fmov.s    @%[m]+, fr1
        fmov.s    @%[m]+, fr2
        fmov.s    @%[m]+, fr3
        fmov.s    @%[m]+, fr8
        fmov.s    @%[m]+, fr9
        fipr      fv4, fv0
        fmov.s    @%[m]+, fr10
        fmov.s    @%[m]+, fr11
        fmov.s    @%[m]+, fr12
        fmov.s    @%[m]+, fr13
        fipr      fv4, fv8
        fmov.s    @%[m]+, fr14
        fmov.s    @%[m]+, fr15
        fmov.s    @%[m]+, fr0
        fmov.s    @%[m]+, fr1
        fmov.s    @%[m]+, fr2
        fmov      fr3, fr10
        fmov.s    @%[m]+, fr3
        fipr      fv4, fv12
        add       #-64, %[m]
        fipr      fv4, fv0
    )"
    : "=&f" (ox), "=&f" (oy), "=&f" (oz), "=&f" (ow)
    : [m] "u" (mat), "m" (*mat),
      "f" (vx), "f" (vy), "f" (vz), "f" (vw)
    : "fr0", "fr1", "fr2", "fr8", "fr9", "fr12", "fr13", "fr14");

    return shz_vec4_init(ox, oy, oz, ow);
}

SHZ_FORCE_INLINE shz_vec3_t shz_mat4x4_transform_vec3_transpose_sh4(const shz_mat4x4_t* m, shz_vec3_t v) SHZ_NOEXCEPT {
    register float vx asm("fr4") = v.x;
    register float vy asm("fr5") = v.y;
    register float vz asm("fr6") = v.z;

    register float ox asm("fr3");
    register float oy asm("fr11");
    register float oz asm("fr15");

    asm(R"(
        fmov.s    @%[m]+, fr0
        add       #28, %[m]
        pref      @%[m]
        add       #-28, %[m]
        fmov.s    @%[m]+, fr1
        fmov.s    @%[m]+, fr2
        add       #4, %[m]
        fldi0     fr3
        fmov.s    @%[m]+, fr8
        fmov.s    @%[m]+, fr9
        fipr      fv4, fv0
        fmov.s    @%[m]+, fr10
        add       #4, %[m]
        fldi0     fr11
        fmov.s    @%[m]+, fr12
        fmov.s    @%[m]+, fr13
        fmov.s    @%[m]+, fr14
        fipr      fv4, fv8
        fldi0     fr15
        add       #-44, %[m]
        fipr      fv4, fv12
    )"
    : "=&f" (ox), "=&f" (oy), "=&f" (oz)
    : [m] "u" (m), "m" (*m),
      "f" (vx), "f" (vy), "f" (vz)
    : "fr0", "fr1", "fr2", "fr8", "fr9", "fr10", "fr12", "fr13", "fr14");

    return shz_vec3_init(ox, oy, oz);
}

SHZ_INLINE void shz_mat4x4_copy_sh4(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
    asm(R"(
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
        add     #32, %[dst]
        fmov.d  @%[src]+, dr6

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
    asm(R"(
        fschg

        fmov.d  @%[m1]+, dr0
        pref    @%[m2]
        fmov.d  @%[m1]+, dr2
        fmov.d  @%[m1]+, dr4
        fmov.d  @%[m1]+, dr6

        fmov.d  @%[m2]+, dr8
        pref    @%[m1]
        fmov.d  @%[m2]+, dr10
        fmov.d  @%[m2]+, dr12
        fmov.d  @%[m2]+, dr14

        fmov.d  dr14, @-%[m1]
        fmov.d  dr12, @-%[m1]
        fmov.d  dr10, @-%[m1]
        fmov.d  dr8, @-%[m1]
        add     #32, %[m1]

        fmov.d  dr6, @-%[m2]
        fmov.d  dr4, @-%[m2]
        fmov.d  dr2, @-%[m2]
        fmov.d  dr0, @-%[m2]
        add     #32, %[m2]

        fmov.d  @%[m1]+, dr0
        pref    @%[m2]
        fmov.d  @%[m1]+, dr2
        fmov.d  @%[m1]+, dr4
        fmov.d  @%[m1]+, dr6

        fmov.d  @%[m2]+, dr8
        fmov.d  @%[m2]+, dr10
        fmov.d  @%[m2]+, dr12
        fmov.d  @%[m2]+, dr14

        fmov.d  dr14, @-%[m1]
        fmov.d  dr12, @-%[m1]
        fmov.d  dr10, @-%[m1]
        fmov.d  dr8, @-%[m1]

        fmov.d  dr6, @-%[m2]
        fmov.d  dr4, @-%[m2]
        fmov.d  dr2, @-%[m2]
        fmov.d  dr0, @-%[m2]

        fschg
    )"
    : [m1] "+&u" (matA), [m2] "+&u" (matB),
      "+m" (*matA), "+m" (*matB)
    :
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
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

SHZ_INLINE shz_vec3_t shz_mat4x4_get_scale_sh4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    register float fr3  asm("fr3");
    register float fr6  asm("fr6");
    register float fr10 asm("fr10");
    uintptr_t pref = (uintptr_t)mat + 32;
    uintptr_t zero;

    asm(R"(
        pref      @%[p]
        fschg
        mov       #0, %[z]
        fmov.d    @%[m]+, dr8
        lds       %[z], fpul
        fmov.d    @%[m]+, dr10
        mov       #4, %[z]
        fmov.d    @%[m]+, dr4
        float     fpul, fr11
        fmov.d    @%[m]+, dr6
        shll16    %[z]
        fmov.d    @%[m]+, dr0
        float     fpul, fr7
        fmov.d    @%[m]+, dr2
        float     fpul, fr3
        fipr      fv8, fv8
        shll8     %[z]
        fipr      fv4, fv4
        lds       %[z], fpul
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
      [z] "=&r" (zero), [m] "+&r" (mat)
    : "m" (*mat), [p] "r" (pref)
    : "fpul", "fr0", "fr1", "fr2", "fr4",
      "fr5", "fr7", "fr8", "fr9", "fr11");

    return shz_vec3_init(fr10, fr6, fr3);
}

SHZ_INLINE void shz_mat4x4_init_identity_sh4(shz_mat4x4_t* m) SHZ_NOEXCEPT {
    register float fr4 asm("fr4") = 0.0f;
    register float fr5 asm("fr5") = 0.0f;
    register float fr6 asm("fr6") = 1.0f;

    asm(R"(
        add     #64, %[m]
        fmov.s  fr6, @-%[m]
        add     #-60, %[m]
        pref    @%[m]
        add     #60, %[m]
        fmov.s  fr4, @-%[m]
        fschg
        fmov.d  dr4, @-%[m]
        fschg
        fmov.s  fr4, @-%[m]
        fmov.s  fr6, @-%[m]
        fschg
        fmov.d  dr4, @-%[m]
        fmov.d  dr4, @-%[m]
        fschg
        fmov.s  fr6, @-%[m]
        fmov.s  fr4, @-%[m]
        fschg
        fmov.d  dr4, @-%[m]
        fschg
        fmov.s  fr4, @-%[m]
        fmov.s  fr6, @-%[m]
    )"
    : "=m" (*m)
    : "f" (fr4), "f" (fr5), "f" (fr6),
      [m] "u" (m));
}

SHZ_FORCE_INLINE void shz_mat4x4_init_fill_sh4(shz_mat4x4_t* m, float value) SHZ_NOEXCEPT {
    register float fr4 asm("fr4") = value;
    register float fr5 asm("fr5") = value;

    asm(R"(
        add     #64, %[m]
        fmov.s  fr4, @-%[m]
        add     #-60, %[m]
        pref    @%[m]
        add     #60, %[m]
        fmov.s  fr4, @-%[m]
        fschg
        fmov.d  dr4, @-%[m]
        fmov.d  dr4, @-%[m]
        fmov.d  dr4, @-%[m]
        fmov.d  dr4, @-%[m]
        fmov.d  dr4, @-%[m]
        fmov.d  dr4, @-%[m]
        fmov.d  dr4, @-%[m]
        fschg
    )"
    : "=m" (*m)
    : "f" (fr4), "f" (fr5),
      [m] "u" (m));
}

SHZ_INLINE void shz_mat4x4_init_translation_sh4(shz_mat4x4_t* m, float x, float y, float z, float w) SHZ_NOEXCEPT {
    register float fr4 asm("fr4") = x;
    register float fr5 asm("fr5") = y;
    register float fr6 asm("fr6") = z;
    register float fr7 asm("fr7") = w;

    asm(R"(
        add     #64, %[m]
        fschg
        fmov.d  dr6, @-%[m]
        add     #-56, %[m]
        fldi0   fr7
        pref    @%[m]
        add     #56, %[m]
        fldi0   fr6
        fmov.d  dr4, @-%[m]
        fldi1   fr4
        fldi0   fr5
        fmov.d  dr4, @-%[m]
        fmov.d  dr6, @-%[m]
        fmov.d  dr6, @-%[m]
        fldi0   fr4
        fldi1   fr5
        fmov.d  dr4, @-%[m]
        fmov.d  dr6, @-%[m]
        fschg
        fmov.s  fr6, @-%[m]
        fmov.s  fr5, @-%[m]
    )"
    : "+&f" (fr4), "+&f" (fr5), "+&f" (fr6), "+&f" (fr7),
      "=m" (*m)
    : [m] "u" (m));
}

SHZ_INLINE void shz_mat4x4_init_diagonal_sh4(shz_mat4x4_t* m, float x, float y, float z, float w) SHZ_NOEXCEPT {
    register float fr4 asm("fr4") = x;
    register float fr5 asm("fr5") = y;
    register float fr6 asm("fr6") = z;
    register float fr7 asm("fr7") = w;
    register float fr8 asm("fr8") = 0.0f;
    register float fr9 asm("fr9") = 0.0f;

    asm(R"(
        add     #64, %[m]
        fmov.s  fr7, @-%[m]
        add     #-60, %[m]
        pref    @%[m]
        add     #60, %[m]
        fmov.s  fr8, @-%[m]
        fschg
        fmov.d  dr8, @-%[m]
        fschg
        fmov.s  fr8, @-%[m]
        fmov.s  fr6, @-%[m]
        fschg
        fmov.d  dr8, @-%[m]
        fmov.d  dr8, @-%[m]
        fschg
        fmov.s  fr5, @-%[m]
        fmov.s  fr8, @-%[m]
        fschg
        fmov.d  dr8, @-%[m]
        fschg
        fmov.s  fr8, @-%[m]
        fmov.s  fr4, @-%[m]
    )"
    : "=m" (*m)
    : "f" (fr4), "f" (fr5), "f" (fr6), "f" (fr7), "f" (fr8), "f" (fr9),
      [m] "u" (m));
}

SHZ_INLINE void shz_mat4x4_init_rotation_x_yz_sh4(shz_mat4x4_t* m, float y, float z) SHZ_NOEXCEPT {
    register float fr4 asm("fr4") = z;
    register float fr5 asm("fr5") = y;
    register float fr6 asm("fr6") = 0.0f;
    register float fr7 asm("fr7") = 1.0f;

    asm(R"(
        add     #64, %[m]
        fmov.s  fr7, @-%[m]
        add     #-60, %[m]
        pref    @%[m]
        add     #60, %[m]
        fmov.s  fr6, @-%[m]
        fmov.s  fr6, @-%[m]
        fmov.s  fr6, @-%[m]

        fmov.s  fr6, @-%[m]
        fmov.s  fr4, @-%[m]
        fmov.s  fr5, @-%[m]
        fmov.s  fr6, @-%[m]

        fneg    fr5
        fmov.s  fr6, @-%[m]
        fmov.s  fr5, @-%[m]
        fmov.s  fr4, @-%[m]
        fmov.s  fr6, @-%[m]

        fmov.s  fr6, @-%[m]
        fmov.s  fr6, @-%[m]
        fmov.s  fr6, @-%[m]
        fmov.s  fr7, @-%[m]
    )"
    : "+f" (fr5), "=m" (*m)
    : "f" (fr4), "f" (fr6), "f" (fr7),
      [m] "u" (m));
}

#endif