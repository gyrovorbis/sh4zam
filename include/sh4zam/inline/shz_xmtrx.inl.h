//! \cond INTERNAL
/*! \file
 *  \brief   XMTRX API Implementation.
 *  \ingroup xmtrx
 *
 *  This file provides provides the inlined function implementations for the
 *  XMTRX API, forwarding on the function calls to the routines provided by a
 *  particular back-end.
 *
 *  \author 2026 Falco Girgis
 *
 *  \copyright MIT License
 */

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_xmtrx_sh4.inl.h"
#else
#   include "sw/shz_xmtrx_sw.inl.h"
#endif


SHZ_FORCE_INLINE float shz_xmtrx_read(shz_xmtrx_reg_t xf) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_xmtrx_read_sh4(xf);
#else
    return shz_xmtrx_read_sw(xf);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_write(shz_xmtrx_reg_t xf, float value) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_write_sh4(xf, value);
#else
    shz_xmtrx_write_sw(xf, value);
#endif
}

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_read_row(unsigned int index) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_xmtrx_read_row_sh4(index);
#else
    return shz_xmtrx_read_row_sw(index);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_write_row(unsigned int index, shz_vec4_t value) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_write_row_sh4(index, value);
#else
    shz_xmtrx_write_row_sw(index, value);
#endif
}

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_read_col(unsigned int index) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_xmtrx_read_col_sh4(index);
#else
    return shz_xmtrx_read_col_sw(index);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_write_col(unsigned int index, shz_vec4_t value) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_write_col_sh4(index, value);
