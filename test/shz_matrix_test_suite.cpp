#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_matrix.hpp"
#include <cglm/cglm.h>

#define GBL_SELF_TYPE   shz_matrix_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

GBL_TEST_CASE(inverse)
   auto test = [&](const shz::mat4x4& mat) {
        shz::mat4x4 shzInverted, glmInverted;

        benchmark(nullptr, [&]{
            glm_mat4_inv(*(mat4*)&mat, *(mat4*)&glmInverted);
        });

        benchmark(nullptr, [&]{ mat.inverse(&shzInverted); });

        return shzInverted == glmInverted;
    };

    shz::mat4x4 mat, inverted, invertedInverted;

    mat.init_identity();
    mat.apply_scale(2.0f, 3.0f, 4.0f);
    mat.apply_translation(10.0f, 20.0f, 30.0f);
    mat.apply_rotation_x(SHZ_F_PI);
    mat.inverse(&inverted);
    inverted.inverse(&invertedInverted);

    GBL_TEST_VERIFY(mat == invertedInverted);
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