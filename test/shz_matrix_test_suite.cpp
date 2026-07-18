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

union shz_glm_vec4 {
    shz::vec4 shz;
    vec4      glm;
};

bool operator==(const shz::quat& shz, const versor& glm) {
    return (shz_equalf(shz.x, glm[0]) && shz_equalf(shz.y, glm[1]) && shz_equalf(shz.z, glm[2]) && shz_equalf(shz.w, glm[3]))
        || (shz_equalf(-shz.x, glm[0]) && shz_equalf(-shz.y, glm[1]) && shz_equalf(-shz.z, glm[2]) && shz_equalf(-shz.w, glm[3]));
}

GBL_TEST_CASE(copy)
    alignas(32) shz::mat4x4 m1, m2, m3, m4;

    m1.init_diagonal(1.0f, 2.0f, 3.0f, 4.0f);
    m3.init_rotation_xyz(shz::pi_f, shz::pi_f_2, shz::pi_f_4);

    shz::mat4x4::copy(&m2, m1);
    GBL_TEST_VERIFY(m1 == m2);

    shz::mat4x4::copy(&m4, m3);
    GBL_TEST_VERIFY(m3 == m4);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::copy",
                              [](shz::mat4x4* dst, const shz::mat4x4& src) {
                                  shz::mat4x4::copy(dst, src);
                              },
                              "memcpy",
                              [](shz::mat4x4* dst, const shz::mat4x4& src) {
                                  memcpy(reinterpret_cast<void*>(dst),
                                         reinterpret_cast<const void*>(&src),
                                         sizeof(shz::mat4x4));
                              },
                              &m1, m2)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(swap)
    alignas(32) shz::mat4x4 m1, m2, m3, m4;

    m1.init_diagonal(1.0f, 2.0f, 3.0f, 4.0f);
    m3.init_rotation_xyz(shz::pi_f, shz::pi_f_2, shz::pi_f_4);

    shz::mat4x4::copy(&m2, m1);
    shz::mat4x4::copy(&m4, m3);

    swap(m1, m3);
    GBL_TEST_VERIFY(m3 == m2);
    GBL_TEST_VERIFY(m1 == m4);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::swap",
                              [](shz::mat4x4& a, shz::mat4x4& b) {
                                  swap(a, b);
                              },
                              "std::swap",
                              [](shz::mat4x4& a, shz::mat4x4& b) {
                                  std::swap(a, b);
                              },
                              m1, m2)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(inverse)
   auto test = [&](shz_glm_mat4& mat) {
        shz_glm_mat4 shzInverted, glmInverted;

        benchmark(nullptr, [&]{
            glm_mat4_inv(mat.glm, glmInverted.glm);
        });

        benchmark(nullptr, [&]{ mat.shz.inverse(&shzInverted.shz); });

        return shzInverted.shz == glmInverted.shz;
    };

    shz_glm_mat4 mat;
    shz::mat4x4 inverted, invertedInverted;

    mat.shz.init_identity();
    mat.shz.apply_scale(2.0f, 3.0f, 4.0f);
    mat.shz.apply_translation(10.0f, 20.0f, 30.0f);
    mat.shz.apply_rotation_x(SHZ_F_PI);
    mat.shz.inverse(&inverted);
    inverted.inverse(&invertedInverted);

    GBL_TEST_VERIFY(mat.shz == invertedInverted);
    GBL_TEST_VERIFY(test(mat));
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec4)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4) + alignof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data() + alignof(shz_glm_mat4));

    mat.shz.init_identity();
    GBL_TEST_VERIFY(mat.shz.transform({ 1.0f, 2.0f, 3.0f, 4.0f }) ==
                        shz::vec4(  1.0f, 2.0f, 3.0f, 4.0f ));

    mat.shz.apply_scale(1.0f, 2.0f, 3.0f);
    GBL_TEST_VERIFY(mat.shz.transform({ 1.0f, 2.0f, 3.0f, 4.0f }) ==
                        shz::vec4(  1.0f, 4.0f, 9.0f, 4.0f ));

    mat.shz.apply_translation(-3.0f, -2.0f, -1.0f);
    GBL_TEST_VERIFY(mat.shz.transform({ 1.0f, 2.0f, 3.0f, 1.0f }) ==
                        shz::vec4( -2.0f, 2.0f, 8.0f, 1.0f ));

    mat.shz.apply_self();
    {
        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec4>)(
                "shz::mat4x4::transform_vec4", [](const shz_glm_mat4& m, const shz_glm_vec4& v) {
                    return m.shz.transform(v.shz);
                },
                "glm_mat4_mulv", [](const shz_glm_mat4& m, const shz_glm_vec4& v) {
                    shz_glm_vec4 res;
                    glm_mat4_mulv(const_cast<mat4&>(m.glm), const_cast<vec4&>(v.glm), const_cast<vec4&>(res.glm));
                    return res.shz;
                },
                mat, shz_glm_vec4({ gblRandf(), gblRandf(), gblRandf(), gblRandf() })
            )
        );
    }

    {
        alignas(32) shz_glm_mat4 mat2;
        mat2.shz = mat.shz;
        mat2.shz.apply_self();
        auto v = shz_glm_vec4({ gblRandf(), gblRandf(), gblRandf(), gblRandf() });

        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec4>)(
                "shz::mat4x4::transform_vec4 [alignas(32)]", [](const shz_glm_mat4& m, const shz_glm_vec4& v) {
                    return m.shz.transform(v.shz);
                },
                "glm_mat4_mulv", [](const shz_glm_mat4& m, const shz_glm_vec4& v) {
                    shz_glm_vec4 res;
                    glm_mat4_mulv(const_cast<mat4&>(m.glm), const_cast<vec4&>(v.glm), const_cast<vec4&>(res.glm));
                    return res.shz;
                },
                mat2, v
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(to_quat)
    auto test = [](shz_glm_mat4& mat) {
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
    mat.shz.apply_self();

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::quat>)(
            "shz::mat4x4::to_quat", [](const shz_glm_mat4& mat) {
               return mat.shz.to_quat();
            },
            "glm_mat4_quat", [](const shz_glm_mat4& mat) {
                versor glmQuat;
                glm_mat4_quat(const_cast<mat4&>(mat.glm), glmQuat);
                return shz_quat_init(glmQuat[3], glmQuat[0], glmQuat[1], glmQuat[2]);
            },
            mat
        )
    );
GBL_TEST_CASE_END

GBL_TEST_REGISTER(copy,
                  swap,
                  inverse,
                  transform_vec4,
                  to_quat)