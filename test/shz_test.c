#include "shz_test.h"

int main(int argc, const char* argv[]) {
    GblTestScenario *scenario = GblTestScenario_create("SH4ZAM");

    GblContext_setLogFilter(GBL_CONTEXT(scenario), GBL_LOG_LEVEL_INFO    |
                                                   GBL_LOG_LEVEL_WARNING |
                                                   GBL_LOG_LEVEL_ERROR);
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_SCALAR_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_TRIG_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_VECTOR_TEST_SUITE_TYPE));

    return GblTestScenario_exec(scenario, argc, argv);
}