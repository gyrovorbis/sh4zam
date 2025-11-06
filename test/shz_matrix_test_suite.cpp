#include "shz_test.h"
#include "sh4zam/shz_matrix.hpp"

#define GBL_SELF_TYPE   shz_matrix_test_suite

GBL_TEST_FIXTURE {
    char dummy;
};

GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

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

GBL_TEST_REGISTER(transform_vec4)