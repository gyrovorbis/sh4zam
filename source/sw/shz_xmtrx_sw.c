#include "sh4zam/shz_xmtrx.h"
#include "sh4zam/shz_matrix.h"

thread_local struct shz_xmtrx_ shz_xmtrx_state_;

void shz_xmtrx_load_apply_store_4x4_sw(shz_mat4x4_t* out,
                                       const shz_mat4x4_t* matrix1,
                                       const shz_mat4x4_t* matrix2) {
    shz_xmtrx_load_4x4(matrix1);
    shz_xmtrx_apply_4x4(matrix2);
    shz_xmtrx_store_4x4(out);
}

void shz_xmtrx_load_apply_store_3x4_sw(shz_mat3x4_t* out,
                                       const shz_mat3x4_t* matrix1,
                                       const shz_mat3x4_t* matrix2) {
    shz_xmtrx_load_3x4(matrix1);
    shz_xmtrx_apply_3x4(matrix2);
    shz_xmtrx_store_3x4(out);
}

void shz_xmtrx_load_apply_store_3x3_sw(shz_mat3x3_t* out,
                                       const shz_mat3x3_t* matrix1,
                                       const shz_mat3x3_t* matrix2) {
    shz_xmtrx_load_3x3(matrix1);
    shz_xmtrx_apply_3x3(matrix2);
    shz_xmtrx_store_3x3(out);
}
