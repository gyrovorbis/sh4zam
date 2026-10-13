#include "shz_test.h"

static GBL_RESULT shz_test_constructed_(GblObject* pSelf) {
    GBL_CTX_BEGIN(NULL);

    GBL_VCALL_DEFAULT(GblObject, pFnConstructed, pSelf);

    GblObject_setName(pSelf, "SH4ZAM");

    GblTestScenario* scenario = GBL_TEST_SCENARIO(pSelf);

    GblContext_setLogFilter(GBL_CONTEXT(scenario), GBL_LOG_LEVEL_INFO    |
                                                   GBL_LOG_LEVEL_WARNING |
                                                   GBL_LOG_LEVEL_ERROR);

    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_SCALAR_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_TRIG_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_VECTOR_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_QUAT_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_XMTRX_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_MATRIX_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_MEM_TEST_SUITE_TYPE));
    GblTestScenario_enqueueSuite(scenario,
                                 GblTestSuite_create(SHZ_COMPLEX_TEST_SUITE_TYPE));

    GBL_CTX_END();
}

static GBL_RESULT shz_test_class_init_(GblClass* pClass, const void* pData) {
    GBL_UNUSED(pData);
    GBL_OBJECT_CLASS(pClass)->pFnConstructed = shz_test_constructed_;
    return GBL_RESULT_SUCCESS;
}

GBL_EXPORT GblType shz_test_type(void) {
    static GblType type = GBL_INVALID_TYPE;
    static const GblTypeInfo info = {
        .pFnClassInit = shz_test_class_init_,
        .classSize    = sizeof(shz_testClass),
        .instanceSize = sizeof(shz_test)
    };

    if(type == GBL_INVALID_TYPE) {
        type = GblType_register(GblQuark_internStatic("shz_test"),
                               GBL_TEST_SCENARIO_TYPE,
                               &info,
                               GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }
    return type;
}