#else
    shz_xmtrx_write_col_sw(index, value);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_swap_rows(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT {
    shz_vec4_t v1 = shz_xmtrx_read_row(index1);
    shz_vec4_t v2 = shz_xmtrx_read_row(index2);
    shz_xmtrx_write_row(index1, v2);
    shz_xmtrx_write_row(index2, v1);
}

SHZ_FORCE_INLINE void shz_xmtrx_swap_cols(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT {
    shz_vec4_t v1 = shz_xmtrx_read_col(index1);
    shz_vec4_t v2 = shz_xmtrx_read_col(index2);
    shz_xmtrx_write_col(index1, v2);
    shz_xmtrx_write_col(index2, v1);
}

/* ========== Loading ========== */

SHZ_FORCE_INLINE void shz_xmtrx_load_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_4x4_sh4(matrix);
#else
    shz_xmtrx_load_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_wxyz_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_wxyz_4x4_sh4(matrix);
#else
    shz_xmtrx_load_wxyz_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_aligned4_4x4(const float matrix[16]) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_aligned4_4x4_sh4(matrix);
#else
    shz_xmtrx_load_aligned4_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
    if(!((uintptr_t)matrix & 7))
        shz_xmtrx_load_4x4((const shz_mat4x4_t*)matrix);
    else
        shz_xmtrx_load_aligned4_4x4(matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_cols_4x4(const shz_vec4_t* c1,
                                              const shz_vec4_t* c2,
                                              const shz_vec4_t* c3,
                                              const shz_vec4_t* c4) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_cols_4x4_sh4(c1, c2, c3, c4);
#else
    shz_xmtrx_load_cols_4x4_sw(c1, c2, c3, c4);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_rows_4x4(const shz_vec4_t* r1,
                                              const shz_vec4_t* r2,
                                              const shz_vec4_t* r3,
                                              const shz_vec4_t* r4) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_rows_4x4_sh4(r1, r2, r3, r4);
#else
    shz_xmtrx_load_rows_4x4_sw(r1, r2, r3, r4);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_transpose_4x4_sh4(matrix);
#else
    shz_xmtrx_load_transpose_4x4_sw(matrix);
#endif
}

// Just straight up forward it, as shz_xmtrx_load_transpose_4x4() never required alignment anyway.
SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_transpose_4x4((const shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_3x4(const shz_mat3x4_t* mat) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_3x4_sh4(mat);
#else
    shz_xmtrx_load_3x4_sw(mat);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_cols_4x3(const shz_vec4_t* c1,
                                              const shz_vec4_t* c2,
                                              const shz_vec4_t* c3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_cols_4x3_sh4(c1, c2, c3);
#else
    shz_xmtrx_load_cols_4x3_sw(c1, c2, c3);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_rows_3x4(const shz_vec4_t* r1,
                                              const shz_vec4_t* r2,
                                              const shz_vec4_t* r3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_rows_3x4_sh4(r1, r2, r3);
#else
    shz_xmtrx_load_rows_3x4_sw(r1, r2, r3);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_3x3_sh4(matrix);
#else
    shz_xmtrx_load_3x3_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_3x3(const float* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_transpose_3x3_sh4(matrix);
#else
    shz_xmtrx_load_transpose_3x3_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_load_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_2x2_sh4(matrix);
#else
    shz_xmtrx_load_2x2_sw(matrix);
#endif
}

/* ========== Storing ========== */

SHZ_FORCE_INLINE void shz_xmtrx_store_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_store_4x4_sh4(matrix);
#else
    shz_xmtrx_store_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_store_aligned4_4x4(float matrix[16]) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_store_aligned4_4x4_sh4(matrix);
#else
    shz_xmtrx_store_aligned4_4x4_sw(matrix);
#endif
}

SHZ_INLINE void shz_xmtrx_store_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT {
    if(!((uintptr_t)matrix & 7))
        shz_xmtrx_store_4x4((shz_mat4x4_t*)matrix);
    else
        shz_xmtrx_store_aligned4_4x4(matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_store_transpose_4x4_sh4(matrix);
#else
    shz_xmtrx_store_transpose_4x4_sw(matrix);
#endif
}

// Just forward it directly to shz_xmtrx_store_transpose_4x4(), which never even required over-alignment...
SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_store_transpose_4x4((shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_store_3x4(shz_mat3x4_t* mat) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_store_3x4_sh4(mat);
#else
    shz_xmtrx_store_3x4_sw(mat);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_store_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_store_3x3_sh4(matrix);
#else
    shz_xmtrx_store_3x3_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_store_transpose_3x3_sh4(matrix);
#else
    shz_xmtrx_store_transpose_3x3_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_store_2x2(shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_store_2x2_sh4(matrix);
#else
    shz_xmtrx_store_2x2_sw(matrix);
#endif
}

/* ========== Initialization ========== */

SHZ_FORCE_INLINE void shz_xmtrx_init_identity(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_identity_sh4();
#else
    shz_xmtrx_init_identity_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_identity_safe(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_identity_safe_sh4();
#else
    shz_xmtrx_init_identity_safe_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_zero(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_zero_sh4();
#else
    shz_xmtrx_init_zero_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_one(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_one_sh4();
#else
    shz_xmtrx_init_one_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_fill(float value) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_fill_sh4(value);
#else
    shz_xmtrx_init_fill_sw(value);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_diagonal_sh4(x, y, z, w);
#else
    shz_xmtrx_init_diagonal_sw(x, y, z, w);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_upper_triangular(float col1, shz_vec2_t col2, shz_vec3_t col3, shz_vec4_t col4) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_upper_triangular_sh4(col1, col2, col3, col4);
#else
    shz_xmtrx_init_upper_triangular_sw(col1, col2, col3, col4);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_lower_triangular(shz_vec4_t col1, shz_vec3_t col2, shz_vec2_t col3, float col4) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_lower_triangular_sh4(col1, col2, col3, col4);
#else
    shz_xmtrx_init_lower_triangular_sw(col1, col2, col3, col4);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_scale(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_diagonal(x, y, z, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_x(float x) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_rotation_x_sh4(x);
#else
    shz_xmtrx_init_rotation_x_sw(x);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_y(float y) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_rotation_y_sh4(y);
#else
    shz_xmtrx_init_rotation_y_sw(y);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_z(float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_rotation_z_sh4(z);
#else
    shz_xmtrx_init_rotation_z_sw(z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation(float angle, float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_rotation_sh4(angle, x, y, z);
#else
    shz_xmtrx_init_rotation_sw(angle, x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_translation(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_translation_sh4(x, y, z);
#else
    shz_xmtrx_init_translation_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_symmetric_skew_sh4(x, y, z);
#else
    shz_xmtrx_init_symmetric_skew_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_outer_product(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_outer_product_sh4(a, b);
#else
    shz_xmtrx_init_outer_product_sw(a, b);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_permutation_wxyz(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_permutation_wxyz_sh4();
#else
    shz_xmtrx_init_permutation_wxyz_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_permutation_yzwx(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_permutation_yzwx_sh4();
#else
    shz_xmtrx_init_permutation_yzwx_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_screen(float width, float height) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_init_screen_sh4(width, height);
#else
    shz_xmtrx_init_screen_sw(width, height);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

// Same as yaw, pitch, roll
SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_z(zAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_zxy(float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_z(zAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

/* ========== Apply Operations ========== */

SHZ_FORCE_INLINE void shz_xmtrx_apply_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_4x4_sh4(matrix);
#else
    shz_xmtrx_apply_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_aligned4_4x4(const float matrix[16]) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_aligned4_4x4_sh4(matrix);
#else
    shz_xmtrx_apply_aligned4_4x4_sw(matrix);
#endif
}

SHZ_INLINE void shz_xmtrx_apply_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
    if(!((uintptr_t)matrix & 0x7))
        shz_xmtrx_apply_4x4((const shz_mat4x4_t*)matrix);
    else
        shz_xmtrx_apply_aligned4_4x4(matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_transpose_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_transpose_4x4_sh4(matrix);
#else
    shz_xmtrx_apply_transpose_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_apply_transpose_4x4((const shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_reverse_4x4_sh4(matrix);
#else
    shz_xmtrx_apply_reverse_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_aligned4_4x4(const float matrix[16]) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_reverse_aligned4_4x4_sh4(matrix);
#else
    shz_xmtrx_apply_reverse_aligned4_4x4_sw(matrix);
#endif
}

SHZ_INLINE void shz_xmtrx_apply_reverse_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
    if(!((uintptr_t)matrix & 0x7))
        shz_xmtrx_apply_reverse_4x4((const shz_mat4x4_t*)matrix);
    else
        shz_xmtrx_apply_reverse_aligned4_4x4(matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_transpose_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_reverse_transpose_4x4_sh4(matrix);
#else
    shz_xmtrx_apply_reverse_transpose_4x4_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_apply_reverse_transpose_4x4((const shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_3x4(const shz_mat3x4_t* mat) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_3x4_sh4(mat);
#else
    shz_xmtrx_apply_3x4_sw(mat);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_3x3_sh4(matrix);
#else
    shz_xmtrx_apply_3x3_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_transpose_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_transpose_3x3_sh4(matrix);
#else
    shz_xmtrx_apply_transpose_3x3_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_2x2_sh4(matrix);
#else
    shz_xmtrx_apply_2x2_sw(matrix);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_set_translation(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_set_translation_sh4(x, y, z);
#else
    shz_xmtrx_set_translation_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_translation(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_translation_sh4(x, y, z);
#else
    shz_xmtrx_apply_translation_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_scale(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_scale_sh4(x, y, z);
#else
    shz_xmtrx_apply_scale_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_x(float x) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_rotation_x_sh4(x);
#else
    shz_xmtrx_apply_rotation_x_sw(x);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_y(float y) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_rotation_y_sh4(y);
#else
    shz_xmtrx_apply_rotation_y_sw(y);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_z(float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_rotation_z_sh4(z);
#else
    shz_xmtrx_apply_rotation_z_sw(z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation(float angle, float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_rotation_sh4(angle, x, y, z);
#else
    shz_xmtrx_apply_rotation_sw(angle, x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_quat(shz_quat_t q) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_rotation_quat_sh4(q);
#else
    shz_xmtrx_apply_rotation_quat_sw(q);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_apply_rotation_z(zAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_zxy(float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_apply_rotation_z(zAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_y(yAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_apply_rotation_z(zAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_symmetric_skew_sh4(x, y, z);
#else
    shz_xmtrx_apply_symmetric_skew_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_lookat(shz_vec3_t eye,
                                             shz_vec3_t center,
                                             shz_vec3_t up) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_lookat_sh4(eye, center, up);
#else
    shz_xmtrx_apply_lookat_sw(eye, center, up);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_ortho(float left, float right, float bottom, float top, float near, float far) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_ortho_sh4(left, right, bottom, top, near, far);
#else
    shz_xmtrx_apply_ortho_sw(left, right, bottom, top, near, far);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_frustum(float left, float right, float bottom, float top, float near, float far) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_frustum_sh4(left, right, bottom, top, near, far);
#else
    shz_xmtrx_apply_frustum_sw(left, right, bottom, top, near, far);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_perspective(float fov, float aspect, float near_z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_perspective_sh4(fov, aspect, near_z);
#else
    shz_xmtrx_apply_perspective_sw(fov, aspect, near_z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_screen(float width, float height) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_screen_sh4(width, height);
#else
    shz_xmtrx_apply_screen_sw(width, height);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_permutation_wxyz(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_permutation_wxyz_sh4();
#else
    shz_xmtrx_apply_permutation_wxyz_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_permutation_yzwx(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_permutation_yzwx_sh4();
#else
    shz_xmtrx_apply_permutation_yzwx_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_self(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_apply_self_sh4();
#else
    shz_xmtrx_apply_self_sw();
#endif
}

/* ========== GL Transformations ========== */

SHZ_FORCE_INLINE void shz_xmtrx_translate(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_translate_sh4(x, y, z);
#else
    shz_xmtrx_translate_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_scale(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_scale_sh4(x, y, z);
#else
    shz_xmtrx_scale_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_x(float radians) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_rotate_x_sh4(radians);
#else
    shz_xmtrx_rotate_x_sw(radians);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_y(float radians) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_rotate_y_sh4(radians);
#else
    shz_xmtrx_rotate_y_sw(radians);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_z(float radians) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_rotate_z_sh4(radians);
#else
    shz_xmtrx_rotate_z_sw(radians);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_rotate_x(xAngle);
    shz_xmtrx_rotate_y(yAngle);
    shz_xmtrx_rotate_z(zAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_rotate_z(zAngle);
    shz_xmtrx_rotate_y(yAngle);
    shz_xmtrx_rotate_x(xAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_zxy(float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_rotate_z(zAngle);
    shz_xmtrx_rotate_x(xAngle);
    shz_xmtrx_rotate_y(yAngle);
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_rotate_y(yAngle);
    shz_xmtrx_rotate_x(xAngle);
    shz_xmtrx_rotate_z(zAngle);
}

SHZ_INLINE void shz_xmtrx_rotate(float angle, float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_rotate_sh4(angle, x, y, z);
#else
    shz_xmtrx_rotate_sw(angle, x, y, z);
#endif
}

/* ========== Compound Operations ========== */

SHZ_FORCE_INLINE void shz_xmtrx_load_apply_4x4(const shz_mat4x4_t* matrix1,
                                               const shz_mat4x4_t* matrix2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_apply_4x4_sh4(matrix1, matrix2);
#else
    shz_xmtrx_load_apply_4x4_sw(matrix1, matrix2);
#endif
}

SHZ_INLINE void shz_xmtrx_load_apply_unaligned_4x4(const float matrix1[16],
                                                   const float matrix2[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_unaligned_4x4(matrix1);
    shz_xmtrx_apply_unaligned_4x4(matrix2);
}

SHZ_INLINE void shz_xmtrx_load_apply_store_unaligned_4x4(float out[16],
                                                        const float mat1[16],
                                                        const float mat2[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_apply_unaligned_4x4(mat1, mat2);
    shz_xmtrx_store_unaligned_4x4(out);
}

SHZ_INLINE void shz_xmtrx_load_apply_store_4x4(shz_mat4x4_t* out,
                                               const shz_mat4x4_t* mat1,
                                               const shz_mat4x4_t* mat2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_apply_store_4x4_sh4(out, mat1, mat2);
#else
    shz_xmtrx_load_apply_store_4x4_sw(out, mat1, mat2);
#endif
}

SHZ_INLINE void shz_xmtrx_load_apply_store_3x4(shz_mat3x4_t* out,
                                               const shz_mat3x4_t* mat1,
                                               const shz_mat3x4_t* mat2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_apply_store_3x4_sh4(out, mat1, mat2);
#else
    shz_xmtrx_load_apply_store_3x4_sw(out, mat1, mat2);
#endif
}

SHZ_INLINE void shz_xmtrx_load_apply_store_3x3(shz_mat3x3_t* out,
                                               const shz_mat3x3_t* mat1,
                                               const shz_mat3x3_t* mat2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_load_apply_store_3x3_sh4(out, mat1, mat2);
#else
    shz_xmtrx_load_apply_store_3x3_sw(out, mat1, mat2);
#endif
}

/* ========== Miscellaneous ========== */

SHZ_FORCE_INLINE void shz_xmtrx_add_4x4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4    
    shz_xmtrx_add_4x4_sh4(mat);
#else
    shz_xmtrx_add_4x4_sw(mat);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_sub_4x4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_sub_4x4_sh4(mat);
#else
    shz_xmtrx_sub_4x4_sw(mat);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_add_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_add_symmetric_skew_sh4(x, y, z);
#else
    shz_xmtrx_add_symmetric_skew_sw(x, y, z);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_add_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_add_diagonal_sh4(x, y, z, w);
#else
    shz_xmtrx_add_diagonal_sw(x, y, z, w);
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_transpose(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_transpose_sh4();
#else
    shz_xmtrx_transpose_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_negate(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_negate_sh4();
#else
    shz_xmtrx_negate_sw();
#endif
}

SHZ_FORCE_INLINE void shz_xmtrx_abs(void) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_xmtrx_abs_sh4();
#else
    shz_xmtrx_abs_sw();
#endif
}

shz_quat_t shz_xmtrx_to_quat(void) SHZ_NOEXCEPT;
float shz_xmtrx_determinant(void) SHZ_NOEXCEPT;

/* ========== Transformations ========== */

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_transform_vec4(shz_vec4_t vec) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_xmtrx_transform_vec4_sh4(vec);
#else
    return shz_xmtrx_transform_vec4_sw(vec);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_xmtrx_transform_vec3(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_xmtrx_transform_vec4(shz_vec3_vec4(vec, 0.0f)).xyz;
}

SHZ_FORCE_INLINE shz_vec2_t shz_xmtrx_transform_vec2(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_xmtrx_transform_vec3(shz_vec2_vec3(vec, 0.0f)).xy;
}

SHZ_FORCE_INLINE shz_vec2_t shz_xmtrx_transform_point2(shz_vec2_t pt) SHZ_NOEXCEPT {
    return shz_xmtrx_transform_vec4(shz_vec2_vec4(pt, 0.0f, 1.0f)).xy;
}

SHZ_FORCE_INLINE shz_vec3_t shz_xmtrx_transform_point3(shz_vec3_t pt) SHZ_NOEXCEPT {
    return shz_xmtrx_transform_vec4(shz_vec3_vec4(pt, 1.0f)).xyz;
}

/* ========== Init Compositions ========== */

SHZ_FORCE_INLINE void shz_xmtrx_init_lookat(shz_vec3_t eye, shz_vec3_t center, shz_vec3_t up) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity();
    shz_xmtrx_apply_lookat(eye, center, up);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_ortho(float left, float right, float bottom, float top, float near, float far) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity();
    shz_xmtrx_apply_ortho(left, right, bottom, top, near, far);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_frustum(float left, float right, float bottom, float top, float near, float far) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity();
    shz_xmtrx_apply_frustum(left, right, bottom, top, near, far);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_perspective(float fov, float aspect, float near_z) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity();
    shz_xmtrx_apply_perspective(fov, aspect, near_z);
}
