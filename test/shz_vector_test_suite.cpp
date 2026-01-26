#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_vector.hpp"

#include <print>

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
        auto vec4 = shz::vec2((shz_vec2_t) { 3.0f, 4.0f });
        GBL_TEST_VERIFY(vec4.x == 3.0f);
        GBL_TEST_VERIFY(vec4.y == 4.0f);
        GBL_TEST_VERIFY(vec4[0] == 3.0f);
        GBL_TEST_VERIFY(vec4[1] == 4.0f);
    }
    {
        auto vec5 = shz::vec2((shz_vec2_t) { 5.0f, 6.0f });
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

    vec = (shz_vec2_t) { 3.0f, 4.0f };
    GBL_TEST_VERIFY(vec.x == 3.0f);
    GBL_TEST_VERIFY(vec.y == 4.0f);

    vec = shz::vec2(-5.0f, -6.0f);
    GBL_TEST_VERIFY(vec.x == -5.0f);
    GBL_TEST_VERIFY(vec.y == -6.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Lerp)
    auto vec =
        shz::vec2::lerp({ 0.0f, 0.0f},               // Flex C++ ctor dick
                        (shz_vec2_t) { 2.0f, 2.0f }, // Flex C conversion ctor dick
                        0.5f);
    GBL_TEST_VERIFY(vec.x == 1.0f);  // Flex C member access dick
    GBL_TEST_VERIFY(vec[1] == 1.0f); // Flex C++ overloaded subscript dick
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2AddEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec += (shz_vec2_t) { 3.0f, 5.0f };
    vec += shz::vec2 { -4.0f, -6.0f };

    GBL_TEST_VERIFY(vec.x == 0.0f);
    GBL_TEST_VERIFY(vec.y == -3.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2SubEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec -= (shz_vec2_t) { 3.0f, 5.0f };
    vec -= shz::vec2 { -4.0f, -6.0f };

    GBL_TEST_VERIFY(vec.x == 2.0f);
    GBL_TEST_VERIFY(vec.y == -1.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MultEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec *= (shz_vec2_t) { 3.0f, 5.0f };
    vec *= shz::vec2 { -4.0f, -6.0f };

    GBL_TEST_VERIFY(vec.x == -12.0f);
    GBL_TEST_VERIFY(vec.y == 60.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2DivEquals)
    auto vec = shz::vec2(1.0f, -2.0f);

    vec /= (shz_vec2_t) { 3.0f, 5.0f };
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
    GBL_TEST_VERIFY((vec == (shz_vec2_t){1.0f, 4.0f}));
    GBL_TEST_VERIFY(vec <= shz::vec2(2.0f, 5.0f));
    GBL_TEST_VERIFY(vec > shz::vec2(1.0f, 3.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Dot)
    auto base = shz::vec2(2.0f, 3.0f);
    GBL_TEST_VERIFY(base.dot({1.0f, -1.0f}) == -1.0f);
    GBL_TEST_VERIFY(base.dot((shz_vec2_t){-3.0f, 4.0f}) == 6.0f);
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
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() }));
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

    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf() }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Magnitude)
    GBL_TEST_ERROR((shz::vec2{1.0f, 1.0f}.magnitude()), sqrtf(2.0f), 0.0001f, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR((shz::vec2(2.0f, 3.0f).magnitude()), 3.60555127546f, 0.0001f, GBL_TEST_ERROR_FUZZY);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MagnitudeSqr)
    GBL_TEST_VERIFY((shz::vec2{1.0f, 1.0f}.magnitude_sqr() == 2.0f));
    GBL_TEST_VERIFY((shz::vec2(2.0f, 3.0f).magnitude_sqr() == 13.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2MagnitudeInv)
    GBL_TEST_ERROR((shz::vec2{1.0f, 1.0f}.magnitude_inv()), 1.0f / sqrtf(2.0f), 0.0001f, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz::vec2(2.0f, 3.0f).magnitude_inv(), 1.0f / sqrtf(13.0f), 0.0001f, GBL_TEST_ERROR_FUZZY);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec2Direction)
    GBL_TEST_VERIFY((shz::vec2{0.0f, 0.0f}.direction() == shz::vec2(0.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Dot)
    auto base = shz::vec3(2.0f, 3.0f, 4.0f);
    GBL_TEST_VERIFY(base.dot({1.0f, -1.0f, 1.0f}) == 3.0f);
    GBL_TEST_VERIFY(base.dot((shz_vec3_t){-3.0f, 4.0f, -2.0f}) == -2.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Dot2)
   auto test = [&](shz::vec3 vec1,
                   shz::vec3 vec2,
                   shz::vec3 vec3)
   {
        shz::vec2 fipr_res, c_res;

        benchmark(&fipr_res, [&]{ return vec1.dot(vec2, vec3); });

        benchmark(&c_res, [&]{ return shz::vec2 {
                                    vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z,
                                    vec1.x * vec3.x + vec1.y * vec3.y + vec1.z * vec3.z
                                };
                           });

        return gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error);
    };
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec3Dot3)
   auto test = [&](shz::vec3 vec1,
                   shz::vec3 vec2,
                   shz::vec3 vec3,
                   shz::vec3 vec4)
   {
        shz::vec3 fipr_res, c_res;

        benchmark(&fipr_res, [&]{ return vec1.dot(vec2, vec3, vec4); });

        benchmark(&c_res, [&]{ return shz::vec3 {
                            vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z,
                            vec1.x * vec3.x + vec1.y * vec3.y + vec1.z * vec3.z,
                            vec1.x * vec4.x + vec1.y * vec4.y + vec1.z * vec3.z
                        };
                   });

        return gblFloatEquals(fipr_res.x, c_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, c_res.y, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.z, c_res.z, shz::fipr_max_error);
    };

    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf() }));
GBL_TEST_CASE_END

GBL_TEST_CASE(vec4Dot)
    auto base = shz::vec4(2.0f, 3.0f, 4.0f, 1.0f);
    GBL_TEST_VERIFY(base.dot({1.0f, -1.0f, 1.0f, 2.0f}) == 5.0f);
    GBL_TEST_VERIFY(base.dot((shz_vec4_t){-3.0f, 4.0f, -2.0f, -3.0f}) == -5.0f);
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
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() }));
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

    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() }));
    GBL_TEST_VERIFY(test({ gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() },
                         { gblRandf(), gblRandf(), gblRandf(), gblRandf() }));
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

        return shz_rotated == c_rotated;
    };

    GBL_TEST_VERIFY(test({1.0f, 2.0f }, SHZ_F_PI));
    GBL_TEST_VERIFY(test({gblRandf(), gblRandf()}, gblRandf()));
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

        return shzResult == shz::vec3::from(rayResult);
    };

    GBL_TEST_VERIFY(test({ 1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f },
                         { 0.0f, 0.0f, 0.0f },
                         { 1.0f, 0.0f, 0.0f },
                         { 0.0f, 1.0f, 0.0f }));
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
                  vec3Dot,
                  vec3Dot2,
                  vec3Dot3,
                  vec4Dot,
                  vec4Dot2,
                  vec4Dot3,
                  vec2Rotate,
                  vec3Orthonormalize,
                  vec3Barycenter)