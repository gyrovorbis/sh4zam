#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_scalar.hpp"

#include <print>
#include <array>

#define GBL_SELF_TYPE shz_scalar_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

GBL_TEST_CASE(min)
    GBL_TEST_VERIFY((shz::fminf(-3.0f, 12.0f)) == -3.0f);
    GBL_TEST_VERIFY((shz::fminf(8, 8)) == 8);
GBL_TEST_CASE_END

GBL_TEST_CASE(max)
    GBL_TEST_VERIFY(shz::fmaxf(-3.0f, 12.0f) == 12.0f);
    GBL_TEST_VERIFY(shz::fmaxf(8, 8) == 8);
GBL_TEST_CASE_END

GBL_TEST_CASE(clamp)
    GBL_TEST_VERIFY(shz::clampf(-3.0f, 0.0f, 1.0f) == 0.0f);
    GBL_TEST_VERIFY(shz::clampf(12, -13, 33) == 12);
GBL_TEST_CASE_END

GBL_TEST_CASE(norm)
    GBL_TEST_COMPARE(shz::normalizef(-3.0f, 0.0f, 1.0f), -3.0f);
    GBL_TEST_VERIFY(shz::equalf(shz::normalizef(12.0f, -12.0f, 24.0f), 0.66666666666f));
GBL_TEST_CASE_END

