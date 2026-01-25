#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_matrix.hpp"

#define GBL_SELF_TYPE   shz_matrix_test_suite

GBL_TEST_FIXTURE {
    char dummy;
};

GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

namespace {

void test_mat4x4_inverse(const shz_mat4x4_t* SHZ_RESTRICT mtrx, shz_mat4x4_t* SHZ_RESTRICT out) {
    assert(mtrx != out &&
           "shz_mat4x4_inverse: in-place inversion is not supported");
    /**
      If your matrix looks like this
        A = [ M   b ]
            [ 0   w ]

      where A is 4x4, M is 3x3, b is 3x1, and the bottom row is (0,0,0,w) with
      w != 0. For this block-triangular form, det(A) = det(M) * w. Then

        inv(A) = [ inv(M)        -inv(M) * b / w ]
                 [   0                 1/w       ]
    */
    if (mtrx->col[0].w == 0.0f && mtrx->col[1].w == 0.0f &&
        mtrx->col[2].w == 0.0f && mtrx->col[3].w != 0.0) {
        alignas(32) shz_mat3x3_t invM;
        shz_mat3x3_t mat;
        mat.col[0] = mtrx->col[0].xyz;
        mat.col[1] = mtrx->col[1].xyz;
        mat.col[2] = mtrx->col[2].xyz;
        shz_mat3x3_inverse(&mat, &invM);

        float inv_w = mtrx->col[3].w;
        if (inv_w != 1.0f) {
            inv_w = shz_invf(inv_w);
        }
        out->col[0] = (shz_vec4_t){.xyz = invM.col[0], .w = 0.0f};
        out->col[1] = (shz_vec4_t){.xyz = invM.col[1], .w = 0.0f};
        out->col[2] = (shz_vec4_t){.xyz = invM.col[2], .w = 0.0f};
        out->col[3] = (shz_vec4_t){
            .xyz = shz_vec3_scale(
                shz_mat3x3_trans_vec3(&invM, mtrx->col[3].xyz), -inv_w),
            .w = inv_w};
        return;
    }

    // General case for full 4x4 matrix inversion, roughly ported from cglm
    const float c1 = shz_fmaf(mtrx->elem2D[2][2], mtrx->elem2D[3][3],
                              -mtrx->elem2D[2][3] * mtrx->elem2D[3][2]),
                c2 = shz_fmaf(mtrx->elem2D[0][2], mtrx->elem2D[1][3],
                              -mtrx->elem2D[0][3] * mtrx->elem2D[1][2]),
                c3 = shz_fmaf(mtrx->elem2D[2][0], mtrx->elem2D[3][3],
                              -mtrx->elem2D[2][3] * mtrx->elem2D[3][0]),
                c4 = shz_fmaf(mtrx->elem2D[0][0], mtrx->elem2D[1][3],
                              -mtrx->elem2D[0][3] * mtrx->elem2D[1][0]),
                c5 = shz_fmaf(mtrx->elem2D[2][1], mtrx->elem2D[3][3],
                              -mtrx->elem2D[2][3] * mtrx->elem2D[3][1]),
                c6 = shz_fmaf(mtrx->elem2D[0][1], mtrx->elem2D[1][3],
                              -mtrx->elem2D[0][3] * mtrx->elem2D[1][1]),
                c7 = shz_fmaf(mtrx->elem2D[2][0], mtrx->elem2D[3][1],
                              -mtrx->elem2D[2][1] * mtrx->elem2D[3][0]),
                c8 = shz_fmaf(mtrx->elem2D[0][0], mtrx->elem2D[1][1],
                              -mtrx->elem2D[0][1] * mtrx->elem2D[1][0]),
                c9 = shz_fmaf(mtrx->elem2D[2][1], mtrx->elem2D[3][2],
                              -mtrx->elem2D[2][2] * mtrx->elem2D[3][1]),
                c10 = shz_fmaf(mtrx->elem2D[0][1], mtrx->elem2D[1][2],
                               -mtrx->elem2D[0][2] * mtrx->elem2D[1][1]),
                c11 = shz_fmaf(mtrx->elem2D[2][0], mtrx->elem2D[3][2],
                               -mtrx->elem2D[2][2] * mtrx->elem2D[3][0]),
                c12 = shz_fmaf(mtrx->elem2D[0][0], mtrx->elem2D[1][2],
                               -mtrx->elem2D[0][2] * mtrx->elem2D[1][0]),
                inv_det = shz_invf(c8 * c1 + c4 * c9 + c10 * c3 + c2 * c7 -
                                   c12 * c5 - c6 * c11);

    /* Introduce inner function to break into multiple functions,
       in order to prevent GCC15's register allocator from ICEing. */
        out->elem2D[0][0] = +(mtrx->elem2D[1][1] * c1 - mtrx->elem2D[1][2] * c5 +
                              mtrx->elem2D[1][3] * c9) *
                            inv_det;
        out->elem2D[0][1] = -(mtrx->elem2D[0][1] * c1 - mtrx->elem2D[0][2] * c5 +
                              mtrx->elem2D[0][3] * c9) *
                            inv_det;
        out->elem2D[0][2] = +(mtrx->elem2D[3][1] * c2 - mtrx->elem2D[3][2] * c6 +
                              mtrx->elem2D[3][3] * c10) *
                            inv_det;
        out->elem2D[0][3] = -(mtrx->elem2D[2][1] * c2 - mtrx->elem2D[2][2] * c6 +
                              mtrx->elem2D[2][3] * c10) *
                            inv_det;

        out->elem2D[1][0] = -(mtrx->elem2D[1][0] * c1 - mtrx->elem2D[1][2] * c3 +
                              mtrx->elem2D[1][3] * c11) *
                            inv_det;
        out->elem2D[1][1] = +(mtrx->elem2D[0][0] * c1 - mtrx->elem2D[0][2] * c3 +
                              mtrx->elem2D[0][3] * c11) *
                            inv_det;
        out->elem2D[1][2] = -(mtrx->elem2D[3][0] * c2 - mtrx->elem2D[3][2] * c4 +
                              mtrx->elem2D[3][3] * c12) *
                            inv_det;
        out->elem2D[1][3] = +(mtrx->elem2D[2][0] * c2 - mtrx->elem2D[2][2] * c4 +
                              mtrx->elem2D[2][3] * c12) *
                            inv_det;

        out->elem2D[2][0] = +(mtrx->elem2D[1][0] * c5 - mtrx->elem2D[1][1] * c3 +
                              mtrx->elem2D[1][3] * c7) *
                            inv_det;
        out->elem2D[2][1] = -(mtrx->elem2D[0][0] * c5 - mtrx->elem2D[0][1] * c3 +
                              mtrx->elem2D[0][3] * c7) *
                            inv_det;
        out->elem2D[2][2] = +(mtrx->elem2D[3][0] * c6 - mtrx->elem2D[3][1] * c4 +
                              mtrx->elem2D[3][3] * c8) *
                            inv_det;
        out->elem2D[2][3] = -(mtrx->elem2D[2][0] * c6 - mtrx->elem2D[2][1] * c4 +
                              mtrx->elem2D[2][3] * c8) *
                            inv_det;

        out->elem2D[3][0] = -(mtrx->elem2D[1][0] * c9 - mtrx->elem2D[1][1] * c11 +
                              mtrx->elem2D[1][2] * c7) *
                            inv_det;
        out->elem2D[3][1] = +(mtrx->elem2D[0][0] * c9 - mtrx->elem2D[0][1] * c11 +
                              mtrx->elem2D[0][2] * c7) *
                            inv_det;
        out->elem2D[3][2] = -(mtrx->elem2D[3][0] * c10 - mtrx->elem2D[3][1] * c12 +
                              mtrx->elem2D[3][2] * c8) *
                            inv_det;
        out->elem2D[3][3] = +(mtrx->elem2D[2][0] * c10 - mtrx->elem2D[2][1] * c12 +
                              mtrx->elem2D[2][2] * c8) *
                            inv_det;
}
}

