//! \cond INTERNAL
/*! \file
    \brief Software implementation of the XMTRX API.
    \ingroup xmtrx

    This file contains the generic implementation routines for
    the XMTRX (active matrix) API, which are platform-independent,
    shared, and may be run anywhere. They're offered as part of the
    SW back-end.

    The SH4's XMTRX lives in 16 hardware FP back-bank registers.
    This SW backend simulates it with a file-scoped static matrix.

    \note
    Because shz_xmtrx.h is included before shz_matrix.h, the matrix
    struct types (shz_mat4x4_t, etc.) are only forward-declared at
    this point. All access to matrix pointer parameters must go
    through casts to float, shz_vec4_t, shz_vec3_t, or shz_vec2_t pointers.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_XMTRX_SW_INL_H
#define SHZ_XMTRX_SW_INL_H

#include <math.h>
#include <string.h>
#include <assert.h>

/* Internal state: 4 column vectors = 16 floats, matching shz_mat4x4_t layout */
static struct {
    union {
        float      elem[16];
        shz_vec4_t col[4];
    };
} shz_xmtrx_state_;

/* Cast helpers for opaque matrix pointer params */
#define SHZ_XMTRX_COLS_(ptr)  ((const shz_vec4_t*)(ptr))
#define SHZ_XMTRX_ELEMS_(ptr) ((const float*)(ptr))
#define SHZ_XMTRX_V3S_(ptr)   ((const shz_vec3_t*)(ptr))
#define SHZ_XMTRX_V2S_(ptr)   ((const shz_vec2_t*)(ptr))

/* Mutable cast helpers for output params */
#define SHZ_XMTRX_MCOLS_(ptr)  ((shz_vec4_t*)(ptr))
#define SHZ_XMTRX_MELEMS_(ptr) ((float*)(ptr))
#define SHZ_XMTRX_MV3S_(ptr)   ((shz_vec3_t*)(ptr))
#define SHZ_XMTRX_MV2S_(ptr)   ((shz_vec2_t*)(ptr))

void shz_xmtrx_load_apply_store_4x4_sw(shz_mat4x4_t* out, const shz_mat4x4_t* mat1, const shz_mat4x4_t* mat2);
void shz_xmtrx_load_apply_store_3x4_sw(shz_mat3x4_t* out, const shz_mat3x4_t* mat1, const shz_mat3x4_t* mat2);
void shz_xmtrx_load_apply_store_3x3_sw(shz_mat3x3_t* out, const shz_mat3x3_t* mat1, const shz_mat3x3_t* mat2);

/* ========== Internal Helpers ========== */

/* result = state * vec (simulates ftrv instruction) */
static inline shz_vec4_t shz_xmtrx_ftrv_(shz_vec4_t v) {
    const shz_vec4_t* SHZ_RESTRICT c = shz_xmtrx_state_.col;
    return shz_vec4_init(
        c[0].x * v.x + c[1].x * v.y + c[2].x * v.z + c[3].x * v.w,
        c[0].y * v.x + c[1].y * v.y + c[2].y * v.z + c[3].y * v.w,
        c[0].z * v.x + c[1].z * v.y + c[2].z * v.z + c[3].z * v.w,
        c[0].w * v.x + c[1].w * v.y + c[2].w * v.z + c[3].w * v.w
    );
}

/* state = state * B (forward multiply: transform each column of B by state) */
static inline void shz_xmtrx_mul4x4_cols_(const shz_vec4_t* b) {
    shz_vec4_t r0 = shz_xmtrx_ftrv_(b[0]);
    shz_vec4_t r1 = shz_xmtrx_ftrv_(b[1]);
    shz_vec4_t r2 = shz_xmtrx_ftrv_(b[2]);
    shz_vec4_t r3 = shz_xmtrx_ftrv_(b[3]);
    shz_xmtrx_state_.col[0] = r0;
    shz_xmtrx_state_.col[1] = r1;
    shz_xmtrx_state_.col[2] = r2;
    shz_xmtrx_state_.col[3] = r3;
}

/* state = A * state (reverse multiply: load A, transform each column of old state) */
static inline void shz_xmtrx_rmul4x4_cols_(const shz_vec4_t* a) {
    shz_vec4_t old[4];
    old[0] = shz_xmtrx_state_.col[0];
    old[1] = shz_xmtrx_state_.col[1];
    old[2] = shz_xmtrx_state_.col[2];
    old[3] = shz_xmtrx_state_.col[3];

    shz_xmtrx_state_.col[0] = a[0];
    shz_xmtrx_state_.col[1] = a[1];
    shz_xmtrx_state_.col[2] = a[2];
    shz_xmtrx_state_.col[3] = a[3];

    shz_vec4_t r0 = shz_xmtrx_ftrv_(old[0]);
    shz_vec4_t r1 = shz_xmtrx_ftrv_(old[1]);
    shz_vec4_t r2 = shz_xmtrx_ftrv_(old[2]);
    shz_vec4_t r3 = shz_xmtrx_ftrv_(old[3]);
    shz_xmtrx_state_.col[0] = r0;
    shz_xmtrx_state_.col[1] = r1;
    shz_xmtrx_state_.col[2] = r2;
    shz_xmtrx_state_.col[3] = r3;
}

