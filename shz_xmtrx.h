#ifndef SHZ_XMTRX_H
#define SHZ_XMTRX_H

#include "shz_matrix.h"

SHZ_BEGIN_DECLS

SHZ_HOT SHZ_ICACHE_ALIGNED
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

SHZ_HOT SHZ_ICACHE_ALIGNED
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

SHZ_HOT SHZ_ICACHE_ALIGNED
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

SHZ_HOT SHZ_ICACHE_ALIGNED
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

void shz_xmtrx_load_3x4(const shz_matrix_3x4_t *matrix) {
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

void shz_xmtrx_load_3x3(const shz_matrix_3x3_t *matrix) {
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

void shz_xmtrx_load_2x2(const shz_matrix_2x2_t *matrix) {
    asm volatile(R"(
        pref    @%[mat]
        frchg

        fldi1   fr10
        fldi1   fr15

        fmov.s  @%[mat]+, fr0
        fldi0   fr2
        fmov.s  @%[mat]+, fr1
        fldi0   fr3

        fmov.s  @%[mat]+, fr4
        fldi0   fr11
        fmov.s  @%[mat]+, fr5
        fldi0   fr14

        fschg
        fmov    dr2, dr6
        fmov    dr2, dr8
        fmov    dr2, dr12
        fschg

        frchg
    )"
    : [mat] "+r" (matrix));
}

SHZ_FORCE_INLINE unsigned shz_xmtrx_reg(unsigned row, unsigned col) {
    return col * 4 + row;
}

// go through FPUL instead of taking value
#define SHZ_XMTRX_LOAD_SCALAR(val, reg) do { \
        asm volatile(R"(                     \
            frchg                            \
            fmov.s  @%[s], reg               \
            frchg                            \
        )"                                   \
        :                                    \
        : [s] "r" (&var));                   \
    } while(false)

void shz_xmtrx_set_reg(float value, unsigned reg) {
    switch(reg) {
    case 0 : SHZ_XMTRX_LOAD_SCALAR(value, fr0);  break;
    case 1 : SHZ_XMTRX_LOAD_SCALAR(value, fr1);  break;
    case 2 : SHZ_XMTRX_LOAD_SCALAR(value, fr2);  break;
    case 3 : SHZ_XMTRX_LOAD_SCALAR(value, fr3);  break;
    case 4 : SHZ_XMTRX_LOAD_SCALAR(value, fr4);  break;
    case 5 : SHZ_XMTRX_LOAD_SCALAR(value, fr5);  break;
    case 6 : SHZ_XMTRX_LOAD_SCALAR(value, fr6);  break;
    case 7 : SHZ_XMTRX_LOAD_SCALAR(value, fr7);  break;
    case 8 : SHZ_XMTRX_LOAD_SCALAR(value, fr8);  break;
    case 9 : SHZ_XMTRX_LOAD_SCALAR(value, fr9);  break;
    case 10: SHZ_XMTRX_LOAD_SCALAR(value, fr10); break;
    case 11: SHZ_XMTRX_LOAD_SCALAR(value, fr11); break;
    case 12: SHZ_XMTRX_LOAD_SCALAR(value, fr12); break;
    case 13: SHZ_XMTRX_LOAD_SCALAR(value, fr13); break;
    case 14: SHZ_XMTRX_LOAD_SCALAR(value, fr14); break;
    case 15: SHZ_XMTRX_LOAD_SCALAR(value, fr15); break;
    default: break;
    }
}

SHZ_HOT SHZ_ICACHE_ALIGNED
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

SHZ_HOT SHZ_ICACHE_ALIGNED
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

SHZ_HOT SHZ_ICACHE_ALIGNED
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

SHZ_HOT SHZ_ICACHE_ALIGNED
SHZ_INLINE void shz_xmtrx_store_2x2(shz_matrix_2x2_t *matrix) {
    asm volatile(R"(
        frchg
        add     #16, %[mtx]

        fmov.s  fr5, @-%[mtx]
        fmov.s  fr4, @-%[mtx]

        fmov.s  fr1, @-%[mtx]
        fmov.s  fr0, @%[mtx]

        frchg
    )"
    : [mtx] "+&r" (matrix), "=m" (*matrix));
}

SHZ_HOT SHZ_ICACHE_ALIGNED
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

SHZ_HOT SHZ_ICACHE_ALIGNED
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
    : [x] "r" (&x), [y] "r" (&y), [z] "r" (&z));
}

SHZ_FORCE_INLINE void shz_xmtrx_set_scale(float x, float y, float z) {
    shz_xmtrx_set_diagonal(x, y, z, 1.0f);
}

void shz_xmtrx_set_rotation_x(float angle);
void shz_xmtrx_set_rotation_y(float angle);
void shz_xmtrx_set_rotation_z(float angle);
void shz_xmtrx_set_rotate(float roll, float pitch, float yaw);
void shz_xmtrx_set_translation(float x, float y, float z);
void shz_xmtrx_set_symmetric_skew(float x, float y, float z);
void shz_xmtrx_set_frustum(float left, float right, float bottom, float top, float near, float far);
void shz_xmtrx_set_orthographic(float left, float right, float bottom, float top);
void shz_xmtrx_set_perspective(float fovy, float aspect, float znear, float zfar);

void shz_xmtrx_apply_4x4(const shz_matrix_4x4_t *matrix);
void shz_xmtrx_apply_4x3(const shz_matrix_3x3_t *matrix);
void shz_xmtrx_apply_3x3(const shz_matrix_3x3_t *matrix);
void shz_xmtrx_apply_2x2(const shz_matrix_2x2_t *matrix, shz_xmtrx_quadrant_t pos);
void shz_xmtrx_apply_translation(float x, float y, float z);
void shz_xmtrx_apply_scale(float x, float y, float z);
void shz_xmtrx_apply_rotation_x(float angle);
void shz_xmtrx_apply_rotation_y(float angle);
void shz_xmtrx_apply_rotation_z(float angle);
void shz_xmtrx_apply_rotation(float roll, float pitch, float yaw);

void shz_xmtrx_outer_product_2x2(shz_vec2_t col, shz_vec2_t row);
void shz_xmtrx_outer_product_3x3(shz_vec3_t col, shz_vec3_t row);
void shz_xmtrx_outer_product_3x4(shz_vec4_t col, shz_vec3_t row);
void shz_xmtrx_outer_product_4x4(shz_vec4_t col, shz_vec4_t row);

void shz_xmtrx_invert(void);
void shz_xmtrx_invert_full(void);
float shz_xmtrx_determinant(void);
void shz_xmtrx_transpose(void);

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

SHZ_END_DECLS

#endif // SHZ_XMTRX_H