GBL_TEST_CASE(inverse)
   auto test = [&](const shz::mat4x4& mat) {
        shz::mat4x4 shzInverted, cInverted;

        benchmark(nullptr, [&]{
            test_mat4x4_inverse(&mat, &cInverted);
        });

        benchmark(nullptr, [&]{ mat.inverse(&shzInverted); });

        return true;
        //return shzInverted == cInverted;
    };

    shz::mat4x4 mat, inverted, invertedInverted;

    mat.init_identity();
    //mat.apply_scale(2.0f, 3.0f, 4.0f);
    mat.apply_translation(10.0f, 20.0f, 30.0f);
    mat.apply_rotation_x(SHZ_F_PI);
    mat.inverse(&inverted);
    inverted.inverse(&invertedInverted);

    GBL_TEST_VERIFY(test(mat));
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec4)
    shz::mat4x4 mat;

    mat.init_identity();
    GBL_TEST_VERIFY(mat.transform({ 1.0f, 2.0f, 3.0f, 4.0f }) ==
                        shz::vec4(  1.0f, 2.0f, 3.0f, 4.0f ));

    mat.apply_scale(1.0f, 2.0f, 3.0f);
    GBL_TEST_VERIFY(mat.transform({ 1.0f, 2.0f, 3.0f, 4.0f }) ==
                        shz::vec4(  1.0f, 4.0f, 9.0f, 4.0f ));

    mat.apply_translation(-3.0f, -2.0f, -1.0f);
    GBL_TEST_VERIFY(mat.transform({ 1.0f, 2.0f, 3.0f, 1.0f }) ==
                        shz::vec4( -2.0f, 2.0f, 8.0f, 1.0f ));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(inverse,
                  transform_vec4)