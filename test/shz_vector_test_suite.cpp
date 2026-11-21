#include "shz_test_scenario.h"
#include "shz_test.hpp"
#include "sh4zam/shz_vector.hpp"

#include <cglm/call.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>

#include <print>
#include <string>
#include <tuple>

#define GBL_SELF_TYPE shz_vector_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

GBL_TEST_CASE(vec2Construct)
    auto vec1 = shz::vec2();
    GBL_UNUSED(vec1);
    {
        auto vec2 = shz::vec2(0.0f);
        GBL_TEST_VERIFY(vec2.x == 0.0f);
        GBL_TEST_VERIFY(vec2.y == 0.0f);
        GBL_TEST_VERIFY(vec2[0] == 0.0f);
        GBL_TEST_VERIFY(vec2[1] == 0.0f);
    }
    {
        auto vec3 = shz::vec2(1.0f, 2.0f);
        GBL_TEST_VERIFY(vec3.x == 1.0f);
        GBL_TEST_VERIFY(vec3.y == 2.0f);
        GBL_TEST_VERIFY(vec3[0] == 1.0f);
        GBL_TEST_VERIFY(vec3[1] == 2.0f);
    }
    {
        auto vec4 = shz::vec2{ 3.0f, 4.0f };
        GBL_TEST_VERIFY(vec4.x == 3.0f);
        GBL_TEST_VERIFY(vec4.y == 4.0f);
        GBL_TEST_VERIFY(vec4[0] == 3.0f);
        GBL_TEST_VERIFY(vec4[1] == 4.0f);
    }
    {
        auto vec5 = shz::vec2{ 5.0f, 6.0f };
        GBL_TEST_VERIFY(vec5.x == 5.0f);
        GBL_TEST_VERIFY(vec5.y == 6.0f);
        GBL_TEST_VERIFY(vec5[0] == 5.0f);
        GBL_TEST_VERIFY(vec5[1] == 6.0f);
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Set)
    shz::vec2 vec;

    vec.x = 100.0f;
    vec.y = 200.0f;
    GBL_TEST_VERIFY(vec.x == 100.0f);
    GBL_TEST_VERIFY(vec.y == 200.0f);

    vec[0] = -1.0f;
    vec[1] = -2.0f;
    GBL_TEST_VERIFY(vec.x == -1.0f);
    GBL_TEST_VERIFY(vec.y == -2.0f);

    vec = shz_vec2_t{ 3.0f, 4.0f };
    GBL_TEST_VERIFY(vec.x == 3.0f);
    GBL_TEST_VERIFY(vec.y == 4.0f);

    vec = shz::vec2(-5.0f, -6.0f);
    GBL_TEST_VERIFY(vec.x == -5.0f);
    GBL_TEST_VERIFY(vec.y == -6.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Lerp)
    auto vec =
        shz::vec2::lerp({ 0.0f, 0.0f},               // Flex C++ ctor dick
                        { 2.0f, 2.0f }, // Flex C conversion ctor dick
                        0.5f);
    GBL_TEST_VERIFY(vec.x == 1.0f);  // Flex C member access dick
    GBL_TEST_VERIFY(vec[1] == 1.0f); // Flex C++ overloaded subscript dick
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2AddEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec += shz_vec2_t{ 3.0f, 5.0f };
    vec += shz::vec2 { -4.0f, -6.0f };

    GBL_TEST_VERIFY(vec.x == 0.0f);
    GBL_TEST_VERIFY(vec.y == -3.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2SubEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec -= shz_vec2_t { 3.0f, 5.0f };
    vec -= shz::vec2 { -4.0f, -6.0f };

    GBL_TEST_VERIFY(vec.x == 2.0f);
    GBL_TEST_VERIFY(vec.y == -1.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MultEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec *= shz_vec2_t { 3.0f, 5.0f };
    vec *= shz::vec2 { -4.0f, -6.0f };

    GBL_TEST_VERIFY(vec.x == -12.0f);
    GBL_TEST_VERIFY(vec.y == 60.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2DivEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec /= shz_vec2_t { 3.0f, 5.0f };
    vec /= shz::vec2 { -4.0f, -6.0f };

    GBL_TEST_VERIFY(gblFloatEquals(vec.x, -0.0833333333f));
    GBL_TEST_VERIFY(gblFloatEquals(vec.y, 0.06666666666f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MultEqualsScalar)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec *= -4.0f;
    vec *= 2.0f;

    GBL_TEST_VERIFY(vec.x == -8.0f);
    GBL_TEST_VERIFY(vec.y == 16.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2DivEqualsScalar)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec /= -4.0f;
    vec /= 2.0f;

    GBL_TEST_VERIFY(vec.x == -0.125f);
    GBL_TEST_VERIFY(vec.y == 0.25f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Compare)
    auto vec = shz::vec2(1.0f, 4.0f);
    GBL_TEST_VERIFY((vec == shz_vec2_t {1.0f, 4.0f}));
    GBL_TEST_VERIFY(vec <= shz::vec2(2.0f, 5.0f));
    GBL_TEST_VERIFY(vec > shz::vec2(1.0f, 3.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Dot)
    auto base = shz::vec2(2.0f, 3.0f);
    GBL_TEST_VERIFY(base.dot({1.0f, -1.0f}) == -1.0f);
    GBL_TEST_VERIFY(base.dot(shz_vec2_t{-3.0f, 4.0f}) == 6.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Dot2)
   auto test = [&](shz::vec2 vec1,
                   shz::vec2 vec2,
                   shz::vec2 vec3)
   {
        shz::vec2 fipr_res, c_res;

        benchmark(&fipr_res, [&]{ return vec1.dot(vec2, vec3); });

        benchmark(&c_res, [&]{ return shz::vec2 {
                                    vec1.x * vec2.x + vec1.y * vec2.y,
                                    vec1.x * vec3.x + vec1.y * vec3.y
                                };
                           });

        return gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error);
    };

    GBL_TEST_VERIFY(test({ -0.342344f,    890432084.0f },
                         { 0.0000001f,    34342.0324f },
                         { 12343455.0f,   -34234324.4444f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Dot3)
   auto test = [&](shz::vec2 vec1,
                   shz::vec2 vec2,
                   shz::vec2 vec3,
                   shz::vec2 vec4)
   {
        shz::vec3 fipr_res, c_res;

        benchmark(&fipr_res, [&]{ return vec1.dot(vec2, vec3, vec4); });

        benchmark(&c_res, [&]{ return shz::vec3 {
                            vec1.x * vec2.x + vec1.y * vec2.y,
                            vec1.x * vec3.x + vec1.y * vec3.y,
                            vec1.x * vec4.x + vec1.y * vec4.y
                        };
                   });

        return gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.z, c_res.z, shz::fipr_max_error);
    };

    GBL_TEST_VERIFY(test({ -0.342344f,    890432084.0f },
                         { 0.0000001f,    34342.0324f },
                         { 12343455.0f,   -34234324.4444f },
                         { 3243124.3434f, shz::pi_f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Magnitude)
    GBL_TEST_ERROR((shz::vec2{1.0f, 1.0f}.magnitude()), sqrtf(2.0f), 0.0001f, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR((shz::vec2(2.0f, 3.0f).magnitude()), 3.60555127546f, 0.0001f, GBL_TEST_ERROR_FUZZY);

    (benchmark_cmp<float>)("shz::vec2::magnitude",
                           [](const shz::vec2& v) { return v.magnitude(); },
                           "glm_vec2_norm",
                           [](const shz::vec2& v) { return glm_vec2_norm(const_cast<float*>(&v[0])); },
                           shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MagnitudeSqr)
    GBL_TEST_VERIFY((shz::vec2{1.0f, 1.0f}.magnitude_sqr() == 2.0f));
    GBL_TEST_VERIFY((shz::vec2(2.0f, 3.0f).magnitude_sqr() == 13.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MagnitudeInv)
    GBL_TEST_ERROR((shz::vec2{1.0f, 1.0f}.magnitude_inv()), 1.0f / sqrtf(2.0f), 0.0001f, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz::vec2(2.0f, 3.0f).magnitude_inv(), 1.0f / sqrtf(13.0f), 0.0001f, GBL_TEST_ERROR_FUZZY);

    (benchmark_cmp<float>)("shz::vec2::magnitude_inv",
                           [](const shz::vec2& v) { return v.magnitude_inv(); },
                           "1.0f / glm_vec2_norm",
                           [](const shz::vec2& v) { return 1.0f / glm_vec2_norm(const_cast<float*>(&v[0])); },
                           shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Direction)
    GBL_TEST_VERIFY((shz::vec2{0.0f, 2.0f}.direction() == shz::vec2(0.0f, 1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Dot)
    auto base = shz::vec3(2.0f, 3.0f, 4.0f);
    GBL_TEST_VERIFY(base.dot({1.0f, -1.0f, 1.0f}) == 3.0f);
    GBL_TEST_VERIFY(base.dot(shz_vec3_t{-3.0f, 4.0f, -2.0f}) == -2.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Dot2)
   auto test = [&](shz::vec3 vec1_,
                   shz::vec3 vec2_,
                   shz::vec3 vec3_)
   {
        volatile shz::vec3 vec1 = vec1_;
        volatile shz::vec3 vec2 = vec2_;
        volatile shz::vec3 vec3 = vec3_;
        shz::vec3 v1 = vec1;
        shz::vec3 v2 = vec2;
        shz::vec3 v3 = vec3;

        auto scalar = [](const shz::vec3& l,
                         const shz::vec3& r1,
                         const shz::vec3& r2) {
                            return shz::vec2 {
                                l.x * r1.x + l.y * r1.y + l.z * r1.z,
                                l.x * r2.x + l.y * r2.y + l.z * r2.z
                            };
                        };

        shz::vec2 fipr_res = v1.dot(v2, v3);
        shz::vec2 c_res = scalar(v1, v2, v3);

        bool success = gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
                       gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error);

        /*success &=*/ [&](const shz::vec3& l,
                       const shz::vec3& r1,
                       const shz::vec3& r2) SHZ_NO_INLINE {
            return (benchmark_cmp<shz::vec2>)("shz::vec3::dot2",
                                   [](const shz::vec3& l,
                                      const shz::vec3& r1,
                                      const shz::vec3& r2) {
                                       return l.dot(r1, r2);
                                   },
                                   "scalar",
                                   bench_fn(scalar),
                                   l, r1, r2);
        }(v1, v2, v3);
        return success;
    };

    GBL_TEST_VERIFY(test({ -0.342344f,    890432084.0f, 343244.0f },
                         { 0.0000001f,    34342.0324f, -0.3243242f },
                         { 12343455.0f,   -34234324.4444f, 0.034234f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Dot3)
    auto scalar = [](const shz::vec3& l, const shz::vec3& r1, const shz::vec3& r2, const shz::vec3& r3) {
        return shz::vec3 {
            l.x * r1.x + l.y * r1.y + l.z * r1.z,
            l.x * r2.x + l.y * r2.y + l.z * r2.z,
            l.x * r3.x + l.y * r3.y + l.z * r3.z
        };
   };

   auto test = [&](shz::vec3 vec1,
                   shz::vec3 vec2,
                   shz::vec3 vec3,
                   shz::vec3 vec4)
   {
        shz::vec3 fipr_res, c_res;

        fipr_res = vec1.dot(vec2, vec3, vec4);
        c_res    = scalar(vec1, vec2, vec3, vec4);

        return gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.z, c_res.z, shz::fipr_max_error);
    };

    GBL_TEST_VERIFY(test({ -0.342344f,   890432084.0f,   343244.0f  },
                         { 0.0000001f,   34342.0324f,   -0.3243242f },
                         { 12343455.0f, -34234324.4444f, 0.034234f  },
                         { -1213.433f,   0.0020423f,    -32432.324f }));
    {
        auto l  = shz::vec3{ gblRandf(), gblRandf(), gblRandf() };
        auto r1 = shz::vec3{ gblRandf(), gblRandf(), gblRandf() };
        auto r2 = shz::vec3{ gblRandf(), gblRandf(), gblRandf() };
        auto r3 = shz::vec3{ gblRandf(), gblRandf(), gblRandf() };

        (benchmark_cmp<shz::vec3>)("shz::vec3::dot3",
                                   [](const shz::vec3& l,
                                      const shz::vec3& r1,
                                      const shz::vec3& r2,
                                      const shz::vec3& r3) {
                                       return l.dot(r1, r2, r3);
                                   },
                                   "scalar",
                                   bench_fn(scalar),
                                   l, r1, r2, r3);
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Dot)
    auto base = shz::vec4(2.0f, 3.0f, 4.0f, 1.0f);
    GBL_TEST_VERIFY(base.dot({1.0f, -1.0f, 1.0f, 2.0f}) == 5.0f);
    GBL_TEST_VERIFY(base.dot(shz_vec4_t{-3.0f, 4.0f, -2.0f, -3.0f}) == -5.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Dot2)
   auto test = [&](shz::vec4 vec1,
                   shz::vec4 vec2,
                   shz::vec4 vec3)
   {
        shz::vec2 fipr_res, c_res;

        benchmark(&fipr_res, [&]{ return vec1.dot(vec2, vec3); });

        benchmark(&c_res, [&]{ return shz::vec2 {
                                    vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w,
                                    vec1.x * vec3.x + vec1.y * vec3.y + vec1.z * vec3.z + vec1.w * vec3.w
                                };
                           });

        return gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error);
    };
    GBL_TEST_VERIFY(test({ -0.342344f,    890432084.0f, 343244.0f, -343242.0324f },
                         { 0.0000001f,    34342.0324f, -0.3243242f, 0.023432f },
                         { 12343455.0f,   -34234324.4444f, 0.034234f, -3432.0324f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Dot3)
   auto test = [&](shz::vec4 vec1,
                   shz::vec4 vec2,
                   shz::vec4 vec3,
                   shz::vec4 vec4)
   {
        shz::vec3 fipr_res, c_res;

        benchmark(&fipr_res, [&]{ return vec1.dot(vec2, vec3, vec4); });

        benchmark(&c_res, [&]{ return shz::vec3 {
                            vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w,
                            vec1.x * vec3.x + vec1.y * vec3.y + vec1.z * vec3.z + vec1.w * vec3.w,
                            vec1.x * vec4.x + vec1.y * vec4.y + vec1.z * vec4.z + vec1.w * vec4.w
                        };
                   });

        return gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.z, c_res.z, shz::fipr_max_error);
    };

    GBL_TEST_VERIFY(test({ -0.342344f,    890432084.0f, 343244.0f, -312432.04232f },
                         { 0.0000001f,    34342.0324f, -0.3243242f, 0.234324f },
                         { 12343455.0f,   -34234324.4444f, 0.034234f, -0.34324432f },
                         { 3243124.3434f, shz::pi_f, 3143124.0342f, 1.0000043224324f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Rotate)
   auto test = [&](shz::vec2 vec, float radians) {
        shz::vec2 shz_rotated, c_rotated;

        benchmark(&c_rotated, [&]{
            float cosres = cosf(radians);
            float sinres = sinf(radians);

            return shz_vec2_init(vec.x * cosres - vec.y * sinres,
                                 vec.x * sinres + vec.y * cosres);
        });

        benchmark(&shz_rotated, [&]{ return vec.rotate(radians); });

        return gblFloatEquals(shz_rotated.x, c_rotated.x, shz::fipr_max_error) &&
               gblFloatEquals(shz_rotated.y, c_rotated.y, shz::fipr_max_error);
    };

    GBL_TEST_VERIFY(test({1.0f, 2.0f }, shz::pi_f));
GBL_TEST_CASE_END

static bool vec3RotateVerify_(const shz::vec3& vec, float radians, const shz::vec3& axis, const shz::vec3& rotated) {
    shz_glm_vec3 glm_vec, glm_axis;

    glm_vec.shz  = vec;
    glm_axis.shz = axis;
    glmc_vec3_rotate(glm_vec.glm, radians, glm_axis.glm);

    return shz_equalf(rotated.x, glm_vec.shz.x) &&
           shz_equalf(rotated.y, glm_vec.shz.y) &&
           shz_equalf(rotated.z, glm_vec.shz.z);
}

static shz::vec3 vec3RotateRand_() {
    return shz::vec3(gblRandUniform(-100.0f, 100.0f),
                     gblRandUniform(-100.0f, 100.0f),
                     gblRandUniform(-100.0f, 100.0f));
}

GBL_TEST_CASE(vec3RotateX)
    GBL_TEST_VERIFY(shz::vec3(1.0f, 0.0f, 0.0f).rotate_x(shz::pi_f_2) == shz::vec3(1.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec3(1.0f, 2.0f, 3.0f).rotate_x(0.0f)        == shz::vec3(1.0f, 2.0f, 3.0f));

    {
        shz::vec3 r = shz::vec3(0.0f, 1.0f, 0.0f).rotate_x(shz::pi_f_2);
        GBL_TEST_VERIFY(shz_equalf(r.x, 0.0f) && shz_equalf(r.y, 0.0f) && shz_equalf(r.z, 1.0f));

        r = shz::vec3(0.0f, 0.0f, 1.0f).rotate_x(shz::pi_f_2);
        GBL_TEST_VERIFY(shz_equalf(r.x, 0.0f) && shz_equalf(r.y, -1.0f) && shz_equalf(r.z, 0.0f));

        r = shz::vec3(0.0f, 1.0f, 0.0f).rotate_x(shz::pi_f);
        GBL_TEST_VERIFY(shz_equalf(r.x, 0.0f) && shz_equalf(r.y, -1.0f) && shz_equalf(r.z, 0.0f));
    }

    for(int i = 0; i < 32; ++i) {
        const shz::vec3 vec     = vec3RotateRand_();
        const float     radians = gblRandUniform(-shz::tau_f, shz::tau_f);

        GBL_TEST_VERIFY(vec3RotateVerify_(vec, radians, shz::vec3(1.0f, 0.0f, 0.0f), vec.rotate_x(radians)));
    }

    {
        alignas(32) static volatile float out[3];

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)("shz::vec3::rotate_x",
                                  [&](const shz::vec3& v, float r) {
                                      const shz::vec3 o = v.rotate_x(r);
                                      out[0] = o.x; out[1] = o.y; out[2] = o.z;
                                  },
                                  "glm_vec3_rotate",
                                  [&](const shz::vec3& v, float r) {
                                      shz_glm_vec3 u;
                                      vec3         axis = { 1.0f, 0.0f, 0.0f };
                                      u.shz = v;
                                      glm_vec3_rotate(u.glm, r, axis);
                                      out[0] = u.glm[0]; out[1] = u.glm[1]; out[2] = u.glm[2];
                                  },
                                  vec3RotateRand_(), gblRandUniform(-shz::tau_f, shz::tau_f))
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3RotateY)
    GBL_TEST_VERIFY(shz::vec3(0.0f, 1.0f, 0.0f).rotate_y(shz::pi_f_2) == shz::vec3(0.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec3(1.0f, 2.0f, 3.0f).rotate_y(0.0f)        == shz::vec3(1.0f, 2.0f, 3.0f));

    {
        shz::vec3 r = shz::vec3(0.0f, 0.0f, 1.0f).rotate_y(shz::pi_f_2);
        GBL_TEST_VERIFY(shz_equalf(r.x, 1.0f) && shz_equalf(r.y, 0.0f) && shz_equalf(r.z, 0.0f));

        r = shz::vec3(1.0f, 0.0f, 0.0f).rotate_y(shz::pi_f_2);
        GBL_TEST_VERIFY(shz_equalf(r.x, 0.0f) && shz_equalf(r.y, 0.0f) && shz_equalf(r.z, -1.0f));

        r = shz::vec3(1.0f, 0.0f, 0.0f).rotate_y(shz::pi_f);
        GBL_TEST_VERIFY(shz_equalf(r.x, -1.0f) && shz_equalf(r.y, 0.0f) && shz_equalf(r.z, 0.0f));
    }

    for(int i = 0; i < 32; ++i) {
        const shz::vec3 vec     = vec3RotateRand_();
        const float     radians = gblRandUniform(-shz::tau_f, shz::tau_f);

        GBL_TEST_VERIFY(vec3RotateVerify_(vec, radians, shz::vec3(0.0f, 1.0f, 0.0f), vec.rotate_y(radians)));
    }

    {
        alignas(32) static volatile float out[3];

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)("shz::vec3::rotate_y",
                                  [&](const shz::vec3& v, float r) {
                                      const shz::vec3 o = v.rotate_y(r);
                                      out[0] = o.x; out[1] = o.y; out[2] = o.z;
                                  },
                                  "glm_vec3_rotate",
                                  [&](const shz::vec3& v, float r) {
                                      shz_glm_vec3 u;
                                      vec3         axis = { 0.0f, 1.0f, 0.0f };
                                      u.shz = v;
                                      glm_vec3_rotate(u.glm, r, axis);
                                      out[0] = u.glm[0]; out[1] = u.glm[1]; out[2] = u.glm[2];
                                  },
                                  vec3RotateRand_(), gblRandUniform(-shz::tau_f, shz::tau_f))
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3RotateZ)
    GBL_TEST_VERIFY(shz::vec3(0.0f, 0.0f, 1.0f).rotate_z(shz::pi_f_2) == shz::vec3(0.0f, 0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3(1.0f, 2.0f, 3.0f).rotate_z(0.0f)        == shz::vec3(1.0f, 2.0f, 3.0f));

    {
        shz::vec3 r = shz::vec3(1.0f, 0.0f, 0.0f).rotate_z(shz::pi_f_2);
        GBL_TEST_VERIFY(shz_equalf(r.x, 0.0f) && shz_equalf(r.y, 1.0f) && shz_equalf(r.z, 0.0f));

        r = shz::vec3(0.0f, 1.0f, 0.0f).rotate_z(shz::pi_f_2);
        GBL_TEST_VERIFY(shz_equalf(r.x, -1.0f) && shz_equalf(r.y, 0.0f) && shz_equalf(r.z, 0.0f));

        r = shz::vec3(1.0f, 0.0f, 0.0f).rotate_z(shz::pi_f);
        GBL_TEST_VERIFY(shz_equalf(r.x, -1.0f) && shz_equalf(r.y, 0.0f) && shz_equalf(r.z, 0.0f));
    }

    {
        const shz::vec3 vec     = vec3RotateRand_();
        const float     radians = gblRandUniform(-shz::tau_f, shz::tau_f);
        const shz::vec2 xy      = shz::vec2(vec.x, vec.y).rotate(radians);
        const shz::vec3 r       = vec.rotate_z(radians);

        GBL_TEST_VERIFY(r.x == xy.x && r.y == xy.y && r.z == vec.z);
    }

    for(int i = 0; i < 32; ++i) {
        const shz::vec3 vec     = vec3RotateRand_();
        const float     radians = gblRandUniform(-shz::tau_f, shz::tau_f);

        GBL_TEST_VERIFY(vec3RotateVerify_(vec, radians, shz::vec3(0.0f, 0.0f, 1.0f), vec.rotate_z(radians)));
    }

    {
        alignas(32) static volatile float out[3];

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)("shz::vec3::rotate_z",
                                  [&](const shz::vec3& v, float r) {
                                      const shz::vec3 o = v.rotate_z(r);
                                      out[0] = o.x; out[1] = o.y; out[2] = o.z;
                                  },
                                  "glm_vec3_rotate",
                                  [&](const shz::vec3& v, float r) {
                                      shz_glm_vec3 u;
                                      vec3         axis = { 0.0f, 0.0f, 1.0f };
                                      u.shz = v;
                                      glm_vec3_rotate(u.glm, r, axis);
                                      out[0] = u.glm[0]; out[1] = u.glm[1]; out[2] = u.glm[2];
                                  },
                                  vec3RotateRand_(), gblRandUniform(-shz::tau_f, shz::tau_f))
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Orthonormalize)
   auto test = [&](shz::vec3 v1, shz::vec3 v2,
                   shz::vec3 e1, shz::vec3 e2) {
        shz::vec3 out1, out2;

        benchmark(nullptr, [&]{
            std::tie(out1, out2) = shz::vec3::orthonormalize(v1, v2);
        });

        return out1 == e1 && out2 == e2;
    };

    GBL_TEST_VERIFY(test({ 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f },
                         { 0.267261241912424f, 0.534522483824849f, 0.801783725737273f },
                         { 0.87287156094397f,  0.218217890235992f, -0.436435780471985f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Barycenter)
   auto test = [&](shz::vec3 p, shz::vec3 a, shz::vec3 b, shz::vec3 c)
    {
        // Written by yours truly
        shz::vec3 shzResult;
        benchmark(&shzResult, [&] {
            return p.barycenter(a, b, c);
        });

        //warezed straight from raymath. :mink:
        struct Vector3 { float x, y, z;} rayResult;
        benchmark(&rayResult, [&] {
            Vector3 result = { 0 };
            Vector3 v0 = { b.x - a.x, b.y - a.y, b.z - a.z };   // Vector3Subtract(b, a)
            Vector3 v1 = { c.x - a.x, c.y - a.y, c.z - a.z };   // Vector3Subtract(c, a)
            Vector3 v2 = { p.x - a.x, p.y - a.y, p.z - a.z };   // Vector3Subtract(p, a)
            float d00 = (v0.x*v0.x + v0.y*v0.y + v0.z*v0.z);    // Vector3DotProduct(v0, v0)
            float d01 = (v0.x*v1.x + v0.y*v1.y + v0.z*v1.z);    // Vector3DotProduct(v0, v1)
            float d11 = (v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);    // Vector3DotProduct(v1, v1)
            float d20 = (v2.x*v0.x + v2.y*v0.y + v2.z*v0.z);    // Vector3DotProduct(v2, v0)
            float d21 = (v2.x*v1.x + v2.y*v1.y + v2.z*v1.z);    // Vector3DotProduct(v2, v1)
            float denom = d00*d11 - d01*d01;
            result.y = (d11*d20 - d01*d21)/denom;
            result.z = (d00*d21 - d01*d20)/denom;
            result.x = 1.0f - (result.z + result.y);
            return result;
        });

        std::println("<{}, {}, {}> vs <{}, {}, {}>",
                     shzResult.x, shzResult.y, shzResult.z,
                     rayResult.x, rayResult.y, rayResult.z);

        return shzResult == shz::vec3::from(rayResult);
    };

    GBL_TEST_SKIP("SOMETHING IS FRAGILE HERE!");
    GBL_TEST_VERIFY(test({ 1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f },
                         { 0.0f, 0.0f, 0.0f },
                         { 1.0f, 0.0f, 0.0f },
                         { 0.0f, 1.0f, 0.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Move)
   auto test = [&](shz::vec2 v, shz::vec2 target, float maxDistance)
    {
        // Written by yours truly
        shz::vec2 shzResult;
        (benchmark)(&shzResult, "shz::vec2::move()", [&] {
            return v.move(target, maxDistance);
        });

        //warezed straight from raymath. :mink:
        struct Vector2 { float x, y; } rayResult;
        (benchmark)(&rayResult, "Vec2MoveTowards()", [&] {
            Vector2 result = { 0 };

            float dx = target.x - v.x;
            float dy = target.y - v.y;
            float value = (dx*dx) + (dy*dy);

            if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance)))
                return Vector2 { target.x, target.y };

            float dist = sqrtf(value);

            result.x = v.x + dx/dist*maxDistance;
            result.y = v.y + dy/dist*maxDistance;

            return result;
        });

        return shzResult == shz::vec2::from(rayResult);
    };

    GBL_TEST_VERIFY(test({ 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Move)
   auto test = [&](shz::vec3 v, shz::vec3 target, float maxDistance)
    {
        // Written by yours truly
        shz::vec3 shzResult;
        (benchmark)(&shzResult, "shz::vec3::move()", [&] {
            return v.move(target, maxDistance);
        });

        //warezed straight from raymath. :mink:
        struct Vector3 { float x, y, z; } rayResult;
        (benchmark)(&rayResult, "Vec3MoveTowards()", [&] {
            Vector3 result = { 0 };

            float dx = target.x - v.x;
            float dy = target.y - v.y;
            float dz = target.z - v.z;
            float value = (dx*dx) + (dy*dy) + (dz*dz);

            if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance)))
                return target.to<Vector3>();

            float dist = sqrtf(value);

            result.x = v.x + dx/dist*maxDistance;
            result.y = v.y + dy/dist*maxDistance;
            result.z = v.z + dz/dist*maxDistance;

            return result;
        });

        return shzResult == shz::vec3::from(rayResult);
    };

    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Move)
   auto test = [&](shz::vec4 v, shz::vec4 target, float maxDistance)
    {
        // Written by yours truly
        shz::vec4 shzResult;
        (benchmark)(&shzResult, "shz::vec4::move()", [&] {
            return v.move(target, maxDistance);
        });

        //warezed straight from raymath. :mink:
        struct Vector4 { float x, y, z, w; } rayResult;
        (benchmark)(&rayResult, "Vec4MoveTowards()", [&] {
            Vector4 result = { 0 };

            float dx = target.x - v.x;
            float dy = target.y - v.y;
            float dz = target.z - v.z;
            float dw = target.w - v.w;
            float value = (dx*dx) + (dy*dy) + (dz*dz) + (dw*dw);

            if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance)))
                return target.to<Vector4>();

            float dist = sqrtf(value);

            result.x = v.x + dx/dist*maxDistance;
            result.y = v.y + dy/dist*maxDistance;
            result.z = v.z + dz/dist*maxDistance;
            result.w = v.w + dw/dist*maxDistance;

            return result;
        });

        return shzResult == shz::vec4::from(rayResult);
    };

    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3CubicHermite)
   auto test = [&](shz::vec3 v1, shz::vec3 tangent1,
                   shz::vec3 v2, shz::vec3 tangent2, float amount)
    {
        //warezed straight from raymath. :mink:
        struct Vector3 { float x, y, z; } rayResult;
        (benchmark)(&rayResult, "Vec3CubicHermite()", [&] SHZ_NO_INLINE {
            Vector3 result = { 0 };

            float amountPow2 = amount*amount;
            float amountPow3 = amount*amount*amount;

            result.x = (2*amountPow3 - 3*amountPow2 + 1)*v1.x + (amountPow3 - 2*amountPow2 + amount)*tangent1.x + (-2*amountPow3 + 3*amountPow2)*v2.x + (amountPow3 - amountPow2)*tangent2.x;
            result.y = (2*amountPow3 - 3*amountPow2 + 1)*v1.y + (amountPow3 - 2*amountPow2 + amount)*tangent1.y + (-2*amountPow3 + 3*amountPow2)*v2.y + (amountPow3 - amountPow2)*tangent2.y;
            result.z = (2*amountPow3 - 3*amountPow2 + 1)*v1.z + (amountPow3 - 2*amountPow2 + amount)*tangent1.z + (-2*amountPow3 + 3*amountPow2)*v2.z + (amountPow3 - amountPow2)*tangent2.z;

            return result;
        });

        // Written by yours truly
        shz::vec3 shzResult;
        (benchmark)(&shzResult, "shz::vec3::cubic_hermite()", [&] SHZ_NO_INLINE {
            return shz::vec3::cubic_hermite(v1, tangent1, v2, tangent2, amount);
        });
#if 0
        std::println("<{}, {}, {}> vs <{}, {}, {}>",
                     rayResult.x, rayResult.y, rayResult.z,
                     shzResult.x, shzResult.y, shzResult.z);
#endif
        return shzResult == shz::vec3::from(rayResult);
    };

    shz::vec3 p0 = {0.0f, 0.0f, 0.0f}, m0 = {1.0f, 0.0f, 0.0f};
    shz::vec3 p1 = {1.0f, 1.0f, 1.0f}, m1 = {1.0f, 0.0f, 0.0f};

    GBL_TEST_VERIFY(test(m0, p0, m1, p1, 0.3f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Step)
    // Edge == 0, scalar version
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(-1.0f, 0.0f), 0.0f) == shz::vec2(0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(0.0f, 1.0f), 0.0f) == shz::vec2(1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(-0.001f, 0.001f), 0.0f) == shz::vec2(0.0f, 1.0f));

    // Edge == 1, scalar version
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(0.0f, 1.0f), 1.0f) == shz::vec2(0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(1.0f, 2.0f), 1.0f) == shz::vec2(1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(0.999f,1.001f), 1.0f) == shz::vec2(0.0f, 1.0f));

    // Edges for vector version, per component
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(-1.0f, 1.0f), shz::vec2(0.0f, 0.0f)) == shz::vec2(0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(0.0f, 0.0f), shz::vec2(0.0f, 1.0f)) == shz::vec2(1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(0.0f, 0.0f), shz::vec2(1.0f, 0.0f)) == shz::vec2(0.0f, 1.0f));

    // Vector with mixed signs
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(2.0f, -2.0f), shz::vec2(1.0f, 1.0f)) == shz::vec2(1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(-2.0f, 2.0f), shz::vec2(-2.0f, 2.0f)) == shz::vec2(1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(-3.0f, 3.0f), shz::vec2(-2.0f, 2.0f)) == shz::vec2(0.0f, 1.0f));

    // Vector verison with fractional values
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(0.49f, 0.50f), shz::vec2(0.5f, 0.5f)) == shz::vec2(0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(0.50f, 0.51f), shz::vec2(0.5f, 0.5f)) == shz::vec2(1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::step(shz::vec2(1.5f, 0.5f), shz::vec2(0.5f, 1.5f)) == shz::vec2(1.0f, 0.0f));

GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Step)
    // Edge == 0, scalar version
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(-1.0f, 0.0f, 1.0f), 0.0f) == shz::vec3(0.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(0.0f, 1.0f, -1.0f), 0.0f) == shz::vec3(1.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(-0.001f, 0.001f, 0.0f), 0.0f) == shz::vec3(0.0f, 1.0f, 1.0f));

    // Edge == 1, scalar version
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(0.0f, 1.0f, 2.0f), 1.0f) == shz::vec3(0.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(1.0f, 2.0f, 0.0f), 1.0f) == shz::vec3(1.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(0.999f, 1.001f, 1.0f), 1.0f) == shz::vec3(0.0f, 1.0f, 1.0f));

    // Edges for vector version, per component
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(-1.0f, 1.0f, 0.0f), shz::vec3(0.0f, 0.0f, 0.0f)) == shz::vec3(0.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(0.0f, 0.0f, 1.0f), shz::vec3(0.0f, 1.0f, 0.0f)) == shz::vec3(1.0f, 0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(0.0f, 0.0f, 0.0f), shz::vec3(1.0f, 0.0f, 1.0f)) == shz::vec3(0.0f, 1.0f, 0.0f));

    // Vector with mixed signs
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(2.0f, -2.0f, 1.0f), shz::vec3(1.0f, 1.0f, 1.0f)) == shz::vec3(1.0f, 0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(-2.0f, 2.0f, 0.0f), shz::vec3(-2.0f, 2.0f, 0.0f)) == shz::vec3(1.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(-3.0f, 3.0f, -1.0f), shz::vec3(-2.0f, 2.0f, 0.0f)) == shz::vec3(0.0f, 1.0f, 0.0f));

    // Vector version with fractional values
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(0.49f, 0.50f, 0.51f), shz::vec3(0.5f, 0.5f, 0.5f)) == shz::vec3(0.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(0.50f, 0.51f, 0.49f), shz::vec3(0.5f, 0.5f, 0.5f)) == shz::vec3(1.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec3::step(shz::vec3(1.5f, 0.5f, 0.5f), shz::vec3(0.5f, 1.5f, 0.5f)) == shz::vec3(1.0f, 0.0f, 1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Step)
    // Edge == 0, scalar version
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(-1.0f, 0.0f, 1.0f, -0.001f), 0.0f) == shz::vec4(0.0f, 1.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(0.0f, 1.0f, -1.0f, 0.001f), 0.0f) == shz::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(-0.001f, 0.001f, 0.0f, 0.0f), 0.0f) == shz::vec4(0.0f, 1.0f, 1.0f, 1.0f));

    // Edge == 1, scalar version
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(0.0f, 1.0f, 2.0f, 0.999f), 1.0f) == shz::vec4(0.0f, 1.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(1.0f, 2.0f, 0.0f, 1.001f), 1.0f) == shz::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(0.999f, 1.001f, 1.0f, 2.0f), 1.0f) == shz::vec4(0.0f, 1.0f, 1.0f, 1.0f));

    // Edges for vector version, per component
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(-1.0f, 1.0f, 0.0f, 2.0f), shz::vec4(0.0f, 0.0f, 0.0f, 0.0f)) == shz::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4( 0.0f, 0.0f, 1.0f, 1.0f), shz::vec4(0.0f, 1.0f, 0.0f, 1.0f)) == shz::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4( 0.0f, 0.0f, 0.0f, 0.0f), shz::vec4(1.0f, 0.0f, 1.0f, 0.0f)) == shz::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    // Vector with mixed signs
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(2.0f, -2.0f, 1.0f, -1.0f), shz::vec4(1.0f, 1.0f, 1.0f, 1.0f)) == shz::vec4(1.0f, 0.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(-2.0f, 2.0f, 0.0f, -1.0f), shz::vec4(-2.0f, 2.0f, 0.0f, -1.0f)) == shz::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(-3.0f, 3.0f, -1.0f, 0.0f), shz::vec4(-2.0f, 2.0f, 0.0f, -1.0f)) == shz::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    // Vector version with fractional values
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(0.49f, 0.50f, 0.51f, 0.50f), shz::vec4(0.5f, 0.5f, 0.5f, 0.5f)) == shz::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(0.50f, 0.51f, 0.49f, 0.49f), shz::vec4(0.5f, 0.5f, 0.5f, 0.5f)) == shz::vec4(1.0f, 1.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec4::step(shz::vec4(1.5f, 0.5f, 0.5f, 1.5f), shz::vec4(0.5f, 1.5f, 0.5f, 1.5f)) == shz::vec4(1.0f, 0.0f, 1.0f, 1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Smoothstep)
    // Edge == (0,1), scalar-edge overload, per-component clamping
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(-1.0f, 0.0f), 0.0f, 1.0f) == shz::vec2(0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(1.0f, 2.0f), 0.0f, 1.0f) == shz::vec2(1.0f, 1.0f));

    // Edge == (0,1), exact values
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.5f, 0.25f), 0.0f, 1.0f) == shz::vec2(0.5f, 0.15625f));   // 1/2, 5/32
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.75f, 0.5f), 0.0f, 1.0f)  == shz::vec2(0.84375f, 0.5f));    // 27/32, 1/2

    // Reversed edges (undefined in spec)
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(-1.0f, 0.0f), 1.0f, 0.0f) == shz::vec2(1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(1.0f, 2.0f),  1.0f, 0.0f) == shz::vec2(0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.25f, 0.75f), 1.0f, 0.0f) == shz::vec2(0.84375f, 0.15625f)); // 27/32, 5/32
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.5f, 0.5f), 1.0f, 0.0f)  == shz::vec2(0.5f, 0.5f));

    // Different edges, test edges and midpoint (2..6 => midpoint at 4 gives 0.5)
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(2.0f, 4.0f), 2.0f, 6.0f) == shz::vec2(0.0f, 0.5f));
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(6.0f, 8.0f), 2.0f, 6.0f) == shz::vec2(1.0f, 1.0f));

    // Ensure monotonic per component (forward edges)
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.25f, 0.25f), 0.0f, 1.0f).x < shz::vec2::smoothstep_safe(shz::vec2(0.5f, 0.5f), 0.0f, 1.0f).x);
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.5f, 0.5f), 0.0f, 1.0f).y  < shz::vec2::smoothstep_safe(shz::vec2(0.75f, 0.75f), 0.0f, 1.0f).y);

    // Mirrored monotonic per component (reversed edges)
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.25f, 0.25f), 1.0f, 0.0f).x > shz::vec2::smoothstep_safe(shz::vec2(0.5f, 0.5f), 1.0f, 0.0f).x);
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(0.5f, 0.5f), 1.0f, 0.0f).y  > shz::vec2::smoothstep_safe(shz::vec2(0.75f, 0.75f), 1.0f, 0.0f).y);

    // Per component test
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(shz::vec2(1.5f, 0.5f), 
                                               shz::vec2(1.0f, 0.0f), 
                                               shz::vec2(2.0f, 1.0f)) == shz::vec2(0.5f, 0.5f));

    // Conversion case, sideffect of vector constructor
    GBL_TEST_VERIFY(shz::vec2::smoothstep_safe(0.5f, 0.0f, 1.0f) == shz::vec2(0.5f, 0.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Smoothstep)
    // Edge == (0,1), scalar-edge overload, per-component clamping
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(-1.0f, 0.0f, 2.0f), 0.0f, 1.0f) == shz::vec3(0.0f, 0.0f, 1.0f));

    // Edge == (0,1), exact values
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(0.25f, 0.5f, 0.75f), 0.0f, 1.0f) == shz::vec3(0.15625f, 0.5f, 0.84375f)); // 5/32, 1/2, 27/32

    // Reversed edges (undefined in spec)
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(-1.0f, 0.0f, 2.0f), 1.0f, 0.0f) == shz::vec3(1.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(0.25f, 0.5f, 0.75f), 1.0f, 0.0f) == shz::vec3(0.84375f, 0.5f, 0.15625f)); // 27/32, 1/2, 5/32

    // Different edges, test edges and midpoint (2..6 => midpoint at 4 gives 0.5)
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(2.0f, 4.0f, 6.0f), 2.0f, 6.0f) == shz::vec3(0.0f, 0.5f, 1.0f));

    // Ensure monotonic per component (forward edges)
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(0.25f, 0.25f, 0.25f), 0.0f, 1.0f).x < shz::vec3::smoothstep_safe(shz::vec3(0.5f, 0.5f, 0.5f), 0.0f, 1.0f).x);
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(0.5f, 0.5f, 0.5f), 0.0f, 1.0f).y  < shz::vec3::smoothstep_safe(shz::vec3(0.75f, 0.75f, 0.75f), 0.0f, 1.0f).y);

    // Mirrored monotonic per component (reversed edges)
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(0.25f, 0.25f, 0.25f), 1.0f, 0.0f).x > shz::vec3::smoothstep_safe(shz::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.0f).x);
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(0.5f, 0.5f, 0.5f), 1.0f, 0.0f).z  > shz::vec3::smoothstep_safe(shz::vec3(0.75f, 0.75f, 0.75f), 1.0f, 0.0f).z);

    // Per component test
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(shz::vec3(1.5f, 0.5f, 3.0f),
                                               shz::vec3(1.0f, 0.0f, 2.0f),
                                               shz::vec3(2.0f, 1.0f, 4.0f)) == shz::vec3(0.5f, 0.5f, 0.5f));

    // Conversion case, sideffect of vector constructor
    GBL_TEST_VERIFY(shz::vec3::smoothstep_safe(0.5f, 0.0f, 1.0f) == shz::vec3(0.5f, 0.5f, 0.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Smoothstep)
    // Edge == (0,1), scalar-edge overload, per-component clamping
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(-1.0f, 0.0f, 1.0f, 2.0f), 0.0f, 1.0f) == shz::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Edge == (0,1), exact values
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(0.25f, 0.5f, 0.75f, 0.0f), 0.0f, 1.0f) == shz::vec4(0.15625f, 0.5f, 0.84375f, 0.0f)); // 5/32, 1/2, 27/32, 0

    // Reversed edges (undefined in spec)
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(-1.0f, 0.0f, 1.0f, 2.0f), 1.0f, 0.0f) == shz::vec4(1.0f, 1.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(0.25f, 0.5f, 0.75f, 0.5f), 1.0f, 0.0f) == shz::vec4(0.84375f, 0.5f, 0.15625f, 0.5f)); // 27/32, 1/2, 5/32, 1/2

    // Different edges, test edges and midpoint (2..6 => midpoint at 4 gives 0.5)
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(2.0f, 4.0f, 6.0f, 8.0f), 2.0f, 6.0f) == shz::vec4(0.0f, 0.5f, 1.0f, 1.0f));

    // Ensure monotonic per component (forward edges)
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(0.25f, 0.25f, 0.25f, 0.25f), 0.0f, 1.0f).x < shz::vec4::smoothstep_safe(shz::vec4(0.5f, 0.5f, 0.5f, 0.5f), 0.0f, 1.0f).x);
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(0.5f, 0.5f, 0.5f, 0.5f), 0.0f, 1.0f).w  < shz::vec4::smoothstep_safe(shz::vec4(0.75f, 0.75f, 0.75f, 0.75f), 0.0f, 1.0f).w);

    // Mirrored monotonic per component (reversed edges)
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(0.25f, 0.25f, 0.25f, 0.25f), 1.0f, 0.0f).x > shz::vec4::smoothstep_safe(shz::vec4(0.5f, 0.5f, 0.5f, 0.5f), 1.0f, 0.0f).x);
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(0.5f, 0.5f, 0.5f, 0.5f), 1.0f, 0.0f).w  > shz::vec4::smoothstep_safe(shz::vec4(0.75f, 0.75f, 0.75f, 0.75f), 1.0f, 0.0f).w);

    // Per component test
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(shz::vec4(1.5f, 0.5f, 3.0f, 0.25f),
                                               shz::vec4(1.0f, 0.0f, 2.0f, 0.0f),
                                               shz::vec4(2.0f, 1.0f, 4.0f, 0.5f)) == shz::vec4(0.5f, 0.5f, 0.5f, 0.5f));

    // Conversion case, sideffect of vector constructor
    GBL_TEST_VERIFY(shz::vec4::smoothstep_safe(0.5f, 0.0f, 1.0f) == shz::vec4(0.5f, 0.5f, 0.5f, 0.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4InvSqrtf)
   auto test = [&](auto value) {
        shz::vec4 shzv = shz::vec4(value).inv_sqrtf();
        float ceev = 1.0f / sqrtf(fabsf(value));
#if 0
        std::println("{} vs {}", shzv, ceev);
#endif
        if(isnan(shzv.x) && isnan(ceev)) return true;
        return shz::equalf(shzv.x, ceev) 
                && shz::equalf(shzv.y, ceev)
                && shz::equalf(shzv.z, ceev)
                && shz::equalf(shzv.w, ceev);
   };
   GBL_TEST_VERIFY(test(333333.33f));
   GBL_TEST_VERIFY(test(1.001f));
   GBL_TEST_VERIFY(test((volatile float)1.0f));
   //GBL_TEST_VERIFY(test(0.55f));
   //GBL_TEST_VERIFY(test(0.01f));
   //GBL_TEST_VERIFY(test(-0.001f));
   //GBL_TEST_VERIFY(test(-0.55f));
#if !defined(__FAST_MATH__) && (!defined(__FINITE_MATH_ONLY__) || (__FINITE_MATH_ONLY__ == 0))
   GBL_TEST_VERIFY(test(-1.0f));
   GBL_TEST_VERIFY(test(-1.001f));
   GBL_TEST_VERIFY(test((volatile float)-33333.33f));
#endif
GBL_TEST_CASE_END

template<typename V>
static constexpr std::size_t vec_dim_ = sizeof(V) / sizeof(float);

static bool vec_close_(float a, float b) {
    return std::fabs(a - b) <= 1e-4f * std::fmax(1.0f, std::fabs(b));
}

template<typename V>
static bool vec_close_(const V& a, const V& b) {
    for(std::size_t i = 0; i < vec_dim_<V>; ++i)
        if(!vec_close_(a[i], b[i]))
            return false;
    return true;
}

template<typename V>
static bool vec_magnitude_safety_(const V& v) {
    const V     zero(0.0f);
    const float ref = std::sqrt(v.magnitude_sqr());

    return zero.magnitude()     == 0.0f      &&
           zero.magnitude_inv() == 0.0f      &&
           vec_close_(v.magnitude(),           ref)        &&
           vec_close_(v.magnitude_fsrra(),     ref)        &&
           vec_close_(v.magnitude_inv(),       1.0f / ref) &&
           vec_close_(v.magnitude_inv_fsrra(), 1.0f / ref);
}

template<typename V>
static bool vec_subscript_() {
    V v(0.0f);

    for(std::size_t i = 0; i < vec_dim_<V>; ++i)
        v[i] = static_cast<float>(i + 1) * 1.5f;

    const V& cv = v;
    float    sum = 0.0f;
    for(float e : cv)
        sum += e;

    bool ok = (cv.end() - cv.begin()) == static_cast<std::ptrdiff_t>(vec_dim_<V>);
    for(std::size_t i = 0; i < vec_dim_<V>; ++i) {
        ok = ok && cv[i] == static_cast<float>(i + 1) * 1.5f;
        ok = ok && &cv[i] == cv.begin() + i;
        ok = ok && reinterpret_cast<const float*>(&cv)[i] == cv[i];
    }

    for(float& e : v)
        e = -e;
    for(std::size_t i = 0; i < vec_dim_<V>; ++i)
        ok = ok && v[i] == static_cast<float>(i + 1) * -1.5f;

    return ok && sum == 1.5f * static_cast<float>(vec_dim_<V> * (vec_dim_<V> + 1) / 2);
}

GBL_TEST_CASE(vecSubscript)
    GBL_TEST_VERIFY(vec_subscript_<shz::vec2>());
    GBL_TEST_VERIFY(vec_subscript_<shz::vec3>());
    GBL_TEST_VERIFY(vec_subscript_<shz::vec4>());
GBL_TEST_CASE_END

template<typename V>
static bool vec_normalize_or_(const V& v, const V& alt, const V& safe_alt) {
    const V zero(0.0f);
    const V ref = v / std::sqrt(v.magnitude_sqr());

    V inPlaceOr   = zero;
    V inPlaceSafe = zero;
    inPlaceOr.normalize_or(alt);
    inPlaceSafe.normalize_safe();

    V nonZeroOr   = v;
    V nonZeroSafe = v;
    nonZeroOr.normalize_or(alt);
    nonZeroSafe.normalize_safe();

    return zero.normalized_or(alt) == alt      &&
           zero.direction_or(alt)  == alt      &&
           zero.normalized_safe()  == safe_alt &&
           zero.direction_safe()   == safe_alt &&
           inPlaceOr               == alt      &&
           inPlaceSafe             == safe_alt &&
           vec_close_(v.normalized(),         ref) &&
           vec_close_(v.normalized_or(alt),   ref) &&
           vec_close_(v.direction_or(alt),    ref) &&
           vec_close_(v.normalized_safe(),    ref) &&
           vec_close_(nonZeroOr,              ref) &&
           vec_close_(nonZeroSafe,            ref);
}

GBL_TEST_CASE(vecMagnitudeSafety)
    GBL_TEST_VERIFY(vec_magnitude_safety_(shz::vec2(3.0f, -4.0f)));
    GBL_TEST_VERIFY(vec_magnitude_safety_(shz::vec2(0.001f, 0.0f)));
    GBL_TEST_VERIFY(vec_magnitude_safety_(shz::vec3(2.0f, -3.0f, 6.0f)));
    GBL_TEST_VERIFY(vec_magnitude_safety_(shz::vec3(0.0f, 0.0f, 1234.5f)));
    GBL_TEST_VERIFY(vec_magnitude_safety_(shz::vec4(1.0f, -2.0f, 3.0f, -4.0f)));
    GBL_TEST_VERIFY(vec_magnitude_safety_(shz::vec4(0.0f, 0.5f, 0.0f, 0.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vecNormalizeOr)
    GBL_TEST_VERIFY(vec_normalize_or_(shz::vec2(3.0f, -4.0f),
                                      shz::vec2(-1.0f, 0.0f),
                                      shz::vec2(0.0f)));
    GBL_TEST_VERIFY(vec_normalize_or_(shz::vec3(2.0f, -3.0f, 6.0f),
                                      shz::vec3(1.0f, 0.0f, 0.0f),
                                      shz::vec3(0.0f)));
    GBL_TEST_VERIFY(vec_normalize_or_(shz::vec4(1.0f, -2.0f, 3.0f, -4.0f),
                                      shz::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                                      shz::vec4(0.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MagnitudeFsrra)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec2(3.0f, 4.0f).magnitude_fsrra(), 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec2(3.0f, comp).magnitude_fsrra(), 5.0f));

    (benchmark_cmp<float>)("shz::vec2::magnitude_fsrra",
                           [](const shz::vec2& v) { return v.magnitude_fsrra(); },
                           "glm_vec2_norm",
                           [](const shz::vec2& v) { return glm_vec2_norm(const_cast<float*>(&v[0])); },
                           shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MagnitudeInvFsrra)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec2(3.0f, 4.0f).magnitude_inv_fsrra(), 1.0f / 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec2(3.0f, comp).magnitude_inv_fsrra(), 1.0f / 5.0f));

    (benchmark_cmp<float>)("shz::vec2::magnitude_inv_fsrra",
                           [](const shz::vec2& v) { return v.magnitude_inv_fsrra(); },
                           "1.0f / glm_vec2_norm",
                           [](const shz::vec2& v) { return 1.0f / glm_vec2_norm(const_cast<float*>(&v[0])); },
                           shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Normalized)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::vec2(3.0f, 4.0f).normalized() == shz::vec2(0.6f, 0.8f));
    GBL_TEST_VERIFY(shz::vec2(3.0f, comp).normalized() == shz::vec2(0.6f, 0.8f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::normalized",
                               [](const shz::vec2& v) { return v.normalized(); },
                               "glm_vec2_normalize_to",
                               [](const shz::vec2& v) {
                                   shz::vec2 out;
                                   glm_vec2_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2NormalizedOr)
    volatile float comp = 4.0f;
    volatile float zero = 0.0f;

    GBL_TEST_VERIFY(shz::vec2(3.0f, 4.0f).normalized_or(shz::vec2(1.0f, 0.0f)) == shz::vec2(0.6f, 0.8f));
    GBL_TEST_VERIFY(shz::vec2(3.0f, comp).normalized_or(shz::vec2(1.0f, 0.0f)) == shz::vec2(0.6f, 0.8f));
    GBL_TEST_VERIFY(shz::vec2(0.0f).normalized_or(shz::vec2(1.0f, 0.0f)) == shz::vec2(1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec2(zero).normalized_or(shz::vec2(1.0f, 0.0f)) == shz::vec2(1.0f, 0.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::normalized_or",
                               [](const shz::vec2& v) { return v.normalized_or(shz::vec2(0.0f)); },
                               "glm_vec2_normalize_to",
                               [](const shz::vec2& v) {
                                   shz::vec2 out;
                                   glm_vec2_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2NormalizedSafe)
    volatile float comp = 4.0f;
    volatile float zero = 0.0f;

    GBL_TEST_VERIFY(shz::vec2(3.0f, 4.0f).normalized_safe() == shz::vec2(0.6f, 0.8f));
    GBL_TEST_VERIFY(shz::vec2(3.0f, comp).normalized_safe() == shz::vec2(0.6f, 0.8f));
    GBL_TEST_VERIFY(shz::vec2(0.0f).normalized_safe() == shz::vec2(0.0f));
    GBL_TEST_VERIFY(shz::vec2(zero).normalized_safe() == shz::vec2(0.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::normalized_safe",
                               [](const shz::vec2& v) { return v.normalized_safe(); },
                               "glm_vec2_normalize_to",
                               [](const shz::vec2& v) {
                                   shz::vec2 out;
                                   glm_vec2_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Magnitude)
    volatile float comp = 6.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, 6.0f).magnitude(), 7.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, comp).magnitude(), 7.0f));

    (benchmark_cmp<float>)("shz::vec3::magnitude",
                           [](const shz::vec3& v) { return v.magnitude(); },
                           "glm_vec3_norm",
                           [](const shz::vec3& v) { return glm_vec3_norm(const_cast<float*>(&v[0])); },
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3MagnitudeFsrra)
    volatile float comp = 6.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, 6.0f).magnitude_fsrra(), 7.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, comp).magnitude_fsrra(), 7.0f));

    (benchmark_cmp<float>)("shz::vec3::magnitude_fsrra",
                           [](const shz::vec3& v) { return v.magnitude_fsrra(); },
                           "glm_vec3_norm",
                           [](const shz::vec3& v) { return glm_vec3_norm(const_cast<float*>(&v[0])); },
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3MagnitudeInv)
    volatile float comp = 6.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, 6.0f).magnitude_inv(), 1.0f / 7.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, comp).magnitude_inv(), 1.0f / 7.0f));

    (benchmark_cmp<float>)("shz::vec3::magnitude_inv",
                           [](const shz::vec3& v) { return v.magnitude_inv(); },
                           "1.0f / glm_vec3_norm",
                           [](const shz::vec3& v) { return 1.0f / glm_vec3_norm(const_cast<float*>(&v[0])); },
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3MagnitudeInvFsrra)
    volatile float comp = 6.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, 6.0f).magnitude_inv_fsrra(), 1.0f / 7.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec3(2.0f, 3.0f, comp).magnitude_inv_fsrra(), 1.0f / 7.0f));

    (benchmark_cmp<float>)("shz::vec3::magnitude_inv_fsrra",
                           [](const shz::vec3& v) { return v.magnitude_inv_fsrra(); },
                           "1.0f / glm_vec3_norm",
                           [](const shz::vec3& v) { return 1.0f / glm_vec3_norm(const_cast<float*>(&v[0])); },
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Normalized)
    volatile float comp = 6.0f;

    GBL_TEST_VERIFY(shz::vec3(2.0f, 3.0f, 6.0f).normalized() == shz::vec3(2.0f / 7.0f, 3.0f / 7.0f, 6.0f / 7.0f));
    GBL_TEST_VERIFY(shz::vec3(2.0f, 3.0f, comp).normalized() == shz::vec3(2.0f / 7.0f, 3.0f / 7.0f, 6.0f / 7.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::normalized",
                               [](const shz::vec3& v) { return v.normalized(); },
                               "glm_vec3_normalize_to",
                               [](const shz::vec3& v) {
                                   shz::vec3 out;
                                   glm_vec3_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3NormalizedOr)
    volatile float comp = 6.0f;
    volatile float zero = 0.0f;

    GBL_TEST_VERIFY(shz::vec3(2.0f, 3.0f, 6.0f).normalized_or(shz::vec3(1.0f, 0.0f, 0.0f)) == shz::vec3(2.0f / 7.0f, 3.0f / 7.0f, 6.0f / 7.0f));
    GBL_TEST_VERIFY(shz::vec3(2.0f, 3.0f, comp).normalized_or(shz::vec3(1.0f, 0.0f, 0.0f)) == shz::vec3(2.0f / 7.0f, 3.0f / 7.0f, 6.0f / 7.0f));
    GBL_TEST_VERIFY(shz::vec3(0.0f).normalized_or(shz::vec3(1.0f, 0.0f, 0.0f)) == shz::vec3(1.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec3(zero).normalized_or(shz::vec3(1.0f, 0.0f, 0.0f)) == shz::vec3(1.0f, 0.0f, 0.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::normalized_or",
                               [](const shz::vec3& v) { return v.normalized_or(shz::vec3(0.0f)); },
                               "glm_vec3_normalize_to",
                               [](const shz::vec3& v) {
                                   shz::vec3 out;
                                   glm_vec3_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3NormalizedSafe)
    volatile float comp = 6.0f;
    volatile float zero = 0.0f;

    GBL_TEST_VERIFY(shz::vec3(2.0f, 3.0f, 6.0f).normalized_safe() == shz::vec3(2.0f / 7.0f, 3.0f / 7.0f, 6.0f / 7.0f));
    GBL_TEST_VERIFY(shz::vec3(2.0f, 3.0f, comp).normalized_safe() == shz::vec3(2.0f / 7.0f, 3.0f / 7.0f, 6.0f / 7.0f));
    GBL_TEST_VERIFY(shz::vec3(0.0f).normalized_safe() == shz::vec3(0.0f));
    GBL_TEST_VERIFY(shz::vec3(zero).normalized_safe() == shz::vec3(0.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::normalized_safe",
                               [](const shz::vec3& v) { return v.normalized_safe(); },
                               "glm_vec3_normalize_to",
                               [](const shz::vec3& v) {
                                   shz::vec3 out;
                                   glm_vec3_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Magnitude)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, 4.0f).magnitude(), 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, comp).magnitude(), 5.0f));

    (benchmark_cmp<float>)("shz::vec4::magnitude",
                           [](const shz::vec4& v) { return v.magnitude(); },
                           "glm_vec4_norm",
                           [](const shz::vec4& v) { return glm_vec4_norm(const_cast<float*>(&v[0])); },
                           shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4MagnitudeFsrra)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, 4.0f).magnitude_fsrra(), 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, comp).magnitude_fsrra(), 5.0f));

    (benchmark_cmp<float>)("shz::vec4::magnitude_fsrra",
                           [](const shz::vec4& v) { return v.magnitude_fsrra(); },
                           "glm_vec4_norm",
                           [](const shz::vec4& v) { return glm_vec4_norm(const_cast<float*>(&v[0])); },
                           shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4MagnitudeInv)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, 4.0f).magnitude_inv(), 1.0f / 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, comp).magnitude_inv(), 1.0f / 5.0f));

    (benchmark_cmp<float>)("shz::vec4::magnitude_inv",
                           [](const shz::vec4& v) { return v.magnitude_inv(); },
                           "1.0f / glm_vec4_norm",
                           [](const shz::vec4& v) { return 1.0f / glm_vec4_norm(const_cast<float*>(&v[0])); },
                           shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4MagnitudeInvFsrra)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, 4.0f).magnitude_inv_fsrra(), 1.0f / 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 2.0f, comp).magnitude_inv_fsrra(), 1.0f / 5.0f));

    (benchmark_cmp<float>)("shz::vec4::magnitude_inv_fsrra",
                           [](const shz::vec4& v) { return v.magnitude_inv_fsrra(); },
                           "1.0f / glm_vec4_norm",
                           [](const shz::vec4& v) { return 1.0f / glm_vec4_norm(const_cast<float*>(&v[0])); },
                           shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Normalized)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::vec4(1.0f, 2.0f, 2.0f, 4.0f).normalized() == shz::vec4(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::vec4(1.0f, 2.0f, 2.0f, comp).normalized() == shz::vec4(0.2f, 0.4f, 0.4f, 0.8f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::normalized",
                               [](const shz::vec4& v) { return v.normalized(); },
                               "glm_vec4_normalize_to",
                               [](const shz::vec4& v) {
                                   shz::vec4 out;
                                   glm_vec4_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4NormalizedOr)
    volatile float comp = 4.0f;
    volatile float zero = 0.0f;

    GBL_TEST_VERIFY(shz::vec4(1.0f, 2.0f, 2.0f, 4.0f).normalized_or(shz::vec4(1.0f, 0.0f, 0.0f, 0.0f)) == shz::vec4(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::vec4(1.0f, 2.0f, 2.0f, comp).normalized_or(shz::vec4(1.0f, 0.0f, 0.0f, 0.0f)) == shz::vec4(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::vec4(0.0f).normalized_or(shz::vec4(1.0f, 0.0f, 0.0f, 0.0f)) == shz::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::vec4(zero).normalized_or(shz::vec4(1.0f, 0.0f, 0.0f, 0.0f)) == shz::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::normalized_or",
                               [](const shz::vec4& v) { return v.normalized_or(shz::vec4(0.0f)); },
                               "glm_vec4_normalize_to",
                               [](const shz::vec4& v) {
                                   shz::vec4 out;
                                   glm_vec4_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4NormalizedSafe)
    volatile float comp = 4.0f;
    volatile float zero = 0.0f;

    GBL_TEST_VERIFY(shz::vec4(1.0f, 2.0f, 2.0f, 4.0f).normalized_safe() == shz::vec4(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::vec4(1.0f, 2.0f, 2.0f, comp).normalized_safe() == shz::vec4(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::vec4(0.0f).normalized_safe() == shz::vec4(0.0f));
    GBL_TEST_VERIFY(shz::vec4(zero).normalized_safe() == shz::vec4(0.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::normalized_safe",
                               [](const shz::vec4& v) { return v.normalized_safe(); },
                               "glm_vec4_normalize_to",
                               [](const shz::vec4& v) {
                                   shz::vec4 out;
                                   glm_vec4_normalize_to(const_cast<float*>(&v[0]), &out[0]);
                                   return out;
                               },
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Div)
    volatile float denom = -3.0f;

    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).div(shz::vec2(2.0f, -3.0f)) == shz::vec2(3.0f, 3.0f));
    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).div(shz::vec2(2.0f, denom)) == shz::vec2(3.0f, 3.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::div",
                         [](const shz::vec2& a, const shz::vec2& b) { return a.div(b); },
                         "glm_vec2_div",
                         [](const shz::vec2& a, const shz::vec2& b) {
                             shz::vec2 r;
                             glm_vec2_div(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]), &r[0]);
                             return r;
                         },
                         shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         shz::vec2(gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2DivFsrra)
    volatile float denom = 3.0f;

    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).div_fsrra(shz::vec2(2.0f, 3.0f)) == shz::vec2(3.0f, -3.0f));
    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).div_fsrra(shz::vec2(2.0f, denom)) == shz::vec2(3.0f, -3.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::div_fsrra",
                         [](const shz::vec2& a, const shz::vec2& b) { return a.div_fsrra(b); },
                         "glm_vec2_div",
                         [](const shz::vec2& a, const shz::vec2& b) {
                             shz::vec2 r;
                             glm_vec2_div(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]), &r[0]);
                             return r;
                         },
                         shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         shz::vec2(gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Inv)
    volatile float value = -4.0f;

    GBL_TEST_VERIFY(shz::vec2(2.0f, -4.0f).inv() == shz::vec2(0.5f, -0.25f));
    GBL_TEST_VERIFY(shz::vec2(2.0f, value).inv() == shz::vec2(0.5f, -0.25f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::inv",
                         [](const shz::vec2& v) { return v.inv(); },
                         "glm_vec2_div(1, v)",
                         [](const shz::vec2& v) {
                             shz::vec2 one(1.0f), r;
                             glm_vec2_div(&one[0], const_cast<float*>(&v[0]), &r[0]);
                             return r;
                         },
                         shz::vec2(gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2InvFsrra)
    volatile float value = 4.0f;

    GBL_TEST_VERIFY(shz::vec2(2.0f, 4.0f).inv_fsrra() == shz::vec2(0.5f, 0.25f));
    GBL_TEST_VERIFY(shz::vec2(2.0f, value).inv_fsrra() == shz::vec2(0.5f, 0.25f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::inv_fsrra",
                         [](const shz::vec2& v) { return v.inv_fsrra(); },
                         "glm_vec2_div(1, v)",
                         [](const shz::vec2& v) {
                             shz::vec2 one(1.0f), r;
                             glm_vec2_div(&one[0], const_cast<float*>(&v[0]), &r[0]);
                             return r;
                         },
                         shz::vec2(gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Divf)
    volatile float denom = -3.0f;

    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).divf(-3.0f) == shz::vec2(-2.0f, 3.0f));
    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).divf(denom) == shz::vec2(-2.0f, 3.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::divf",
                         [](const shz::vec2& v, float s) { return v.divf(s); },
                         "v / s",
                         [](const shz::vec2& v, float s) { return shz::vec2(v.x / s, v.y / s); },
                         shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(-100.0f, -1.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::divf",
                         [](const shz::vec2& v, float s) { return v.divf(s); },
                         "v * (1.0f / s)",
                         [](const shz::vec2& v, float s) { const float inv = 1.0f / s; return shz::vec2(v.x * inv, v.y * inv); },
                         shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(-100.0f, -1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2DivfFsrra)
    volatile float denom = 3.0f;

    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).divf_fsrra(3.0f) == shz::vec2(2.0f, -3.0f));
    GBL_TEST_VERIFY(shz::vec2(6.0f, -9.0f).divf_fsrra(denom) == shz::vec2(2.0f, -3.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::divf_fsrra",
                         [](const shz::vec2& v, float s) { return v.divf_fsrra(s); },
                         "v / s",
                         [](const shz::vec2& v, float s) { return shz::vec2(v.x / s, v.y / s); },
                         shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(1.0f, 100.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::divf_fsrra",
                         [](const shz::vec2& v, float s) { return v.divf_fsrra(s); },
                         "v * (1.0f / s)",
                         [](const shz::vec2& v, float s) { const float inv = 1.0f / s; return shz::vec2(v.x * inv, v.y * inv); },
                         shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(1.0f, 100.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Distance)
    volatile float coord = 6.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec2(1.0f, 2.0f).distance(shz::vec2(4.0f, 6.0f)), 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec2(1.0f, 2.0f).distance(shz::vec2(4.0f, coord)), 5.0f));

    (benchmark_cmp<float>)("shz::vec2::distance",
                           [](const shz::vec2& a, const shz::vec2& b) { return a.distance(b); },
                           "glm_vec2_distance",
                           [](const shz::vec2& a, const shz::vec2& b) {
                               return glm_vec2_distance(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]));
                           },
                           shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                           shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Div)
    volatile float denom = -4.0f;

    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 20.0f).div(shz::vec3(2.0f, -3.0f, -4.0f)) == shz::vec3(3.0f, 3.0f, -5.0f));
    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 20.0f).div(shz::vec3(2.0f, -3.0f, denom)) == shz::vec3(3.0f, 3.0f, -5.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::div",
                         [](const shz::vec3& a, const shz::vec3& b) { return a.div(b); },
                         "glm_vec3_div",
                         [](const shz::vec3& a, const shz::vec3& b) {
                             shz::vec3 r;
                             glm_vec3_div(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]), &r[0]);
                             return r;
                         },
                         shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         shz::vec3(gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3DivFsrra)
    volatile float denom = 4.0f;

    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 20.0f).div_fsrra(shz::vec3(2.0f, 3.0f, 4.0f)) == shz::vec3(3.0f, -3.0f, 5.0f));
    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 20.0f).div_fsrra(shz::vec3(2.0f, 3.0f, denom)) == shz::vec3(3.0f, -3.0f, 5.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::div_fsrra",
                         [](const shz::vec3& a, const shz::vec3& b) { return a.div_fsrra(b); },
                         "glm_vec3_div",
                         [](const shz::vec3& a, const shz::vec3& b) {
                             shz::vec3 r;
                             glm_vec3_div(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]), &r[0]);
                             return r;
                         },
                         shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         shz::vec3(gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Inv)
    volatile float value = 0.5f;

    GBL_TEST_VERIFY(shz::vec3(2.0f, -4.0f, 0.5f).inv() == shz::vec3(0.5f, -0.25f, 2.0f));
    GBL_TEST_VERIFY(shz::vec3(2.0f, -4.0f, value).inv() == shz::vec3(0.5f, -0.25f, 2.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::inv",
                         [](const shz::vec3& v) { return v.inv(); },
                         "glm_vec3_div(1, v)",
                         [](const shz::vec3& v) {
                             shz::vec3 one(1.0f), r;
                             glm_vec3_div(&one[0], const_cast<float*>(&v[0]), &r[0]);
                             return r;
                         },
                         shz::vec3(gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3InvFsrra)
    volatile float value = 0.5f;

    GBL_TEST_VERIFY(shz::vec3(2.0f, 4.0f, 0.5f).inv_fsrra() == shz::vec3(0.5f, 0.25f, 2.0f));
    GBL_TEST_VERIFY(shz::vec3(2.0f, 4.0f, value).inv_fsrra() == shz::vec3(0.5f, 0.25f, 2.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::inv_fsrra",
                         [](const shz::vec3& v) { return v.inv_fsrra(); },
                         "glm_vec3_div(1, v)",
                         [](const shz::vec3& v) {
                             shz::vec3 one(1.0f), r;
                             glm_vec3_div(&one[0], const_cast<float*>(&v[0]), &r[0]);
                             return r;
                         },
                         shz::vec3(gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Divf)
    volatile float denom = -3.0f;

    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 12.0f).divf(-3.0f) == shz::vec3(-2.0f, 3.0f, -4.0f));
    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 12.0f).divf(denom) == shz::vec3(-2.0f, 3.0f, -4.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::divf",
                         [](const shz::vec3& v, float s) { return v.divf(s); },
                         "v / s",
                         [](const shz::vec3& v, float s) { return shz::vec3(v.x / s, v.y / s, v.z / s); },
                         shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(-100.0f, -1.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::divf",
                         [](const shz::vec3& v, float s) { return v.divf(s); },
                         "v * (1.0f / s)",
                         [](const shz::vec3& v, float s) { const float inv = 1.0f / s; return shz::vec3(v.x * inv, v.y * inv, v.z * inv); },
                         shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(-100.0f, -1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3DivfFsrra)
    volatile float denom = 3.0f;

    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 12.0f).divf_fsrra(3.0f) == shz::vec3(2.0f, -3.0f, 4.0f));
    GBL_TEST_VERIFY(shz::vec3(6.0f, -9.0f, 12.0f).divf_fsrra(denom) == shz::vec3(2.0f, -3.0f, 4.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::divf_fsrra",
                         [](const shz::vec3& v, float s) { return v.divf_fsrra(s); },
                         "v / s",
                         [](const shz::vec3& v, float s) { return shz::vec3(v.x / s, v.y / s, v.z / s); },
                         shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(1.0f, 100.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::divf_fsrra",
                         [](const shz::vec3& v, float s) { return v.divf_fsrra(s); },
                         "v * (1.0f / s)",
                         [](const shz::vec3& v, float s) { const float inv = 1.0f / s; return shz::vec3(v.x * inv, v.y * inv, v.z * inv); },
                         shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(1.0f, 100.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Distance)
    volatile float coord = 9.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec3(1.0f, 2.0f, 3.0f).distance(shz::vec3(3.0f, 5.0f, 9.0f)), 7.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec3(1.0f, 2.0f, 3.0f).distance(shz::vec3(3.0f, 5.0f, coord)), 7.0f));

    (benchmark_cmp<float>)("shz::vec3::distance",
                           [](const shz::vec3& a, const shz::vec3& b) { return a.distance(b); },
                           "glm_vec3_distance",
                           [](const shz::vec3& a, const shz::vec3& b) {
                               return glm_vec3_distance(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]));
                           },
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Div)
    volatile float denom = 0.5f;

    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 20.0f, -1.0f).div(shz::vec4(2.0f, -3.0f, -4.0f, 0.5f)) == shz::vec4(3.0f, 3.0f, -5.0f, -2.0f));
    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 20.0f, -1.0f).div(shz::vec4(2.0f, -3.0f, -4.0f, denom)) == shz::vec4(3.0f, 3.0f, -5.0f, -2.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::div",
                         [](const shz::vec4& a, const shz::vec4& b) { return a.div(b); },
                         "glm_vec4_div",
                         [](const shz::vec4& a, const shz::vec4& b) {
                             shz::vec4 r;
                             glm_vec4_div(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]), &r[0]);
                             return r;
                         },
                         shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         shz::vec4(gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4DivFsrra)
    volatile float denom = 0.5f;

    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 20.0f, -1.0f).div_fsrra(shz::vec4(2.0f, 3.0f, 4.0f, 0.5f)) == shz::vec4(3.0f, -3.0f, 5.0f, -2.0f));
    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 20.0f, -1.0f).div_fsrra(shz::vec4(2.0f, 3.0f, 4.0f, denom)) == shz::vec4(3.0f, -3.0f, 5.0f, -2.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::div_fsrra",
                         [](const shz::vec4& a, const shz::vec4& b) { return a.div_fsrra(b); },
                         "glm_vec4_div",
                         [](const shz::vec4& a, const shz::vec4& b) {
                             shz::vec4 r;
                             glm_vec4_div(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]), &r[0]);
                             return r;
                         },
                         shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         shz::vec4(gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Inv)
    volatile float value = -0.25f;

    GBL_TEST_VERIFY(shz::vec4(2.0f, -4.0f, 0.5f, -0.25f).inv() == shz::vec4(0.5f, -0.25f, 2.0f, -4.0f));
    GBL_TEST_VERIFY(shz::vec4(2.0f, -4.0f, 0.5f, value).inv() == shz::vec4(0.5f, -0.25f, 2.0f, -4.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::inv",
                         [](const shz::vec4& v) { return v.inv(); },
                         "glm_vec4_div(1, v)",
                         [](const shz::vec4& v) {
                             shz::vec4 one(1.0f), r;
                             glm_vec4_div(&one[0], const_cast<float*>(&v[0]), &r[0]);
                             return r;
                         },
                         shz::vec4(gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f), gblRandUniform(-100.0f, -1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4InvFsrra)
    volatile float value = 0.25f;

    GBL_TEST_VERIFY(shz::vec4(2.0f, 4.0f, 0.5f, 0.25f).inv_fsrra() == shz::vec4(0.5f, 0.25f, 2.0f, 4.0f));
    GBL_TEST_VERIFY(shz::vec4(2.0f, 4.0f, 0.5f, value).inv_fsrra() == shz::vec4(0.5f, 0.25f, 2.0f, 4.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::inv_fsrra",
                         [](const shz::vec4& v) { return v.inv_fsrra(); },
                         "glm_vec4_div(1, v)",
                         [](const shz::vec4& v) {
                             shz::vec4 one(1.0f), r;
                             glm_vec4_div(&one[0], const_cast<float*>(&v[0]), &r[0]);
                             return r;
                         },
                         shz::vec4(gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f), gblRandUniform(1.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Divf)
    volatile float denom = -3.0f;

    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 12.0f, -3.0f).divf(-3.0f) == shz::vec4(-2.0f, 3.0f, -4.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 12.0f, -3.0f).divf(denom) == shz::vec4(-2.0f, 3.0f, -4.0f, 1.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::divf",
                         [](const shz::vec4& v, float s) { return v.divf(s); },
                         "v / s",
                         [](const shz::vec4& v, float s) { return shz::vec4(v.x / s, v.y / s, v.z / s, v.w / s); },
                         shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(-100.0f, -1.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::divf",
                         [](const shz::vec4& v, float s) { return v.divf(s); },
                         "v * (1.0f / s)",
                         [](const shz::vec4& v, float s) { const float inv = 1.0f / s; return shz::vec4(v.x * inv, v.y * inv, v.z * inv, v.w * inv); },
                         shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(-100.0f, -1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4DivfFsrra)
    volatile float denom = 3.0f;

    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 12.0f, -3.0f).divf_fsrra(3.0f) == shz::vec4(2.0f, -3.0f, 4.0f, -1.0f));
    GBL_TEST_VERIFY(shz::vec4(6.0f, -9.0f, 12.0f, -3.0f).divf_fsrra(denom) == shz::vec4(2.0f, -3.0f, 4.0f, -1.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::divf_fsrra",
                         [](const shz::vec4& v, float s) { return v.divf_fsrra(s); },
                         "v / s",
                         [](const shz::vec4& v, float s) { return shz::vec4(v.x / s, v.y / s, v.z / s, v.w / s); },
                         shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(1.0f, 100.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::divf_fsrra",
                         [](const shz::vec4& v, float s) { return v.divf_fsrra(s); },
                         "v * (1.0f / s)",
                         [](const shz::vec4& v, float s) { const float inv = 1.0f / s; return shz::vec4(v.x * inv, v.y * inv, v.z * inv, v.w * inv); },
                         shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                         gblRandUniform(1.0f, 100.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Distance)
    volatile float coord = 8.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 3.0f, 4.0f).distance(shz::vec4(2.0f, 4.0f, 5.0f, 8.0f)), 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::vec4(1.0f, 2.0f, 3.0f, 4.0f).distance(shz::vec4(2.0f, 4.0f, 5.0f, coord)), 5.0f));

    (benchmark_cmp<float>)("shz::vec4::distance",
                           [](const shz::vec4& a, const shz::vec4& b) { return a.distance(b); },
                           "glm_vec4_distance",
                           [](const shz::vec4& a, const shz::vec4& b) {
                               return glm_vec4_distance(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]));
                           },
                           shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                           shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Triple)
    const shz::vec3 x(1.0f, 0.0f, 0.0f), y(0.0f, 1.0f, 0.0f), z(0.0f, 0.0f, 1.0f);

    GBL_TEST_VERIFY(x.triple(y, z) ==  1.0f);
    GBL_TEST_VERIFY(x.triple(z, y) == -1.0f);
    GBL_TEST_VERIFY(x.triple(x, y) ==  0.0f);

    for(const auto& [a, b, c] : { std::tuple{ shz::vec3(2.0f, 3.0f, 5.0f),   shz::vec3(7.0f, 11.0f, 13.0f), shz::vec3(17.0f, 19.0f, 23.0f) },
                                  std::tuple{ shz::vec3(-1.5f, 4.0f, 0.25f), shz::vec3(3.0f, -2.0f, 8.0f),  shz::vec3(0.5f, 6.0f, -7.0f)   },
                                  std::tuple{ shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                                              shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                                              shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)) } }) {
        shz_glm_vec3 ga, gb, gc, cross;
        ga.shz = a; gb.shz = b; gc.shz = c;
        glm_vec3_cross(gb.glm, gc.glm, cross.glm);
        const float ref = glm_vec3_dot(ga.glm, cross.glm);
        const float tol = 1e-4f * std::fmax(1.0f, std::sqrt(a.magnitude_sqr() * b.magnitude_sqr() * c.magnitude_sqr()));

        GBL_TEST_VERIFY(std::fabs(a.triple(b, c) - ref)  <= tol);
        GBL_TEST_VERIFY(std::fabs(b.triple(c, a) - ref)  <= tol);
        GBL_TEST_VERIFY(std::fabs(a.triple(c, b) + ref)  <= tol);
    }

    (benchmark_cmp<float>)("shz::vec3::triple",
                           [](const shz::vec3& a, const shz::vec3& b, const shz::vec3& c) {
                               return a.triple(b, c);
                           },
                           "glm_vec3_cross + glm_vec3_dot",
                           [](const shz::vec3& a, const shz::vec3& b, const shz::vec3& c) {
                               vec3 cross;
                               glm_vec3_cross(const_cast<float*>(&b[0]), const_cast<float*>(&c[0]), cross);
                               return glm_vec3_dot(const_cast<float*>(&a[0]), cross);
                           },
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                           shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

template<unsigned N, typename V>
static bool verify_reject_(const V& a, const V& b) {
    double ab = 0.0, bb = 0.0, aa = 0.0;
    for(unsigned i = 0; i < N; ++i) {
        ab += static_cast<double>(a[i]) * b[i];
        bb += static_cast<double>(b[i]) * b[i];
        aa += static_cast<double>(a[i]) * a[i];
    }

    const V      r   = a.reject(b);
    const double tol = 2e-3 * std::fmax(1.0, std::sqrt(aa));
    double       rb  = 0.0;

    for(unsigned i = 0; i < N; ++i) {
        if(std::fabs(r[i] - (a[i] - b[i] * (ab / bb))) > tol)
            return false;
        rb += static_cast<double>(r[i]) * b[i];
    }

    return std::fabs(rb) <= tol * std::sqrt(bb);
}

template<unsigned N, typename V>
static bool test_reject_() {
    auto rand_vec = [] {
        V v;
        for(unsigned i = 0; i < N; ++i)
            v[i] = gblRandUniform(-100.0f, 100.0f);
        return v;
    };

    V a, b, zero;
    for(unsigned i = 0; i < N; ++i) {
        a[i]    = static_cast<float>(i + 2) * (i & 1 ? -1.5f : 3.0f);
        b[i]    = static_cast<float>(N - i);
        zero[i] = 0.0f;
    }

    if(!(a.reject_safe(zero) == a))
        return false;

    if(!(a.reject_safe(b) == a.reject(b)) || !(b.reject_safe(a) == b.reject(a)))
        return false;

    if(!(b.reject(b * 3.0f) == zero))
        return false;

    if(!verify_reject_<N>(a, b) || !verify_reject_<N>(b, a))
        return false;

    for(unsigned i = 0; i < 32; ++i) {
        const V r1 = rand_vec(), r2 = rand_vec();

        if(!verify_reject_<N>(r1, r2) || !(r1.reject_safe(r2) == r1.reject(r2)))
            return false;
    }

    return true;
}

GBL_TEST_CASE(vec2Reject)
    GBL_TEST_VERIFY((test_reject_<2, shz::vec2>()));
    GBL_TEST_VERIFY(shz::vec2(3.0f, 4.0f).reject(shz::vec2(1.0f, 0.0f)) == shz::vec2(0.0f, 4.0f));

    (benchmark_cmp<shz::vec2>)("shz::vec2::reject",
                               [](const shz::vec2& a, const shz::vec2& b) {
                                   return a.reject(b);
                               },
                               "glm_vec2_dot + glm_vec2_scale + glm_vec2_sub",
                               [](const shz::vec2& a, const shz::vec2& b) {
                                   shz::vec2 proj, r;
                                   float* pa = const_cast<float*>(&a[0]);
                                   float* pb = const_cast<float*>(&b[0]);
                                   glm_vec2_scale(pb, glm_vec2_dot(pa, pb) / glm_vec2_dot(pb, pb), &proj[0]);
                                   glm_vec2_sub(pa, &proj[0], &r[0]);
                                   return r;
                               },
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Reject)
    GBL_TEST_VERIFY((test_reject_<3, shz::vec3>()));
    GBL_TEST_VERIFY(shz::vec3(3.0f, 4.0f, 5.0f).reject(shz::vec3(0.0f, 0.0f, 2.0f)) == shz::vec3(3.0f, 4.0f, 0.0f));

    (benchmark_cmp<shz::vec3>)("shz::vec3::reject",
                               [](const shz::vec3& a, const shz::vec3& b) {
                                   return a.reject(b);
                               },
                               "glm_vec3_proj + glm_vec3_sub",
                               [](const shz::vec3& a, const shz::vec3& b) {
                                   shz::vec3 proj, r;
                                   glm_vec3_proj(const_cast<float*>(&a[0]), const_cast<float*>(&b[0]), &proj[0]);
                                   glm_vec3_sub(const_cast<float*>(&a[0]), &proj[0], &r[0]);
                                   return r;
                               },
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Reject)
    GBL_TEST_VERIFY((test_reject_<4, shz::vec4>()));
    GBL_TEST_VERIFY(shz::vec4(3.0f, 4.0f, 5.0f, 6.0f).reject(shz::vec4(0.0f, 0.0f, 0.0f, 2.0f)) == shz::vec4(3.0f, 4.0f, 5.0f, 0.0f));

    (benchmark_cmp<shz::vec4>)("shz::vec4::reject",
                               [](const shz::vec4& a, const shz::vec4& b) {
                                   return a.reject(b);
                               },
                               "glm_vec4_dot + glm_vec4_scale + glm_vec4_sub",
                               [](const shz::vec4& a, const shz::vec4& b) {
                                   shz::vec4 proj, r;
                                   float* pa = const_cast<float*>(&a[0]);
                                   float* pb = const_cast<float*>(&b[0]);
                                   glm_vec4_scale(pb, glm_vec4_dot(pa, pb) / glm_vec4_dot(pb, pb), &proj[0]);
                                   glm_vec4_sub(pa, &proj[0], &r[0]);
                                   return r;
                               },
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

template<unsigned N, typename V>
static bool verify_bisector_(const V& a, const V& b) {
    double aa = 0.0, bb = 0.0;
    for(unsigned i = 0; i < N; ++i) {
        aa += static_cast<double>(a[i]) * a[i];
        bb += static_cast<double>(b[i]) * b[i];
    }

    const double la = std::sqrt(aa), lb = std::sqrt(bb);
    const double tol = 2e-3 * std::fmax(1.0, la);
    const V      bis = a.bisector(b);
    const V      hlf = a.half_angle(b);
    double       ref[N], rr = 0.0, ba = 0.0, bbis = 0.0, hh = 0.0;

    for(unsigned i = 0; i < N; ++i) {
        ref[i] = a[i] + b[i] * (la / lb);
        rr    += ref[i] * ref[i];
        if(std::fabs(bis[i] - ref[i]) > tol)
            return false;
    }

    const double lr = std::sqrt(rr);
    if(lr < 1e-2 * la)
        return true;

    for(unsigned i = 0; i < N; ++i) {
        ba   += static_cast<double>(bis[i]) * a[i];
        bbis += static_cast<double>(bis[i]) * b[i];
        hh   += static_cast<double>(hlf[i]) * hlf[i];
        if(std::fabs(hlf[i] - ref[i] / lr) > 2e-3)
            return false;
    }

    if(std::fabs(ba / (la * lr) - bbis / (lb * lr)) > 2e-3)
        return false;

    return std::fabs(std::sqrt(hh) - 1.0) <= 2e-3;
}

template<unsigned N, typename V>
static bool test_bisector_() {
    auto rand_vec = [] {
        V v;
        for(unsigned i = 0; i < N; ++i)
            v[i] = gblRandUniform(-100.0f, 100.0f);
        return v;
    };

    V a, b;
    for(unsigned i = 0; i < N; ++i) {
        a[i] = static_cast<float>(i + 2) * (i & 1 ? -1.5f : 3.0f);
        b[i] = static_cast<float>(N - i);
    }

    if(!(a.bisector(a * 4.0f) == a * 2.0f))
        return false;

    if(!verify_bisector_<N>(a, b) || !verify_bisector_<N>(b, a))
        return false;

    for(unsigned i = 0; i < 32; ++i)
        if(!verify_bisector_<N>(rand_vec(), rand_vec()))
            return false;

    return true;
}

GBL_TEST_CASE(vec2Bisector)
    GBL_TEST_VERIFY((test_bisector_<2, shz::vec2>()));
    GBL_TEST_VERIFY(shz::vec2(1.0f, 0.0f).bisector(shz::vec2(0.0f, 2.0f)) == shz::vec2(1.0f, 1.0f));
    GBL_TEST_VERIFY(shz::vec2(1.0f, 0.0f).half_angle(shz::vec2(0.0f, 2.0f)) == shz::vec2(shz::sqrtf(0.5f), shz::sqrtf(0.5f)));

    (benchmark_cmp<shz::vec2>)("shz::vec2::bisector",
                               [](const shz::vec2& a, const shz::vec2& b) {
                                   return a.bisector(b);
                               },
                               "glm_vec2_norm + glm_vec2_scale + glm_vec2_add",
                               [](const shz::vec2& a, const shz::vec2& b) {
                                   shz::vec2 scaled, r;
                                   float* pa = const_cast<float*>(&a[0]);
                                   float* pb = const_cast<float*>(&b[0]);
                                   glm_vec2_scale(pb, glm_vec2_norm(pa) / glm_vec2_norm(pb), &scaled[0]);
                                   glm_vec2_add(pa, &scaled[0], &r[0]);
                                   return r;
                               },
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));

    (benchmark_cmp<shz::vec2>)("shz::vec2::half_angle",
                               [](const shz::vec2& a, const shz::vec2& b) {
                                   return a.half_angle(b);
                               },
                               "glm_vec2_normalize_to + glm_vec2_add + glm_vec2_normalize",
                               [](const shz::vec2& a, const shz::vec2& b) {
                                   shz::vec2 na, nb, r;
                                   glm_vec2_normalize_to(const_cast<float*>(&a[0]), &na[0]);
                                   glm_vec2_normalize_to(const_cast<float*>(&b[0]), &nb[0]);
                                   glm_vec2_add(&na[0], &nb[0], &r[0]);
                                   glm_vec2_normalize(&r[0]);
                                   return r;
                               },
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec2(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Bisector)
    GBL_TEST_VERIFY((test_bisector_<3, shz::vec3>()));
    GBL_TEST_VERIFY(shz::vec3(2.0f, 0.0f, 0.0f).bisector(shz::vec3(0.0f, 0.0f, 5.0f)) == shz::vec3(2.0f, 0.0f, 2.0f));
    GBL_TEST_VERIFY(shz::vec3(2.0f, 0.0f, 0.0f).half_angle(shz::vec3(0.0f, 0.0f, 5.0f)) == shz::vec3(shz::sqrtf(0.5f), 0.0f, shz::sqrtf(0.5f)));

    (benchmark_cmp<shz::vec3>)("shz::vec3::bisector",
                               [](const shz::vec3& a, const shz::vec3& b) {
                                   return a.bisector(b);
                               },
                               "glm_vec3_norm + glm_vec3_scale + glm_vec3_add",
                               [](const shz::vec3& a, const shz::vec3& b) {
                                   shz::vec3 scaled, r;
                                   float* pa = const_cast<float*>(&a[0]);
                                   float* pb = const_cast<float*>(&b[0]);
                                   glm_vec3_scale(pb, glm_vec3_norm(pa) / glm_vec3_norm(pb), &scaled[0]);
                                   glm_vec3_add(pa, &scaled[0], &r[0]);
                                   return r;
                               },
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));

    (benchmark_cmp<shz::vec3>)("shz::vec3::half_angle",
                               [](const shz::vec3& a, const shz::vec3& b) {
                                   return a.half_angle(b);
                               },
                               "glm_vec3_normalize_to + glm_vec3_add + glm_vec3_normalize",
                               [](const shz::vec3& a, const shz::vec3& b) {
                                   shz::vec3 na, nb, r;
                                   glm_vec3_normalize_to(const_cast<float*>(&a[0]), &na[0]);
                                   glm_vec3_normalize_to(const_cast<float*>(&b[0]), &nb[0]);
                                   glm_vec3_add(&na[0], &nb[0], &r[0]);
                                   glm_vec3_normalize(&r[0]);
                                   return r;
                               },
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec3(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Bisector)
    GBL_TEST_VERIFY((test_bisector_<4, shz::vec4>()));
    GBL_TEST_VERIFY(shz::vec4(0.0f, 3.0f, 0.0f, 0.0f).bisector(shz::vec4(0.0f, 0.0f, 0.0f, 6.0f)) == shz::vec4(0.0f, 3.0f, 0.0f, 3.0f));
    GBL_TEST_VERIFY(shz::vec4(0.0f, 3.0f, 0.0f, 0.0f).half_angle(shz::vec4(0.0f, 0.0f, 0.0f, 6.0f)) == shz::vec4(0.0f, shz::sqrtf(0.5f), 0.0f, shz::sqrtf(0.5f)));

    (benchmark_cmp<shz::vec4>)("shz::vec4::bisector",
                               [](const shz::vec4& a, const shz::vec4& b) {
                                   return a.bisector(b);
                               },
                               "glm_vec4_norm + glm_vec4_scale + glm_vec4_add",
                               [](const shz::vec4& a, const shz::vec4& b) {
                                   shz::vec4 scaled, r;
                                   float* pa = const_cast<float*>(&a[0]);
                                   float* pb = const_cast<float*>(&b[0]);
                                   glm_vec4_scale(pb, glm_vec4_norm(pa) / glm_vec4_norm(pb), &scaled[0]);
                                   glm_vec4_add(pa, &scaled[0], &r[0]);
                                   return r;
                               },
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));

    (benchmark_cmp<shz::vec4>)("shz::vec4::half_angle",
                               [](const shz::vec4& a, const shz::vec4& b) {
                                   return a.half_angle(b);
                               },
                               "glm_vec4_normalize_to + glm_vec4_add + glm_vec4_normalize",
                               [](const shz::vec4& a, const shz::vec4& b) {
                                   shz::vec4 na, nb, r;
                                   glm_vec4_normalize_to(const_cast<float*>(&a[0]), &na[0]);
                                   glm_vec4_normalize_to(const_cast<float*>(&b[0]), &nb[0]);
                                   glm_vec4_add(&na[0], &nb[0], &r[0]);
                                   glm_vec4_normalize(&r[0]);
                                   return r;
                               },
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)),
                               shz::vec4(gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f), gblRandUniform(-100.0f, 100.0f)));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(vec2Construct,
                  vec2Set,
                  vec2Lerp,
                  vec2AddEquals,
                  vec2SubEquals,
                  vec2MultEquals,
                  vec2DivEquals,
                  vec2MultEqualsScalar,
                  vec2DivEqualsScalar,
                  vec2Compare,
                  vec2Dot,
                  vec2Dot2,
                  vec2Dot3,
                  vec2Magnitude,
                  vec2MagnitudeSqr,
                  vec2MagnitudeInv,
                  vec2Direction,
                  vecMagnitudeSafety,
                  vecSubscript,
                  vecNormalizeOr,
                  vec2MagnitudeFsrra,
                  vec2MagnitudeInvFsrra,
                  vec2Normalized,
                  vec2NormalizedOr,
                  vec2NormalizedSafe,
                  vec3Magnitude,
                  vec3MagnitudeFsrra,
                  vec3MagnitudeInv,
                  vec3MagnitudeInvFsrra,
                  vec3Normalized,
                  vec3NormalizedOr,
                  vec3NormalizedSafe,
                  vec4Magnitude,
                  vec4MagnitudeFsrra,
                  vec4MagnitudeInv,
                  vec4MagnitudeInvFsrra,
                  vec4Normalized,
                  vec4NormalizedOr,
                  vec4NormalizedSafe,
                  vec2Div,
                  vec2DivFsrra,
                  vec2Inv,
                  vec2InvFsrra,
                  vec2Divf,
                  vec2DivfFsrra,
                  vec2Distance,
                  vec3Div,
                  vec3DivFsrra,
                  vec3Inv,
                  vec3InvFsrra,
                  vec3Divf,
                  vec3DivfFsrra,
                  vec3Distance,
                  vec4Div,
                  vec4DivFsrra,
                  vec4Inv,
                  vec4InvFsrra,
                  vec4Divf,
                  vec4DivfFsrra,
                  vec4Distance,
                  vec3Dot,
                  vec3Dot2,
                  vec3Dot3,
                  vec4Dot,
                  vec4Dot2,
                  vec4Dot3,
                  vec2Rotate,
                  vec3RotateX,
                  vec3RotateY,
                  vec3RotateZ,
                  vec3Orthonormalize,
                  vec3Triple,
                  vec2Reject,
                  vec3Reject,
                  vec4Reject,
                  vec2Bisector,
                  vec3Bisector,
                  vec4Bisector,
                  vec3Barycenter,
                  vec2Move,
                  vec3Move,
                  vec4Move,
                  vec3CubicHermite,
                  vec2Step,
                  vec3Step,
                  vec4Step,
                  vec2Smoothstep,
                  vec3Smoothstep,
                  vec4Smoothstep,
                  vec4InvSqrtf)