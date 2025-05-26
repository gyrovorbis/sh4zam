#include "shz_test.h"

#include "sh4zam/shz_vector.hpp"

#include <kos.h>
#include <print>

#define GBL_SELF_TYPE shz_vector_test_suite

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

GBL_TEST_CASE(vec2Construct)
    auto vec1 = shz::vec2();
    auto vec2 = shz::vec2(0.0f);
    auto vec3 = shz::vec2(1.0f, 2.0f);
    auto vec4 = shz::vec2((shz_vec2_t) { 3.0f, 4.0f });
    auto vec5 = shz::vec2((shz_vec2_t) { 5.0f, 6.0f });
GBL_TEST_CASE_END

GBL_TEST_REGISTER(vec2Construct)