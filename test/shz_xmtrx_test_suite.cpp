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

static GBL_RESULT verify_matrix(GblTestSuite* pSelf, std::array<float, 16> mat, double epsilon=0.000100000) {
    GBL_CTX_BEGIN(pSelf);

    std::array<float, 16> transpose;

    for(unsigned r = 0; r < 4; ++r)
        for(unsigned c = 0; c < 4; ++c)
            transpose[c * 4 + r] = mat[r * 4 + c];

    auto error_check = [&](shz_xmtrx_reg_t reg, bool rel=false) {
        auto approx   = shz_xmtrx_read_reg(reg);
        auto precise  = transpose[reg];
        auto relative = gblRelativeError(approx, precise);
        auto absolute = gblAbsoluteError(approx, precise);
        auto result   = ((rel? relative : absolute) < epsilon);

        if(!result) {
            GBL_CTX_ERROR("(SHZ_XMTRX_XF%u == %.9f) != (expected == %.9f)\n"
                          "\t    ERROR: Rel == %.9f, Abs == %.9f, Max == %.9f",
                          static_cast<unsigned>(reg), approx, precise,
                          relative, absolute, epsilon);
            return GBL_RESULT_ERROR;
        } else return GBL_RESULT_SUCCESS;
    };

    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF0 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF1 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF2 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF3 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF4 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF5 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF6 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF7 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF8 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF9 ));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF10));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF11));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF12));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF13));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF14));
    GBL_TEST_CALL(error_check(SHZ_XMTRX_XF15));

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
    shz_xmtrx_init_rotation_x(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, cosf(SHZ_F_PI), -sinf(SHZ_F_PI), 0.0f,
                    0.0f, sinf(SHZ_F_PI), cosf(SHZ_F_PI), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_y)
    randomize_xmtrx_();
    shz_xmtrx_init_rotation_y(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cosf(SHZ_F_PI), 0.0f, sinf(SHZ_F_PI), 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    -sinf(SHZ_F_PI), 0.0f, cosf(SHZ_F_PI), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_z)
    randomize_xmtrx_();
    shz_xmtrx_init_rotation_z(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cosf(SHZ_F_PI), sinf(SHZ_F_PI), 0.0f, 0.0f,
                    sinf(SHZ_F_PI), cosf(SHZ_F_PI), 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_translation)
    randomize_xmtrx_();
    shz_xmtrx_init_translation(10.0f, -20.0f, 30.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 10.0f,
                    0.0f, 1.0f, 0.0f, -20.0f,
                    0.0f, 0.0f, 1.0f, 30.0f,
                    0.0f, 0.0f, 0.0f, 1.0f }));
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