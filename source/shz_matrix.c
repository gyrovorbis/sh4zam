/*! \file
 *  \brief   Out-of-line in-memory matrix routines.
 *  \ingroup matrix
 *
 *  This file contains the implementations for any routines within
 *  the matrix API which have been declared out-of-line.
 *
 *  \author    2025 Daniel Fairchild
 *  \author    2026 Falco Girgis
 *  \copyright MIT License
 */

#include "sh4zam/shz_matrix.h"
#include "sh4zam/shz_mem.h"

void shz_mat4x4_inverse_block_triangular(const shz_mat4x4_t* mtrx, shz_mat4x4_t* out) {
    alignas(32) shz_mat3x3_t invM;
    shz_vec3_t out3xyz;
    float inv_w;

    //shz_dcache_alloc_line(&invM);
    //SHZ_MEMORY_BARRIER_SOFT();

    shz_mat4x4_3x3_inverse(mtrx, &invM);
    inv_w = shz_invf(mtrx->col[3].w);

    out->col[0] = shz_vec3_vec4(invM.col[0], 0.0f);
    out->col[1] = shz_vec3_vec4(invM.col[1], 0.0f);
    out->col[2] = shz_vec3_vec4(invM.col[2], 0.0f);

    out3xyz = shz_mat3x3_trans_vec3(&invM, mtrx->col[3].xyz);
    out3xyz = shz_vec3_scale(out3xyz, -inv_w);

    out->col[3] = shz_vec3_vec4(out3xyz, inv_w);
}

SHZ_HOT
void shz_mat4x4_inverse(const shz_mat4x4_t* SHZ_RESTRICT mtrx, shz_mat4x4_t* SHZ_RESTRICT out) {
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
        [ 0] = shz_fmaf(mtrx->elem2D[2][2],  mtrx->elem2D[3][3],
                       -mtrx->elem2D[2][3] * mtrx->elem2D[3][2]),
        [ 1] = shz_fmaf(mtrx->elem2D[0][2],  mtrx->elem2D[1][3],
                       -mtrx->elem2D[0][3] * mtrx->elem2D[1][2]),
        [ 2] = shz_fmaf(mtrx->elem2D[2][0],  mtrx->elem2D[3][3],
                       -mtrx->elem2D[2][3] * mtrx->elem2D[3][0]),
        [ 3] = shz_fmaf(mtrx->elem2D[0][0],  mtrx->elem2D[1][3],
                       -mtrx->elem2D[0][3] * mtrx->elem2D[1][0]),
        [ 4] = shz_fmaf(mtrx->elem2D[2][1],  mtrx->elem2D[3][3],
                       -mtrx->elem2D[2][3] * mtrx->elem2D[3][1]),
        [ 5] = shz_fmaf(mtrx->elem2D[0][1],  mtrx->elem2D[1][3],
                       -mtrx->elem2D[0][3] * mtrx->elem2D[1][1]),
        [ 6] = shz_fmaf(mtrx->elem2D[2][0],  mtrx->elem2D[3][1],
                       -mtrx->elem2D[2][1] * mtrx->elem2D[3][0]),
        [ 7] = shz_fmaf(mtrx->elem2D[0][0],  mtrx->elem2D[1][1],
                       -mtrx->elem2D[0][1] * mtrx->elem2D[1][0]),
        [ 8] = shz_fmaf(mtrx->elem2D[2][1],  mtrx->elem2D[3][2],
                       -mtrx->elem2D[2][2] * mtrx->elem2D[3][1]),
        [ 9] = shz_fmaf(mtrx->elem2D[0][1],  mtrx->elem2D[1][2],
                       -mtrx->elem2D[0][2] * mtrx->elem2D[1][1]),
        [10] = shz_fmaf(mtrx->elem2D[2][0],  mtrx->elem2D[3][2],
                       -mtrx->elem2D[2][2] * mtrx->elem2D[3][0]),
        [11] = shz_fmaf(mtrx->elem2D[0][0],  mtrx->elem2D[1][2],
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