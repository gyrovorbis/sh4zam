#include "shz_test.h"
#include "sh4zam/shz_mem.hpp"

#define GBL_SELF_TYPE   shz_mem_test_suite

#define THRASH_CACHE
#define BUFFER_ALIGN     64
#define BUFFER_KB        8

#ifdef THRASH_CACHE
#   define BUFFER_ELEMENTS  (BUFFER_KB * 1024 - 32)
#else
#   define BUFFER_ELEMENTS  (BUFFER_KB * 1024)
#endif

GBL_TEST_FIXTURE {
    uint8_t top[1024];
    alignas(BUFFER_ALIGN) uint8_t src[BUFFER_ELEMENTS + BUFFER_ALIGN];
    alignas(BUFFER_ALIGN) uint8_t dst[BUFFER_ELEMENTS + BUFFER_ALIGN];
    uint8_t bottom[1024];
};

GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

GBL_TEST_CASE(placeholder)
    GBL_TEST_SKIP("Unimplemented!");
GBL_TEST_CASE_END

GBL_TEST_REGISTER(placeholder)