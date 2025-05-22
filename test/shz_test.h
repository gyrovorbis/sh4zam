#ifndef SHZ_TEST_H
#define SHZ_TEST_H

#include <gimbal/gimbal_test.h>

#define SHZ_SCALAR_TEST_SUITE_TYPE   (GBL_TYPEID(shz_scalar_test_suite))
#define SHZ_TRIG_TEST_SUITE_TYPE     (GBL_TYPEID(shz_trig_test_suite))
#define SHZ_VECTOR_TEST_SUITE_TYPE   (GBL_TYPEID(shz_vector_test_suite))
#define SHZ_XMTRX_TEST_SUITE_TYPE    (GBL_TYPEID(shz_xmtrx_test_suite))
#define SHZ_MATRIX_TEST_SUITE_TYPE   (GBL_TYPEID(shz_matrix_test_suite))
#define SHZ_QUAT_TEST_SUITE_TYPE     (GBL_TYPEID(shz_quat_test_suite))

GBL_DECLS_BEGIN

GBL_DERIVE_EMPTY_TYPE(shz_scalar_test_suite, GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_trig_test_suite,   GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_vector_test_suite, GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_xmtrx_test_suite,  GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_matrix_test_suite, GblTestSuite)
GBL_DERIVE_EMPTY_TYPE(shz_quat_test_suite,   GblTestSuite)

GBL_DECLS_END

#endif