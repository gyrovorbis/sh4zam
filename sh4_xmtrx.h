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

void sh4_xmtrx_load_4x4(const sh4_matrix_4x4_t *matrix);
void sh4_xmtrx_load_4x4_rows(const sh4_vec4_t *r1, const sh4_vec4_t *r2, const sh4_vec4_t *r3, const sh4_vec4_t *r4);
void sh4_xmtrx_load_4x4_transpose(const sh4_matrix_4x4_t *matrix);
void sh4_xmtrx_load_4x4_apply(const sh4_matrix_4x4_t *matrix1, const sh4_matrix_4x4_t *matrix2);

void sh4_xmtrx_load_3x4(const sh4_matrix_3x4_t *matrix);
void sh4_xmtrx_load_3x3(const sh4_matrix_3x3_t *matrix);
void sh4_xmtrx_load_2x2(const sh4_matrix_2x2_t *matrix, sh4_xmtrx_quadrant_t pos);
void sh4_xmtrx_load_scalar(float value, sh4_xmtrx_regs_t reg);

void sh4_xmtrx_store_4x4(sh4_matrix_4x4_t *matrix);
void sh4_xmtrx_store_3x4(sh4_matrix_3x4_t *matrix);
void sh4_xmtrx_store_3x3(sh4_matrix_3x3_t *matrix);
void sh4_xmtrx_store_2x2(sh4_matrix_2x2_t *matrix, sh4_xmtrx_quadrant_t pos);
float sh4_xmtrx_store_scalar(sh4_xmtrx_regs_t reg);

void sh4_xmtrx_set_identity(void);
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

sh4_vec4_t sh4_xmtrx_trans_vec4(sh4_vec4_t vec4);
sh4_vec3_t sh4_xmtrx_trans_vec3(sh4_vec3_t vec3);
sh4_vec2_t sh4_xmtrx_trans_vec2(sh4_vec2_t vec2);

SH4_END_DECLS

#endif // SH4_XMTRX_H
