#include "shz_test.h"
#include "shz_trig.hpp"
#include "gimbal/gimbal_random.h"
#include <print>
#include <cmath>
#include <kos.h>

#define GBL_SELF_TYPE shz_trig_test_suite

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
    irq_restore(irq_state);
GBL_TEST_CASE_END

GBL_TEST_CASE(sincos_from_radians)
    auto test = [&](float radians) {
        auto sincos = shz::sincos::from_radians(radians);

        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.sinf(), sinf(radians), SHZ_FSCA_ERROR)
        );
        GBL_TEST_VERIFY(
            gblFloatEquals(sincos.cosf(), cosf(radians), SHZ_FSCA_ERROR)
        );
    };
    
    test(0.0f);
    test(SHZ_F_PI);
    test(SHZ_F_PI * -2.0f);
    test(SHZ_F_PI * 3.0f / 2.0f);

    for(unsigned int i = 0; i < 1000; ++i)
        test(gblRandUniform(-SHZ_F_PI * 2.0f, SHZ_F_PI * 2.0f);
GBL_TEST_CASE_END

GBL_TEST_REGISTER(sincos_from_radians)