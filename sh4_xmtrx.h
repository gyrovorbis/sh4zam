#ifndef SH4_XMTRX_H
#define SH4_XMTRX_H

#include "sh4_matrix.h"

SH4_BEGIN_DECLS

typedef enum sh4_xmtrx_regs {
    SH4_XMTRX_XF0,  SH4_XMTRX_XF1,  SH4_XMTRX_XF2,  SH4_XMTRX_XF3,  // XV0  (LEFT)
    SH4_XMTRX_XF4,  SH4_XMTRX_XF5,  SH4_XMTRX_XF6,  SH4_XMTRX_XF7,  // XV4  (UP)
    SH4_XMTRX_XF8,  SH4_XMTRX_XF9,  SH4_XMTRX_XF10, SH4_XMTRX_XF11, // XV8  (FORWARD)
    SH4_XMTRX_XF12, SH4_XMTRX_XF13, SH4_XMTRX_XF14, SH4_XMTRX_XF15  // XV12 (POS)
} sh4_xmtrx_regs_t;

typedef enum sh4_xmtrx_quadrant {
    SH4_XMTRX_TOP_LEFT,
    SH4_XMTRX_TOP_RIGHT,
    SH4_XMTRX_BOTTOM_LEFT,
    SH4_XMTRX_BOTTOM_RIGHT
} sh4_xmtrx_quadrant_t;

// Shear + reflection
SH4_HOT SH4_ICACHE_ALIGNED SH4_INLINE
void sh4_xmtrx_load_4x4(const sh4_matrix_4x4_t *matrix) {
    asm volatile(R"(
        fschg
        fmov.d	@%[mtx],xd0
        add	    #32,%[mtx]
        pref	@%[mtx]
        add	    #-(32-8),%[mtx]
        fmov.d	@%[mtx]+,xd2
        fmov.d	@%[mtx]+,xd4
        fmov.d	@%[mtx]+,xd6
        fmov.d	@%[mtx]+,xd8
        fmov.d	@%[mtx]+,xd10
        fmov.d	@%[mtx]+,xd12
        fmov.d	@%[mtx]+,xd14
        fschg
    )"
    : [mtx] "+r" (matrix));
}

SH4_HOT SH4_ICACHE_ALIGNED SH4_INLINE
void sh4_xmtrx_load_4x4_rows(const sh4_vec4_t *r1,
                             const sh4_vec4_t *r2,
                             const sh4_vec4_t *r3,
                             const sh4_vec4_t *r4) {
    asm volatile (R"(
        frchg

        pref    @%1
        fmov.s  @%0+,fr0
        fmov.s  @%0+,fr4
        fmov.s  @%0+,fr8
        fmov.s  @%0, fr12

        pref    @%2
        fmov.s  @%1+,fr1
        fmov.s  @%1+,fr5
        fmov.s  @%1+,fr9
        fmov.s  @%1, fr13

        pref    @%3
        fmov.s  @%2+,fr2
        fmov.s  @%2+,fr6
        fmov.s  @%2+,fr10
        fmov.s  @%2,fr14

        fmov.s  @%3+,fr3
        fmov.s  @%3+,fr7
        fmov.s  @%3+,fr11
        fmov.s  @%3,fr15

        frchg
    )"
    : "+&r" (r1), "+&r" (r2), "+&r" (r3), "+&r" (r4));
}