GBL_TEST_CASE(floorf)
    GBL_TEST_VERIFY(shz::floorf( 1.8f) ==  1.0f);
    GBL_TEST_VERIFY(shz::floorf( 1.3f) ==  1.0f);
    GBL_TEST_VERIFY(shz::floorf( 0.8f) ==  0.0f);
    GBL_TEST_VERIFY(shz::floorf( 0.3f) ==  0.0f);
    GBL_TEST_VERIFY(shz::floorf( 0.0f) ==  0.0f);
    GBL_TEST_VERIFY(shz::floorf(-0.3f) == -1.0f);
    GBL_TEST_VERIFY(shz::floorf(-0.8f) == -1.0f);
    GBL_TEST_VERIFY(shz::floorf(-1.3f) == -2.0f);
    GBL_TEST_VERIFY(shz::floorf(-1.8f) == -2.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(ceilf)
    GBL_TEST_VERIFY(shz::ceilf( 1.8f) ==  2.0f);
    GBL_TEST_VERIFY(shz::ceilf( 1.3f) ==  2.0f);
    GBL_TEST_VERIFY(shz::ceilf( 0.8f) ==  1.0f);
    GBL_TEST_VERIFY(shz::ceilf( 0.3f) ==  1.0f);
    GBL_TEST_VERIFY(shz::ceilf( 0.0f) ==  0.0f);
    GBL_TEST_VERIFY(shz::ceilf(-0.3f) ==  0.0f);
    GBL_TEST_VERIFY(shz::ceilf(-0.8f) ==  0.0f);
    GBL_TEST_VERIFY(shz::ceilf(-1.3f) == -1.0f);
    GBL_TEST_VERIFY(shz::ceilf(-1.8f) == -1.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(fmacf)
    GBL_TEST_VERIFY(shz::fmaf(1.0f,  2.0f,  3.0f) ==  5.0f);
    GBL_TEST_VERIFY(shz::fmaf(0.0f,  1.0f,  4.0f) ==  4.0f);
    GBL_TEST_VERIFY(shz::fmaf(3.0f, -1.0f, -5.0f) == -8.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(lerpf)
    GBL_TEST_VERIFY(shz::lerpf(0.0f,    1.0f, 0.5f) ==   0.5f);
    GBL_TEST_VERIFY(shz::lerpf(0.0f,    0.0f, 1.0f) ==   0.0f);
    GBL_TEST_VERIFY(shz::lerpf(1.0f,   -1.0f, 0.5f) ==   0.0f);
    GBL_TEST_VERIFY(shz::lerpf(-12.0f, 33.0f, 1.0f) ==  33.0f);
    GBL_TEST_VERIFY(shz::lerpf(12.0f, -33.0f, 1.0f) == -33.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(barycentric_lerpf)
    // A: u=0, v=0, w=1 -> should return 'a' (value 10)
    GBL_TEST_VERIFY(shz_barycentric_lerpf(10.0f, 20.0f, 30.0f, 0.0f, 0.0f) == 10.0f);
    // B: u=1, v=0, w=0 -> should return 'b' (value 20)
    GBL_TEST_VERIFY(shz_barycentric_lerpf(10.0f, 20.0f, 30.0f, 1.0f, 0.0f) == 20.0f);
    // C: u=0, v=1, w=0 -> should return 'c' (value 30)
    GBL_TEST_VERIFY(shz_barycentric_lerpf(10.0f, 20.0f, 30.0f, 0.0f, 1.0f) == 30.0f);

    // Test center point: u=1/3, v=1/3, w=1/3 -> should be average
    {
        float u = 1.0f/3.0f;
        float v = 1.0f/3.0f;
        float expected = (10.0f + 20.0f + 30.0f) / 3.0f;
        GBL_TEST_VERIFY(shz_barycentric_lerpf(10.0f, 20.0f, 30.0f, u, v) == expected);
    }

    // Midpoint A-B: u=0.5, v=0, w=0.5 -> should be (a+b)/2
    GBL_TEST_VERIFY(shz_barycentric_lerpf(10.0f, 20.0f, 30.0f, 0.5f, 0.0f) == 15.0f);
    // Midpoint B-C: u=0.5, v=0.5, w=0 -> should be (b+c)/2
    GBL_TEST_VERIFY(shz_barycentric_lerpf(10.0f, 20.0f, 30.0f, 0.5f, 0.5f) == 25.0f);
    // Midpoint A-C: u=0, v=0.5, w=0.5 -> should be (a+c)/2
    GBL_TEST_VERIFY(shz_barycentric_lerpf(10.0f, 20.0f, 30.0f, 0.0f, 0.5f) == 20.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(inv_sqrtf)
   auto test = [&](float value) {
        return gblFloatEquals(shz::inv_sqrtf(value),
                              1.0f / sqrtf(value),
                              0.01f);
    
   };
   GBL_TEST_VERIFY(test(333333.33f));
   GBL_TEST_VERIFY(test(1.001f));
   GBL_TEST_VERIFY(test(1.0f));
   //GBL_TEST_VERIFY(test(0.55f));
   //GBL_TEST_VERIFY(test(0.01f));
   //GBL_TEST_VERIFY(test(-0.001f));
   //GBL_TEST_VERIFY(test(-0.55f));
   GBL_TEST_VERIFY(test(-1.0f));
   GBL_TEST_VERIFY(test(-1.001f));
   GBL_TEST_VERIFY(test(-33333.33f));
GBL_TEST_CASE_END

GBL_TEST_CASE(invf_fsrra)
    auto test = [&](float value) {
        return gblFloatEquals(shz::invf_fsrra(value),
                              1.0f / sqrtf(value * value),
                              0.01f);
   };
   GBL_TEST_VERIFY(test(333333.33f));
   GBL_TEST_VERIFY(test(1.001f));
   GBL_TEST_VERIFY(test(1.0f));
   //GBL_TEST_VERIFY(test(0.55f));
   //GBL_TEST_VERIFY(test(0.01f));
   //GBL_TEST_VERIFY(test(-0.001f));
   //GBL_TEST_VERIFY(test(-0.55f));
   GBL_TEST_VERIFY(test(-1.0f));
   GBL_TEST_VERIFY(test(-1.001f));
   //GBL_TEST_VERIFY(test(-33333.33f));
GBL_TEST_CASE_END

GBL_TEST_CASE(divf_fsrra)
    auto test = [&](float num, float denom) {
        return gblFloatEquals(shz::divf_fsrra(num, denom),
                              num / sqrtf(denom * denom),
                              0.001f);
   };
   GBL_TEST_VERIFY(test(1.0f, 333333.33f));
   //GBL_TEST_VERIFY(test(0.001f, 1.001f));
   GBL_TEST_VERIFY(test(0.0f, 1.0f));
   //GBL_TEST_VERIFY(test(-3.0f, 0.55f));
   //GBL_TEST_VERIFY(test(0.01f, 0.01f));
   //GBL_TEST_VERIFY(test(9999.0f, -0.001f));
   //GBL_TEST_VERIFY(test(0.55f, -0.55f));
   //GBL_TEST_VERIFY(test(-1.0f, 0.3333f));
   //GBL_TEST_VERIFY(test(-1.001f, 0.666f));
   //GBL_TEST_VERIFY(test(-33333.33f, 0.0001f));
GBL_TEST_CASE_END

GBL_TEST_CASE(invf)
    auto test = [&](float value) {
        return gblFloatEquals(shz::invf(value),
                              1.0f / value,
                              0.001f);
   };
   GBL_TEST_VERIFY(test(333333.33f));
   GBL_TEST_VERIFY(test(1.001f));
   GBL_TEST_VERIFY(test(1.0f));
   GBL_TEST_VERIFY(test(0.55f));
   GBL_TEST_VERIFY(test(0.01f));
   GBL_TEST_VERIFY(test(-0.001f));
   GBL_TEST_VERIFY(test(-0.55f));
   GBL_TEST_VERIFY(test(-1.0f));
   GBL_TEST_VERIFY(test(-1.001f));
   GBL_TEST_VERIFY(test(-33333.33f));
GBL_TEST_CASE_END

GBL_TEST_CASE(dot6f)
   auto test = [&](std::array<float, 3> vec1,
                   std::array<float, 3> vec2)
   {
        float fipr_res = shz::dot6f(vec1[0], vec1[1], vec1[2],
                                    vec2[0], vec2[1], vec2[2]);
        float c_res    = vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
        return gblFloatEquals(fipr_res, c_res, shz::fipr_max_error);
    };
    GBL_TEST_VERIFY(test({ 1.0f,   2.0f, 3.0f   },
                         { 1.0f,   2.0f, 3.0f   }));
    GBL_TEST_VERIFY(test({-1.0f,   2.0f, 3.333f },
                         { 1.0f, -22.0f, 3.0f   }));
    GBL_TEST_VERIFY(test({-1.0f,   0.0f, 3.333f },
                         { 1.0f, -22.0f, 3.0f   }));
GBL_TEST_CASE_END

GBL_TEST_CASE(dot8f)
   auto test = [&](std::array<float, 4> vec1,
                   std::array<float, 4> vec2)
   {
        float fipr_res = shz::dot8f(vec1[0], vec1[1], vec1[2], vec1[3],
                                    vec2[0], vec2[1], vec2[2], vec2[3]);    
        float c_res    = vec1[0] * vec2[0] + vec1[1] * vec2[1] +
                         vec1[2] * vec2[2] + vec1[3] * vec2[3];
        return gblFloatEquals(fipr_res, c_res, shz::fipr_max_error);
    };
    GBL_TEST_VERIFY(test({ 1.0f,   2.0f, 3.0f,    4.0f   }, 
                         { 1.0f,   2.0f, 3.0f,    4.0f   }));
    GBL_TEST_VERIFY(test({-1.0f,   2.0f, 3.333f,  4.0f   }, 
                         { 1.0f, -22.0f, 3.0f,    4.5555f}));
    GBL_TEST_VERIFY(test({-1.0f,   0.0f, 3.333f, -4.4345f},
                         { 1.0f, -22.0f, 3.0f,    4.5555f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(mag_sqr3f)
   auto test = [&](std::array<float, 3> vec1) {
        float fipr_res = shz::mag_sqr3f(vec1[0], vec1[1], vec1[2]);
        float c_res    = vec1[0] * vec1[0] + vec1[1] * vec1[1] + vec1[2] * vec1[2];
        return gblFloatEquals(fipr_res, c_res, shz::fipr_max_error);
    };
    GBL_TEST_VERIFY(test({ 1.0f,   2.0f, 3.0f   }));
    GBL_TEST_VERIFY(test({ 1.0f, -22.0f, 3.0f   }));
    GBL_TEST_VERIFY(test({-1.0f,   0.0f, 3.333f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(mag_sqr4f)
   auto test = [&](std::array<float, 4> vec1) {
        float fipr_res = shz::mag_sqr4f(vec1[0], vec1[1], vec1[2], vec1[3]);    
        float c_res    = vec1[0] * vec1[0] + vec1[1] * vec1[1] +
                         vec1[2] * vec1[2] + vec1[3] * vec1[3];
        return gblFloatEquals(fipr_res, c_res, shz::fipr_max_error);
    };
    GBL_TEST_VERIFY(test({ 1.0f,   2.0f, 3.0f,    4.0f   }));
    GBL_TEST_VERIFY(test({ 1.0f, -22.0f, 3.0f,    4.5555f}));
    GBL_TEST_VERIFY(test({-1.0f,   0.0f, 3.333f, -4.4345f}));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(min,
                  max,
                  clamp,
                  norm,
                  floorf,
                  ceilf,
                  fmacf,
                  lerpf,
                  barycentric_lerpf,
                  inv_sqrtf,
                  invf_fsrra,
                  divf_fsrra,
                  invf,
                  dot6f,
                  dot8f,
                  mag_sqr3f,
                  mag_sqr4f)
