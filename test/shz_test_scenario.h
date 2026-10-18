#ifndef SHZ_TEST_SCENARIO_H
#define SHZ_TEST_SCENARIO_H

#include <gimbal/gimbal_test.h>
#include <gimbal/meta/classes/gimbal_enum.h>

#define SHZ_TEST_SCENARIO_TYPE       (GBL_TYPEID(shz_test_scenario))

#define SHZ_BENCHMARK_RESULT_TYPE    (GBL_TYPEID(shz_benchmark_result))
#define SHZ_BENCHMARK_RESULT_GROUP   (GblQuark_fromStatic("Benchmark Totals"))

#define SHZ_SCALAR_TEST_SUITE_TYPE   (GBL_TYPEID(shz_scalar_test_suite))
#define SHZ_TRIG_TEST_SUITE_TYPE     (GBL_TYPEID(shz_trig_test_suite))
#define SHZ_VECTOR_TEST_SUITE_TYPE   (GBL_TYPEID(shz_vector_test_suite))
#define SHZ_QUAT_TEST_SUITE_TYPE     (GBL_TYPEID(shz_quat_test_suite))
#define SHZ_XMTRX_TEST_SUITE_TYPE    (GBL_TYPEID(shz_xmtrx_test_suite))
#define SHZ_MATRIX_TEST_SUITE_TYPE   (GBL_TYPEID(shz_matrix_test_suite))
#define SHZ_MEM_TEST_SUITE_TYPE      (GBL_TYPEID(shz_mem_test_suite))
#define SHZ_COMPLEX_TEST_SUITE_TYPE  (GBL_TYPEID(shz_complex_test_suite))

GBL_DECLS_BEGIN

GBL_ENUM(shz_benchmark_result,
    (SHZ_BENCHMARK_EQUAL,  "EQUAL",  0),
    (SHZ_BENCHMARK_GAINZ,  "GAINZ",  1),
    (SHZ_BENCHMARK_APPROX, "APPROX", 2),
    (SHZ_BENCHMARK_SKETCH, "SKETCH", 3),
    (SHZ_BENCHMARK_LOSSEZ, "LOSSEZ", 4)
)

GBL_DERIVE_EMPTY_TYPE(shz_test_scenario, GblTestScenario)

GBL_DERIVE_EMPTY_TYPE(shz_scalar_test_suite,  GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_trig_test_suite,    GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_vector_test_suite,  GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_quat_test_suite,    GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_xmtrx_test_suite,   GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_matrix_test_suite,  GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_mem_test_suite,     GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_complex_test_suite, GblTestSuite)

GBL_DECLS_END

#endif