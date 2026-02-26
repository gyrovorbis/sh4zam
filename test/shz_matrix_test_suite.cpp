#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_matrix.hpp"
#include <cglm/cglm.h>

#define GBL_SELF_TYPE   shz_matrix_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

union shz_glm_mat4 {
    shz::mat4x4 shz;
    mat4        glm;
};

bool operator==(const shz::quat& shz, const versor& glm) {
    return (shz_equalf(shz.x, glm[0]) && shz_equalf(shz.y, glm[1]) && shz_equalf(shz.z, glm[2]) && shz_equalf(shz.w, glm[3]))
        || (shz_equalf(-shz.x, glm[0]) && shz_equalf(-shz.y, glm[1]) && shz_equalf(-shz.z, glm[2]) && shz_equalf(-shz.w, glm[3]));
}

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

GBL_TEST_CASE(to_quat)
    auto test = [&](shz_glm_mat4& mat) {
        shz::quat shzQuat = mat.shz.to_quat();
        versor    glmQuat;

        glm_mat4_quat(mat.glm, glmQuat);

        return shzQuat == glmQuat;
    };

    shz_glm_mat4 mat;
    mat.shz.init_identity();
    GBL_TEST_VERIFY(test(mat));
    mat.shz.init_rotation_x(shz::deg_to_rad(90.0f));
    GBL_TEST_VERIFY(test(mat));
    mat.shz.init_rotation_y(shz::deg_to_rad(45.0f));
    GBL_TEST_VERIFY(test(mat));
    mat.shz.init_rotation_z(shz::deg_to_rad(180.0f));
    GBL_TEST_VERIFY(test(mat));

    shz::quat shzQuat;
    versor    glmQuat;
    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::mat4x4::to_quat", [&] {
               shzQuat = mat.shz.to_quat();
            },
            "glm_mat4_quat", [&] {
                glm_mat4_quat(mat.glm, glmQuat);
            }
        )
    );

#if 0
    std::println("{} == {}, {} == {}, {} == {}, {} == {}",
                 shzQuat.x, glmQuat[0], shzQuat.y, glmQuat[1],
                 shzQuat.z, glmQuat[2], shzQuat.w, glmQuat[3]);
#endif

    GBL_TEST_VERIFY(shzQuat == glmQuat);
GBL_TEST_CASE_END

GBL_TEST_REGISTER(inverse,
                  transform_vec4,
                  to_quat)