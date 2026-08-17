#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_matrix.hpp"
#include <cglm/cglm.h>

#define GBL_SELF_TYPE   shz_matrix_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

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

GBL_TEST_CASE(transform_vec3)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4) + alignof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data() + alignof(shz_glm_mat4));

    mat.shz.init_identity();
    GBL_TEST_VERIFY(mat.shz.transform(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3(  1.0f, 2.0f, 3.0f ));

    mat.shz.apply_scale(1.0f, 2.0f, 3.0f);
    GBL_TEST_VERIFY(mat.shz.transform(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3(  1.0f, 4.0f, 9.0f ));

    mat.shz.apply_translation(-3.0f, -2.0f, -1.0f);
    GBL_TEST_VERIFY(mat.shz.transform(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3(  1.0f, 4.0f, 9.0f ));

    mat.shz.apply_self();
    {
        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)(
                "shz::mat4x4::transform_vec3", [](const shz_glm_mat4& m, const shz_glm_vec3& v) {
                    return m.shz.transform(v.shz);
                },
                "glm_mat4_mulv3", [](const shz_glm_mat4& m, const shz_glm_vec3& v) {
                    shz_glm_vec3 res;
                    glm_mat4_mulv3(const_cast<mat4&>(m.glm), const_cast<vec3&>(v.glm), 0.0f, res.glm);
                    return res.shz;
                },
                mat, shz_glm_vec3({ gblRandf(), gblRandf(), gblRandf() })
            )
        );
    }
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

GBL_TEST_CASE(translate_reverse)
    alignas(32) shz_glm_mat4 mat;

    mat.shz.init_identity();
    mat.shz.apply_scale(2.0f, 3.0f, 4.0f);

    shz_glm_mat4 before;
    before.shz = mat.shz;

    mat.shz.translate_reverse(10.0f, -20.0f, 30.0f);

    // Expectation: mat = T(x,y,z) * mat_old--i.e. the pre-existing scale must
    // NOT affect the newly-applied translation column, unlike translate().
    shz_glm_mat4 expected;
    {
        mat4 t;
        vec3 tv = { 10.0f, -20.0f, 30.0f };
        glm_translate_make(t, tv);
        glm_mat4_mul(t, before.glm, expected.glm);
    }

    GBL_TEST_VERIFY(mat.shz == expected.shz);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::translate_reverse",
                              [](shz_glm_mat4& m, float x, float y, float z) {
                                  m.shz.translate_reverse(x, y, z);
                              },
                              "glm translate_reverse",
                              [](shz_glm_mat4& m, float x, float y, float z) {
                                  mat4 t;
                                  vec3 v = { x, y, z };
                                  glm_translate_make(t, v);
                                  glm_mat4_mul(t, m.glm, m.glm);
                              },
                              mat, 100.0f, 200.0f, 300.0f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(scale_reverse)
    alignas(32) shz_glm_mat4 mat;

    mat.shz.init_identity();
    mat.shz.apply_translation(1.0f, 2.0f, 3.0f);

    shz_glm_mat4 before;
    before.shz = mat.shz;

    mat.shz.scale_reverse(2.0f, 3.0f, 4.0f);

    // Expectation: mat = S(x,y,z) * mat_old--i.e. the pre-existing translation
    // column DOES get scaled, unlike scale(), which leaves it untouched.
    shz_glm_mat4 expected;
    {
        mat4 s;
        vec3 sv = { 2.0f, 3.0f, 4.0f };
        glm_scale_make(s, sv);
        glm_mat4_mul(s, before.glm, expected.glm);
    }

    GBL_TEST_VERIFY(mat.shz == expected.shz);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::scale_reverse",
                              [](shz_glm_mat4& m, float x, float y, float z) {
                                  m.shz.scale_reverse(x, y, z);
                              },
                              "glm scale_reverse",
                              [](shz_glm_mat4& m, float x, float y, float z) {
                                  volatile mat4 s;
                                  volatile vec3 v = { x, y, z };
                                  glm_scale_make(*const_cast<mat4*>(&s), *const_cast<vec3*>(&v));
                                  glm_mat4_mul(*const_cast<mat4*>(&s), m.glm, m.glm);
                              },
                              mat, 2.0f, 3.0f, 4.0f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(set_scale)
    alignas(32) shz_glm_mat4 mat;

    // Set against identity.
    {
        mat.shz.init_identity();
        mat.shz.set_scale(-2.0f, -3.0f, -4.0f);

        shz_glm_mat4 expected;
        glm_mat4_identity(expected.glm);
        expected.glm[0][0] = -2.0f;
        expected.glm[1][1] = -3.0f;
        expected.glm[2][2] = -4.0f;

        GBL_TEST_VERIFY(mat.shz == expected.shz);
    }

    // Set against a matrix with existing rotation + translation.
    // Expectation: set_scale() replaces the linear 3x3 part wholesale--the
    // rotation's off-diagonal terms get wiped out, not blended with. The
    // translation column is left completely untouched.
    {
        mat.shz.init_identity();
        mat.shz.apply_rotation_x(shz::deg_to_rad(42.0f));
        mat.shz.apply_translation(1.0f, 2.0f, 3.0f);

        shz::vec3 priorTranslation = mat.shz.get_translation();

        mat.shz.set_scale(5.0f, 6.0f, 7.0f);

        GBL_TEST_VERIFY(mat.shz.elem2D[0][0] == 5.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[1][1] == 6.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[2][2] == 7.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[0][1] == 0.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[0][2] == 0.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[1][0] == 0.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[1][2] == 0.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[2][0] == 0.0f);
        GBL_TEST_VERIFY(mat.shz.elem2D[2][1] == 0.0f);
        GBL_TEST_VERIFY(mat.shz.get_translation() == priorTranslation);
    }

    // Test overwrite (NOT composition) semantics.
    {
        mat.shz.init_identity();
        mat.shz.set_scale(2.0f, 3.0f, -4.0f);
        mat.shz.set_scale(5.0f, 6.0f, 7.0f);

        GBL_TEST_VERIFY(mat.shz.get_scale() == shz::vec3(5.0f, 6.0f, 7.0f));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::set_scale",
                              [](shz_glm_mat4& m, float x, float y, float z) {
                                  m.shz.set_scale(x, y, z);
                              },
                              "glm direct diagonal write",
                              [](shz_glm_mat4& m, float x, float y, float z) {
                                  m.glm[0][0] = x;    m.glm[0][1] = 0.0f; m.glm[0][2] = 0.0f;
                                  m.glm[1][0] = 0.0f; m.glm[1][1] = y;    m.glm[1][2] = 0.0f;
                                  m.glm[2][0] = 0.0f; m.glm[2][1] = 0.0f; m.glm[2][2] = z;
                              },
                              mat, -2.0f, -3.0f, -4.0f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(get_scale)
    alignas(32) shz_glm_mat4 mat;

    // Extract from a fresh, axis-aligned scale.
    {
        mat.shz.init_identity();
        mat.shz.set_scale(10.0f, -20.0f, 30.0f);

        GBL_TEST_VERIFY(mat.shz.get_scale() == shz::vec3(10.0f, 20.0f, 30.0f));
    }

    // Extract from a matrix with scale composed under a rotation.
    // Expectation: Per-axis magnitude extraction recovers the correct scale
    // regardless of rotation, since rotating a column doesn't change its length.
    {
        mat.shz.init_identity();
        mat.shz.apply_rotation_x(shz::deg_to_rad(30.0f));
        mat.shz.apply_rotation_y(shz::deg_to_rad(60.0f));
        mat.shz.apply_scale(3.0f, 4.0f, 5.0f);

        vec3 glmScale;
        glm_decompose_scalev(mat.glm, glmScale);

        shz::vec3 scale = mat.shz.get_scale();
        GBL_TEST_VERIFY(shz_equalf(scale.x, glmScale[0]));
        GBL_TEST_VERIFY(shz_equalf(scale.y, glmScale[1]));
        GBL_TEST_VERIFY(shz_equalf(scale.z, glmScale[2]));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec3>)("shz::mat4x4::get_scale",
                                   [](const shz_glm_mat4& m) {
                                       return m.shz.get_scale();
                                   },
                                   "glm_decompose_scalev",
                                   [](const shz_glm_mat4& m) {
                                       volatile shz_glm_vec3 s;
                                       glm_decompose_scalev(const_cast<mat4&>(m.glm), const_cast<vec3&>(s.glm));
                                       return s.shz;
                                   },
                                   mat)
    );
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
                  transform_vec3,
                  transform_vec4,
                  translate_reverse,
                  scale_reverse,
                  set_scale,
                  get_scale,
                  to_quat)