/*! \file
    \brief Non-inlined XMTRX API implementations.
    \ingroup quat

    This file contains the non-inlined functions implementing the XMTRX C API.

    \author 2025, 2026 Falco Girgis

    \copyright MIT License
*/

#include "sh4zam/shz_xmtrx.h"
#include "sh4zam/shz_matrix.h"

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

void shz_xmtrx_init_rotation_quat(shz_quat_t q) {
    shz_mat4x4_t mat;
    shz_mat4x4_init_rotation_quat(&mat, q);
    shz_xmtrx_load_4x4(&mat);
}

void shz_xmtrx_invert(void) {
    shz_mat4x4_t mat, inv;
    shz_xmtrx_store_4x4(&mat);
    shz_mat4x4_inverse(&mat, &inv);
    shz_xmtrx_load_4x4(&inv);
}