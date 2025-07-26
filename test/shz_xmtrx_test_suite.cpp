#include "shz_test.h"

#include "sh4zam/shz_xmtrx.hpp"

#include <kos.h>
#include <print>

#define GBL_SELF_TYPE shz_xmtrx_test_suite

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

static inline random_float() {
    return gblRandUniform(FLT_MIN, FLT_MAX);
}

static void randomize_matrix() {
    shz_xmtrx_set_reg( 0, random_float());
    shz_xmtrx_set_reg( 1, random_float());
    shz_xmtrx_set_reg( 2, random_float());
    shz_xmtrx_set_reg( 3, random_float());
    shz_xmtrx_set_reg( 4, random_float());
    shz_xmtrx_set_reg( 5, random_float());
    shz_xmtrx_set_reg( 6, random_float());
    shz_xmtrx_set_reg( 7, random_float());
    shz_xmtrx_set_reg( 8, random_float());
    shz_xmtrx_set_reg( 9, random_float());
    shz_xmtrx_set_reg(10, random_float());
    shz_xmtrx_set_reg(11, random_float());
    shz_xmtrx_set_reg(12, random_float());
    shz_xmtrx_set_reg(13, random_float());
    shz_xmtrx_set_reg(14, random_float());
    shz_xmtrx_set_reg(15, random_float());
}

static GBL_RESULT verify_matrix(GblTestSuite* pSelf, std::array<float, 16> mat, float epsilon=FLT_EPSILON) {
    GBL_CTX_BEGIN(pSelf);
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 0), mat.elem[ 0], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 1), mat.elem[ 1], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 2), mat.elem[ 2], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 3), mat.elem[ 3], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 4), mat.elem[ 4], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 5), mat.elem[ 5], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 6), mat.elem[ 6], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 7), mat.elem[ 7], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 8), mat.elem[ 8], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg( 9), mat.elem[ 9], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg(10), mat.elem[10], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg(11), mat.elem[11], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg(12), mat.elem[12], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg(13), mat.elem[13], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg(14), mat.elem[14], epsilon));
    GBL_TEST_VERIFY(gblFloatEquals(shz_xmtrx_get_reg(15), mat.elem[15], epsilon));
    GBL_CTX_END();
}

GBL_TEST_CASE(register_access)
    shz_xmtrx_set_reg( 0, 100.0f);
    shz_xmtrx_set_reg( 1, 101.0f);
    shz_xmtrx_set_reg( 2, 102.0f);
    shz_xmtrx_set_reg( 3, 103.0f);
    shz_xmtrx_set_reg( 4, 104.0f);
    shz_xmtrx_set_reg( 5, 100.0f);
    shz_xmtrx_set_reg( 6, 101.0f);
    shz_xmtrx_set_reg( 7, 102.0f);
    shz_xmtrx_set_reg( 8, 103.0f);
    shz_xmtrx_set_reg( 9, 104.0f);
    shz_xmtrx_set_reg(10, 110.0f);
    shz_xmtrx_set_reg(11, 111.0f);
    shz_xmtrx_set_reg(12, 112.0f);
    shz_xmtrx_set_reg(13, 113.0f);
    shz_xmtrx_set_reg(14, 114.0f);
    shz_xmtrx_set_reg(15, 115.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 0) == 100.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 1) == 101.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 2) == 102.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 3) == 103.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 4) == 104.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 5) == 105.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 6) == 106.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 7) == 107.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 8) == 108.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg( 9) == 109.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg(10) == 110.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg(11) == 111.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg(12) == 112.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg(13) == 113.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg(14) == 114.0f);
    GBL_TEST_VERIFY(shz_xmtrx_get_reg(15) == 115.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(set_identity)
    randomize_matrix();
    shz_xmtrx_set_identity();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                  shz_matrix_4x4_t{ 1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f});
GBL_TEST_CASE_END

GBL_TEST_CASE(set_diagonal)
    randomize_matrix();
    shz_xmtrx_set_diagonal(-1.0f, 2.0f, 3.0f, 4.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                  { -1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 2.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 3.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 4.0f});
GBL_TEST_CASE_END

GBL_TEST_CASE(set_scale)
    randomize_matrix();
    shz_xmtrx_set_diagonal(-2.0f, -3.0f, -4.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                  { -2.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, -3.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, -4.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f});
GBL_TEST_CASE_END

GBL_TEST_CASE(set_rotation_x)
    randomize_matrix();
    shz_xmtrx_set_rotation_x(3.14f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, cos(3.14f), sin(3.14f), 0.0f,
                    0.0f, -sin(3.14f), cos(3.14f), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f});
GBL_TEST_CASE_END

GBL_TEST_CASE(set_rotation_y)
    randomize_matrix();
    shz_xmtrx_set_rotation_y(3.14f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                  { cos(3.14), 0.0f, sin(3.14), 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    -sin(3.14), 0.0f, cos(3.14f), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f});
GBL_TEST_CASE_END

GBL_TEST_CASE(set_rotation_z)
    randomize_matrix();
    shz_xmtrx_set_rotation_z(3.14f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                  { cos(3.14), sin(3.14), 0.0f, 0.0f,
                    sin(3.14), cos(3.14f), 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 0.0f, 1.0f });
GBL_TEST_CASE_END

GBL_TEST_CASE(set_translation)
    randomize_matrix();
    shz_xmtrx_set_translation(10.0f, -20.0f, 30.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    10.0f, -20.0f, 30.0f, 1.0f });
GBL_TEST_CASE_END

GBL_TEST_CASE(set_symmetric_skew)
    randomize_matrix();
    shz_xmtrx_set_symmetric_skew(-1.5f, 2.7f, -3.4f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME),
                 { 0.0f, -3.4f, -2.7f, 0.0f,
                   3.4f, 0.0f, -1.5f, 0.0f,
                   2.7f, 1.5f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f });
GBL_TEST_CASE_END

GBL_TEST_REGISTER(register_accessors,
                  set_identity,
                  set_diagonal,
                  set_scale,
                  set_rotation_x,
                  set_rotation_y,
                  set_rotation_z,
                  set_translation,
                  set_symmetric_skew,
                  )