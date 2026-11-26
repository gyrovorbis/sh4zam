#include "shz_test_scenario.h"
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

static shz_glm_mat4 dense_matrix() {
    return { .glm = {
        { 1, 2, 3, 4 },
        { 2, 5, 7, 1 },
        { 3, 1, 4, 2 },
        { 4, 3, 2, 1 }
    } };
}

GBL_TEST_CASE(init_identity)
    alignas(32) static shz_glm_mat4 res, ref;

    res = dense_matrix();
    res.shz.init_identity();
    glm_mat4_identity(ref.glm);

    GBL_TEST_VERIFY(res.shz == ref.shz);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_identity", [&]() {
            res.shz.init_identity();
        },
        "glm_mat4_identity", [&]() {
            glm_mat4_identity(ref.glm);
        }));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_identity", [&]() {
            res.shz.init_identity();
        },
        "shz::xmtrx::init_identity + store", [&]() {
            shz::xmtrx::init_identity();
            shz::xmtrx::store(&ref.shz);
        }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_zero)
    alignas(32) static shz_glm_mat4 res, ref;

    res = dense_matrix();
    res.shz.init_zero();
    glm_mat4_zero(ref.glm);

    GBL_TEST_VERIFY(res.shz == ref.shz);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_zero", [&]() {
            res.shz.init_zero();
        },
        "glm_mat4_zero", [&]() {
            glm_mat4_zero(ref.glm);
        }));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_zero", [&]() {
            res.shz.init_zero();
        },
        "shz::xmtrx::init_zero + store", [&]() {
            shz::xmtrx::init_zero();
            shz::xmtrx::store(&ref.shz);
        }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_one)
    alignas(32) static shz_glm_mat4 res;

    res = dense_matrix();
    res.shz.init_one();

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(res.shz.elem[i] == 1.0f);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_one", [&]() {
            res.shz.init_one();
        },
        "shz::xmtrx::init_one + store", [&]() {
            shz::xmtrx::init_one();
            shz::xmtrx::store(&res.shz);
        }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_fill)
    alignas(32) static shz_glm_mat4 res, ref;
    volatile float                  value = -3.5f;

    res = dense_matrix();
    res.shz.init_fill(-3.5f);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(res.shz.elem[i] == -3.5f);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_fill", [&](const volatile float* v) {
            res.shz.init_fill(*v);
        },
        "C fill", [&](const volatile float* v) {
            const float f = *v;
            for(unsigned i = 0; i < 16; ++i)
                ref.shz.elem[i] = f;
        },
        &value));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_fill", [&](const volatile float* v) {
            res.shz.init_fill(*v);
        },
        "shz::xmtrx::init_fill + store", [&](const volatile float* v) {
            shz::xmtrx::init_fill(*v);
            shz::xmtrx::store(&ref.shz);
        },
        &value));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_diagonal)
    alignas(32) static shz_glm_mat4 res, ref;

    res = dense_matrix();
    res.shz.init_diagonal(2.0f, -3.0f, 4.5f, 0.25f);
    glm_mat4_zero(ref.glm);
    ref.glm[0][0] = 2.0f;
    ref.glm[1][1] = -3.0f;
    ref.glm[2][2] = 4.5f;
    ref.glm[3][3] = 0.25f;

    GBL_TEST_VERIFY(res.shz == ref.shz);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_diagonal", [&](float x, float y, float z, float w) {
            res.shz.init_diagonal(x, y, z, w);
        },
        "glm_mat4_zero + diagonal", [&](float x, float y, float z, float w) {
            glm_mat4_zero(ref.glm);
            ref.glm[0][0] = x;
            ref.glm[1][1] = y;
            ref.glm[2][2] = z;
            ref.glm[3][3] = w;
        },
        2.0f, -3.0f, 4.5f, 0.25f));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_diagonal", [&](float x, float y, float z, float w) {
            res.shz.init_diagonal(x, y, z, w);
        },
        "shz::xmtrx::init_diagonal + store", [&](float x, float y, float z, float w) {
            shz::xmtrx::init_diagonal(x, y, z, w);
            shz::xmtrx::store(&ref.shz);
        },
        2.0f, -3.0f, 4.5f, 0.25f));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_scale)
    alignas(32) static shz_glm_mat4 res, ref;

    vec3 scale = { 2.0f, -3.0f, 4.5f };

    res = dense_matrix();
    res.shz.init_scale(2.0f, -3.0f, 4.5f);
    glm_scale_make(ref.glm, scale);

    GBL_TEST_VERIFY(res.shz == ref.shz);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_scale", [&](float x, float y, float z) {
            res.shz.init_scale(x, y, z);
        },
        "glm_scale_make", [&](float x, float y, float z) {
            vec3 v = { x, y, z };
            glm_scale_make(ref.glm, v);
        },
        2.0f, -3.0f, 4.5f));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_scale", [&](float x, float y, float z) {
            res.shz.init_scale(x, y, z);
        },
        "shz::xmtrx::init_scale + store", [&](float x, float y, float z) {
            shz::xmtrx::init_scale(x, y, z);
            shz::xmtrx::store(&ref.shz);
        },
        2.0f, -3.0f, 4.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(determinant)
    shz_glm_mat4 mat = dense_matrix();
    GBL_TEST_VERIFY(mat.shz.determinant() == glm_mat4_det(mat.glm));

    mat.shz.init_identity();
    GBL_TEST_VERIFY(mat.shz.determinant() == 1.0f);

    mat = dense_matrix();
    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::mat4x4::determinant",
                               [](const shz_glm_mat4& m) {
                                   return m.shz.determinant();
                               },
                               "glm_mat4_det",
                               [](const shz_glm_mat4& m) {
                                   return glm_mat4_det(const_cast<mat4&>(m.glm));
                               },
                               mat)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(inverse)
    shz_glm_mat4 mat, shzInverted, glmInverted;
    shz_glm_mat4 dense = dense_matrix();
    shz::mat4x4 inverted, invertedInverted;

    mat.shz.init_identity();
    mat.shz.scale_3x3(2.0f, 3.0f, 4.0f);
    mat.shz.add_translation(10.0f, 20.0f, 30.0f);
    mat.shz.rotate_x(shz::pi_f);
    mat.shz.inverse(&inverted);
    inverted.inverse(&invertedInverted);

    GBL_TEST_VERIFY(mat.shz == invertedInverted);

    mat.shz.inverse(&shzInverted.shz);
    glm_mat4_inv(mat.glm, glmInverted.glm);
    GBL_TEST_VERIFY(shzInverted.shz == glmInverted.shz);

    dense.shz.inverse(&shzInverted.shz);
    glm_mat4_inv(dense.glm, glmInverted.glm);
    GBL_TEST_VERIFY(shzInverted.shz == glmInverted.shz);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::inverse",
                              [](const shz_glm_mat4& src, shz_glm_mat4* dst) {
                                  src.shz.inverse(&dst->shz);
                              },
                              "glm_mat4_inv",
                              [](const shz_glm_mat4& src, shz_glm_mat4* dst) {
                                  glm_mat4_inv(const_cast<mat4&>(src.glm), dst->glm);
                              },
                              mat, &shzInverted)
    );

    (benchmark_cmp<void>)("shz::mat4x4::inverse [dense]",
                          [](const shz_glm_mat4& src, shz_glm_mat4* dst) {
                              src.shz.inverse(&dst->shz);
                          },
                          "glm_mat4_inv [dense]",
                          [](const shz_glm_mat4& src, shz_glm_mat4* dst) {
                              glm_mat4_inv(const_cast<mat4&>(src.glm), dst->glm);
                          },
                          dense, &shzInverted);
GBL_TEST_CASE_END

