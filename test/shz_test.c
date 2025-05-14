#include "shz_test.h"

int main(int argc, char* argv[]) {
    GblTestScenario *scenario = GblTestScenario_create("sh4zam");

    GblContext_setLogFilter(GBL_CONTEXT(pScenario), GBL_LOG_LEVEL_INFO    |
                                                    GBL_LOG_LEVEL_WARNING |
                                                    GBL_LOG_LEVEL_ERROR);

    return GblTestScenario_exec(scenario, argc, argv);
}