#include "shz_test.h"
#include "shz_scalar.hpp"

#include <print>
#include <array>

#include <kos.h>

#define GBL_SELF_TYPE shz_scalar_test_suite

GBL_TEST_FIXTURE {
    uint64_t ns_start;
    int      irq_state;   
};

GBL_TEST_INIT()
    pFixture->irq_state = irq_disable();
    pFixture->ns_start  = perf_cntr_timer_ns();
GBL_TEST_CASE_END

GBL_TEST_FINAL()
    uint64_t ns_end = perf_cntr_timer_ns();
    std::println("Test suite took {}ns!", 
                 ns_end - pFixture->ns_start);
    irq_restore(pFixture->irq_state);
GBL_TEST_CASE_END

GBL_TEST_CASE(min)
    GBL_TEST_VERIFY((shz::min(-3.0f, 12.0f)) == -3.0f);
    GBL_TEST_VERIFY((shz::min('b', 'a')) == 'a');
    GBL_TEST_VERIFY((shz::min(8, 8)) == 8);
GBL_TEST_CASE_END

GBL_TEST_CASE(max)
    GBL_TEST_VERIFY(shz::max(-3.0f, 12.0f) == 12.0f);
    GBL_TEST_VERIFY(shz::max('b', 'a') == 'b');
    GBL_TEST_VERIFY(shz::max(8, 8) == 8);
GBL_TEST_CASE_END

GBL_TEST_CASE(clamp)
    GBL_TEST_VERIFY(shz::clamp(-3.0f, 0.0f, 1.0f) == 0.0f);
    GBL_TEST_VERIFY(shz::clamp('g', 'a', 'c') == 'c');
    GBL_TEST_VERIFY(shz::clamp(12, -13, 33) == 12);
GBL_TEST_CASE_END

GBL_TEST_CASE(norm)
    GBL_TEST_VERIFY(shz::norm(-3.0f, 0.0f, 1.0f) == 0.0f);
    GBL_TEST_VERIFY(shz::norm('g', 'a', 'c') == 1.0f);
    GBL_TEST_VERIFY(gblFloatEquals(shz::norm(12.0f, -12.0f, 24.0f), 0.66666666666f, FLT_EPSILON));
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
    GBL_TEST_VERIFY(shz::fmacf(1.0f,  2.0f,  3.0f) ==  5.0f);
    GBL_TEST_VERIFY(shz::fmacf(0.0f,  1.0f,  4.0f) ==  4.0f);
    GBL_TEST_VERIFY(shz::fmacf(3.0f, -1.0f, -5.0f) == -8.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(lerpf)
    GBL_TEST_VERIFY(shz::lerpf(0.0f,    1.0f, 0.5f) ==   0.5f);
    GBL_TEST_VERIFY(shz::lerpf(0.0f,    0.0f, 1.0f) ==   0.0f);
    GBL_TEST_VERIFY(shz::lerpf(1.0f,   -1.0f, 0.5f) ==   0.0f);
    GBL_TEST_VERIFY(shz::lerpf(-12.0f, 33.0f, 1.0f) ==  33.0f);
    GBL_TEST_VERIFY(shz::lerpf(12.0f, -33.0f, 1.0f) == -33.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(barycentric_lerpf)

GBL_TEST_CASE_END

GBL_TEST_CASE(inverse_sqrtf)
   auto test = [&](float value) {
        return gblFloatEquals(shz::inverse_sqrtf(value),
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

GBL_TEST_CASE(inverse_posf)
    auto test = [&](float value) {
        return gblFloatEquals(shz::inverse_posf(value),
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

GBL_TEST_CASE(div_posf)
    auto test = [&](float num, float denom) {
        return gblFloatEquals(shz::div_posf(num, denom),
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

GBL_TEST_CASE(fast_invf)
    auto test = [&](float value) {
        return gblFloatEquals(shz::fast_invf(value),
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
                  inverse_sqrtf,
                  inverse_posf,
                  div_posf,
                  fast_invf,
                  dot8f,
                  mag_sqr4f)