GBL_TEST_CASE(inverse_rigid)
    struct rigid { float rx, ry, rz, tx, ty, tz, mirror; };
    constexpr rigid cases[] = {
        {  0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,  1.0f },
        {  0.4f, -0.8f,  1.2f,   3.0f, -2.0f,  5.0f,  1.0f },
        { -2.1f,  1.3f,  0.7f, -10.0f,  0.5f, -7.5f,  1.0f },
        {  0.9f,  0.2f, -1.6f,   4.0f,  6.0f, -1.0f, -1.0f }
    };

    shz_glm_mat4 mat, glmInverted, shzInverted;
    shz::mat4x4  invertedInverted;

    for(const auto& c : cases) {
        mat.shz.init_rotation_xyz(c.rx, c.ry, c.rz);
        mat.shz.scale_3x3(c.mirror, 1.0f, 1.0f);
        mat.shz.add_translation(c.tx, c.ty, c.tz);

        mat.shz.inverse_rigid(&shzInverted.shz);
        glm_mat4_copy(mat.glm, glmInverted.glm);
        glm_inv_tr(glmInverted.glm);

        GBL_TEST_VERIFY(shzInverted.shz == glmInverted.shz);
        GBL_TEST_VERIFY(shzInverted.shz.elem[3]  == 0.0f && shzInverted.shz.elem[7]  == 0.0f &&
                        shzInverted.shz.elem[11] == 0.0f && shzInverted.shz.elem[15] == 1.0f);

        shzInverted.shz.inverse_rigid(&invertedInverted);
        GBL_TEST_VERIFY(mat.shz == invertedInverted);
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::inverse_rigid",
                              [](const shz_glm_mat4& src, shz_glm_mat4* dst) {
                                  src.shz.inverse_rigid(&dst->shz);
                              },
                              "glm_inv_tr",
                              [](const shz_glm_mat4& src, shz_glm_mat4* dst) {
                                  glm_mat4_copy(const_cast<mat4&>(src.glm), dst->glm);
                                  glm_inv_tr(dst->glm);
                              },
                              mat, &shzInverted)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec3)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data());

    mat.shz.init_identity();
    GBL_TEST_VERIFY(mat.shz.transform(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3(  1.0f, 2.0f, 3.0f ));

    mat.shz.scale_3x3(1.0f, 2.0f, 3.0f);
    GBL_TEST_VERIFY(mat.shz.transform(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3(  1.0f, 4.0f, 9.0f ));

    mat.shz.add_translation(-3.0f, -2.0f, -1.0f);
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
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data());

    mat.shz.init_identity();
    GBL_TEST_VERIFY(mat.shz.transform({ 1.0f, 2.0f, 3.0f, 4.0f }) ==
                        shz::vec4(  1.0f, 2.0f, 3.0f, 4.0f ));

    mat.shz.scale_3x3(1.0f, 2.0f, 3.0f);
    GBL_TEST_VERIFY(mat.shz.transform({ 1.0f, 2.0f, 3.0f, 4.0f }) ==
                        shz::vec4(  1.0f, 4.0f, 9.0f, 4.0f ));

    mat.shz.add_translation(-3.0f, -2.0f, -1.0f);
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

GBL_TEST_CASE(transform_vec3_transpose)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data());

    mat.shz.init_identity();
    GBL_TEST_VERIFY(mat.shz.transform_transpose(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3( 1.0f, 2.0f, 3.0f ));

    mat.shz.scale_3x3(1.0f, 2.0f, 3.0f);
    GBL_TEST_VERIFY(mat.shz.transform_transpose(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3( 1.0f, 4.0f, 9.0f ));

    mat.shz.add_translation(-3.0f, -2.0f, -1.0f);
    GBL_TEST_VERIFY(mat.shz.transform_transpose(shz::vec3( 1.0f, 2.0f, 3.0f )) ==
                    shz::vec3( 1.0f, 4.0f, 9.0f ));

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec3>)(
            "shz::mat4x4::transform_vec3_transpose", [](const shz_glm_mat4& m, const shz_glm_vec3& v) {
                return m.shz.transform_transpose(v.shz);
            },
            "glm_vec3_dot x3", [](const shz_glm_mat4& m, const shz_glm_vec3& v) {
                shz_glm_vec3 res;
                for(unsigned c = 0; c < 3; ++c)
                    res.glm[c] = glm_vec3_dot(const_cast<float*>(m.glm[c]), const_cast<vec3&>(v.glm));
                return res.shz;
            },
            mat, shz_glm_vec3({ gblRandf(), gblRandf(), gblRandf() })
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec4_transpose)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data());

    mat.shz.init_identity();
    GBL_TEST_VERIFY(mat.shz.transform_transpose(shz::vec4( 1.0f, 2.0f, 3.0f, 4.0f )) ==
                    shz::vec4( 1.0f, 2.0f, 3.0f, 4.0f ));

    mat.shz.scale_3x3(1.0f, 2.0f, 3.0f);
    GBL_TEST_VERIFY(mat.shz.transform_transpose(shz::vec4( 1.0f, 2.0f, 3.0f, 4.0f )) ==
                    shz::vec4( 1.0f, 4.0f, 9.0f, 4.0f ));

    mat.shz.add_translation(-3.0f, -2.0f, -1.0f);
    GBL_TEST_VERIFY(mat.shz.transform_transpose(shz::vec4( 1.0f, 2.0f, 3.0f, 1.0f )) ==
                    shz::vec4( 1.0f, 4.0f, 9.0f, -9.0f ));

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec4>)(
            "shz::mat4x4::transform_vec4_transpose", [](const shz_glm_mat4& m, const shz_glm_vec4& v) {
                return m.shz.transform_transpose(v.shz);
            },
            "glm_vec4_dot x4", [](const shz_glm_mat4& m, const shz_glm_vec4& v) {
                shz_glm_vec4 res;
                for(unsigned c = 0; c < 4; ++c)
                    res.glm[c] = glm_vec4_dot(const_cast<vec4&>(m.glm[c]), const_cast<vec4&>(v.glm));
                return res.shz;
            },
            mat, shz_glm_vec4({ gblRandf(), gblRandf(), gblRandf(), gblRandf() })
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(translate_reverse)
    alignas(32) shz_glm_mat4 mat;

    mat.shz.init_identity();
    mat.shz.scale_3x3(2.0f, 3.0f, 4.0f);

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
    mat.shz.add_translation(1.0f, 2.0f, 3.0f);

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
    {
        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)("shz::mat4x4::scale_reverse",
                                  [](shz_glm_mat4& m, float x, float y, float z) {
                                      m.shz.scale_reverse(x, y, z);
                                  },
                                  "glm scale_reverse",
                                  [](shz_glm_mat4& m, float x, float y, float z) {
                                      static volatile mat4 s;
                                      volatile vec3 v = { x, y, z };
                                      glm_scale_make(*const_cast<mat4*>(&s), *const_cast<vec3*>(&v));
                                      glm_mat4_mul(*const_cast<mat4*>(&s), m.glm, m.glm);
                                  },
                                  mat, 2.0f, 3.0f, 4.0f)
        );
    }
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
        mat.shz.rotate_x(shz::deg_to_rad(42.0f));
        mat.shz.add_translation(1.0f, 2.0f, 3.0f);

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
        mat.shz.rotate_x(shz::deg_to_rad(30.0f));
        mat.shz.rotate_y(shz::deg_to_rad(60.0f));
        mat.shz.scale_3x3(3.0f, 4.0f, 5.0f);

        vec3 glmScale;
        glm_decompose_scalev(mat.glm, glmScale);

        shz::vec3 scale = mat.shz.get_scale();
        GBL_TEST_VERIFY(shz_equalf(scale.x, glmScale[0]));
        GBL_TEST_VERIFY(shz_equalf(scale.y, glmScale[1]));
        GBL_TEST_VERIFY(shz_equalf(scale.z, glmScale[2]));
    }

    {
        mat.shz.init_identity();
        mat.shz.set_scale(0.0f, 0.0f, 0.0f);

        shz::vec3 scale = mat.shz.get_scale();

        GBL_TEST_VERIFY(scale == shz::vec3(0.0f, 0.0f, 0.0f));
    }

    {
        mat.shz.init_zero();

        shz::vec3 scale = mat.shz.get_scale();

        GBL_TEST_VERIFY(scale == shz::vec3(0.0f, 0.0f, 0.0f));
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

GBL_TEST_CASE(get_rotation)
    alignas(32) shz_glm_mat4 mat;

    auto reference = [](const shz_glm_mat4& m, float* angle, shz::vec3* axis) {
        versor q;
        vec3   a;

        glm_mat4_quat(const_cast<mat4&>(m.glm), q);
        glm_quat_axis(q, a);

        *angle = glm_quat_angle(q);
        *axis  = shz::vec3(a[0], a[1], a[2]);

        if(*angle > shz::pi_f) {
            *angle = shz::pi_f * 2.0f - *angle;
            *axis  = -*axis;
        }
    };

    auto matches = [&](const shz_glm_mat4& m, bool signFree = false) {
        float     angle, refAngle;
        shz::vec3 axis,  refAxis;

        m.shz.get_rotation(&angle, &axis);
        reference(m, &refAngle, &refAxis);

        const float alignment = axis.dot(refAxis);

        return shz_fabsf(angle - refAngle) <= 1.0e-2f &&
               (signFree? shz_fabsf(alignment) : alignment) >= 0.9999f;
    };

    // Rotations about each basis axis.
    {
        mat.shz.init_rotation_x(0.5f);
        GBL_TEST_VERIFY(matches(mat));

        mat.shz.init_rotation_y(0.5f);
        GBL_TEST_VERIFY(matches(mat));

        mat.shz.init_rotation_z(0.5f);
        GBL_TEST_VERIFY(matches(mat));
    }

    // Arbitrary axis across the angle range, both directions.
    for(float angle : { 0.3f, 1.2f, 2.5f, 3.0f, -1.2f, -2.5f }) {
        mat.shz.init_rotation(angle, shz::vec3(1.0f, 2.0f, 3.0f).direction());
        GBL_TEST_VERIFY(matches(mat));
    }

    // Composed rotations with translation, which must be ignored.
    {
        mat.shz.init_identity();
        mat.shz.rotate_x(shz::deg_to_rad(30.0f));
        mat.shz.rotate_y(shz::deg_to_rad(60.0f));
        mat.shz.rotate_z(shz::deg_to_rad(-45.0f));
        mat.shz.add_translation(5.0f, -6.0f, 7.0f);
        GBL_TEST_VERIFY(matches(mat));
    }

    // Identity: zero angle with the fallback X axis.
    {
        mat.shz.init_identity();

        float     angle;
        shz::vec3 axis;
        mat.shz.get_rotation(&angle, &axis);

        GBL_TEST_VERIFY(angle == 0.0f);
        GBL_TEST_VERIFY(axis == shz::vec3(1.0f, 0.0f, 0.0f));
    }

    // Half-turns, where the axis is only defined up to sign.
    {
        mat.shz.init_rotation_x(shz::pi_f);
        GBL_TEST_VERIFY(matches(mat, true));

        mat.shz.init_rotation_y(shz::pi_f);
        GBL_TEST_VERIFY(matches(mat, true));

        mat.shz.init_rotation_z(shz::pi_f);
        GBL_TEST_VERIFY(matches(mat, true));

        mat.shz.init_rotation(shz::pi_f, shz::vec3(1.0f, 2.0f, 3.0f).direction());
        GBL_TEST_VERIFY(matches(mat, true));

        mat.shz.init_rotation(shz::pi_f, shz::vec3(-1.0f, 2.0f, -3.0f).direction());
        GBL_TEST_VERIFY(matches(mat, true));
    }

    // std::pair overload agrees with the pointer overload.
    {
        mat.shz.init_rotation(1.2f, shz::vec3(1.0f, 2.0f, 3.0f).direction());

        float     angle;
        shz::vec3 axis;
        mat.shz.get_rotation(&angle, &axis);

        const auto [pairAngle, pairAxis] = mat.shz.get_rotation();

        GBL_TEST_VERIFY(pairAngle == angle);
        GBL_TEST_VERIFY(pairAxis  == axis);
    }

    mat.shz.init_rotation(1.2f, shz::vec3(1.0f, 2.0f, 3.0f).direction());

    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::mat4x4::get_rotation",
                               [](const shz_glm_mat4& m) {
                                   float     angle;
                                   shz::vec3 axis;
                                   m.shz.get_rotation(&angle, &axis);
                                   return angle + axis.x;
                               },
                               "glm_mat4_quat + glm_quat_angle/axis",
                               [](const shz_glm_mat4& m) {
                                   versor q;
                                   vec3   axis;
                                   glm_mat4_quat(const_cast<mat4&>(m.glm), q);
                                   glm_quat_axis(q, axis);
                                   return glm_quat_angle(q) + axis[0];
                               },
                               mat)
    );

    mat.shz.init_rotation(3.1f, shz::vec3(1.0f, 2.0f, 3.0f).direction());

    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::mat4x4::get_rotation [near pi]",
                               [](const shz_glm_mat4& m) {
                                   float     angle;
                                   shz::vec3 axis;
                                   m.shz.get_rotation(&angle, &axis);
                                   return angle + axis.x;
                               },
                               "glm_mat4_quat + glm_quat_angle/axis [near pi]",
                               [](const shz_glm_mat4& m) {
                                   versor q;
                                   vec3   axis;
                                   glm_mat4_quat(const_cast<mat4&>(m.glm), q);
                                   glm_quat_axis(q, axis);
                                   return glm_quat_angle(q) + axis[0];
                               },
                               mat)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_dir)
    alignas(32) shz_glm_mat4 mat;

    // Rotating about a standard basis axis should match the dedicated
    // init_rotation_x() routine exactly (axis-angle is a strict generalization).
    {
        float angle = shz::deg_to_rad(42.0f);

        mat.shz.init_rotation_dir(angle, shz::vec3(1.0f, 0.0f, 0.0f));

        shz_glm_mat4 expected;
        expected.shz.init_rotation_x(angle);

        GBL_TEST_VERIFY(mat.shz == expected.shz);
    }

    // Rotate around a non-trivial (non-axis-aligned) normalized direction,
    // cross-checked against cglm's equivalent from-scratch construction.
    {
        shz::vec3 dir   = shz::vec3(1.0f, 1.0f, 1.0f).direction();
        float     angle = shz::deg_to_rad(42.0f);

        mat.shz.init_rotation_dir(angle, dir);

        shz_glm_mat4 expected;
        vec3 glmDir = { dir.x, dir.y, dir.z };
        glm_rotate_make(expected.glm, angle, glmDir);

        GBL_TEST_VERIFY(mat.shz == expected.shz);
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::init_rotation_dir",
                              [](shz_glm_mat4& m, float angle, float x, float y, float z) {
                                  m.shz.init_rotation_dir(angle, x, y, z);
                              },
                              "glm_rotate_make",
                              [](shz_glm_mat4& m, float angle, float x, float y, float z) {
                                  vec3 axis = { x, y, z };
                                  glm_rotate_make(m.glm, angle, axis);
                              },
                              mat, shz::deg_to_rad(42.0f), 0.5773503f, 0.5773503f, 0.5773503f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(blend)
    // Blend a single joint matrix onto a known starting matrix.
    // Expectation: dst[i] = dst_old[i] + joint[i] * weight, for all 16
    // elements--a straight per-element weighted accumulate (skeletal-skinning
    // blend semantics, mirroring shz::xmtrx::blend()).
    {
        alignas(32) shz::mat4x4 dst;
        dst.init_fill(2.0f);

        alignas(32) shz_mat4x4_t joint = { .elem = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        }};

        dst.blend(joint, 0.5f);

        for(int i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz_equalf(dst.elem[i], 2.0f + joint.elem[i] * 0.5f));
    }

    // Accumulate multiple weighted joints from zero--the standard skeletal
    // skinning pattern: start the working matrix at zero and blend() in each
    // influencing bone's weighted contribution.
    {
        alignas(32) shz::mat4x4 dst;
        dst.init_zero();

        alignas(32) shz_mat4x4_t joint1 = { .elem = {
              1.0f,   0.0f,   0.0f,  0.0f,
              0.0f,   1.0f,   0.0f,  0.0f,
              0.0f,   0.0f,   1.0f,  0.0f,
             10.0f,  20.0f,  30.0f,  1.0f
        }};
        alignas(32) shz_mat4x4_t joint2 = { .elem = {
              2.0f,   0.0f,   0.0f,  0.0f,
              0.0f,   2.0f,   0.0f,  0.0f,
              0.0f,   0.0f,   2.0f,  0.0f,
             -5.0f,  -6.0f,  -7.0f,  1.0f
        }};

        dst.blend(joint1, 0.25f);
        dst.blend(joint2, 0.75f);

        for(int i = 0; i < 16; ++i) {
            float expected = joint1.elem[i] * 0.25f + joint2.elem[i] * 0.75f;
            GBL_TEST_VERIFY(shz_equalf(dst.elem[i], expected));
        }
    }

    {
        alignas(32) shz_glm_mat4 dst;
        glm_mat4_identity(dst.glm);

        alignas(32) shz_mat4x4_t joint = { .elem = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        }};

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)("shz::mat4x4::blend",
                                  [&](const shz_mat4x4_t& j, float w) {
                                      dst.shz.blend(j, w);
                                  },
                                  "naive per-element accum",
                                  [&](const shz_mat4x4_t& j, float w) {
                                      for(int i = 0; i < 16; ++i)
                                          dst.shz.elem[i] += j.elem[i] * w;
                                  },
                                  joint, 0.5f)
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

    shz_glm_mat4                 mat;
    const shz_glm_mat4* volatile matp = &mat;

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
            "shz::mat4x4::to_quat", [](const shz_glm_mat4* const volatile* pp) {
               return (*pp)->shz.to_quat();
            },
            "glm_mat4_quat", [](const shz_glm_mat4* const volatile* pp) {
                versor glmQuat;
                glm_mat4_quat(const_cast<mat4&>((*pp)->glm), glmQuat);
                return shz_quat_init(glmQuat[3], glmQuat[0], glmQuat[1], glmQuat[2]);
            },
            &matp
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate)
    {
        shz::mat4x4 m;
        m.elem[0]  = 1.0f; m.elem[1]  = 0.0f; m.elem[2]  = 0.0f; m.elem[3]  = 0.0f;
        m.elem[4]  = 0.0f; m.elem[5]  = 1.0f; m.elem[6]  = 0.0f; m.elem[7]  = 0.0f;
        m.elem[8]  = 0.0f; m.elem[9]  = 0.0f; m.elem[10] = 1.0f; m.elem[11] = 0.0f;
        m.elem[12] = 7.0f; m.elem[13] = 8.0f; m.elem[14] = 9.0f; m.elem[15] = 1.0f;

        m.rotate(0.0f, 1.0f, 0.0f, 0.0f);

        GBL_TEST_VERIFY(m.elem[12] == 7.0f && m.elem[13] == 8.0f &&
                        m.elem[14] == 9.0f && m.elem[15] == 1.0f);
        for(int c = 0; c < 3; ++c)
            for(int r = 0; r < 4; ++r)
                GBL_TEST_VERIFY(shz::fabsf(m.elem2D[c][r] - (c == r ? 1.0f : 0.0f)) <= 1e-4f);
    }

    {
        const float start[16] = { 2.0f,  0.5f, -1.0f, 0.0f,
                                  0.25f, 3.0f,  0.75f, 0.0f,
                                 -1.5f,  1.0f,  1.25f, 0.0f,
                                  7.0f,  8.0f,  9.0f,  1.0f };

        for(const auto& [angle, axis] : { std::pair{ 1.1f,        shz::vec3(0.3f, -0.5f, 0.8f) },
                                          std::pair{ -2.5f,       shz::vec3(0.0f,  0.0f, 3.0f) },
                                          std::pair{ shz::pi_f_2, shz::vec3(1.0f,  1.0f, 1.0f) } }) {
            shz_glm_mat4 m, ref;
            shz_glm_vec3 glmAxis;
            memcpy(m.glm,   start, sizeof(start));
            memcpy(ref.glm, start, sizeof(start));
            glmAxis.shz = axis;
            glm_rotate(ref.glm, angle, glmAxis.glm);

            m.shz.rotate(angle, axis);

            for(int i = 0; i < 16; ++i)
                GBL_TEST_VERIFY(shz::fabsf(m.shz.elem[i] - ref.glm[i / 4][i % 4]) <= 1e-3f * shz::fmaxf(1.0f, shz::fabsf(ref.glm[i / 4][i % 4])));
        }

        {
            shz_glm_mat4 shzMat, glmMat;
            memcpy(shzMat.glm, start, sizeof(start));
            memcpy(glmMat.glm, start, sizeof(start));

            GBL_TEST_VERIFY(
                (benchmark_cmp<void>)(
                    "shz::mat4x4::rotate", [&](float angle, float x, float y, float z) {
                        shzMat.shz.rotate(angle, x, y, z);
                    },
                    "glm_rotate", [&](float angle, float x, float y, float z) {
                        vec3 axis = { x, y, z };
                        glm_rotate(glmMat.glm, angle, axis);
                    },
                    1.1f, 0.3f, -0.5f, 0.8f
                )
            );
        }
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_dir)
    {
        shz::mat4x4 m;
        m.elem[0]  = 1.0f; m.elem[1]  = 0.0f; m.elem[2]  = 0.0f; m.elem[3]  = 0.0f;
        m.elem[4]  = 0.0f; m.elem[5]  = 1.0f; m.elem[6]  = 0.0f; m.elem[7]  = 0.0f;
        m.elem[8]  = 0.0f; m.elem[9]  = 0.0f; m.elem[10] = 1.0f; m.elem[11] = 0.0f;
        m.elem[12] = 7.0f; m.elem[13] = 8.0f; m.elem[14] = 9.0f; m.elem[15] = 1.0f;

        m.rotate_dir(0.0f, 1.0f, 0.0f, 0.0f);

        GBL_TEST_VERIFY(m.elem[12] == 7.0f && m.elem[13] == 8.0f &&
                        m.elem[14] == 9.0f && m.elem[15] == 1.0f);
        for(int c = 0; c < 3; ++c)
            for(int r = 0; r < 4; ++r)
                GBL_TEST_VERIFY(shz::fabsf(m.elem2D[c][r] - (c == r ? 1.0f : 0.0f)) <= 1e-4f);
    }

    {
        const float start[16] = { 2.0f,  0.5f, -1.0f, 0.0f,
                                  0.25f, 3.0f,  0.75f, 0.0f,
                                 -1.5f,  1.0f,  1.25f, 0.0f,
                                  7.0f,  8.0f,  9.0f,  1.0f };

        for(const auto& [angle, axis] : { std::pair{ 1.1f,        shz::vec3(0.3f, -0.5f, 0.8f).direction() },
                                          std::pair{ -2.5f,       shz::vec3(0.0f,  0.0f, 1.0f)             },
                                          std::pair{ shz::pi_f_2, shz::vec3(1.0f,  1.0f, 1.0f).direction() } }) {
            shz_glm_mat4 m, ref;
            shz_glm_vec3 glmAxis;
            memcpy(m.glm,   start, sizeof(start));
            memcpy(ref.glm, start, sizeof(start));
            glmAxis.shz = axis;
            glm_rotate(ref.glm, angle, glmAxis.glm);

            m.shz.rotate_dir(angle, axis);

            for(int i = 0; i < 16; ++i)
                GBL_TEST_VERIFY(shz::fabsf(m.shz.elem[i] - ref.glm[i / 4][i % 4]) <= 1e-3f * shz::fmaxf(1.0f, shz::fabsf(ref.glm[i / 4][i % 4])));
        }

        {
            shz_glm_mat4 shzMat, glmMat;
            memcpy(shzMat.glm, start, sizeof(start));
            memcpy(glmMat.glm, start, sizeof(start));

            const shz::vec3 dir = shz::vec3(0.3f, -0.5f, 0.8f).direction();

            GBL_TEST_VERIFY(
                (benchmark_cmp<void>)(
                    "shz::mat4x4::rotate_dir", [&](float angle, float x, float y, float z) {
                        shzMat.shz.rotate_dir(angle, x, y, z);
                    },
                    "glm_rotate", [&](float angle, float x, float y, float z) {
                        vec3 axis = { x, y, z };
                        glm_rotate(glmMat.glm, angle, axis);
                    },
                    1.1f, dir.x, dir.y, dir.z
                )
            );
        }
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_u16)
    const uint16_t  angles[]  = { 0x0000, 0x2000, 0x4000, 0x8000, 0xc000, 0x04d2, 0xfedc, 0xffff };
    const shz::vec3 axes[]    = { shz::vec3(0.3f, -0.5f, 0.8f),
                                  shz::vec3(0.0f,  0.0f, 3.0f),
                                  shz::vec3(1.0f,  1.0f, 1.0f) };
    const float     start[16] = { 2.0f,  0.5f, -1.0f, 0.0f,
                                   0.25f, 3.0f,  0.75f, 0.0f,
                                  -1.5f,  1.0f,  1.25f, 0.0f,
                                   7.0f,  8.0f,  9.0f,  1.0f };

    for(uint16_t angle : angles) {
        for(const shz::vec3& axis : axes) {
            shz::mat4x4 expected, actual;

            memcpy(expected.elem, start, sizeof(start));
            memcpy(actual.elem,   start, sizeof(start));
            expected.rotate(static_cast<int16_t>(angle) * (1.0f / shz::fsca_rad_factor), axis);
            actual.rotate(angle, axis);

            GBL_TEST_VERIFY(actual == expected);
        }
    }

    {
        alignas(32) static shz_glm_mat4 res, ref;
        const shz::vec3                 dir     = shz::vec3(0.3f, -0.5f, 0.8f);
        volatile uint16_t               angle16 = 0x2000;
        volatile float                  in[4]   = { 0x2000 * (1.0f / shz::fsca_rad_factor), dir.x, dir.y, dir.z };

        memcpy(res.glm, start, sizeof(start));
        memcpy(ref.glm, start, sizeof(start));

        (benchmark_cmp<void>)(
            "shz::mat4x4::rotate(uint16_t)", [&](const volatile uint16_t* a, const volatile float* v) {
                res.shz.rotate(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "shz::mat4x4::rotate(float)", [&](const volatile uint16_t*, const volatile float* v) {
                ref.shz.rotate(v[0], v[1], v[2], v[3]);
            },
            &angle16, &in[0]);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::mat4x4::rotate(uint16_t)", [&](const volatile uint16_t* a, const volatile float* v) {
                res.shz.rotate(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "glm_rotate", [&](const volatile uint16_t*, const volatile float* v) {
                vec3 axis = { v[1], v[2], v[3] };
                glm_rotate(ref.glm, v[0], axis);
            },
            &angle16, &in[0]));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_dir_u16)
    const uint16_t  angles[]  = { 0x0000, 0x2000, 0x4000, 0x8000, 0xc000, 0x04d2, 0xfedc, 0xffff };
    const shz::vec3 axes[]    = { shz::vec3(0.3f, -0.5f, 0.8f).direction(),
                                  shz::vec3(0.0f,  0.0f, 3.0f).direction(),
                                  shz::vec3(1.0f,  1.0f, 1.0f).direction() };
    const float     start[16] = { 2.0f,  0.5f, -1.0f, 0.0f,
                                   0.25f, 3.0f,  0.75f, 0.0f,
                                  -1.5f,  1.0f,  1.25f, 0.0f,
                                   7.0f,  8.0f,  9.0f,  1.0f };

    for(uint16_t angle : angles) {
        for(const shz::vec3& axis : axes) {
            shz::mat4x4 expected, actual;

            memcpy(expected.elem, start, sizeof(start));
            memcpy(actual.elem,   start, sizeof(start));
            expected.rotate_dir(static_cast<int16_t>(angle) * (1.0f / shz::fsca_rad_factor), axis);
            actual.rotate_dir(angle, axis);

            GBL_TEST_VERIFY(actual == expected);
        }
    }

    {
        alignas(32) static shz_glm_mat4 res, ref;
        const shz::vec3                 dir     = shz::vec3(0.3f, -0.5f, 0.8f).direction();
        volatile uint16_t               angle16 = 0x2000;
        volatile float                  in[4]   = { 0x2000 * (1.0f / shz::fsca_rad_factor), dir.x, dir.y, dir.z };

        memcpy(res.glm, start, sizeof(start));
        memcpy(ref.glm, start, sizeof(start));

        (benchmark_cmp<void>)(
            "shz::mat4x4::rotate_dir(uint16_t)", [&](const volatile uint16_t* a, const volatile float* v) {
                res.shz.rotate_dir(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "shz::mat4x4::rotate_dir(float)", [&](const volatile uint16_t*, const volatile float* v) {
                ref.shz.rotate_dir(v[0], v[1], v[2], v[3]);
            },
            &angle16, &in[0]);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::mat4x4::rotate_dir(uint16_t)", [&](const volatile uint16_t* a, const volatile float* v) {
                res.shz.rotate_dir(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "glm_rotate", [&](const volatile uint16_t*, const volatile float* v) {
                vec3 axis = { v[1], v[2], v[3] };
                glm_rotate(ref.glm, v[0], axis);
            },
            &angle16, &in[0]));
    }
GBL_TEST_CASE_END

static void glm_rotate_ordered_(mat4 m, const char* order, float a0, float a1, float a2) {
    const float angles[3] = { a0, a1, a2 };
    for(int i = 0; i < 3; ++i) {
        switch(order[i]) {
        case 'x': glm_rotate_x(m, angles[i], m); break;
        case 'y': glm_rotate_y(m, angles[i], m); break;
        case 'z': glm_rotate_z(m, angles[i], m); break;
        }
    }
}

template<typename F>
static bool check_mat4x4_rotation_(const char* order, F&& fn) {
    static constexpr float angles[][3] = {
        { 0.0f,        0.0f,        0.0f        },
        { shz::pi_f_2, 0.0f,        0.0f        },
        { 0.0f,        shz::pi_f_2, 0.0f        },
        { 0.0f,        0.0f,        shz::pi_f_2 },
        { 0.3f,       -1.2f,        2.5f        },
        {-2.9f,        0.7f,       -0.4f        },
        { 1.1f,        1.3f,       -2.2f        },
    };
    static constexpr float start[16] = { 2.0f,  0.5f, -1.0f, 0.0f,
                                         0.25f, 3.0f,  0.75f, 0.0f,
                                        -1.5f,  1.0f,  1.25f, 0.0f,
                                         7.0f,  8.0f,  9.0f,  1.0f };

    for(const auto& a : angles) {
        shz_glm_mat4 res, ref;
        memcpy(res.glm, start, sizeof(start));
        memcpy(ref.glm, start, sizeof(start));

        fn(res.shz, a[0], a[1], a[2]);
        glm_rotate_ordered_(ref.glm, order, a[0], a[1], a[2]);

        for(int c = 0; c < 4; ++c)
            for(int r = 0; r < 4; ++r)
                if(shz::fabsf(res.glm[c][r] - ref.glm[c][r]) > 1e-3f * shz::fmaxf(1.0f, shz::fabsf(ref.glm[c][r])))
                    return false;
    }
    return true;
}

template<typename F>
static bool bench_mat4x4_rotation_(const char* shzName, const char* glmName, const char* order, F&& fn) {
    static shz_glm_mat4 shzMat, glmMat;
    glm_mat4_identity(shzMat.glm);
    glm_mat4_identity(glmMat.glm);
    return (benchmark_cmp<void>)(
        shzName, [&fn](float a0, float a1, float a2) { fn(shzMat.shz, a0, a1, a2); },
        glmName, [order](float a0, float a1, float a2) { glm_rotate_ordered_(glmMat.glm, order, a0, a1, a2); },
        0.3f, -1.2f, 2.5f);
}

GBL_TEST_CASE(rotate_xyz)
    GBL_TEST_VERIFY(check_mat4x4_rotation_("xyz", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_xyz(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_mat4x4_rotation_("xyz", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_xyz(shz::vec3(a0, a1, a2));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_mat4x4_rotation_("xyz", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_xyz(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_xyz(0.3, -1.2f, 2.5)));
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_xyz(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_xyz", "glm_rotate_x/y/z", "xyz",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_xyz(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zyx)
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zyx", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zyx(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zyx", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zyx(shz::vec3(a2, a1, a0));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zyx", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zyx(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_zyx(0.3, -1.2f, 2.5)));
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_zyx(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_zyx", "glm_rotate_z/y/x", "zyx",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_zyx(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zxy)
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zxy", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zxy(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zxy", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zxy(shz::vec3(a1, a2, a0));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zxy", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zxy(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_zxy(0.3, -1.2f, 2.5)));
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_zxy(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_zxy", "glm_rotate_z/x/y", "zxy",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_zxy(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_yxz)
    GBL_TEST_VERIFY(check_mat4x4_rotation_("yxz", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_yxz(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_mat4x4_rotation_("yxz", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_yxz(shz::vec3(a1, a0, a2));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_mat4x4_rotation_("yxz", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_yxz(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_yxz(0.3, -1.2f, 2.5)));
    static_assert(noexcept(std::declval<shz::mat4x4&>().rotate_yxz(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_yxz", "glm_rotate_y/x/z", "yxz",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_yxz(a0, a1, a2); }));
GBL_TEST_CASE_END

static float u16_radians_(uint16_t angle) {
    return static_cast<int16_t>(angle) * (1.0f / SHZ_FSCA_RAD_FACTOR);
}

static uint16_t fsca_angle_(float radians) {
    return static_cast<uint16_t>(static_cast<int32_t>(radians * SHZ_FSCA_RAD_FACTOR));
}

template<typename U, typename R>
static bool check_mat4x4_rotation_u16_(U&& u16, R&& radians) {
    static constexpr uint16_t angles[] = { 0x0000, 0x2000, 0x4000, 0x8000, 0xc000, 0x04d2, 0xfedc, 0xffff };
    static constexpr float start[16] = { 2.0f,  0.5f, -1.0f, 0.0f,
                                         0.25f, 3.0f,  0.75f, 0.0f,
                                        -1.5f,  1.0f,  1.25f, 0.0f,
                                         7.0f,  8.0f,  9.0f,  1.0f };
    constexpr unsigned count = sizeof(angles) / sizeof(angles[0]);

    for(unsigned i = 0; i < count; ++i) {
        const uint16_t a0 = angles[i];
        const uint16_t a1 = angles[(i + 3) % count];
        const uint16_t a2 = angles[(i + 5) % count];
        shz_glm_mat4 expected, actual;

        memcpy(expected.glm, start, sizeof(start));
        memcpy(actual.glm, start, sizeof(start));

        radians(expected.shz, u16_radians_(a0), u16_radians_(a1), u16_radians_(a2));
        u16(actual.shz, a0, a1, a2);

        if(!(actual.shz == expected.shz))
            return false;
    }
    return true;
}

static void glm_rotate_axis_(mat4 m, char axis, float angle) {
    switch(axis) {
    case 'x': glm_rotate_x(m, angle, m); break;
    case 'y': glm_rotate_y(m, angle, m); break;
    case 'z': glm_rotate_z(m, angle, m); break;
    }
}

template<typename F>
static bool bench_mat4x4_rotation_u16_(const char* shzName, const char* glmName, const char* order, bool init, F&& fn) {
    static shz_glm_mat4 shzMat, glmMat;
    glm_mat4_identity(shzMat.glm);
    glm_mat4_identity(glmMat.glm);
    return (benchmark_cmp<void>)(
        shzName, [&fn](uint16_t a0, uint16_t a1, uint16_t a2, float, float, float) { fn(shzMat.shz, a0, a1, a2); },
        glmName, [order, init](uint16_t, uint16_t, uint16_t, float r0, float r1, float r2) {
            if(init)
                glm_mat4_identity(glmMat.glm);
            if(order[1])
                glm_rotate_ordered_(glmMat.glm, order, r0, r1, r2);
            else
                glm_rotate_axis_(glmMat.glm, order[0], r0);
        },
        fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f),
        u16_radians_(fsca_angle_(0.3f)), u16_radians_(fsca_angle_(-1.2f)), u16_radians_(fsca_angle_(2.5f)));
}

#define SHZ_MAT4X4_ROTATION_U16_CASE_1(name, axis, glmName, init)                                                    \
    GBL_TEST_CASE(name##_u16)                                                                                         \
        GBL_TEST_VERIFY(check_mat4x4_rotation_u16_(                                                                  \
            [](shz::mat4x4& m, uint16_t a, uint16_t, uint16_t) { m.name(a); },                                       \
            [](shz::mat4x4& m, float a, float, float) { m.name(a); }));                                              \
        GBL_TEST_VERIFY(bench_mat4x4_rotation_u16_("shz::mat4x4::" #name "(uint16_t)", glmName, axis, init,          \
            [](shz::mat4x4& m, uint16_t a, uint16_t, uint16_t) { m.name(a); }));                                     \
    GBL_TEST_CASE_END

#define SHZ_MAT4X4_ROTATION_U16_CASE_3(name, order, glmName, init)                                                   \
    GBL_TEST_CASE(name##_u16)                                                                                         \
        GBL_TEST_VERIFY(check_mat4x4_rotation_u16_(                                                                  \
            [](shz::mat4x4& m, uint16_t a0, uint16_t a1, uint16_t a2) { m.name(a0, a1, a2); },                       \
            [](shz::mat4x4& m, float a0, float a1, float a2) { m.name(a0, a1, a2); }));                              \
        if(!(init)) {                                                                                                 \
            GBL_TEST_VERIFY(check_mat4x4_rotation_(order, [](shz::mat4x4& m, float a0, float a1, float a2) {         \
                m.name(fsca_angle_(a0), fsca_angle_(a1), fsca_angle_(a2));                                            \
            }));                                                                                                      \
        }                                                                                                             \
        GBL_TEST_VERIFY(bench_mat4x4_rotation_u16_("shz::mat4x4::" #name "(uint16_t)", glmName, order, init,         \
            [](shz::mat4x4& m, uint16_t a0, uint16_t a1, uint16_t a2) { m.name(a0, a1, a2); }));                     \
    GBL_TEST_CASE_END

SHZ_MAT4X4_ROTATION_U16_CASE_1(init_rotation_x,   "x",   "glm_mat4_identity + glm_rotate_x",     true)
SHZ_MAT4X4_ROTATION_U16_CASE_1(init_rotation_y,   "y",   "glm_mat4_identity + glm_rotate_y",     true)
SHZ_MAT4X4_ROTATION_U16_CASE_1(init_rotation_z,   "z",   "glm_mat4_identity + glm_rotate_z",     true)
SHZ_MAT4X4_ROTATION_U16_CASE_3(init_rotation_xyz, "xyz", "glm_mat4_identity + glm_rotate_x/y/z", true)
SHZ_MAT4X4_ROTATION_U16_CASE_3(init_rotation_zyx, "zyx", "glm_mat4_identity + glm_rotate_z/y/x", true)
SHZ_MAT4X4_ROTATION_U16_CASE_3(init_rotation_zxy, "zxy", "glm_mat4_identity + glm_rotate_z/x/y", true)
SHZ_MAT4X4_ROTATION_U16_CASE_3(init_rotation_yxz, "yxz", "glm_mat4_identity + glm_rotate_y/x/z", true)
SHZ_MAT4X4_ROTATION_U16_CASE_1(rotate_x,          "x",   "glm_rotate_x",                         false)
SHZ_MAT4X4_ROTATION_U16_CASE_1(rotate_y,          "y",   "glm_rotate_y",                         false)
SHZ_MAT4X4_ROTATION_U16_CASE_1(rotate_z,          "z",   "glm_rotate_z",                         false)
SHZ_MAT4X4_ROTATION_U16_CASE_3(rotate_xyz,        "xyz", "glm_rotate_x/y/z",                     false)
SHZ_MAT4X4_ROTATION_U16_CASE_3(rotate_zyx,        "zyx", "glm_rotate_z/y/x",                     false)
SHZ_MAT4X4_ROTATION_U16_CASE_3(rotate_zxy,        "zxy", "glm_rotate_z/x/y",                     false)
SHZ_MAT4X4_ROTATION_U16_CASE_3(rotate_yxz,        "yxz", "glm_rotate_y/x/z",                     false)

// Applies a rotation to identity for comparing overloads.
template<typename F>
static shz::mat4x4 rotated_mat4x4_(F&& rotate) {
    shz::mat4x4 mat;
    mat.init_identity();
    rotate(mat);
    return mat;
}

// The rotations with no test case of their own: double arguments must select the radian overload.
GBL_TEST_CASE(init_rotation_x_yz)
    alignas(32) static shz_glm_mat4 res, ref;
    volatile float                  angle = 0.6f;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        res.shz.init_rotation_x_yz(-sinf(theta), cosf(theta));
        glm_mat4_identity(ref.glm);
        glm_rotate_x(ref.glm, theta, ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    (benchmark_cmp<void>)(
        "shz::mat4x4::init_rotation_x_yz", [&](float y, float z) {
            res.shz.init_rotation_x_yz(y, z);
        },
        "C init_rotation_x_yz", [&](float y, float z) {
            glm_mat4_identity(ref.glm);
            ref.glm[1][1] = z;
            ref.glm[1][2] = -y;
            ref.glm[2][1] = y;
            ref.glm[2][2] = z;
        },
        -sinf(0.6f), cosf(0.6f));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_rotation_x_yz", [&](float y, float z, const volatile float*) {
            res.shz.init_rotation_x_yz(y, z);
        },
        "glm_mat4_identity + glm_rotate_x", [&](float, float, const volatile float* r) {
            glm_mat4_identity(ref.glm);
            glm_rotate_x(ref.glm, *r, ref.glm);
        },
        -sinf(0.6f), cosf(0.6f), &angle));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_y_xz)
    alignas(32) static shz_glm_mat4 res, ref;
    volatile float                  angle = 0.6f;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        res.shz.init_rotation_y_xz(sinf(theta), cosf(theta));
        glm_mat4_identity(ref.glm);
        glm_rotate_y(ref.glm, theta, ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    (benchmark_cmp<void>)(
        "shz::mat4x4::init_rotation_y_xz", [&](float x, float z) {
            res.shz.init_rotation_y_xz(x, z);
        },
        "C init_rotation_y_xz", [&](float x, float z) {
            glm_mat4_identity(ref.glm);
            ref.glm[0][0] = z;
            ref.glm[0][2] = -x;
            ref.glm[2][0] = x;
            ref.glm[2][2] = z;
        },
        sinf(0.6f), cosf(0.6f));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_rotation_y_xz", [&](float x, float z, const volatile float*) {
            res.shz.init_rotation_y_xz(x, z);
        },
        "glm_mat4_identity + glm_rotate_y", [&](float, float, const volatile float* r) {
            glm_mat4_identity(ref.glm);
            glm_rotate_y(ref.glm, *r, ref.glm);
        },
        sinf(0.6f), cosf(0.6f), &angle));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_z_xy)
    alignas(32) static shz_glm_mat4 res, ref;
    volatile float                  angle = 0.6f;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        res.shz.init_rotation_z_xy(cosf(theta), sinf(theta));
        glm_mat4_identity(ref.glm);
        glm_rotate_z(ref.glm, theta, ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    (benchmark_cmp<void>)(
        "shz::mat4x4::init_rotation_z_xy", [&](float x, float y) {
            res.shz.init_rotation_z_xy(x, y);
        },
        "C init_rotation_z_xy", [&](float x, float y) {
            glm_mat4_identity(ref.glm);
            ref.glm[0][0] = x;
            ref.glm[0][1] = y;
            ref.glm[1][0] = -y;
            ref.glm[1][1] = x;
        },
        cosf(0.6f), sinf(0.6f));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_rotation_z_xy", [&](float x, float y, const volatile float*) {
            res.shz.init_rotation_z_xy(x, y);
        },
        "glm_mat4_identity + glm_rotate_z", [&](float, float, const volatile float* r) {
            glm_mat4_identity(ref.glm);
            glm_rotate_z(ref.glm, *r, ref.glm);
        },
        cosf(0.6f), sinf(0.6f), &angle));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_x_yz)
    alignas(32) static shz_glm_mat4 res, ref;
    mat4                            rot;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        res = dense_matrix();
        ref = dense_matrix();
        res.shz.rotate_x_yz(-sinf(theta), cosf(theta));
        glm_rotate_x(ref.glm, theta, ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res = dense_matrix();
    ref = dense_matrix();

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::rotate_x_yz", [&](float y, float z) {
            res.shz.rotate_x_yz(y, z);
        },
        "glm_mul_rot", [&](float y, float z) {
            glm_mat4_identity(rot);
            rot[1][1] = z;
            rot[1][2] = -y;
            rot[2][1] = y;
            rot[2][2] = z;
            glm_mul_rot(ref.glm, rot, ref.glm);
        },
        -sinf(0.6f), cosf(0.6f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_y_xz)
    alignas(32) static shz_glm_mat4 res, ref;
    mat4                            rot;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        res = dense_matrix();
        ref = dense_matrix();
        res.shz.rotate_y_xz(sinf(theta), cosf(theta));
        glm_rotate_y(ref.glm, theta, ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res = dense_matrix();
    ref = dense_matrix();

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::rotate_y_xz", [&](float x, float z) {
            res.shz.rotate_y_xz(x, z);
        },
        "glm_mul_rot", [&](float x, float z) {
            glm_mat4_identity(rot);
            rot[0][0] = z;
            rot[0][2] = -x;
            rot[2][0] = x;
            rot[2][2] = z;
            glm_mul_rot(ref.glm, rot, ref.glm);
        },
        sinf(0.6f), cosf(0.6f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_z_xy)
    alignas(32) static shz_glm_mat4 res, ref;
    mat4                            rot;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        res = dense_matrix();
        ref = dense_matrix();
        res.shz.rotate_z_xy(cosf(theta), sinf(theta));
        glm_rotate_z(ref.glm, theta, ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res = dense_matrix();
    ref = dense_matrix();

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::rotate_z_xy", [&](float x, float y) {
            res.shz.rotate_z_xy(x, y);
        },
        "glm_mul_rot", [&](float x, float y) {
            glm_mat4_identity(rot);
            rot[0][0] = x;
            rot[0][1] = y;
            rot[1][0] = -y;
            rot[1][1] = x;
            glm_mul_rot(ref.glm, rot, ref.glm);
        },
        cosf(0.6f), sinf(0.6f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_billboard_x)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 9.0f, 2.0f, 3.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    alignas(32) static shz_glm_mat4 res, ref;

    for(const auto& c : cases) {
        res.shz.init_billboard_x(c[0], c[1]);

        vec2 d = { c[1].y - c[0].y, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        glm_mat4_identity(ref.glm);
        glm_rotate_x(ref.glm, atan2f(-d[0], d[1]), ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res.shz.init_billboard_x(onAxis[0], onAxis[1]);
    glm_mat4_identity(ref.glm);
    GBL_TEST_VERIFY(res.shz == ref.shz);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_billboard_x", [&](const volatile float* v) {
            res.shz.init_billboard_x(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + rotation", [&](const volatile float* v) {
            vec2 d = { v[4] - v[1], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(ref.glm);
            ref.glm[1][1] = d[1];
            ref.glm[1][2] = -d[0];
            ref.glm[2][1] = d[0];
            ref.glm[2][2] = d[1];
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_billboard_y)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 9.0f, 3.0f } };
    alignas(32) static struct {
        shz_glm_mat4   res, ref;
        volatile float in[6];
    } bench = { {}, {}, { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f } };
    auto& [res, ref, in] = bench;

    for(const auto& c : cases) {
        res.shz.init_billboard_y(c[0], c[1]);

        vec2 d = { c[1].x - c[0].x, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        glm_mat4_identity(ref.glm);
        glm_rotate_y(ref.glm, atan2f(d[0], d[1]), ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res.shz.init_billboard_y(onAxis[0], onAxis[1]);
    glm_mat4_identity(ref.glm);
    GBL_TEST_VERIFY(res.shz == ref.shz);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_billboard_y", [&](const volatile float* v) {
            res.shz.init_billboard_y(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + rotation", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(ref.glm);
            ref.glm[0][0] = d[1];
            ref.glm[0][2] = -d[0];
            ref.glm[2][0] = d[0];
            ref.glm[2][2] = d[1];
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_billboard_z)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 2.0f, 9.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    alignas(32) static shz_glm_mat4 res, ref;

    for(const auto& c : cases) {
        res.shz.init_billboard_z(c[0], c[1]);

        vec2 d = { c[1].x - c[0].x, c[1].y - c[0].y };
        glm_vec2_normalize(d);
        glm_mat4_identity(ref.glm);
        glm_rotate_z(ref.glm, atan2f(d[1], d[0]), ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res.shz.init_billboard_z(onAxis[0], onAxis[1]);
    glm_mat4_identity(ref.glm);
    GBL_TEST_VERIFY(res.shz == ref.shz);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::init_billboard_z", [&](const volatile float* v) {
            res.shz.init_billboard_z(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + rotation", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[4] - v[1] };
            glm_vec2_normalize(d);
            glm_mat4_identity(ref.glm);
            ref.glm[0][0] = d[0];
            ref.glm[0][1] = d[1];
            ref.glm[1][0] = -d[1];
            ref.glm[1][1] = d[0];
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(billboard_x)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 9.0f, 2.0f, 3.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    alignas(32) static shz_glm_mat4 res, ref;
    mat4                            rot;

    for(const auto& c : cases) {
        res = dense_matrix();
        ref = dense_matrix();
        res.shz.billboard_x(c[0], c[1]);

        vec2 d = { c[1].y - c[0].y, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        glm_rotate_x(ref.glm, atan2f(-d[0], d[1]), ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res = dense_matrix();
    ref = dense_matrix();
    res.shz.billboard_x(onAxis[0], onAxis[1]);
    GBL_TEST_VERIFY(res.shz == ref.shz);

    res = dense_matrix();
    ref = dense_matrix();

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::billboard_x", [&](const volatile float* v) {
            res.shz.billboard_x(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + glm_mul_rot", [&](const volatile float* v) {
            vec2 d = { v[4] - v[1], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(rot);
            rot[1][1] = d[1];
            rot[1][2] = -d[0];
            rot[2][1] = d[0];
            rot[2][2] = d[1];
            glm_mul_rot(ref.glm, rot, ref.glm);
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(billboard_y)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 9.0f, 3.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    alignas(32) static shz_glm_mat4 res, ref;
    mat4                            rot;

    for(const auto& c : cases) {
        res = dense_matrix();
        ref = dense_matrix();
        res.shz.billboard_y(c[0], c[1]);

        vec2 d = { c[1].x - c[0].x, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        glm_rotate_y(ref.glm, atan2f(d[0], d[1]), ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res = dense_matrix();
    ref = dense_matrix();
    res.shz.billboard_y(onAxis[0], onAxis[1]);
    GBL_TEST_VERIFY(res.shz == ref.shz);

    res = dense_matrix();
    ref = dense_matrix();

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::billboard_y", [&](const volatile float* v) {
            res.shz.billboard_y(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + glm_mul_rot", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(rot);
            rot[0][0] = d[1];
            rot[0][2] = -d[0];
            rot[2][0] = d[0];
            rot[2][2] = d[1];
            glm_mul_rot(ref.glm, rot, ref.glm);
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(billboard_z)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 2.0f, 9.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    alignas(32) static shz_glm_mat4 res, ref;
    mat4                            rot;

    for(const auto& c : cases) {
        res = dense_matrix();
        ref = dense_matrix();
        res.shz.billboard_z(c[0], c[1]);

        vec2 d = { c[1].x - c[0].x, c[1].y - c[0].y };
        glm_vec2_normalize(d);
        glm_rotate_z(ref.glm, atan2f(d[1], d[0]), ref.glm);

        GBL_TEST_VERIFY(res.shz == ref.shz);
    }

    res = dense_matrix();
    ref = dense_matrix();
    res.shz.billboard_z(onAxis[0], onAxis[1]);
    GBL_TEST_VERIFY(res.shz == ref.shz);

    res = dense_matrix();
    ref = dense_matrix();

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::mat4x4::billboard_z", [&](const volatile float* v) {
            res.shz.billboard_z(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + glm_mul_rot", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[4] - v[1] };
            glm_vec2_normalize(d);
            glm_mat4_identity(rot);
            rot[0][0] = d[0];
            rot[0][1] = d[1];
            rot[1][0] = -d[1];
            rot[1][1] = d[0];
            glm_mul_rot(ref.glm, rot, ref.glm);
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotation_doubles)
    constexpr float  x  = 0.3f, y = -1.2f, z = 2.5f;
    constexpr double xd = x, zd = z;

    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_x(xd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_x(x); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_y(xd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_y(x); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_z(xd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_z(x); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_xyz(xd, y, zd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_xyz(x, y, z); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_zyx(xd, y, zd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_zyx(x, y, z); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_zxy(xd, y, zd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_zxy(x, y, z); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_yxz(xd, y, zd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.init_rotation_yxz(x, y, z); }));

    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.rotate_x(xd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.rotate_x(x); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.rotate_y(xd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.rotate_y(x); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([](shz::mat4x4& m) { m.rotate_z(xd); }) ==
                    rotated_mat4x4_([](shz::mat4x4& m) { m.rotate_z(x); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_vec3)
    constexpr float x = 0.3f, y = -1.2f, z = 2.5f;
    const shz::vec3 angles(x, y, z);

    GBL_TEST_VERIFY(rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_xyz(angles); }) ==
                    rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_xyz(x, y, z); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_zyx(angles); }) ==
                    rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_zyx(z, y, x); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_zxy(angles); }) ==
                    rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_zxy(z, x, y); }));
    GBL_TEST_VERIFY(rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_yxz(angles); }) ==
                    rotated_mat4x4_([&](shz::mat4x4& m) { m.init_rotation_yxz(y, x, z); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(permute_wzyx)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4) + alignof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data() + alignof(shz_glm_mat4));

    mat.shz.init_identity();
    mat.shz.permute_wzyx();

    GBL_TEST_VERIFY(mat.shz.transform(shz::vec4(1.0f, 2.0f, 3.0f, 4.0f)) ==
                    shz::vec4(4.0f, 3.0f, 2.0f, 1.0f));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(init_identity,
                  init_zero,
                  init_one,
                  init_fill,
                  init_diagonal,
                  init_scale,
                  copy,
                  swap,
                  determinant,
                  inverse,
                  inverse_rigid,
                  transform_vec3,
                  transform_vec4,
                  transform_vec3_transpose,
                  transform_vec4_transpose,
                  translate_reverse,
                  scale_reverse,
                  set_scale,
                  get_scale,
                  get_rotation,
                  init_rotation_dir,
                  rotate,
                  rotate_dir,
                  rotate_u16,
                  rotate_dir_u16,
                  rotate_xyz,
                  rotate_zyx,
                  rotate_zxy,
                  rotate_yxz,
                  init_rotation_x_u16,
                  init_rotation_y_u16,
                  init_rotation_z_u16,
                  init_rotation_xyz_u16,
                  init_rotation_zyx_u16,
                  init_rotation_zxy_u16,
                  init_rotation_yxz_u16,
                  rotate_x_u16,
                  rotate_y_u16,
                  rotate_z_u16,
                  rotate_xyz_u16,
                  rotate_zyx_u16,
                  rotate_zxy_u16,
                  rotate_yxz_u16,
                  init_rotation_x_yz,
                  init_rotation_y_xz,
                  init_rotation_z_xy,
                  rotate_x_yz,
                  rotate_y_xz,
                  rotate_z_xy,
                  init_billboard_x,
                  init_billboard_y,
                  init_billboard_z,
                  billboard_x,
                  billboard_y,
                  billboard_z,
                  rotation_doubles,
                  init_rotation_vec3,
                  permute_wzyx,
                  blend,
                  to_quat)