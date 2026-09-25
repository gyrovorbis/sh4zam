/*! \file
 *  \brief   Out-of-line SW implementation of XMTRX routines.
 *  \ingroup complex
 *
 *  This file contains the out-of-line generic SW routines
 *  which back the XMTRX API, including the thread-local
 *  emulated XMTRX registers, which are only fetched via
 *  the shz_xmtrx_state_() accessor for access within the
 *  inline routines for the SW back-end.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

 // Force this translation unit to use the SW back-end.
#undef  SHZ_BACKEND
#define SHZ_BACKEND     SHZ_SW

#include "sh4zam/shz_xmtrx.h"
#include "sh4zam/shz_matrix.h"

alignas(64) SHZ_TLS_DECL(shz_xmtrx__t, xmtrx_state_, { 0 })

shz_xmtrx__t* shz_xmtrx_state_(void) {
    return SHZ_TLS_REF(xmtrx_state_);
}

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

void shz_xmtrx_blend_sw(const shz_mat4x4_t* joint_matrix, float weight) {
    shz_xmtrx__t* xmtrx_state_ = shz_xmtrx_state_();

    for(int i = 0; i < 16; ++i)
        xmtrx_state_->elem[i] += joint_matrix->elem[i] * weight;
}