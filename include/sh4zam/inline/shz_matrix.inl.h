//! \cond INTERNAL
/*! \file
    \brief   Implementation of C Matrix API
    \ingroup matrix

    This file provides the private implementation for all inlined funtions
    declared within the Matrix C API.

    \author    2025, 2026 Falco Girgis
    \copyright MIT License
*/

#include <stdalign.h>
#include <assert.h>

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_matrix_sh4.inl.h"
#else
#   include "sw/shz_matrix_sw.inl.h"
#endif

SHZ_INLINE void shz_mat4x4_init_identity(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_identity_safe(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity_safe();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_zero(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_init_zero();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_one(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_init_one();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_fill(shz_mat4x4_t* mat, float value) SHZ_NOEXCEPT {
    shz_xmtrx_init_fill(value);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_translation(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_scale(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_x(shz_mat4x4_t* mat, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_x(xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_y(shz_mat4x4_t* mat, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_y(yAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_z(shz_mat4x4_t* mat, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_z(zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_xyz(shz_mat4x4_t* mat, float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_xyz(xAngle, yAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_zyx(shz_mat4x4_t* mat, float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_zyx(zAngle, yAngle, xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_zxy(shz_mat4x4_t* mat, float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_zxy(zAngle, xAngle, yAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_yxz(shz_mat4x4_t* mat, float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_yxz(yAngle, xAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation(shz_mat4x4_t* mat, float angle, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation(angle, x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_set_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;
SHZ_INLINE void shz_mat4x4_init_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT {
	shz_mat4x4_set_rotation_quat(m, q);

	m->elem2D[3][0] = 0.0f;
	m->elem2D[3][1] = 0.0f;
	m->elem2D[3][2] = 0.0f;

	m->elem2D[0][3] = 0.0f;
	m->elem2D[1][3] = 0.0f;
	m->elem2D[2][3] = 0.0f;
	m->elem2D[3][3] = 1.0f;
}

SHZ_INLINE void shz_mat4x4_init_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT {
    shz_xmtrx_init_diagonal(x, y, z, w);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_upper_triangular(shz_mat4x4_t* mat, float col1, shz_vec2_t col2, shz_vec3_t col3, shz_vec4_t col4) SHZ_NOEXCEPT {
    shz_xmtrx_init_upper_triangular(col1, col2, col3, col4);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_lower_triangular(shz_mat4x4_t* mat, shz_vec4_t col1, shz_vec3_t col2, shz_vec2_t col3, float col4) SHZ_NOEXCEPT {
    shz_xmtrx_init_lower_triangular(col1, col2, col3, col4);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_symmetric_skew(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_outer_product(shz_mat4x4_t* mat, shz_vec4_t v1, shz_vec4_t v2) SHZ_NOEXCEPT {
    shz_xmtrx_init_outer_product(v1, v2);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_permutation_wxyz(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_init_permutation_wxyz();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_permutation_yzwx(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_init_permutation_yzwx();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_screen(shz_mat4x4_t* mat, float width, float height) SHZ_NOEXCEPT {
    shz_xmtrx_init_screen(width, height);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_set_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    mat->pos.x = x;
    mat->pos.y = y;
    mat->pos.z = z;
}

SHZ_INLINE void shz_mat4x4_set_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    mat->elem2D[0][0] = x;
    mat->elem2D[1][1] = y;
    mat->elem2D[2][2] = z;
}

SHZ_INLINE void shz_mat4x4_set_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT {
	m->elem2D[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	m->elem2D[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
	m->elem2D[2][0] = 2.0f * (q.x * q.z + q.y * q.w);

	m->elem2D[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
	m->elem2D[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
	m->elem2D[2][1] = 2.0f * (q.y * q.z - q.x * q.w);

	m->elem2D[0][2] = 2.0f * (q.x * q.z - q.w * q.y);
	m->elem2D[1][2] = 2.0f * (q.y * q.z + q.w * q.x);
	m->elem2D[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
}

SHZ_INLINE void shz_mat4x4_set_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT {
    shz_mat4x4_set_scale(mat, x, y, z);
    mat->elem2D[3][3] = w;
}

SHZ_INLINE void shz_mat4x4_apply(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
    shz_xmtrx_load_apply_store_4x4(dst, dst, src);
}

SHZ_INLINE void shz_mat4x4_apply_unaligned(shz_mat4x4_t* dst, const float mat[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(dst);
    shz_xmtrx_apply_unaligned_4x4(mat);
    shz_xmtrx_store_4x4(dst);
}

SHZ_INLINE void shz_mat4x4_apply_transpose(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(dst);
    shz_xmtrx_apply_transpose_4x4(src);
    shz_xmtrx_store_4x4(dst);
}

SHZ_INLINE void shz_mat4x4_apply_transpose_unaligned(shz_mat4x4_t* dst, const float src[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(dst);
    shz_xmtrx_apply_transpose_unaligned_4x4(src);
    shz_xmtrx_store_4x4(dst);
}

SHZ_FORCE_INLINE void shz_mat4x4_apply_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    mat->pos.x += x;
    mat->pos.y += y;
    mat->pos.z += z;
}

SHZ_INLINE void shz_mat4x4_apply_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_scale(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_x(shz_mat4x4_t* mat, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_y(shz_mat4x4_t* mat, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_z(shz_mat4x4_t* mat, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_z(zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_xyz(shz_mat4x4_t* mat, float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_xyz(xAngle, yAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_zyx(shz_mat4x4_t* mat, float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_zyx(zAngle, yAngle, xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_zxy(shz_mat4x4_t* mat, float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_zxy(zAngle, xAngle, yAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_yxz(shz_mat4x4_t* mat, float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_yxz(yAngle, xAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation(shz_mat4x4_t* mat, float angle, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation(angle, x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT {
	shz_mat4x4_t rot;

	shz_mat4x4_init_rotation_quat(&rot, q);
	shz_mat4x4_apply(m, &rot);
}

SHZ_INLINE void shz_mat4x4_apply_lookat(shz_mat4x4_t* m, shz_vec3_t pos, shz_vec3_t target, shz_vec3_t up) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(m);
    shz_xmtrx_apply_lookat(pos, target, up);
    shz_xmtrx_store_4x4(m);
}

SHZ_INLINE void shz_mat4x4_apply_perspective(shz_mat4x4_t* m, float fov, float aspect, float near_z) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(m);
    shz_xmtrx_apply_perspective(fov, aspect, near_z);
    shz_xmtrx_store_4x4(m);
}

SHZ_INLINE void shz_mat4x4_apply_screen(shz_mat4x4_t* m, float width, float height) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(m);
    shz_xmtrx_apply_screen(width, height);
    shz_xmtrx_store_4x4(m);
}

SHZ_INLINE void shz_mat4x4_apply_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT{
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_symmetric_skew(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_permutation_wxyz(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_permutation_wxyz();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_permutation_yzwx(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_permutation_yzwx();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_self(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_self();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_translate(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_translate(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_scale(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_rotate_x(shz_mat4x4_t* mat, float radians) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate_x(radians);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_rotate_y(shz_mat4x4_t* mat, float radians) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate_y(radians);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_rotate_z(shz_mat4x4_t* mat, float radians) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate_z(radians);
    shz_xmtrx_store_4x4(mat);
}

SHZ_FORCE_INLINE void shz_mat4x4_rotate_xyz(shz_mat4x4_t* mat, float xRadians, float yRadians, float zRadians) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate_xyz(xRadians, yRadians, zRadians);
    shz_xmtrx_store_4x4(mat);
}

SHZ_FORCE_INLINE void shz_mat4x4_rotate_zyx(shz_mat4x4_t* mat, float zRadians, float yRadians, float xRadians) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate_zyx(zRadians, yRadians, xRadians);
    shz_xmtrx_store_4x4(mat);
}

SHZ_FORCE_INLINE void shz_mat4x4_rotate_zxy(shz_mat4x4_t* mat, float zRadians, float xRadians, float yRadians) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate_zxy(zRadians, xRadians, yRadians);
    shz_xmtrx_store_4x4(mat);
}

SHZ_FORCE_INLINE void shz_mat4x4_rotate_yxz(shz_mat4x4_t* mat, float yRadians, float xRadians, float zRadians) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate_yxz(yRadians, xRadians, zRadians);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_rotate(shz_mat4x4_t* mat, float radians, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_rotate(radians, xAxis, yAxis, zAxis);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_mult(shz_mat4x4_t* mat, const shz_mat4x4_t* lhs, const shz_mat4x4_t* rhs) SHZ_NOEXCEPT {
    shz_xmtrx_load_apply_store_4x4(mat, lhs, rhs);
}

SHZ_INLINE void shz_mat4x4_mult_unaligned(shz_mat4x4_t* mat, const shz_mat4x4_t* lhs, const float rhs[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(lhs);
    shz_xmtrx_apply_unaligned_4x4(rhs);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE shz_vec4_t shz_mat4x4_row(const shz_mat4x4_t* mat, size_t row) SHZ_NOEXCEPT {
    assert(row < 4);

    return shz_vec4_init(mat->elem2D[0][row], mat->elem2D[1][row], mat->elem2D[2][row], mat->elem2D[3][row]);
}

SHZ_INLINE bool shz_mat4x4_equal(const shz_mat4x4_t* mat1, const shz_mat4x4_t* mat2) SHZ_NOEXCEPT {
    return shz_vec4_equal(shz_mat4x4_col(mat1, 0), shz_mat4x4_col(mat2, 0)) &&
           shz_vec4_equal(shz_mat4x4_col(mat1, 1), shz_mat4x4_col(mat2, 1)) &&
           shz_vec4_equal(shz_mat4x4_col(mat1, 2), shz_mat4x4_col(mat2, 2)) &&
           shz_vec4_equal(shz_mat4x4_col(mat1, 3), shz_mat4x4_col(mat2, 3));
}

SHZ_INLINE bool shz_mat4x4_is_block_triangular(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
#if 0
    return  shz_equalf(mat->col[0].w, 0.0f) &&
            shz_equalf(mat->col[1].w, 0.0f) &&
            shz_equalf(mat->col[2].w, 0.0f) &&
           !shz_equalf(mat->col[3].w, 0.0f);
#else
    return mat->col[0].w == 0.0f &&
           mat->col[1].w == 0.0f &&
           mat->col[2].w == 0.0f &&
           mat->col[3].w != 0.0f;
#endif
}

// \todo idk, some baller-ass 'sembly copying? FMOV.D at least first 2 elements of cols?
SHZ_INLINE void shz_mat4x4_3x3(const shz_mat4x4_t* mat4, shz_mat3x3_t* mat3) SHZ_NOEXCEPT {
    mat3->col[0] = mat4->col[0].xyz;
    mat3->col[1] = mat4->col[1].xyz;
    mat3->col[2] = mat4->col[2].xyz;
}

SHZ_FORCE_INLINE shz_vec4_t shz_mat4x4_col(const shz_mat4x4_t* mat, size_t col) SHZ_NOEXCEPT {
    assert(col < 4);

    return mat->col[col];
}

SHZ_FORCE_INLINE void shz_mat4x4_set_row(shz_mat4x4_t* mat, size_t row, shz_vec4_t values) SHZ_NOEXCEPT {
    assert(row < 4);

    mat->elem2D[0][row] = values.x;
    mat->elem2D[1][row] = values.y;
    mat->elem2D[2][row] = values.z;
    mat->elem2D[3][row] = values.w;
}

SHZ_FORCE_INLINE void shz_mat4x4_set_col(shz_mat4x4_t* mat, size_t col, shz_vec4_t values) SHZ_NOEXCEPT {
    assert(col < 4);

    mat->col[col] = values;
}

SHZ_INLINE void shz_mat4x4_swap_rows(shz_mat4x4_t* mat, size_t row1, size_t row2) SHZ_NOEXCEPT {
    shz_vec4_t tmp = shz_mat4x4_row(mat, row1);

    shz_mat4x4_set_row(mat, row1, shz_mat4x4_row(mat, row2));
    shz_mat4x4_set_row(mat, row2, tmp);
}

SHZ_INLINE void shz_mat4x4_swap_cols(shz_mat4x4_t* mat, size_t col1, size_t col2) SHZ_NOEXCEPT {
    shz_vec4_t tmp = shz_mat4x4_col(mat, col1);

    shz_mat4x4_set_col(mat, col1, shz_mat4x4_col(mat, col2));
    shz_mat4x4_set_col(mat, col2, tmp);
}


SHZ_INLINE shz_vec2_t shz_mat4x4_transform_vec2(const shz_mat4x4_t* mat, shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_dot2(vec,
                         shz_mat4x4_row(mat, 0).xy,
                         shz_mat4x4_row(mat, 1).xy);
}

SHZ_INLINE shz_vec2_t shz_mat4x4_transform_point2(const shz_mat4x4_t* mat, shz_vec2_t pt) SHZ_NOEXCEPT {
    return shz_vec4_dot2(shz_vec2_vec4(pt, 0.0f, 1.0f),
                         shz_mat4x4_row(mat, 0),
                         shz_mat4x4_row(mat, 1));
}

SHZ_INLINE shz_vec2_t shz_mat4x4_transform_vec2_transpose(const shz_mat4x4_t* mat, shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_dot2(vec,
                         shz_mat4x4_col(mat, 0).xy,
                         shz_mat4x4_col(mat, 1).xy);
}

SHZ_INLINE shz_vec2_t shz_mat4x4_transform_point2_transpose(const shz_mat4x4_t* mat, shz_vec2_t pt) SHZ_NOEXCEPT {
    return shz_vec4_dot2(shz_vec2_vec4(pt, 0.0f, 1.0f),
                         shz_mat4x4_col(mat, 0),
                         shz_mat4x4_col(mat, 1));
}

SHZ_INLINE shz_vec3_t shz_mat4x4_transform_vec3(const shz_mat4x4_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_mat4x4_transform_vec3_sh4(mat, v);
#else
    return shz_mat4x4_transform_vec3_sw(mat, v);
#endif
}

SHZ_INLINE shz_vec4_t shz_mat4x4_transform_vec4(const shz_mat4x4_t* mat, shz_vec4_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_mat4x4_transform_vec4_sh4(mat, v);
#else
    return shz_mat4x4_transform_vec4_sw(mat, v);
#endif
}

SHZ_INLINE shz_vec4_t shz_mat4x4_transform_vec4_transpose(const shz_mat4x4_t* mat, shz_vec4_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_mat4x4_transform_vec4_transpose_sh4(mat, v);
#else
    return shz_mat4x4_transform_vec4_transpose_sw(mat, v);
#endif
}

SHZ_INLINE shz_vec3_t shz_mat4x4_transform_vec3_transpose(const shz_mat4x4_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_mat4x4_transform_vec3_transpose_sh4(mat, v);
#else
    return shz_mat4x4_transform_vec3_transpose_sw(mat, v);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_mat4x4_transform_point3(const shz_mat4x4_t* mat, shz_vec3_t pt) SHZ_NOEXCEPT {
    return shz_mat4x4_transform_vec4(mat, shz_vec3_vec4(pt, 1.0f)).xyz;
}

SHZ_FORCE_INLINE shz_vec3_t shz_mat4x4_transform_point3_transpose(const shz_mat4x4_t* mat, shz_vec3_t pt) SHZ_NOEXCEPT {
    return shz_vec4_dot3(shz_vec3_vec4(pt, 1.0f),
                         shz_mat4x4_col(mat, 0),
                         shz_mat4x4_col(mat, 1),
                         shz_mat4x4_col(mat, 2));
}

SHZ_INLINE shz_quat_t shz_mat4x4_to_quat(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    float f, s, m;

    if((f = mat->up.y + mat->left.x + mat->forward.z) >= 0.0f) {
        s = shz_inv_sqrtf_fsrra(f += 1.0f);
        m = 0.5f * s;

        return shz_quat_init(
            (0.5f * f *                    s),
            (mat->up.z      - mat->forward.y) * m,
            (mat->forward.x - mat->left.z   ) * m,
            (mat->left.y    - mat->up.x     ) * m
        );
    } else if (mat->left.x >= mat->up.y && mat->left.x >= mat->forward.z) {
        f = 1.0f - mat->up.y - mat->forward.z + mat->left.x;
        s = shz_inv_sqrtf(f);
        m = 0.5f * s;

        return shz_quat_init(
            (mat->up.z      - mat->forward.y) * m,
            (0.5f * f *                    s),
            (mat->up.x      + mat->left.y   ) * m,
            (mat->forward.x + mat->left.z   ) * m
        );
    } else if (mat->up.y >= mat->forward.z) {
        f = 1.0f - mat->left.x - mat->forward.z + mat->up.y;
        s = shz_inv_sqrtf(f);
        m = 0.5f * s;

        return shz_quat_init(
            (mat->forward.x - mat->left.z) * m,
            (mat->up.x      + mat->left.y) * m,
            (0.5f * f *                 s),
            (mat->forward.y + mat->up.z  ) * m
        );
    } else {
        f = 1.0f - mat->left.x - mat->up.y + mat->forward.z;
        s = shz_inv_sqrtf(f);
        m = 0.5f * s;

        return shz_quat_init(
            (mat->left.y    - mat->up.x  ) * m,
            (mat->forward.x + mat->left.z) * m,
            (mat->forward.y + mat->up.z  ) * m,
            (0.5f * f *                 s)
        );
    }
}

SHZ_INLINE void shz_mat4x4_transpose(const shz_mat4x4_t* mat, shz_mat4x4_t* out) SHZ_NOEXCEPT {
    shz_xmtrx_load_transpose_4x4(mat);
    shz_xmtrx_store_4x4(out);
}

SHZ_FORCE_INLINE float shz_mat4x4_trace(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    return mat->elem2D[0][0] + mat->elem2D[1][1] + mat->elem2D[2][2] + mat->elem2D[3][3];
}

SHZ_INLINE float shz_mat4x4_determinant(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
	const float (*m)[4] = mat->elem2D;

#if 0 // Cache the subfactors
    float s0 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    float s1 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    float s2 = m[2][0] * m[3][3] - m[3][0] * m[2][3];

    float s3 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    float s4 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    float s5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
#else // FIPR da subfactors
    shz_vec2_t s01 = shz_vec2_dot2(shz_vec2_init(m[2][1], -m[3][1]),
                                   shz_vec2_init(m[3][3],  m[2][3]),
                                   shz_vec2_init(m[3][2],  m[2][2]));
    float s0 = s01.x;
    float s1 = s01.y;
    float s2 = m[2][0] * m[3][3] - m[3][0] * m[2][3];

    shz_vec3_t s345 = shz_vec2_dot3(shz_vec2_init(m[2][0], -m[3][0]),
                                    shz_vec2_init(m[3][3],  m[2][3]),
                                    shz_vec2_init(m[3][2],  m[2][2]),
                                    shz_vec2_init(m[3][1],  m[2][1]));
    float s3 = s345.x;
    float s4 = s345.y;
    float s5 = s345.z;
#endif
    shz_vec4_t coeff =
        shz_vec4_init(+shz_dot6f(m[1][1], -m[1][2], m[1][3], s0, s1, s2),
                      -shz_dot6f(m[1][0], -m[1][2], m[1][3], s0, s3, s4),
                      +shz_dot6f(m[1][0], -m[1][1], m[1][3], s1, s3, s5),
                      -shz_dot6f(m[1][0], -m[1][1], m[1][2], s2, s4, s5));

    return shz_dot8f(   m[0][0],    m[0][1],    m[0][2],    m[0][3],
                     coeff.e[0], coeff.e[1], coeff.e[2], coeff.e[3]);
}

SHZ_INLINE void shz_mat4x4_copy(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_mat4x4_copy_sh4(dst, src);
#else
    shz_mat4x4_copy_sw(dst, src);
#endif
}

SHZ_INLINE void shz_mat4x4_copy_unaligned(shz_mat4x4_t* dst, const float src[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_unaligned_4x4(src);
    shz_xmtrx_store_4x4(dst);
}

SHZ_INLINE shz_vec3_t shz_mat3x3_transform_vec3(const shz_mat3x3_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_mat3x3_transform_vec3_sh4(mat, v);
#else
    return shz_mat3x3_transform_vec3_sw(mat, v);
#endif
}

SHZ_INLINE shz_vec3_t shz_mat3x3_transform_vec3_transpose(const shz_mat3x3_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_mat3x3_transform_vec3_transpose_sh4(mat, v);
#else
    return shz_mat3x3_transform_vec3_transpose_sw(mat, v);
#endif
}

SHZ_INLINE float shz_mat4x4_3x3_determinant(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    return shz_vec3_dot(mat->col[0].xyz, shz_vec3_cross(mat->col[1].xyz, mat->col[2].xyz));
}

SHZ_INLINE void shz_mat4x4_3x3_inverse_unscaled(const shz_mat4x4_t* mat4, shz_mat3x3_t* mat3) SHZ_NOEXCEPT {
    mat3->col[0] = shz_vec3_cross(shz_mat4x4_row(mat4, 1).xyz, shz_mat4x4_row(mat4, 2).xyz);
    mat3->col[1] = shz_vec3_cross(shz_mat4x4_row(mat4, 2).xyz, shz_mat4x4_row(mat4, 0).xyz);
    mat3->col[2] = shz_vec3_cross(shz_mat4x4_row(mat4, 0).xyz, shz_mat4x4_row(mat4, 1).xyz);
}

SHZ_INLINE void shz_mat4x4_3x3_inverse(const shz_mat4x4_t* mat4, shz_mat3x3_t* invmat3) SHZ_NOEXCEPT {
    const float determinant = shz_mat4x4_3x3_determinant(mat4);

    assert(determinant != 0.0f && "shz_mat4x4_3x3_inverse(): matrix is singular, noninvertible!");

    const float inv_det = shz_invf(determinant);

    shz_mat4x4_3x3_inverse_unscaled(mat4, invmat3);
    shz_mat3x3_scale(invmat3, invmat3, inv_det);
}

SHZ_INLINE void shz_mat3x3_scale(shz_mat3x3_t* dst, const shz_mat3x3_t* src, float value) SHZ_NOEXCEPT {
    dst->col[0] = shz_vec3_scale(src->col[0], value);
    dst->col[1] = shz_vec3_scale(src->col[1], value);
    dst->col[2] = shz_vec3_scale(src->col[2], value);
}

SHZ_INLINE void shz_mat3x3_transpose(const shz_mat3x3_t* mtrx,
                          shz_mat3x3_t* out) SHZ_NOEXCEPT {
    shz_xmtrx_load_transpose_3x3((const float*)mtrx);
    shz_xmtrx_store_3x3(out);
}

SHZ_INLINE void shz_mat3x3_inverse_unscaled(const shz_mat3x3_t* mtrx,
                                 shz_mat3x3_t* out) SHZ_NOEXCEPT {
    // the transpose gives easy access to the rows as columns
    alignas(32) shz_mat3x3_t transpose;
    shz_mat3x3_transpose(mtrx, &transpose);

    out->col[0] = shz_vec3_cross(transpose.col[1], transpose.col[2]),
    out->col[1] = shz_vec3_cross(transpose.col[2], transpose.col[0]),
    out->col[2] = shz_vec3_cross(transpose.col[0], transpose.col[1]);
}

SHZ_INLINE void shz_mat3x3_inverse(const shz_mat3x3_t* mtrx,
                        shz_mat3x3_t* out) SHZ_NOEXCEPT {
    const float determinant =
        shz_vec3_dot(mtrx->col[0], shz_vec3_cross(mtrx->col[1], mtrx->col[2]));
    assert(determinant != 0.0f &&
           "shz_mat3x3_inverse: matrix is singular and cannot be inverted");
    const float inv_det = shz_invf(determinant);
    shz_mat3x3_inverse_unscaled(mtrx, out);
    for (int i = 0; i < 3; i++) {
        out->col[i] = shz_vec3_scale(out->col[i], inv_det);
    }
}

//! \endcond
