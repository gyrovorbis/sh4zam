/*! \file
 *  \brief   Back-end-agnostic out-of-line in-memory matrix routines.
 *  \ingroup matrix
 *
 *  This file contains the implementations of the larger matrix routines,
 *  which are compiled once per back-end rather than inlined. It is never
 *  compiled directly: each back-end's source file defines SHZ_BACKEND and
 *  SHZ_IMPL_SUFFIX, then includes it, producing uniquely suffixed symbols
 *  which the inline dispatchers in shz_matrix.inl.h choose between.
 *
 *  \author    2025 Daniel Fairchild
 *  \author    2026 Falco Girgis
 *  \copyright MIT License
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#include "sh4zam/shz_matrix.h"

#define SHZ_IMPL_CAT__(a, b)    a##b
#define SHZ_IMPL_CAT_(a, b)     SHZ_IMPL_CAT__(a, b)
#define SHZ_IMPL_FN(name)       SHZ_IMPL_CAT_(name, SHZ_IMPL_SUFFIX)

SHZ_HOT
void SHZ_IMPL_FN(shz_mat4x4_inverse)(const shz_mat4x4_t* SHZ_RESTRICT mtrx, shz_mat4x4_t* SHZ_RESTRICT out) SHZ_NOEXCEPT {
    assert(mtrx != out && "shz_mat4x4_inverse: in-place inversion is not supported");
    //SHZ_PREFETCH(mtrx);

    // Check for fast-path.
    if(mtrx->col[0].w == 0.0f &&
       mtrx->col[1].w == 0.0f &&
       mtrx->col[2].w == 0.0f &&
       mtrx->col[3].w != 0.0f)
    {
        shz_mat4x4_inverse_block_triangular(mtrx, out);
        return;
    }

        // General case for full 4x4 matrix inversion, roughly ported from cglm
    const float c[12] = {
        shz_fmaf(mtrx->elem2D[2][2],  mtrx->elem2D[3][3],
                       -mtrx->elem2D[2][3] * mtrx->elem2D[3][2]),
        shz_fmaf(mtrx->elem2D[0][2],  mtrx->elem2D[1][3],
                       -mtrx->elem2D[0][3] * mtrx->elem2D[1][2]),
        shz_fmaf(mtrx->elem2D[2][0],  mtrx->elem2D[3][3],
                       -mtrx->elem2D[2][3] * mtrx->elem2D[3][0]),
        shz_fmaf(mtrx->elem2D[0][0],  mtrx->elem2D[1][3],
                       -mtrx->elem2D[0][3] * mtrx->elem2D[1][0]),
        shz_fmaf(mtrx->elem2D[2][1],  mtrx->elem2D[3][3],
                       -mtrx->elem2D[2][3] * mtrx->elem2D[3][1]),
        shz_fmaf(mtrx->elem2D[0][1],  mtrx->elem2D[1][3],
                       -mtrx->elem2D[0][3] * mtrx->elem2D[1][1]),
        shz_fmaf(mtrx->elem2D[2][0],  mtrx->elem2D[3][1],
                       -mtrx->elem2D[2][1] * mtrx->elem2D[3][0]),
        shz_fmaf(mtrx->elem2D[0][0],  mtrx->elem2D[1][1],
                       -mtrx->elem2D[0][1] * mtrx->elem2D[1][0]),
        shz_fmaf(mtrx->elem2D[2][1],  mtrx->elem2D[3][2],
                       -mtrx->elem2D[2][2] * mtrx->elem2D[3][1]),
        shz_fmaf(mtrx->elem2D[0][1],  mtrx->elem2D[1][2],
                       -mtrx->elem2D[0][2] * mtrx->elem2D[1][1]),
        shz_fmaf(mtrx->elem2D[2][0],  mtrx->elem2D[3][2],
                       -mtrx->elem2D[2][2] * mtrx->elem2D[3][0]),
        shz_fmaf(mtrx->elem2D[0][0],  mtrx->elem2D[1][2],
                       -mtrx->elem2D[0][2] * mtrx->elem2D[1][0])
    };

    const float inv_det = shz_invf(shz_dot6f(c[7],  c[ 3],  c[9], c[0], c[8], c[ 2]) +
                                   shz_dot6f(c[1], -c[11], -c[5], c[6], c[4], c[10]));

    shz_vec2_t c1c5c9 =
        shz_vec3_dot2(shz_vec3_init(c[0], c[4], c[8]),
                      shz_vec3_init(mtrx->elem2D[1][1], mtrx->elem2D[1][2], mtrx->elem2D[1][3]),
                      shz_vec3_init(mtrx->elem2D[0][1], mtrx->elem2D[0][2], mtrx->elem2D[0][3]));

    out->elem2D[0][0] = +c1c5c9.x * inv_det;
    out->elem2D[0][1] = -c1c5c9.y * inv_det;

    shz_vec2_t c2c6c10 =
        shz_vec3_dot2(shz_vec3_init(c[1], c[5], c[9]),
                      shz_vec3_init(mtrx->elem2D[3][1], mtrx->elem2D[3][2], mtrx->elem2D[3][3]),
                      shz_vec3_init(mtrx->elem2D[2][1], mtrx->elem2D[2][2], mtrx->elem2D[2][3]));

    out->elem2D[0][2] = +c2c6c10.x * inv_det;
    out->elem2D[0][3] = -c2c6c10.y * inv_det;

    shz_vec2_t c1c3c11 =
        shz_vec3_dot2(shz_vec3_init(c[0], c[2], c[10]),
                      shz_vec3_init(mtrx->elem2D[1][0], mtrx->elem2D[1][2], mtrx->elem2D[1][3]),
                      shz_vec3_init(mtrx->elem2D[0][0], mtrx->elem2D[0][2], mtrx->elem2D[0][3]));

    out->elem2D[1][0] = -c1c3c11.x * inv_det;
    out->elem2D[1][1] = +c1c3c11.y * inv_det;

    shz_vec2_t c2c4c12 =
        shz_vec3_dot2(shz_vec3_init(c[1], c[3], c[11]),
                      shz_vec3_init(mtrx->elem2D[3][0], mtrx->elem2D[3][2], mtrx->elem2D[3][3]),
                      shz_vec3_init(mtrx->elem2D[2][0], mtrx->elem2D[2][2], mtrx->elem2D[2][3]));

    out->elem2D[1][2] = -c2c4c12.x * inv_det;
    out->elem2D[1][3] = +c2c4c12.y * inv_det;

    shz_vec2_t c5c3c7 =
        shz_vec3_dot2(shz_vec3_init(c[4], c[2], c[6]),
                      shz_vec3_init(mtrx->elem2D[1][0], mtrx->elem2D[1][1], mtrx->elem2D[1][3]),
                      shz_vec3_init(mtrx->elem2D[0][0], mtrx->elem2D[0][1], mtrx->elem2D[0][3]));

    out->elem2D[2][0] = +c5c3c7.x * inv_det;
    out->elem2D[2][1] = -c5c3c7.y * inv_det;

    shz_vec2_t c6c4c8 =
        shz_vec3_dot2(shz_vec3_init(c[5], c[3], c[7]),
                      shz_vec3_init(mtrx->elem2D[3][0], mtrx->elem2D[3][1], mtrx->elem2D[3][3]),
                      shz_vec3_init(mtrx->elem2D[2][0], mtrx->elem2D[2][1], mtrx->elem2D[2][3]));

    out->elem2D[2][2] = +c6c4c8.x * inv_det;
    out->elem2D[2][3] = -c6c4c8.y * inv_det;

    shz_vec2_t c9c11c7 =
        shz_vec3_dot2(shz_vec3_init(c[8], c[10], c[6]),
                      shz_vec3_init(mtrx->elem2D[1][0], mtrx->elem2D[1][1], mtrx->elem2D[1][2]),
                      shz_vec3_init(mtrx->elem2D[0][0], mtrx->elem2D[0][1], mtrx->elem2D[0][2]));

    out->elem2D[3][0] = -c9c11c7.x * inv_det;
    out->elem2D[3][1] = +c9c11c7.y * inv_det;

    shz_vec2_t c10c12c8 =
        shz_vec3_dot2(shz_vec3_init(c[9], c[11], c[7]),
                      shz_vec3_init(mtrx->elem2D[3][0], mtrx->elem2D[3][1], mtrx->elem2D[3][2]),
                      shz_vec3_init(mtrx->elem2D[2][0], mtrx->elem2D[2][1], mtrx->elem2D[2][2]));

    out->elem2D[3][2] = -c10c12c8.x * inv_det;
    out->elem2D[3][3] = +c10c12c8.y * inv_det;
}

void SHZ_IMPL_FN(shz_mat4x4_decompose)(const shz_mat4x4_t* mat,
                                       shz_vec3_t* translation,
                                       shz_quat_t* rotation,
                                       shz_vec3_t* scale) SHZ_NOEXCEPT {
    if(translation)
        *translation = shz_mat4x4_get_translation(mat);

    assert(rotation || scale);

    shz_vec3_t s = shz_mat4x4_get_scale(mat);

    /* DON'T handle negative determinant (reflection). */
    assert(shz_mat4x4_3x3_determinant(mat) >= 0.0f);

    if(scale)
        *scale = s;

    if(rotation) {
        const float inv_sx = shz_invf_fsrra(s.x);
        const float inv_sy = shz_invf_fsrra(s.y);
        const float inv_sz = shz_invf_fsrra(s.z);

        shz_mat4x4_t norm;

        norm.col[0] = shz_vec3_vec4(shz_vec3_scale(mat->left.xyz,    inv_sx), 0.0f);
        norm.col[1] = shz_vec3_vec4(shz_vec3_scale(mat->up.xyz,      inv_sy), 0.0f);
        norm.col[2] = shz_vec3_vec4(shz_vec3_scale(mat->forward.xyz, inv_sz), 0.0f);
        norm.col[3] = shz_vec4_init(0.0f, 0.0f, 0.0f, 0.0f);

        *rotation = shz_mat4x4_to_quat(&norm);
    }
}
