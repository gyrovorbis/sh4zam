#include "shz_test.h"
#include "sh4zam/shz_matrix.hpp"

#define GBL_SELF_TYPE   shz_matrix_test_suite

GBL_TEST_FIXTURE {
    char dummy;
};

GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

GBL_TEST_CASE(placeholder)
    GBL_TEST_SKIP("Unimplemented!");
GBL_TEST_CASE_END

GBL_TEST_REGISTER(placeholder)