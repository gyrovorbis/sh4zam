//! \cond INTERNAL
/*! \file
 *  \brief   Active Matrix API Implementation.
 *  \ingroup xmtrx
 *
 *  This file contains the low-level implementation of the XMTRX API.
 *
 *  \author Falco Girgis
 *  \author Twada
 *
 *  \copyright MIT License
 */

SHZ_INLINE float shz_xmtrx_read(shz_xmtrx_reg_t xf) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_write(shz_xmtrx_reg_t xf, float value) SHZ_NOEXCEPT {
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

SHZ_INLINE shz_vec4_t shz_xmtrx_read_row(unsigned int index) SHZ_NOEXCEPT {
    assert(index < 4);

    shz_vec4_t value;

    SHZ_FRCHG();
    switch(index) {
    case 0:
        asm volatile(R"(
            fmov.s  fr0,  @%[x]
            fmov.s  fr4,  @%[y]
            fmov.s  fr8,  @%[z]
            fmov.s  fr12, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    case 1:
        asm volatile(R"(
            fmov.s  fr1,  @%[x]
            fmov.s  fr5,  @%[y]
            fmov.s  fr9,  @%[z]
            fmov.s  fr13, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    case 2:
        asm volatile(R"(
            fmov.s  fr2,  @%[x]
            fmov.s  fr6,  @%[y]
            fmov.s  fr10, @%[z]
            fmov.s  fr14, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    case 3:
        asm volatile(R"(
            fmov.s  fr3,  @%[x]
            fmov.s  fr7,  @%[y]
            fmov.s  fr11, @%[z]
            fmov.s  fr15, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    }
    SHZ_FRCHG();

    return value;
}

SHZ_INLINE void shz_xmtrx_write_row(unsigned int index, shz_vec4_t value) SHZ_NOEXCEPT {
    assert(index < 4);

    SHZ_FRCHG();
    switch(index) {
    case 0:
        asm volatile(R"(
            fmov.s  @%[x], fr0
            fmov.s  @%[y], fr4
            fmov.s  @%[z], fr8
            fmov.s  @%[w], fr12
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    case 1:
        asm volatile(R"(
            fmov.s  @%[x], fr1
            fmov.s  @%[y], fr5
            fmov.s  @%[z], fr9
            fmov.s  @%[w], fr13
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    case 2:
        asm volatile(R"(
            fmov.s  @%[x], fr2
            fmov.s  @%[y], fr6
            fmov.s  @%[z], fr10
            fmov.s  @%[w], fr14
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    case 3:
        asm volatile(R"(
            fmov.s  @%[x], fr3
            fmov.s  @%[y], fr7
            fmov.s  @%[z], fr11
            fmov.s  @%[w], fr15
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    }
    SHZ_FRCHG();
}

SHZ_INLINE shz_vec4_t shz_xmtrx_read_col(unsigned int index) SHZ_NOEXCEPT {
    assert(index < 4);

    shz_vec4_t value;

    SHZ_FRCHG();
    switch(index) {
    case 0:
        asm volatile(R"(
            fmov.s  fr0, @%[x]
            fmov.s  fr1, @%[y]
            fmov.s  fr2, @%[z]
            fmov.s  fr3, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    case 1:
        asm volatile(R"(
            fmov.s  fr4, @%[x]
            fmov.s  fr5, @%[y]
            fmov.s  fr6, @%[z]
            fmov.s  fr7, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    case 2:
        asm volatile(R"(
            fmov.s  fr8,  @%[x]
            fmov.s  fr9,  @%[y]
            fmov.s  fr10, @%[z]
            fmov.s  fr11, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    case 3:
        asm volatile(R"(
            fmov.s  fr12, @%[x]
            fmov.s  fr13, @%[y]
            fmov.s  fr14, @%[z]
            fmov.s  fr15, @%[w]
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w),
          "m" (value.x), "m" (value.y),
          "m" (value.z), "m" (value.w));
        break;
    }
    SHZ_FRCHG();

    return value;
}

SHZ_INLINE void shz_xmtrx_write_col(unsigned int index, shz_vec4_t value) SHZ_NOEXCEPT {
    assert(index < 4);

    SHZ_FRCHG();
    switch(index) {
    case 0:
        asm volatile(R"(
            fmov.s  @%[x], fr0
            fmov.s  @%[y], fr1
            fmov.s  @%[z], fr2
            fmov.s  @%[w], fr3
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    case 1:
        asm volatile(R"(
            fmov.s  @%[x], fr4
            fmov.s  @%[y], fr5
            fmov.s  @%[z], fr6
            fmov.s  @%[w], fr7
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    case 2:
        asm volatile(R"(
            fmov.s  @%[x], fr8
            fmov.s  @%[y], fr9
            fmov.s  @%[z], fr10
            fmov.s  @%[w], fr11
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    case 3:
        asm volatile(R"(
            fmov.s  @%[x], fr12
            fmov.s  @%[y], fr13
            fmov.s  @%[z], fr14
            fmov.s  @%[w], fr15
        )"
        :
        : [x] "r" (&value.x), [y] "r" (&value.y),
          [z] "r" (&value.z), [w] "r" (&value.w));
        break;
    }
    SHZ_FRCHG();
}

SHZ_INLINE void shz_xmtrx_swap_rows(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT {
    shz_vec4_t v1 = shz_xmtrx_read_row(index1);
    shz_vec4_t v2 = shz_xmtrx_read_row(index2);
    shz_xmtrx_write_row(index1, v2);
    shz_xmtrx_write_row(index2, v1);
}

SHZ_INLINE void shz_xmtrx_swap_cols(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT {
    shz_vec4_t v1 = shz_xmtrx_read_col(index1);
    shz_vec4_t v2 = shz_xmtrx_read_col(index2);
    shz_xmtrx_write_col(index1, v2);
    shz_xmtrx_write_col(index2, v1);
}

SHZ_INLINE void shz_xmtrx_load_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_wxyz_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    asm volatile(R"(
        pref    @%[mtx]
        fschg

        add     #48, %[mtx]
        fmov.d  @%[mtx]+, xd0
        fmov.d  @%[mtx]+, xd2
        add     #-64 %[mtx]

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

SHZ_INLINE void shz_xmtrx_load_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_cols_4x4(const shz_vec4_t* c1,
                                        const shz_vec4_t* c2,
                                        const shz_vec4_t* c3,
                                        const shz_vec4_t* c4) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_rows_4x4(const shz_vec4_t* r1,
                                        const shz_vec4_t* r2,
                                        const shz_vec4_t* r3,
                                        const shz_vec4_t* r4) SHZ_NOEXCEPT {
    asm volatile (R"(
        frchg

        pref    @%1
        fmov.s  @%0+, fr0
        fmov.s  @%0+, fr4
        fmov.s  @%0+, fr8
        fmov.s  @%0,  fr12

        pref    @%2
        fmov.s  @%1+, fr1
        fmov.s  @%1+, fr5
        fmov.s  @%1+, fr9
        fmov.s  @%1,  fr13

        pref    @%3
        fmov.s  @%2+, fr2
        fmov.s  @%2+, fr6
        fmov.s  @%2+, fr10
        fmov.s  @%2,  fr14

        fmov.s  @%3+, fr3
        fmov.s  @%3+, fr7
        fmov.s  @%3+, fr11
        fmov.s  @%3,  fr15

        frchg
    )"
    : "+&r" (r1), "+&r" (r2), "+&r" (r3), "+&r" (r4)
    : "m" (*r1), "m" (*r2), "m" (*r3), "m" (*r4));
}

SHZ_INLINE void shz_xmtrx_load_transpose_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
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

// Just straight up forward it, as shz_xmtrx_load_transpose_4x4() never required alignment anyway.
SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_transpose_4x4((const shz_mat4x4_t*)matrix);
}

SHZ_INLINE void shz_xmtrx_load_apply_4x4(const shz_mat4x4_t* matrix1,
                                         const shz_mat4x4_t* matrix2) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_apply_unaligned_4x4(const float matrix1[16],
                                                   const float matrix2[16]) SHZ_NOEXCEPT {
    asm volatile(R"(
        frchg

        fmov.s  @%[m1]+, fr0
        add     #32, %[m1]
        pref    @%[m1]
        add     #-(32-4), %[m1]
        fmov.s  @%[m1]+, fr1
        fmov.s  @%[m1]+, fr2
        fmov.s  @%[m1]+, fr3
        fmov.s  @%[m1]+, fr4
        fmov.s  @%[m1]+, fr5
        fmov.s  @%[m1]+, fr6
        fmov.s  @%[m1]+, fr7

        pref    @%[m2]
        fmov.s  @%[m1]+, fr8
        fmov.s  @%[m1]+, fr9
        fmov.s  @%[m1]+, fr10
        fmov.s  @%[m1]+, fr11
        fmov.s  @%[m1]+, fr12
        fmov.s  @%[m1]+, fr13
        fmov.s  @%[m1]+, fr14
        add     #32, %[m2]
        fmov.s  @%[m1]+, fr15
        pref    @%[m2]
        frchg
        add     #-32, %[m2]

        fmov.s  @%[m2]+, fr0
        fmov.s  @%[m2]+, fr1
        fmov.s  @%[m2]+, fr2
        fmov.s  @%[m2]+, fr3
        fmov.s  @%[m2]+, fr4
        fmov.s  @%[m2]+, fr5
        ftrv    xmtrx, fv0

        fmov.s  @%[m2]+, fr6
        fmov.s  @%[m2]+, fr7
        fmov.s  @%[m2]+, fr8
        fmov.s  @%[m2]+, fr9
        ftrv    xmtrx, fv4

        fmov.s  @%[m2]+, fr10
        fmov.s  @%[m2]+, fr11
        fmov.s  @%[m2]+, fr12
        fmov.s  @%[m2]+, fr13
        ftrv    xmtrx, fv8

        fmov.s  @%[m2]+, fr14
        fmov.s  @%[m2]+, fr15
        ftrv    xmtrx, fv12

        frchg
    )"
    : [m1] "+&r" (matrix1), [m2] "+&r" (matrix2)
    : "m" (*matrix1), "m" (*matrix2)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_load_apply_store_4x4(shz_mat4x4_t* out,
                                               const shz_mat4x4_t* matrix1,
                                               const shz_mat4x4_t* matrix2) SHZ_NOEXCEPT {
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
    : [m1] "+&r" (matrix1), [m2] "+r" (matrix2), [out] "+&r" (out), "=m" (*out),
      [prefscr] "=&r" (prefetch_scratch)
    : "m" (*matrix1), "m" (*matrix2)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_load_4x3(const shz_mat4x3_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_cols_3x4(const shz_vec4_t* c1,
                                        const shz_vec4_t* c2,
                                        const shz_vec4_t* c3) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_rows_3x4(const shz_vec4_t* r1,
                                        const shz_vec4_t* r2,
                                        const shz_vec4_t* r3) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_transpose_3x3(const float* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_load_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT  {
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

SHZ_INLINE void shz_xmtrx_store_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_store_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_store_transpose_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
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

// Just forward it directly to shz_xmtrx_store_transpose_4x4(), which never even required over-alignment...
SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_store_transpose_4x4((shz_mat4x4_t*)matrix);
}

SHZ_INLINE void shz_xmtrx_store_3x4(shz_mat3x4_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_store_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_store_transpose_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_store_2x2(shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_init_identity(void) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_init_zero(void) SHZ_NOEXCEPT {
    asm volatile(R"(
        frchg
        fschg
        fldi0   fr0
        fldi0   fr1
        fmov    dr0, dr2
        fmov    dr0, dr4
        fmov    dr0, dr6
        fmov    dr0, dr8
        fmov    dr0, dr10
        fmov    dr0, dr12
        fmov    dr0, dr14
        fschg
        frchg
    )");
}

SHZ_INLINE void shz_xmtrx_init_one(void) SHZ_NOEXCEPT {
    asm volatile(R"(
        frchg
        fschg
        fldi1   fr0
        fldi1   fr1
        fmov    dr0, dr2
        fmov    dr0, dr4
        fmov    dr0, dr6
        fmov    dr0, dr8
        fmov    dr0, dr10
        fmov    dr0, dr12
        fmov    dr0, dr14
        fschg
        frchg
    )");
}

SHZ_INLINE void shz_xmtrx_init_fill(float value) SHZ_NOEXCEPT {
    asm volatile(R"(
        ftrc    %0, fpul
        frchg
        fsts    fpul, fr0
        fmov    fr0, fr1
        fschg
        fmov    dr0, dr2
        fmov    dr0, dr4
        fmov    dr0, dr6
        fmov    dr0, dr8
        fmov    dr0, dr10
        fmov    dr0, dr12
        fmov    dr0, dr14
        fschg
        frchg
    )"
    :
    : "f" (value)
    : "fpul");
}

SHZ_INLINE void shz_xmtrx_init_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_init_upper_diagonal(float col1, shz_vec2_t col2, shz_vec3_t col3, shz_vec4_t col4) SHZ_NOEXCEPT {
    asm volatile(R"(
        ftrc    %[c1], fpul
        frchg
        fsts    fpul, fr0
        fldi0   fr1
        fldi0   fr2
        fldi0   fr3
        fmov.s  @%[c2]+, fr4
        fmov.s  @%[c2]+, fr5
        add     #-16, %[c2]
        fldi0   fr6
        fldi0   fr7
        fmov.s  @%[c3]+, fr8
        fmov.s  @%[c3]+, fr9
        fmov.s  @%[c3]+, fr10
        add     #-24, %[c3]
        fldi0   fr11
        fmov.s  @%[c4]+, fr12
        fmov.s  @%[c4]+, fr13
        fmov.s  @%[c4]+, fr14
        fmov.s  @%[c4]+, fr15
        add     #-32, %[c4]
        frchg
    )"
    :
    : [c2] "r" (&col2), [c3] "r" (&col3), [c4] "r" (&col4),
      [c1] "f" (col1), "m" (col2), "m" (col3), "m" (col4)
    : "fpul");
}

SHZ_INLINE void shz_xmtrx_init_lower_diagonal(shz_vec4_t col1, shz_vec3_t col2, shz_vec2_t col3, float col4) SHZ_NOEXCEPT {
    asm volatile(R"(
        ftrc    %[c4], fpul
        frchg
        fmov.s  @%[c1]+, fr0
        fmov.s  @%[c1]+, fr1
        fmov.s  @%[c1]+, fr2
        fmov.s  @%[c1]+, fr3
        add     #-32, %[c1]
        fmov.s  @%[c2]+, fr4
        fmov.s  @%[c2]+, fr5
        fmov.s  @%[c2]+, fr6
        add     #-24, %[c2]
        fldi0   fr7
        fmov.s  @%[c3]+, fr8
        fmov.s  @%[c3]+, fr9
        add     #-16, %[c3]
        fldi0   fr10
        fldi0   fr11
        fsts    fpul, fr12
        fldi0   fr13
        fldi0   fr14
        fldi0   fr15
        frchg
    )"
    :
    : [c1] "r" (&col1), [c2] "r" (&col2), [c3] "r" (&col3),
      [c4] "f" (col4), "m" (col1), "m" (col2), "m" (col3)
    : "fpul");
}


SHZ_FORCE_INLINE void shz_xmtrx_init_scale(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_diagonal(x, y, z, 1.0f);
}

SHZ_INLINE void shz_xmtrx_init_rotation_x(float x) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_init_rotation_y(float y) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_init_rotation_z(float z) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_init_translation(float x, float y, float z) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_init_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT {
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

/* This actually does a 4x4 matrix multiply to do the outer product.
   It's this:

   [ x1 x1 x1 x1 ] [ y1 0 0 0 ]     [ x1y1 x1y2 x1y3 x1y4 ]
   [ x2 x2 x2 x2 ] [ 0 y2 0 0 ]  =  [ x2y1 x2y2 x2y3 x2y4 ]
   [ x3 x3 x3 x3 ] [ 0 0 y3 0 ]     [ x3y1 x3y2 x3y3 x3y4 ]
   [ x4 x4 x4 x4 ] [ 0 0 0 y4 ]     [ x4y1 x4y2 x4y3 x4y4 ]

    -- MoopTheHedgehog
*/
SHZ_INLINE void shz_xmtrx_init_outer_product(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    register float ax asm("fr4") = a.x;
    register float ay asm("fr5") = a.y;
    register float az asm("fr6") = a.z;
    register float aw asm("fr7") = a.w;

    register float bx asm("fr8")  = b.x;
    register float by asm("fr9")  = b.y;
    register float bz asm("fr10") = b.z;
    register float bw asm("fr11") = b.w;

    asm volatile(R"(
        fldi0   fr2
        fmov    fr2, fr3

        fschg
        fmov    dr4, xd0
        fmov    dr4, xd4
        fmov    dr4, xd8
        fmov    dr4, xd12

        fmov    dr6, xd2
        fmov    dr6, xd6
        fmov    dr6, xd10
        fmov    dr6, xd14

        fmov    dr8, dr0
        fmov    dr8, dr4
        fmov    dr10, dr14

        fmov    dr2, dr6
        fmov    dr2, dr8
        fmov    dr2, dr12
        fschg

        fmov    fr2, fr1
        fmov    fr6, fr4
        ftrv    xmtrx, fv0

        fmov    fr8, fr11
        ftrv    xmtrx, fv4

        fmov    fr12, fr14
        ftrv    xmtrx, fv8

        ftrv    xmtrx, fv12

        frchg
    )"
    :
    : "f" (ax), "f" (ay), "f" (az), "f" (aw),
      "f" (bx), "f" (by), "f" (bz), "f" (bw)
    : "fr0", "fr1", "fr2", "fr3", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_apply_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT {
    asm volatile(R"(
        fldi0   fr0
        fmov.s  @%[z], fr1
        fmov.s  @%[y], fr2
        fneg    fr2
        fldi0   fr3
        ftrv    xmtrx, fv0

        fmov    fr1, fr4
        fneg    fr4
        fldi0   fr5
        fmov.s  @%[x], fr6
        fldi0   fr7
        ftrv    xmtrx, fv4

        fmov    fr2, fr8
        fneg    fr8
        fmov    fr6, fr9
        fneg    fr9
        fldi0   fr10
        fldi0   fr11
        ftrv    xmtrx, fv8

        fschg
        fmov.d  xd12, dr12
        fmov.d  xd14, dr14
        fschg

        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z));
}

SHZ_INLINE void shz_xmtrx_add_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT {
    asm volatile(R"(
        fschg
        fmov.d  xd12, dr0
        fmov.d  xd14, dr2
        fschg

        frchg
        fmov.s  fr12, @%[x]
        fmov.s  fr13, @%[y]
        fmov.s  fr14, @%[z]

        fadd    fr14, fr1
        fsub    fr13, fr2
        fsub    fr14, fr4
        fadd    fr12, fr6
        fadd    fr13, fr8
        fsub    fr12, fr9
        frchg

        fschg
        fmov.d dr0, xd12
        fmov.d dr2, xd14
        fschg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z)
    : "fr0", "fr1", "fr2", "fr3", "fr4");
}

SHZ_INLINE void shz_xmtrx_add_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT {
    asm volatile(R"(
        fschg
        fmov.d  xd12, dr0
        fmov.d  xd8, dr2
        fschg

        frchg
        fmov.s  fr12, @%[x]
        fmov.s  fr13, @%[y]
        fmov.s  fr8, @%[z]
        fmov.s  fr9, @%[w]

        fadd    fr12, fr0
        fadd    fr13, fr5
        fadd    fr8, fr10
        fadd    fr9, fr15
        frchg

        fschg
        fmov.d dr0, xd12
        fmov.d dr2, xd8
        fschg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z), [w] "r" (&w),
      "m" (x), "m" (y), "m" (z), "m" (w)
    : "fr0", "fr1", "fr2", "fr3", "fr4");
}

SHZ_INLINE void shz_xmtrx_apply_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_3x4(const shz_mat3x4_t* matrix) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
    asm volatile(R"(
        fmov.s  @%[mtx]+, fr0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-32, %[mtx]
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

SHZ_INLINE void shz_xmtrx_apply_transpose_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
    asm volatile(R"(
        fmov.s  @%[mtx]+, fr0
        add     #32, %[mtx]
        pref    @%[mtx]
        add     #-32, %[mtx]
        fmov.s  @%[mtx]+, fr4
        fmov.s  @%[mtx]+, fr8
        fldi0   fr12

        fmov.s  @%[mtx]+, fr1
        fmov.s  @%[mtx]+, fr5
        fmov.s  @%[mtx]+, fr9
        fldi0   fr13

        ftrv    xmtrx, fv0

        fmov.s  @%[mtx]+, fr2
        fmov.s  @%[mtx]+, fr6
        fmov.s  @%[mtx]+, fr10
        fldi1   fr15

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

SHZ_INLINE void shz_xmtrx_apply_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_xmtrx_set_translation(float x, float y, float z) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_translation(float x, float y, float z) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_scale(float x, float y, float z) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_rotation_x(float x) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_rotation_y(float y) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_rotation_z(float z) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_rotation_axis(float angle, float x, float y, float z) SHZ_NOEXCEPT {
    register float xx asm("fr4") = x;
    register float yy asm("fr5") = y;
    register float zz asm("fr6") = z;
    register float aa asm("fr7") = angle * SHZ_FSCA_RAD_FACTOR;
	
    asm volatile(R"(
        ftrc	fr7, fpul
        fsca	fpul, dr2
        fldi1	fr0
        fsub	fr3, fr0	/* 1-cos */

        fldi0	fr7
        fipr	fv4, fv4
        fsrra	fr7
        fmul	fr7, fr4
        fmul	fr7, fr5
        fmul	fr7, fr6

        fmov	fr4, fr1
        fmul	fr2, fr1	/* xsin */
        fmov	fr5, fr7
        fmul	fr2, fr7	/* ysin */
        fmul	fr6, fr2	/* zsin */

        fmov	fr4, fr8
        fmul	fr0, fr8
        fmov	fr5, fr9
        fmul	fr8, fr9	/* xy(1-cos) */
        fmul	fr6, fr8	/* xz(1-cos) */
        fmov	fr6, fr10
        fmul	fr0, fr6
        fmul	fr6, fr10
        fadd	fr3, fr10	/* zz(1-cos)+cos */
        fmul	fr5, fr6	/* yz(1-cos) */
        fmul	fr5, fr5
        fmul	fr0, fr5
        fadd	fr3, fr5	/* yy(1-cos)+cos */
        fmul	fr4, fr0
        fmul	fr4, fr0
        fadd	fr3, fr0	/* xx(1-cos)+cos */

        fmov	fr8, fr3	/* xz(1-cos) */
        fmov	fr9, fr4	/* xy(1-cos) */
        fadd	fr7, fr8
        fmov	fr6, fr9
        fsub	fr1, fr9
        fldi0	fr11
        ftrv	xmtrx, fv8

        fadd	fr1, fr6
        fmov	fr4, fr1
        fsub	fr2, fr4
        fsub	fr7, fr3
        fldi0	fr7
        ftrv	xmtrx, fv4

        fadd	fr2, fr1
        fmov	fr3, fr2
        fldi0	fr3
        ftrv	xmtrx, fv0

        fschg
        fmov	dr10, xd10
        fmov	dr8, xd8
        fmov	dr6, xd6
        fmov	dr4, xd4
        fmov	dr2, xd2
        fmov	dr0, xd0
        fschg
    )"
    :
    : "f"(xx), "f"(yy), "f"(zz), "f"(aa)
    : "fpul", "fr0", "fr1", "fr2", "fr3", "fr8", "fr9", "fr10", "fr11");
}

/* Tait-Bryan angles, (extrinsic rotation notation) */
SHZ_INLINE void shz_xmtrx_init_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

// Same as yaw, pitch, roll
SHZ_INLINE void shz_xmtrx_init_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_z(zAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
}

SHZ_INLINE void shz_xmtrx_init_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

SHZ_INLINE void shz_xmtrx_apply_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

SHZ_INLINE void shz_xmtrx_apply_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_apply_rotation_z(zAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
}

SHZ_INLINE void shz_xmtrx_apply_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

SHZ_INLINE void shz_xmtrx_init_rotation(shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    shz_sincos_t sincos = shz_sincosf(angle);
    shz_vec3_t skew_scaled1 = shz_vec3_scale(axis, 1.0f - shz_cosf(angle));
    shz_xmtrx_init_symmetric_skew(skew_scaled1.x, skew_scaled1.y, skew_scaled1.z);
    shz_xmtrx_apply_symmetric_skew(skew_scaled1.x, skew_scaled1.y, skew_scaled1.z);
    shz_vec3_t skew_scaled2 = shz_vec3_scale(axis, sincos.sin);
    shz_xmtrx_apply_symmetric_skew(skew_scaled2.x, skew_scaled2.y, skew_scaled2.z);
    shz_xmtrx_add_diagonal(sincos.cos, sincos.cos, sincos.cos, 0.0f);
}

SHZ_INLINE void shz_xmtrx_apply_rotation(shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    shz_sincos_t sincos = shz_sincosf(angle);
    shz_vec3_t skew_scaled1 = shz_vec3_scale(axis, 1.0f - shz_cosf(angle));
    shz_xmtrx_apply_symmetric_skew(skew_scaled1.x, skew_scaled1.y, skew_scaled1.z);
    shz_xmtrx_apply_symmetric_skew(skew_scaled1.x, skew_scaled1.y, skew_scaled1.z);
    shz_vec3_t skew_scaled2 = shz_vec3_scale(axis, sincos.sin);
    shz_xmtrx_apply_symmetric_skew(skew_scaled2.x, skew_scaled2.y, skew_scaled2.z);
    shz_xmtrx_add_diagonal(sincos.cos, sincos.cos, sincos.cos, 0.0f);
}

void shz_xmtrx_apply_rotation_quat(shz_quat_t quat) SHZ_NOEXCEPT;
shz_quat_t shz_xmtrx_to_quat(void) SHZ_NOEXCEPT;
float shz_xmtrx_determinant(void) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_transpose(void) SHZ_NOEXCEPT {
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

SHZ_INLINE void shz_xmtrx_apply_lookat(const float* position_3f,
                                       const float* target_3f,
                                       const float* up_3f) SHZ_NOEXCEPT {
	asm volatile(R"(
        fmov.s  @%[t]+, fr8
        fmov.s  @%[t]+, fr9
        fmov.s  @%[t]+, fr10

        fmov.s  @%[p]+, fr12
        fmov.s  @%[p]+, fr13
        fmov.s  @%[p]+, fr14
        fldi0   fr15

        /* z = position - target */
        fneg    fr8
        fadd    fr12, fr8
        fneg    fr9
        fadd    fr13, fr9
        fneg    fr10
        fadd    fr14, fr10
        fldi0   fr11
        fipr    fv8, fv8

        fmov.s  @%[u]+, fr4
        fmov.s  @%[u]+, fr5
        fmov.s  @%[u]+, fr6

        fsrra   fr11
        fmul    fr11, fr8
        fmul    fr11, fr9
        fmul    fr11, fr10
        fldi0   fr11
        fipr    fv12, fv8

        /* x = cross(up, z) */
        fmov    fr6, fr15
        fmul    fr9, fr15
        fmov    fr5, fr0
        fmul    fr10, fr0
        fmov    fr4, fr3
        fmul    fr10, fr3
        fsub    fr15, fr0
        fmov    fr6, fr1
        fmul    fr8, fr1
        fmov    fr4, fr2
        fmul    fr9, fr2
        fmov    fr5, fr15
        fmul    fr8, fr15
        fsub    fr3, fr1
        fsub    fr15, fr2
        fldi0   fr3
        fldi0   fr15
        fipr    fv0, fv0

        fsrra   fr3
        fmul    fr3, fr0
        fmul    fr3, fr1
        fmul    fr3, fr2
        fldi0   fr3
        fipr    fv12, fv0

        /* y = cross(z, x) */
        fmov    fr10, fr15
        fmul    fr1, fr15
        fmov    fr9, fr4
        fmul    fr2, fr4
        fmov    fr8, fr7
        fmul    fr2, fr7
        fsub    fr15, fr4
        fmov    fr10, fr5
        fmul    fr0, fr5
        fmov    fr8, fr6
        fmul    fr1, fr6
        fmov    fr9, fr15
        fmul    fr0, fr15
        fsub    fr7, fr5
        fsub    fr15, fr6

        fldi0   fr7
        fldi0   fr15
        fipr    fv12, fv4

        fneg    fr3
        fneg    fr11
        fneg    fr7
        fmov    fr3, fr12
        fmov    fr7, fr13
        fmov    fr11, fr14
        fldi1   fr15
        ftrv    xmtrx, fv12

        fmov    fr1, fr7
        fmov    fr2, fr1
        fmov    fr4, fr1
        fmov    fr8, fr2
        fldi0   fr3
        ftrv    xmtrx, fv0

        fmov    fr7, fr4
        fmov    fr6, fr7
        fmov    fr9, fr6
        fmov    fr7, fr9
        fldi0   fr7
        ftrv    xmtrx, fv4

        fmov    fr11, fr8
        fldi0   fr11
        ftrv    xmtrx, fv8

        frchg
    )"
	: [p] "+&r"(position_3f), [t] "+&r"(target_3f), [u] "+&r"(up_3f)
	:
	: "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}


// ****************************************************************
// void shz_xmtrx_apply_perspective(float f, float a, float nz)
// ****************************************************************
//  fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
// -----------+-----------+-----------+-----------
// cot(f)/a   | 0.0f      | 0.0f      | 0.0f
// 	0.0f      | cot(f)    | 0.0f      | 0.0f
// 	0.0f      | 0.0f      | 0.0f      | nz
// 	0.0f      | 0.0f      | -1.0f     | 0.0f
// ****************************************************************
SHZ_INLINE void shz_xmtrx_apply_perspective(float fov, float aspect, float near_z) SHZ_NOEXCEPT {
    register float f asm("fr4") = fov * 0.5f * SHZ_FSCA_RAD_FACTOR;
    register float a asm("fr5") = aspect;
    register float n asm("fr6") = near_z;

    asm volatile(R"(
        fmov    fr5, fr7
        fmul    fr7, fr7
        fsrra   fr7
        fschg
        ftrc    fr4, fpul
        fsca    fpul, dr4
        fdiv    fr4, fr5

        fmov    xd8, dr8
        fmul    fr6, fr8
        fmov    xd10, dr10
        fmul    fr6, fr9
        fmul    fr6, fr10
        fmul    fr6, fr11

        fmov    xd12, xd8
        fmov    xd14, xd10
        fmov    dr8, xd12
        fmov    dr10, xd14

        fmov    xd4, dr8
        fmul    fr5, fr8
        fmul    fr5, fr9
        fmov    xd6, dr10
        fmul    fr5, fr10
        fmul    fr5, fr11
        fmov    dr8, xd4
        fmov    dr10, xd6

        fmov    xd0, dr8
        fmul    fr7, fr5
        fmov    xd2, dr10
        fmul    fr5, fr8
        fmul    fr5, fr9
        fmul    fr5, fr10
        fmul    fr5, fr11
        fmov    dr8, xd0
        fmov    dr10, xd2

        frchg
        fneg    fr8
        fneg    fr9
        fneg    fr10
        fneg    fr11
        frchg

        fschg
    )"
    :
	: "f"(f), "f"(a), "f"(n)
	: "fpul", "fr7", "fr8", "fr9", "fr10", "fr11");
}

// ****************************************************************
// shz_xmtrx_apply_screen(float w, float h)
// ****************************************************************
//  fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
// -----------+-----------+-----------+-----------
//	w*0.5f    | 0.0f      | 0.0f      | w*0.5f
// 	0.0f      | -h*0.5f   | 0.0f      | h*0.5f
// 	0.0f      | 0.0f      | 1.0f      | 0.0f
//  0.0f      | 0.0f      | 0.0f      | 1.0f
// ****************************************************************
SHZ_INLINE void shz_xmtrx_apply_screen(float width, float height) SHZ_NOEXCEPT {
	register float w asm("fr4") = width * 0.5f;
	register float h asm("fr5") = height * 0.5f;

	asm volatile(R"(
        fldi0   fr6
        fldi1   fr7
        fmov    fr4, fr8
        fmov    fr5, fr9
        ftrv    xmtrx, fv4
        fschg
        fneg    fr9
        fmov    xd0, dr10
        fmul    fr8, fr10
        fmul    fr8, fr11
        fmov    dr4, xd12
        fmov    dr6, xd14
        fmov    xd2, dr4
        fmul    fr8, fr4
        fmul    fr8, fr5
        fmov    dr10, xd0
        fmov    xd4, dr6
        fmul    fr9, fr6
        fmul    fr9, fr7
        fmov    dr4, xd2
        fmov    xd6, dr10
        fmul    fr9, fr10
        fmul    fr9, fr11
        fmov    dr6, xd4
        fmov    dr10, xd6
        fschg
    )"
	:
	: "f"(w), "f"(h)
	: "fr6", "fr7", "fr8", "fr9", "fr10", "fr11");
}

SHZ_INLINE void shz_xmtrx_apply_permutation_wxyz(void) SHZ_NOEXCEPT {
    asm volatile(R"(
        fldi0   fr0
        fldi0   fr1
        fldi0   fr2
        fldi1   fr3

        fldi1   fr4
        fldi0   fr5

        fschg
        fmov    dr0, dr6
        fmov    dr2, dr8
        fmov    dr0, dr10
        fmov    dr0, dr12
        fmov    dr4, dr14
        fschg

        ftrv    xmtrx, fv0
        ftrv    xmtrx, fv4
        ftrv    xmtrx, fv8
        ftrv    xmtrx, fv12

        frchg
    )"
    :
    :
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_translate(float x, float y, float z) SHZ_NOEXCEPT {
    asm volatile(R"(
        fldi1   fr0
        fldi0   fr1
        fldi0   fr2
        fldi0   fr3

        fschg
        fmov    dr2, dr6
        fmov    dr2, dr8
        fschg

        fmov.s  @%[x], fr12
        fmov.s  @%[y], fr13
        fmov.s  @%[z], fr14
        fldi1   fr15

        fldi0   fr4
        fldi1   fr5

        fldi1   fr10
        fldi0   fr11

        ftrv    xmtrx, fv0
        ftrv    xmtrx, fv4
        ftrv    xmtrx, fv8
        ftrv    xmtrx, fv12

        frchg
    )"
    :
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z),
      "m" (x), "m" (y), "m" (z)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_INLINE void shz_xmtrx_negate(void) SHZ_NOEXCEPT {
    asm volatile(R"(
        frchg
        fneg    fr0
        fneg    fr1
        fneg    fr2
        fneg    fr3
        fneg    fr4
        fneg    fr5
        fneg    fr6
        fneg    fr7
        fneg    fr8
        fneg    fr9
        fneg    fr10
        fneg    fr11
        fneg    fr12
        fneg    fr13
        fneg    fr14
        fneg    fr15
        frchg
    )");
}

SHZ_INLINE void shz_xmtrx_abs(void) SHZ_NOEXCEPT {
    asm volatile(R"(
        frchg
        fabs    fr0
        fabs    fr1
        fabs    fr2
        fabs    fr3
        fabs    fr4
        fabs    fr5
        fabs    fr6
        fabs    fr7
        fabs    fr8
        fabs    fr9
        fabs    fr10
        fabs    fr11
        fabs    fr12
        fabs    fr13
        fabs    fr14
        fabs    fr15
        frchg
    )");
}

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_transform_vec4(shz_vec4_t vec) SHZ_NOEXCEPT {
    register float rx asm("fr8")  = vec.x;
    register float ry asm("fr9")  = vec.y;
    register float rz asm("fr10") = vec.z;
    register float rw asm("fr11") = vec.w;

    asm volatile("ftrv xmtrx, fv8"
                 : "+f" (rx), "+f" (ry), "+f" (rz), "+f" (rw));

    return shz_vec4_init(rx, ry, rz, rw);
}

SHZ_FORCE_INLINE shz_vec3_t shz_xmtrx_transform_vec3(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_xmtrx_transform_vec4((shz_vec4_t) { .xyz = vec }).xyz;
}

SHZ_FORCE_INLINE shz_vec2_t shz_xmtrx_transform_vec2(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_xmtrx_transform_vec3((shz_vec3_t) { .xy = vec }).xy;
}

//! @}
