#include "shz_test.h"
#include "shz_intrin.h"

#define GBL_SELF_TYPE shz_intrin_test_suite

GBL_TEST_FIXTURE {
    char dummy;
};

GBL_TEST_INIT()
GBL_TEST_CASE_END

GBL_TEST_FINAL()
GBL_TEST_CASE_END

GBL_TEST_CASE(blah)
GBL_TEST_CASE_END

GBL_TEST_REGISTER(blah)