/* ========== Accessors ========== */

SHZ_FORCE_INLINE float shz_xmtrx_read_sw(shz_xmtrx_reg_t xf) SHZ_NOEXCEPT {
    return shz_xmtrx_state_.elem[xf];
}

SHZ_FORCE_INLINE void shz_xmtrx_write_sw(shz_xmtrx_reg_t xf, float value) SHZ_NOEXCEPT {
    shz_xmtrx_state_.elem[xf] = value;
}

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_read_row_sw(unsigned int index) SHZ_NOEXCEPT {
    assert(index < 4);
    return shz_vec4_init(
        shz_xmtrx_state_.col[0].e[index],
        shz_xmtrx_state_.col[1].e[index],
        shz_xmtrx_state_.col[2].e[index],
        shz_xmtrx_state_.col[3].e[index]
    );
}

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_read_col_sw(unsigned int index) SHZ_NOEXCEPT {
    assert(index < 4);
    return shz_xmtrx_state_.col[index];
}

SHZ_FORCE_INLINE void shz_xmtrx_write_row_sw(unsigned int index, shz_vec4_t value) SHZ_NOEXCEPT {
    assert(index < 4);
    shz_xmtrx_state_.col[0].e[index] = value.x;
    shz_xmtrx_state_.col[1].e[index] = value.y;
    shz_xmtrx_state_.col[2].e[index] = value.z;
    shz_xmtrx_state_.col[3].e[index] = value.w;
}

SHZ_FORCE_INLINE void shz_xmtrx_write_col_sw(unsigned int index, shz_vec4_t value) SHZ_NOEXCEPT {
    assert(index < 4);
    shz_xmtrx_state_.col[index] = value;
}

