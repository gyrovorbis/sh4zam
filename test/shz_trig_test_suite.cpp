#include "shz_test.h"
#include "sh4zam/shz_trig.hpp"
#include <gimbal/algorithms/gimbal_random.h>
#include <print>
#include <cmath>
#include <kos.h>

#define SHZ_FSCA_ERROR  0.5f
#define GBL_SELF_TYPE   shz_trig_test_suite

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
    std::println("Test suite '{}' took {}ns!",
                 GblTestSuite_name(pSelf), 
                 ns_end - pFixture->ns_start);
    irq_restore(pFixture->irq_state);
GBL_TEST_CASE_END

GBL_TEST_CASE(sincos_from_radians)
    auto test = [&](float radians) {
        GBL_CTX_BEGIN(pCtx);
        auto sincos = shz::sincos::from_radians(radians);
        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.sinf(), sinf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(shz::sinf(radians), sinf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.cosf(), cosf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(shz::cosf(radians), cosf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.tanf(), tanf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(shz::tanf(radians), tanf(radians), SHZ_FSCA_ERROR)
        );
        GBL_CTX_END();
    };
    GBL_TEST_CALL(test(0.0f));
    GBL_TEST_CALL(test(SHZ_F_PI));
    GBL_TEST_CALL(test(SHZ_F_PI * -2.0f));
    GBL_TEST_CALL(test(SHZ_F_PI * 3.0f / 2.0f));
    GBL_TEST_CALL(test(-SHZ_F_PI));
    GBL_TEST_CALL(test(-SHZ_F_PI * -2.777f));
    GBL_TEST_CALL(test(-SHZ_F_PI * 3.41f / 45.656f));
GBL_TEST_CASE_END

GBL_TEST_CASE(sincos_from_degrees)
    auto test = [&](float radians) {
        GBL_CTX_BEGIN(pCtx);
        float degrees = shz::rad_to_deg(radians);
        auto sincos = shz::sincos::from_degrees(degrees);
        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.sinf(), sinf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(shz::sinf_deg(degrees), sinf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.cosf(), cosf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(shz::cosf_deg(degrees), sinf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.tanf(), tanf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(shz::tanf_deg(degrees), tanf(radians), SHZ_FSCA_ERROR)
        );
        GBL_CTX_END();
    };
    GBL_TEST_CALL(test(0.0f));
    GBL_TEST_CALL(test(SHZ_F_PI));
    GBL_TEST_CALL(test(SHZ_F_PI * -2.0f));
    GBL_TEST_CALL(test(SHZ_F_PI * 3.0f / 2.0f));
    GBL_TEST_CALL(test(-SHZ_F_PI));
    GBL_TEST_CALL(test(-SHZ_F_PI * -2.777f));
    GBL_TEST_CALL(test(-SHZ_F_PI * 3.41f / 45.656f));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(sincos_from_radians,
                  sincos_from_degrees)