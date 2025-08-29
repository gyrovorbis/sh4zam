#include "sh4zam/shz_xmtrx.h"
#include "sh4zam/shz_matrix.h"

void shz_xmtrx_apply_rotation_quat(shz_quat_t q) {
	shz_mat4x4_t rot;

	shz_mat4x4_init_rotation_quat(&rot, q);
	shz_xmtrx_apply_4x4(&rot);
}

shz_quat_t shz_xmtrx_to_quat(void) {
	shz_mat4x4_t xmtrx;

	shz_xmtrx_store_4x4(&xmtrx);
	return shz_mat4x4_to_quat(&xmtrx);
}

float shz_xmtrx_determinant(void) {
    shz_mat4x4_t xmtrx;

    shz_xmtrx_store_4x4(&xmtrx);
    return shz_mat4x4_determinant(&xmtrx);
}