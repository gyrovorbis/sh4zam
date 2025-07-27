#include "shz_test.h"

#include "sh4zam/shz_xmtrx.hpp"

#include <gimbal/gimbal_algorithms.h>
#include <print>

#define GBL_SELF_TYPE shz_xmtrx_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

static void randomize_xmtrx_() {
    for(unsigned reg = SHZ_XMTRX_XF0; reg < SHZ_XMTRX_XF15; ++reg)
        shz_xmtrx_write_reg(static_cast<shz_xmtrx_reg_t>(reg), gblRandUniform(FLT_MIN, FLT_MAX));
}

static GBL_RESULT verify_matrix(GblTestSuite* pSelf, std::array<float, 16> mat, float epsilon=FLT_EPSILON) {
    GBL_CTX_BEGIN(pSelf);

    for(unsigned reg = SHZ_XMTRX_XF0; reg < SHZ_XMTRX_XF15; ++reg)
        GBL_TEST_VERIFY(
            gblFloatEquals(
                shz_xmtrx_read_reg(static_cast<shz_xmtrx_reg_t>(reg)),
                mat[reg],
                epsilon));

    GBL_CTX_END();
}

GBL_TEST_CASE(register_accessors)
    shz_xmtrx_write_reg(SHZ_XMTRX_XF0,  100.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF1,  101.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF2,  102.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF3,  103.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF4,  104.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF5,  100.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF6,  101.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF7,  102.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF8,  103.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF9,  104.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF10, 110.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF11, 111.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF12, 112.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF13, 113.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF14, 114.0f);
    shz_xmtrx_write_reg(SHZ_XMTRX_XF15, 115.0f);

    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF0)  == 100.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF1)  == 101.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF2)  == 102.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF3)  == 103.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF4)  == 104.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF5)  == 100.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF6)  == 101.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF7)  == 102.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF8)  == 103.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF9)  == 104.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF10) == 110.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF11) == 111.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF12) == 112.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF13) == 113.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF14) == 114.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF15) == 115.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(init_identity)
    randomize_xmtrx_();
    shz_xmtrx_init_identity();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_diagonal)
    randomize_xmtrx_();
    shz_xmtrx_init_diagonal(-1.0f, 2.0f, 3.0f, 4.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { -1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 2.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 3.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 4.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_scale)
    randomize_xmtrx_();
    shz_xmtrx_init_scale(-2.0f, -3.0f, -4.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { -2.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, -3.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, -4.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_x)
    randomize_xmtrx_();
    shz_xmtrx_init_rotation_x(3.14f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, cos(3.14f), sin(3.14f), 0.0f,
                    0.0f, -sin(3.14f), cos(3.14f), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_y)
    randomize_xmtrx_();
    shz_xmtrx_init_rotation_y(3.14f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cos(3.14f), 0.0f, sin(3.14f), 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    -sin(3.14f), 0.0f, cos(3.14f), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_z)
    randomize_xmtrx_();
    shz_xmtrx_init_rotation_z(3.14f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cos(3.14f), sin(3.14f), 0.0f, 0.0f,
                    sin(3.14f), cos(3.14f), 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_translation)
    randomize_xmtrx_();
    shz_xmtrx_init_translation(10.0f, -20.0f, 30.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    10.0f, -20.0f, 30.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_symmetric_skew)
    randomize_xmtrx_();
    shz_xmtrx_init_symmetric_skew(-1.5f, 2.7f, -3.4f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { 0.0f, -3.4f, -2.7f, 0.0f,
                   3.4f, 0.0f, -1.5f, 0.0f,
                   2.7f, 1.5f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(register_accessors,
                  init_identity,
                  init_diagonal,
                  init_scale,
                  init_rotation_x,
                  init_rotation_y,
                  init_rotation_z,
                  init_translation,
                  init_symmetric_skew)