SH4_HOT SH4_ICACHE_ALIGNED SH4_INLINE
void sh4_xmtrx_load_4x4_transpose(const sh4_matrix_4x4_t *matrix) {
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

SH4_HOT SH4_ICACHE_ALIGNED SH4_INLINE
void sh4_xmtrx_load_4x4_apply(const sh4_matrix_4x4_t *matrix1,
                              const sh4_matrix_4x4_t *matrix2) {
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
    : [m1] "+&r" (matrix1), [m2] "+r" (matrix2), [prefscr] "=&r" (prefetch_scratch)
    :
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

void sh4_xmtrx_load_3x4(const sh4_matrix_3x4_t *matrix);
void sh4_xmtrx_load_3x3(const sh4_matrix_3x3_t *matrix);
void sh4_xmtrx_load_2x2(const sh4_matrix_2x2_t *matrix, sh4_xmtrx_quadrant_t pos);
void sh4_xmtrx_load_scalar(float value, sh4_xmtrx_regs_t reg);

SH4_HOT SH4_ICACHE_ALIGNED SH4_INLINE
void sh4_xmtrx_store_4x4(sh4_matrix_4x4_t *matrix) {
    asm volatile(R"(
        fschg
        add	    #64-8, %[mtx]
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

void sh4_xmtrx_store_3x4(sh4_matrix_3x4_t *matrix);
void sh4_xmtrx_store_3x3(sh4_matrix_3x3_t *matrix);
void sh4_xmtrx_store_2x2(sh4_matrix_2x2_t *matrix, sh4_xmtrx_quadrant_t pos);
float sh4_xmtrx_store_scalar(sh4_xmtrx_regs_t reg);

SH4_HOT SH4_ICACHE_ALIGNED SH4_INLINE
void sh4_xmtrx_set_identity(void) {
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

void sh4_xmtrx_set_scale(float x, float y, float z);
void sh4_xmtrx_set_diagonal(float x, float y, float z, float w);
void sh4_xmtrx_set_rotation_x(float angle);
void sh4_xmtrx_set_rotation_y(float angle);
void sh4_xmtrx_set_rotation_z(float angle);
void sh4_xmtrx_set_rotate(float roll, float pitch, float yaw);
void sh4_xmtrx_set_translation(float x, float y, float z);
void sh4_xmtrx_set_symmetric_skew(float x, float y, float z);
void sh4_xmtrx_set_frustum(float left, float right, float bottom, float top, float near, float far);
void sh4_xmtrx_set_orthographic(float left, float right, float bottom, float top);
void sh4_xmtrx_set_perspective(float fovy, float aspect, float znear, float zfar);

void sh4_xmtrx_apply_4x4(const sh4_matrix_4x4_t *matrix);
void sh4_xmtrx_apply_4x3(const sh4_matrix_3x3_t *matrix);
void sh4_xmtrx_apply_3x3(const sh4_matrix_3x3_t *matrix);
void sh4_xmtrx_apply_2x2(const sh4_matrix_2x2_t *matrix, sh4_xmtrx_quadrant_t pos);
void sh4_xmtrx_apply_translation(float x, float y, float z);
void sh4_xmtrx_apply_scale(float x, float y, float z);
void sh4_xmtrx_apply_rotation_x(float angle);
void sh4_xmtrx_apply_rotation_y(float angle);
void sh4_xmtrx_apply_rotation_z(float angle);
void sh4_xmtrx_apply_rotation(float roll, float pitch, float yaw);

void sh4_xmtrx_outer_product_2x2(sh4_vec2_t col, sh4_vec2_t row);
void sh4_xmtrx_outer_product_3x3(sh4_vec3_t col, sh4_vec3_t row);
void sh4_xmtrx_outer_product_3x4(sh4_vec4_t col, sh4_vec3_t row);
void sh4_xmtrx_outer_product_4x4(sh4_vec4_t col, sh4_vec4_t row);

void sh4_xmtrx_invert(void);
void sh4_xmtrx_invert_full(void);
float sh4_xmtrx_determinant(void);
void sh4_xmtrx_transpose(void);

SH4_FORCE_INLINE sh4_vec4_t sh4_xmtrx_trans_vec4(sh4_vec4_t vec) {
    register float rx asm("fr0") = vec.x;
    register float ry asm("fr1") = vec.y;
    register float rz asm("fr2") = vec.z;
    register float rw asm("fr3") = vec.w;

    asm volatile("ftrv xmtrx, fv0"
                 : "+f" (rx), "+f" (ry), "+f" (rz), "+f" (rw));

    return (sh4_vec4_t) { rx, ry, rz, rw };
}

SH4_FORCE_INLINE sh4_vec3_t sh4_xmtrx_trans_vec3(sh4_vec3_t vec) {
    return sh4_xmtrx_trans_vec4((sh4_vec4_t) { .vec3 = vec }).vec3;
}

SH4_FORCE_INLINE sh4_vec2_t sh4_xmtrx_trans_vec2(sh4_vec2_t vec) {
    return sh4_xmtrx_trans_vec3((sh4_vec3_t) { .vec2 = vec }).vec2;
}

SH4_END_DECLS

#endif // SH4_XMTRX_H