SHZ_FORCE_INLINE void shz_xmtrx_swap_rows_sw(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT {
    shz_vec4_t v1 = shz_xmtrx_read_row_sw(index1);
    shz_vec4_t v2 = shz_xmtrx_read_row_sw(index2);
    shz_xmtrx_write_row_sw(index1, v2);
    shz_xmtrx_write_row_sw(index2, v1);
}

SHZ_FORCE_INLINE void shz_xmtrx_swap_cols_sw(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT {
    shz_vec4_t v1 = shz_xmtrx_state_.col[index1];
    shz_vec4_t v2 = shz_xmtrx_state_.col[index2];
    shz_xmtrx_state_.col[index1] = v2;
    shz_xmtrx_state_.col[index2] = v1;
}

/* ========== Loading ========== */

SHZ_FORCE_INLINE void shz_xmtrx_load_4x4_sw(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    const shz_vec4_t* cols = SHZ_XMTRX_COLS_(matrix);
    shz_xmtrx_state_.col[0] = cols[0];
    shz_xmtrx_state_.col[1] = cols[1];
    shz_xmtrx_state_.col[2] = cols[2];
    shz_xmtrx_state_.col[3] = cols[3];
}

SHZ_FORCE_INLINE void shz_xmtrx_load_wxyz_4x4_sw(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    const shz_vec4_t* cols = SHZ_XMTRX_COLS_(matrix);
    shz_xmtrx_state_.col[0] = cols[3];
    shz_xmtrx_state_.col[1] = cols[0];
    shz_xmtrx_state_.col[2] = cols[1];
    shz_xmtrx_state_.col[3] = cols[2];
}

SHZ_FORCE_INLINE void shz_xmtrx_load_aligned4_4x4_sw(const float matrix[16]) SHZ_NOEXCEPT {
    memcpy(shz_xmtrx_state_.elem, matrix, 16 * sizeof(float));
}

SHZ_FORCE_INLINE void shz_xmtrx_store_aligned4_4x4_sw(float matrix[16]) SHZ_NOEXCEPT {
    memcpy(matrix, shz_xmtrx_state_.elem, 16 * sizeof(float));
}

SHZ_FORCE_INLINE void shz_xmtrx_load_cols_4x4_sw(const shz_vec4_t* c1,
                                                   const shz_vec4_t* c2,
                                                   const shz_vec4_t* c3,
                                                   const shz_vec4_t* c4) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = *c1;
    shz_xmtrx_state_.col[1] = *c2;
    shz_xmtrx_state_.col[2] = *c3;
    shz_xmtrx_state_.col[3] = *c4;
}

SHZ_FORCE_INLINE void shz_xmtrx_load_rows_4x4_sw(const shz_vec4_t* r1,
                                                   const shz_vec4_t* r2,
                                                   const shz_vec4_t* r3,
                                                   const shz_vec4_t* r4) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(r1->x, r2->x, r3->x, r4->x);
    shz_xmtrx_state_.col[1] = shz_vec4_init(r1->y, r2->y, r3->y, r4->y);
    shz_xmtrx_state_.col[2] = shz_vec4_init(r1->z, r2->z, r3->z, r4->z);
    shz_xmtrx_state_.col[3] = shz_vec4_init(r1->w, r2->w, r3->w, r4->w);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_4x4_sw(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    const shz_vec4_t* c = SHZ_XMTRX_COLS_(matrix);
    shz_xmtrx_state_.col[0] = shz_vec4_init(c[0].x, c[1].x, c[2].x, c[3].x);
    shz_xmtrx_state_.col[1] = shz_vec4_init(c[0].y, c[1].y, c[2].y, c[3].y);
    shz_xmtrx_state_.col[2] = shz_vec4_init(c[0].z, c[1].z, c[2].z, c[3].z);
    shz_xmtrx_state_.col[3] = shz_vec4_init(c[0].w, c[1].w, c[2].w, c[3].w);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_unaligned_4x4_sw(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_transpose_4x4_sw((const shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_3x4_sw(const shz_mat3x4_t* mat) SHZ_NOEXCEPT {
    const shz_vec3_t* c = SHZ_XMTRX_V3S_(mat);
    shz_xmtrx_state_.col[0] = shz_vec4_init(c[0].x, c[0].y, c[0].z, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(c[1].x, c[1].y, c[1].z, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(c[2].x, c[2].y, c[2].z, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(c[3].x, c[3].y, c[3].z, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_cols_4x3_sw(const shz_vec4_t* c1,
                                                   const shz_vec4_t* c2,
                                                   const shz_vec4_t* c3) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = *c1;
    shz_xmtrx_state_.col[1] = *c2;
    shz_xmtrx_state_.col[2] = *c3;
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_rows_3x4_sw(const shz_vec4_t* r1,
                                                   const shz_vec4_t* r2,
                                                   const shz_vec4_t* r3) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(r1->x, r2->x, r3->x, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(r1->y, r2->y, r3->y, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(r1->z, r2->z, r3->z, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(r1->w, r2->w, r3->w, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_3x3_sw(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
    const shz_vec3_t* c = SHZ_XMTRX_V3S_(matrix);
    shz_xmtrx_state_.col[0] = shz_vec4_init(c[0].x, c[0].y, c[0].z, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(c[1].x, c[1].y, c[1].z, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(c[2].x, c[2].y, c[2].z, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_3x3_sw(const float* matrix) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(matrix[0], matrix[3], matrix[6], 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(matrix[1], matrix[4], matrix[7], 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(matrix[2], matrix[5], matrix[8], 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_2x2_sw(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
    const shz_vec2_t* c = SHZ_XMTRX_V2S_(matrix);
    shz_xmtrx_state_.col[0] = shz_vec4_init(c[0].x, c[0].y, 0.0f, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(c[1].x, c[1].y, 0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

/* ========== Storing ========== */

SHZ_FORCE_INLINE void shz_xmtrx_store_4x4_sw(shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    shz_vec4_t* cols = SHZ_XMTRX_MCOLS_(matrix);
    cols[0] = shz_xmtrx_state_.col[0];
    cols[1] = shz_xmtrx_state_.col[1];
    cols[2] = shz_xmtrx_state_.col[2];
    cols[3] = shz_xmtrx_state_.col[3];
}

SHZ_FORCE_INLINE void shz_xmtrx_store_unaligned_4x4_sw(float matrix[16]) SHZ_NOEXCEPT {
    memcpy(matrix, shz_xmtrx_state_.elem, 16 * sizeof(float));
}

SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_4x4_sw(shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    shz_vec4_t* d = SHZ_XMTRX_MCOLS_(matrix);
    const shz_vec4_t* s = shz_xmtrx_state_.col;
    d[0] = shz_vec4_init(s[0].x, s[1].x, s[2].x, s[3].x);
    d[1] = shz_vec4_init(s[0].y, s[1].y, s[2].y, s[3].y);
    d[2] = shz_vec4_init(s[0].z, s[1].z, s[2].z, s[3].z);
    d[3] = shz_vec4_init(s[0].w, s[1].w, s[2].w, s[3].w);
}

SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_unaligned_4x4_sw(float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_store_transpose_4x4_sw((shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_store_3x4_sw(shz_mat3x4_t* mat) SHZ_NOEXCEPT {
    shz_vec3_t* d = SHZ_XMTRX_MV3S_(mat);
    d[0] = shz_xmtrx_state_.col[0].xyz;
    d[1] = shz_xmtrx_state_.col[1].xyz;
    d[2] = shz_xmtrx_state_.col[2].xyz;
    d[3] = shz_xmtrx_state_.col[3].xyz;
}

SHZ_FORCE_INLINE void shz_xmtrx_store_3x3_sw(shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
    shz_vec3_t* d = SHZ_XMTRX_MV3S_(matrix);
    d[0] = shz_xmtrx_state_.col[0].xyz;
    d[1] = shz_xmtrx_state_.col[1].xyz;
    d[2] = shz_xmtrx_state_.col[2].xyz;
}

SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_3x3_sw(shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
    shz_vec3_t* d = SHZ_XMTRX_MV3S_(matrix);
    const shz_vec4_t* s = shz_xmtrx_state_.col;
    d[0] = shz_vec3_init(s[0].x, s[1].x, s[2].x);
    d[1] = shz_vec3_init(s[0].y, s[1].y, s[2].y);
    d[2] = shz_vec3_init(s[0].z, s[1].z, s[2].z);
}

SHZ_FORCE_INLINE void shz_xmtrx_store_2x2_sw(shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
    shz_vec2_t* d = SHZ_XMTRX_MV2S_(matrix);
    d[0] = shz_xmtrx_state_.col[0].xy;
    d[1] = shz_xmtrx_state_.col[1].xy;
}

/* ========== Initialization ========== */

SHZ_FORCE_INLINE void shz_xmtrx_init_identity_sw(void) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_identity_safe_sw(void) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity_sw();
}

SHZ_FORCE_INLINE void shz_xmtrx_init_zero_sw(void) SHZ_NOEXCEPT {
    memset(shz_xmtrx_state_.elem, 0, sizeof(shz_xmtrx_state_));
}

SHZ_FORCE_INLINE void shz_xmtrx_init_one_sw(void) SHZ_NOEXCEPT {
    for(int i = 0; i < 16; ++i)
        shz_xmtrx_state_.elem[i] = 1.0f;
}

SHZ_FORCE_INLINE void shz_xmtrx_init_fill_sw(float value) SHZ_NOEXCEPT {
    for(int i = 0; i < 16; ++i)
        shz_xmtrx_state_.elem[i] = value;
}

SHZ_FORCE_INLINE void shz_xmtrx_init_diagonal_sw(float x, float y, float z, float w) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(x,    0.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(0.0f, y,    0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 0.0f, z,    0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, w);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_upper_triangular_sw(float col1, shz_vec2_t col2, shz_vec3_t col3, shz_vec4_t col4) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(col1,  0.0f,  0.0f,  0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(col2.x, col2.y, 0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(col3.x, col3.y, col3.z, 0.0f);
    shz_xmtrx_state_.col[3] = col4;
}

SHZ_FORCE_INLINE void shz_xmtrx_init_lower_triangular_sw(shz_vec4_t col1, shz_vec3_t col2, shz_vec2_t col3, float col4) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = col1;
    shz_xmtrx_state_.col[1] = shz_vec4_init(0.0f, col2.x, col2.y, col2.z);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 0.0f, col3.x, col3.y);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, col4);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_scale_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_diagonal_sw(x, y, z, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_translation_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(x,    y,    z,    1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_x_sw(float x) SHZ_NOEXCEPT {
    const float s = shz_sinf(x);
    const float c = shz_cosf(x);
    shz_xmtrx_state_.col[0] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(0.0f, c,    s,    0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, -s,   c,    0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_y_sw(float y) SHZ_NOEXCEPT {
    const float s = shz_sinf(y);
    const float c = shz_cosf(y);
    shz_xmtrx_state_.col[0] = shz_vec4_init(c,    0.0f, -s,   0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(s,    0.0f, c,    0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_z_sw(float z) SHZ_NOEXCEPT {
    const float s = shz_sinf(z);
    const float c = shz_cosf(z);
    shz_xmtrx_state_.col[0] = shz_vec4_init(c,    s,    0.0f, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(-s,   c,    0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_rotation_sw(float angle, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT {
    float len2 = xAxis * xAxis + yAxis * yAxis + zAxis * zAxis;
    float inv_len = shz_invf_fsrra(len2);
    xAxis *= inv_len;
    yAxis *= inv_len;
    zAxis *= inv_len;

    const float s = shz_sinf(angle);
    const float c = shz_cosf(angle);
    const float t = 1.0f - c;

    const float xs = xAxis * s, ys = yAxis * s, zs = zAxis * s;
    const float xyt = xAxis * yAxis * t;
    const float xzt = xAxis * zAxis * t;
    const float yzt = yAxis * zAxis * t;

    shz_xmtrx_state_.col[0] = shz_vec4_init(xAxis * xAxis * t + c,  xyt + zs,  xzt - ys,  0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(xyt - zs,  yAxis * yAxis * t + c,  yzt + xs,  0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(xzt + ys,  yzt - xs,  zAxis * zAxis * t + c,  0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_symmetric_skew_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(0.0f, -z,    y,   0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(z,    0.0f, -x,   0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(-y,   x,    0.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_outer_product_sw(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(a.x * b.x, a.y * b.x, a.z * b.x, a.w * b.x);
    shz_xmtrx_state_.col[1] = shz_vec4_init(a.x * b.y, a.y * b.y, a.z * b.y, a.w * b.y);
    shz_xmtrx_state_.col[2] = shz_vec4_init(a.x * b.z, a.y * b.z, a.z * b.z, a.w * b.z);
    shz_xmtrx_state_.col[3] = shz_vec4_init(a.x * b.w, a.y * b.w, a.z * b.w, a.w * b.w);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_permutation_wxyz_sw(void) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_permutation_yzwx_sw(void) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
}

SHZ_FORCE_INLINE void shz_xmtrx_init_screen_sw(float width, float height) SHZ_NOEXCEPT {
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    shz_xmtrx_state_.col[0] = shz_vec4_init(hw,   0.0f, 0.0f, 0.0f);
    shz_xmtrx_state_.col[1] = shz_vec4_init(0.0f, -hh,  0.0f, 0.0f);
    shz_xmtrx_state_.col[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    shz_xmtrx_state_.col[3] = shz_vec4_init(hw,   hh,   0.0f, 1.0f);
}

/* ========== Apply Operations ========== */

SHZ_FORCE_INLINE void shz_xmtrx_apply_4x4_sw(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    shz_xmtrx_mul4x4_cols_(SHZ_XMTRX_COLS_(matrix));
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_aligned4_4x4_sw(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_apply_4x4_sw((const shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_transpose_4x4_sw(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    const shz_vec4_t* c = SHZ_XMTRX_COLS_(matrix);
    shz_vec4_t transposed[4];
    transposed[0] = shz_vec4_init(c[0].x, c[1].x, c[2].x, c[3].x);
    transposed[1] = shz_vec4_init(c[0].y, c[1].y, c[2].y, c[3].y);
    transposed[2] = shz_vec4_init(c[0].z, c[1].z, c[2].z, c[3].z);
    transposed[3] = shz_vec4_init(c[0].w, c[1].w, c[2].w, c[3].w);
    shz_xmtrx_mul4x4_cols_(transposed);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_4x4_sw(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    shz_xmtrx_rmul4x4_cols_(SHZ_XMTRX_COLS_(matrix));
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_aligned4_4x4_sw(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_apply_reverse_4x4_sw((const shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_transpose_4x4_sw(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT {
    const shz_vec4_t* c = SHZ_XMTRX_COLS_(matrix);
    shz_vec4_t transposed[4];
    transposed[0] = shz_vec4_init(c[0].x, c[1].x, c[2].x, c[3].x);
    transposed[1] = shz_vec4_init(c[0].y, c[1].y, c[2].y, c[3].y);
    transposed[2] = shz_vec4_init(c[0].z, c[1].z, c[2].z, c[3].z);
    transposed[3] = shz_vec4_init(c[0].w, c[1].w, c[2].w, c[3].w);
    shz_xmtrx_rmul4x4_cols_(transposed);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_reverse_transpose_aligned4_4x4_sw(const float matrix[16]) SHZ_NOEXCEPT {
    shz_xmtrx_apply_reverse_transpose_4x4_sw((const shz_mat4x4_t*)matrix);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_3x4_sw(const shz_mat3x4_t* mat) SHZ_NOEXCEPT {
    const shz_vec3_t* c = SHZ_XMTRX_V3S_(mat);
    shz_vec4_t m[4];
    m[0] = shz_vec4_init(c[0].x, c[0].y, c[0].z, 0.0f);
    m[1] = shz_vec4_init(c[1].x, c[1].y, c[1].z, 0.0f);
    m[2] = shz_vec4_init(c[2].x, c[2].y, c[2].z, 0.0f);
    m[3] = shz_vec4_init(c[3].x, c[3].y, c[3].z, 1.0f);
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_3x3_sw(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
    const shz_vec3_t* c = SHZ_XMTRX_V3S_(matrix);
    shz_vec4_t m[4];
    m[0] = shz_vec4_init(c[0].x, c[0].y, c[0].z, 0.0f);
    m[1] = shz_vec4_init(c[1].x, c[1].y, c[1].z, 0.0f);
    m[2] = shz_vec4_init(c[2].x, c[2].y, c[2].z, 0.0f);
    m[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_transpose_3x3_sw(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT {
    const shz_vec3_t* c = SHZ_XMTRX_V3S_(matrix);
    shz_vec4_t m[4];
    m[0] = shz_vec4_init(c[0].x, c[1].x, c[2].x, 0.0f);
    m[1] = shz_vec4_init(c[0].y, c[1].y, c[2].y, 0.0f);
    m[2] = shz_vec4_init(c[0].z, c[1].z, c[2].z, 0.0f);
    m[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_2x2_sw(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT {
    const shz_vec2_t* c = SHZ_XMTRX_V2S_(matrix);
    shz_vec4_t m[4];
    m[0] = shz_vec4_init(c[0].x, c[0].y, 0.0f, 0.0f);
    m[1] = shz_vec4_init(c[1].x, c[1].y, 0.0f, 0.0f);
    m[2] = shz_xmtrx_state_.col[2];
    m[3] = shz_xmtrx_state_.col[3];

    shz_vec4_t r0 = shz_xmtrx_ftrv_(m[0]);
    shz_vec4_t r1 = shz_xmtrx_ftrv_(m[1]);
    shz_xmtrx_state_.col[0] = r0;
    shz_xmtrx_state_.col[1] = r1;
}

SHZ_FORCE_INLINE void shz_xmtrx_set_translation_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[3].x = x;
    shz_xmtrx_state_.col[3].y = y;
    shz_xmtrx_state_.col[3].z = z;
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_translation_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[3].x += x;
    shz_xmtrx_state_.col[3].y += y;
    shz_xmtrx_state_.col[3].z += z;
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_scale_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0].x *= x;
    shz_xmtrx_state_.col[0].y *= x;
    shz_xmtrx_state_.col[0].z *= x;

    shz_xmtrx_state_.col[1].x *= y;
    shz_xmtrx_state_.col[1].y *= y;
    shz_xmtrx_state_.col[1].z *= y;

    shz_xmtrx_state_.col[2].x *= z;
    shz_xmtrx_state_.col[2].y *= z;
    shz_xmtrx_state_.col[2].z *= z;
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_x_sw(float x) SHZ_NOEXCEPT {
    const float s = shz_sinf(x);
    const float c = shz_cosf(x);
    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    rot[1] = shz_vec4_init(0.0f, c,    s,    0.0f);
    rot[2] = shz_vec4_init(0.0f, -s,   c,    0.0f);
    rot[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_y_sw(float y) SHZ_NOEXCEPT {
    const float s = shz_sinf(y);
    const float c = shz_cosf(y);
    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(c,    0.0f, -s,   0.0f);
    rot[1] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    rot[2] = shz_vec4_init(s,    0.0f, c,    0.0f);
    rot[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_z_sw(float z) SHZ_NOEXCEPT {
    const float s = shz_sinf(z);
    const float c = shz_cosf(z);
    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(c,    s,    0.0f, 0.0f);
    rot[1] = shz_vec4_init(-s,   c,    0.0f, 0.0f);
    rot[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    rot[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_sw(float angle, float x, float y, float z) SHZ_NOEXCEPT {
    float len2 = x * x + y * y + z * z;
    float inv_len = shz_invf_fsrra(len2);
    x *= inv_len;
    y *= inv_len;
    z *= inv_len;

    const float s = shz_sinf(angle);
    const float c = shz_cosf(angle);
    const float t = 1.0f - c;

    const float xs = x * s, ys = y * s, zs = z * s;
    const float xyt = x * y * t;
    const float xzt = x * z * t;
    const float yzt = y * z * t;

    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(x * x * t + c,  xyt + zs,  xzt - ys,  0.0f);
    rot[1] = shz_vec4_init(xyt - zs,  y * y * t + c,  yzt + xs,  0.0f);
    rot[2] = shz_vec4_init(xzt + ys,  yzt - xs,  z * z * t + c,  0.0f);
    rot[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_rotation_quat_sw(shz_quat_t q) SHZ_NOEXCEPT {
    const float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
    const float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
    const float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy), 0.0f);
    rot[1] = shz_vec4_init(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx), 0.0f);
    rot[2] = shz_vec4_init(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy), 0.0f);
    rot[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_symmetric_skew_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_vec4_t m[4];
    m[0] = shz_vec4_init(0.0f, z,    -y,   0.0f);
    m[1] = shz_vec4_init(-z,   0.0f, x,    0.0f);
    m[2] = shz_vec4_init(y,    -x,   0.0f, 0.0f);
    m[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_lookat_sw(shz_vec3_t eye,
                                                  shz_vec3_t center,
                                                  shz_vec3_t up) SHZ_NOEXCEPT {
    /* f = normalize(eye - center) (camera looks down -z, so f points from target to eye) */
    shz_vec3_t f = shz_vec3_init(eye.x - center.x, eye.y - center.y, eye.z - center.z);
    float flen2 = f.x * f.x + f.y * f.y + f.z * f.z;
    float finv = shz_invf_fsrra(flen2);
    f.x *= finv; f.y *= finv; f.z *= finv;

    /* s = normalize(cross(up, f)) */
    shz_vec3_t s = shz_vec3_init(
        up.y * f.z - up.z * f.y,
        up.z * f.x - up.x * f.z,
        up.x * f.y - up.y * f.x
    );
    float slen2 = s.x * s.x + s.y * s.y + s.z * s.z;
    float sinv = shz_invf_fsrra(slen2);
    s.x *= sinv; s.y *= sinv; s.z *= sinv;

    /* u = cross(f, s) */
    shz_vec3_t u = shz_vec3_init(
        f.y * s.z - f.z * s.y,
        f.z * s.x - f.x * s.z,
        f.x * s.y - f.y * s.x
    );

    float tx = -(s.x * eye.x + s.y * eye.y + s.z * eye.z);
    float ty = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
    float tz = -(f.x * eye.x + f.y * eye.y + f.z * eye.z);

    shz_vec4_t look[4];
    look[0] = shz_vec4_init(s.x, u.x, f.x, 0.0f);
    look[1] = shz_vec4_init(s.y, u.y, f.y, 0.0f);
    look[2] = shz_vec4_init(s.z, u.z, f.z, 0.0f);
    look[3] = shz_vec4_init(tx,  ty,  tz,  1.0f);
    shz_xmtrx_mul4x4_cols_(look);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_ortho_sw(float left, float right, float bottom, float top, float near, float far) SHZ_NOEXCEPT {
    shz_vec3_t box   = shz_vec3_inv(shz_vec3_init(right - left, top - bottom, far - near));
    shz_vec3_t sc    = shz_vec3_mul(shz_vec3_init(2.0f, 2.0f, -2.0f), box);
    shz_vec3_t trans = shz_vec3_mul(shz_vec3_init(-(right + left), -(top + bottom), -(far + near)), box);

    shz_vec4_t m[4];
    m[0] = shz_vec4_init(sc.x,    0.0f,    0.0f,    0.0f);
    m[1] = shz_vec4_init(0.0f,    sc.y,    0.0f,    0.0f);
    m[2] = shz_vec4_init(0.0f,    0.0f,    sc.z,    0.0f);
    m[3] = shz_vec4_init(trans.x, trans.y, trans.z, 1.0f);
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_frustum_sw(float left, float right, float bottom, float top, float near, float far) SHZ_NOEXCEPT {
    const float a = 2.0f * near / (right - left);
    const float b = 2.0f * near / (top - bottom);
    const float c = (right + left) / (right - left);
    const float d = (top + bottom) / (top - bottom);
    const float e = -(far + near) / (far - near);
    const float ff = -(2.0f * far * near) / (far - near);

    shz_vec4_t m[4];
    m[0] = shz_vec4_init(a,    0.0f, 0.0f, 0.0f);
    m[1] = shz_vec4_init(0.0f, b,    0.0f, 0.0f);
    m[2] = shz_vec4_init(c,    d,    e,    -1.0f);
    m[3] = shz_vec4_init(0.0f, 0.0f, ff,   0.0f);
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_perspective_sw(float fov, float aspect, float near_z) SHZ_NOEXCEPT {
    const float half = fov * 0.5f;
    const float c = shz_cosf(half);
    const float s = shz_sinf(half);
    const float cot = c / s;
    const float cot_a = cot / aspect;

    shz_vec4_t m[4];
    m[0] = shz_vec4_init(cot_a, 0.0f, 0.0f,  0.0f);
    m[1] = shz_vec4_init(0.0f,  cot,  0.0f,  0.0f);
    m[2] = shz_vec4_init(0.0f,  0.0f, 0.0f,  -1.0f);
    m[3] = shz_vec4_init(0.0f,  0.0f, near_z, 0.0f);
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_screen_sw(float width, float height) SHZ_NOEXCEPT {
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;

    shz_vec4_t m[4];
    m[0] = shz_vec4_init(hw,   0.0f, 0.0f, hw);
    m[1] = shz_vec4_init(0.0f, -hh,  0.0f, hh);
    m[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    m[3] = shz_vec4_init(0.0f, 0.0f,   0.0f, 1.0f);
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_permutation_wxyz_sw(void) SHZ_NOEXCEPT {
    shz_vec4_t m[4];
    m[0] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    m[1] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    m[2] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    m[3] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_permutation_yzwx_sw(void) SHZ_NOEXCEPT {
    shz_vec4_t m[4];
    m[0] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    m[1] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    m[2] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    m[3] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    shz_xmtrx_mul4x4_cols_(m);
}

SHZ_FORCE_INLINE void shz_xmtrx_apply_self_sw(void) SHZ_NOEXCEPT {
    shz_vec4_t copy[4];
    copy[0] = shz_xmtrx_state_.col[0];
    copy[1] = shz_xmtrx_state_.col[1];
    copy[2] = shz_xmtrx_state_.col[2];
    copy[3] = shz_xmtrx_state_.col[3];
    shz_xmtrx_mul4x4_cols_(copy);
}

/* ========== GL-style Transforms (T * XMTRX) ========== */

SHZ_FORCE_INLINE void shz_xmtrx_translate_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_vec4_t t[4];
    t[0] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    t[1] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    t[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    t[3] = shz_vec4_init(x,    y,    z,    1.0f);
    shz_xmtrx_rmul4x4_cols_(t);
}

SHZ_FORCE_INLINE void shz_xmtrx_scale_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_vec4_t s[4];
    s[0] = shz_vec4_init(x,    0.0f, 0.0f, 0.0f);
    s[1] = shz_vec4_init(0.0f, y,    0.0f, 0.0f);
    s[2] = shz_vec4_init(0.0f, 0.0f, z,    0.0f);
    s[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    shz_xmtrx_rmul4x4_cols_(s);
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_x_sw(float radians) SHZ_NOEXCEPT {
    const float s = shz_sinf(radians);
    const float c = shz_cosf(radians);
    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
    rot[1] = shz_vec4_init(0.0f, c,    s,    0.0f);
    rot[2] = shz_vec4_init(0.0f, -s,   c,    0.0f);
    rot[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    shz_xmtrx_rmul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_y_sw(float radians) SHZ_NOEXCEPT {
    const float s = shz_sinf(radians);
    const float c = shz_cosf(radians);
    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(c,    0.0f, -s,   0.0f);
    rot[1] = shz_vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
    rot[2] = shz_vec4_init(s,    0.0f, c,    0.0f);
    rot[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    shz_xmtrx_rmul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_z_sw(float radians) SHZ_NOEXCEPT {
    const float s = shz_sinf(radians);
    const float c = shz_cosf(radians);
    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(c,    s,    0.0f, 0.0f);
    rot[1] = shz_vec4_init(-s,   c,    0.0f, 0.0f);
    rot[2] = shz_vec4_init(0.0f, 0.0f, 1.0f, 0.0f);
    rot[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    shz_xmtrx_rmul4x4_cols_(rot);
}

SHZ_FORCE_INLINE void shz_xmtrx_rotate_sw(float angle, float x, float y, float z) SHZ_NOEXCEPT {
    float len2 = x * x + y * y + z * z;
    float inv_len = shz_invf_fsrra(len2);
    x *= inv_len;
    y *= inv_len;
    z *= inv_len;

    const float s = shz_sinf(angle);
    const float c = shz_cosf(angle);
    const float t = 1.0f - c;

    const float xs = x * s, ys = y * s, zs = z * s;
    const float xyt = x * y * t;
    const float xzt = x * z * t;
    const float yzt = y * z * t;

    shz_vec4_t rot[4];
    rot[0] = shz_vec4_init(x * x * t + c,  xyt + zs,  xzt - ys,  0.0f);
    rot[1] = shz_vec4_init(xyt - zs,  y * y * t + c,  yzt + xs,  0.0f);
    rot[2] = shz_vec4_init(xzt + ys,  yzt - xs,  z * z * t + c,  0.0f);
    rot[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 1.0f);
    shz_xmtrx_rmul4x4_cols_(rot);
}

/* ========== Compound Operations ========== */

SHZ_FORCE_INLINE void shz_xmtrx_load_apply_4x4_sw(const shz_mat4x4_t* matrix1,
                                                    const shz_mat4x4_t* matrix2) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4_sw(matrix1);
    shz_xmtrx_apply_4x4_sw(matrix2);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_apply_unaligned_4x4_sw(const float matrix1[16],
                                                              const float matrix2[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_unaligned_4x4(matrix1);
    shz_xmtrx_apply_unaligned_4x4(matrix2);
}

SHZ_FORCE_INLINE void shz_xmtrx_load_apply_store_unaligned_4x4_sw(float out[16],
                                                                    const float mat1[16],
                                                                    const float mat2[16]) SHZ_NOEXCEPT {
    shz_xmtrx_load_apply_unaligned_4x4_sw(mat1, mat2);
    shz_xmtrx_store_unaligned_4x4_sw(out);
}

/* ========== Miscellaneous ========== */

SHZ_FORCE_INLINE void shz_xmtrx_add_4x4_sw(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    const float* e = SHZ_XMTRX_ELEMS_(mat);
    for(int i = 0; i < 16; ++i)
        shz_xmtrx_state_.elem[i] += e[i];
}

SHZ_FORCE_INLINE void shz_xmtrx_sub_4x4_sw(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    const float* e = SHZ_XMTRX_ELEMS_(mat);
    for(int i = 0; i < 16; ++i)
        shz_xmtrx_state_.elem[i] -= e[i];
}

SHZ_FORCE_INLINE void shz_xmtrx_add_symmetric_skew_sw(float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0].y += z;
    shz_xmtrx_state_.col[0].z -= y;
    shz_xmtrx_state_.col[1].x -= z;
    shz_xmtrx_state_.col[1].z += x;
    shz_xmtrx_state_.col[2].x += y;
    shz_xmtrx_state_.col[2].y -= x;
}

SHZ_FORCE_INLINE void shz_xmtrx_add_diagonal_sw(float x, float y, float z, float w) SHZ_NOEXCEPT {
    shz_xmtrx_state_.col[0].x += x;
    shz_xmtrx_state_.col[1].y += y;
    shz_xmtrx_state_.col[2].z += z;
    shz_xmtrx_state_.col[3].w += w;
}

SHZ_FORCE_INLINE void shz_xmtrx_transpose_sw(void) SHZ_NOEXCEPT {
    float tmp;
    tmp = shz_xmtrx_state_.col[0].y; shz_xmtrx_state_.col[0].y = shz_xmtrx_state_.col[1].x; shz_xmtrx_state_.col[1].x = tmp;
    tmp = shz_xmtrx_state_.col[0].z; shz_xmtrx_state_.col[0].z = shz_xmtrx_state_.col[2].x; shz_xmtrx_state_.col[2].x = tmp;
    tmp = shz_xmtrx_state_.col[0].w; shz_xmtrx_state_.col[0].w = shz_xmtrx_state_.col[3].x; shz_xmtrx_state_.col[3].x = tmp;
    tmp = shz_xmtrx_state_.col[1].z; shz_xmtrx_state_.col[1].z = shz_xmtrx_state_.col[2].y; shz_xmtrx_state_.col[2].y = tmp;
    tmp = shz_xmtrx_state_.col[1].w; shz_xmtrx_state_.col[1].w = shz_xmtrx_state_.col[3].y; shz_xmtrx_state_.col[3].y = tmp;
    tmp = shz_xmtrx_state_.col[2].w; shz_xmtrx_state_.col[2].w = shz_xmtrx_state_.col[3].z; shz_xmtrx_state_.col[3].z = tmp;
}

SHZ_FORCE_INLINE void shz_xmtrx_negate_sw(void) SHZ_NOEXCEPT {
    for(int i = 0; i < 16; ++i)
        shz_xmtrx_state_.elem[i] = -shz_xmtrx_state_.elem[i];
}

SHZ_FORCE_INLINE void shz_xmtrx_abs_sw(void) SHZ_NOEXCEPT {
    for(int i = 0; i < 16; ++i)
        shz_xmtrx_state_.elem[i] = shz_fabsf(shz_xmtrx_state_.elem[i]);
}

/* ========== Transformations ========== */

SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_transform_vec4_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_xmtrx_ftrv_(vec);
}

/* Clean up cast helper macros */
#undef SHZ_XMTRX_COLS_
#undef SHZ_XMTRX_ELEMS_
#undef SHZ_XMTRX_V3S_
#undef SHZ_XMTRX_V2S_
#undef SHZ_XMTRX_MCOLS_
#undef SHZ_XMTRX_MELEMS_
#undef SHZ_XMTRX_MV3S_
#undef SHZ_XMTRX_MV2S_

//! \endcond

#endif