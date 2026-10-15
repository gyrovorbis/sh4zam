#include <sh4zam/shz_sh4zam.h>
#include "shz_test_scenario.h"

#include <assert.h>

int main(int argc, const char* argv[]) {
    assert(SHZ_VERSION == shz_version_linked());

    return GblTestScenario_exec(GBL_TEST_SCENARIO(GBL_NEW(shz_test_scenario)), argc, argv);
}
