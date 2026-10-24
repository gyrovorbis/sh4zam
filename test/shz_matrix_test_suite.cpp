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
    mat.shz.scale_3x3(2.0f, 3.0f, 4.0f);
    mat.shz.add_translation(10.0f, 20.0f, 30.0f);
    mat.shz.rotate_x(shz::pi_f);
    mat.shz.inverse(&inverted);
    inverted.inverse(&invertedInverted);

    GBL_TEST_VERIFY(mat.shz == invertedInverted);
    GBL_TEST_VERIFY(test(mat));
GBL_TEST_CASE_END

GBL_TEST_CASE(inverse_rigid)
    struct rigid { float rx, ry, rz, tx, ty, tz, mirror; };
    constexpr rigid cases[] = {
        {  0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,  1.0f },
        {  0.4f, -0.8f,  1.2f,   3.0f, -2.0f,  5.0f,  1.0f },
        { -2.1f,  1.3f,  0.7f, -10.0f,  0.5f, -7.5f,  1.0f },
        {  0.9f,  0.2f, -1.6f,   4.0f,  6.0f, -1.0f, -1.0f }
    };

    shz_glm_mat4 mat, glmInverted;
    shz::mat4x4  shzInverted, invertedInverted;

    for(const auto& c : cases) {
        mat.shz.init_rotation_xyz(c.rx, c.ry, c.rz);
        mat.shz.scale_3x3(c.mirror, 1.0f, 1.0f);
        mat.shz.add_translation(c.tx, c.ty, c.tz);

        mat.shz.inverse_rigid(&shzInverted);
        glm_mat4_copy(mat.glm, glmInverted.glm);
        glm_inv_tr(glmInverted.glm);

        GBL_TEST_VERIFY(shzInverted == glmInverted.shz);
        GBL_TEST_VERIFY(shzInverted.elem[3]  == 0.0f && shzInverted.elem[7]  == 0.0f &&
                        shzInverted.elem[11] == 0.0f && shzInverted.elem[15] == 1.0f);

        shzInverted.inverse_rigid(&invertedInverted);
        GBL_TEST_VERIFY(mat.shz == invertedInverted);
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)("shz::mat4x4::inverse_rigid",
                              [](const shz::mat4x4& src, shz::mat4x4* dst) {
                                  src.inverse_rigid(dst);
                              },
                              "glm_inv_tr",
                              [](const shz::mat4x4& src, shz::mat4x4* dst) {
                                  glm_mat4_copy(*reinterpret_cast<mat4*>(const_cast<shz::mat4x4*>(&src)),
                                                *reinterpret_cast<mat4*>(dst));
                                  glm_inv_tr(*reinterpret_cast<mat4*>(dst));
                              },
                              mat.shz, &shzInverted)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec3)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4) + alignof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data() + alignof(shz_glm_mat4));

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
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4) + alignof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data() + alignof(shz_glm_mat4));

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
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_xyz", "glm_rotate_x/y/z", "xyz",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_xyz(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zyx)
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zyx", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zyx(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_zyx", "glm_rotate_z/y/x", "zyx",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_zyx(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zxy)
    GBL_TEST_VERIFY(check_mat4x4_rotation_("zxy", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_zxy(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_zxy", "glm_rotate_z/x/y", "zxy",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_zxy(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_yxz)
    GBL_TEST_VERIFY(check_mat4x4_rotation_("yxz", [](shz::mat4x4& m, float a0, float a1, float a2) {
        m.rotate_yxz(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(bench_mat4x4_rotation_("shz::mat4x4::rotate_yxz", "glm_rotate_y/x/z", "yxz",
                                           [](shz::mat4x4& m, float a0, float a1, float a2) { m.rotate_yxz(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(permute_wzyx)
    alignas(32) std::array<uint8_t, sizeof(shz_glm_mat4) + alignof(shz_glm_mat4)> buffer;
    auto& mat = *reinterpret_cast<shz_glm_mat4*>(buffer.data() + alignof(shz_glm_mat4));

    mat.shz.init_identity();
    mat.shz.permute_wzyx();

    GBL_TEST_VERIFY(mat.shz.transform(shz::vec4(1.0f, 2.0f, 3.0f, 4.0f)) ==
                    shz::vec4(4.0f, 3.0f, 2.0f, 1.0f));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(copy,
                  swap,
                  inverse,
                  inverse_rigid,
                  transform_vec3,
                  transform_vec4,
                  translate_reverse,
                  scale_reverse,
                  set_scale,
                  get_scale,
                  init_rotation_dir,
                  rotate,
                  rotate_dir,
                  rotate_xyz,
                  rotate_zyx,
                  rotate_zxy,
                  rotate_yxz,
                  permute_wzyx,
                  blend,
                  to_quat)