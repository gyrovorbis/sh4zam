#include "shz_test.h"
#include "sh4zam/shz_xmtrx.hpp"

#include <gimbal/gimbal_algorithms.h>
#include <print>

#define GBL_SELF_TYPE shz_xmtrx_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

std::array<float, 16> transpose(std::array<float, 16> mat) {
    std::array<float, 16> transpose;

    for(unsigned r = 0; r < 4; ++r)
        for(unsigned c = 0; c < 4; ++c)
            transpose[c * 4 + r] = mat[r * 4 + c];

    return transpose;
}

static void randomize_xmtrx_() {
    for(unsigned reg = SHZ_XMTRX_XF0; reg < SHZ_XMTRX_XF15; ++reg)
        shz_xmtrx_write_reg(static_cast<shz_xmtrx_reg_t>(reg), gblRandUniform(FLT_MIN, FLT_MAX));
}

static GBL_RESULT verify_matrix(GblTestSuite* pSelf, std::array<float, 16> mat, double epsilon=0.000100000) {
    mat = transpose(mat);

    GBL_CTX_BEGIN(pSelf);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF0 ), mat[SHZ_XMTRX_XF0 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF1 ), mat[SHZ_XMTRX_XF1 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF2 ), mat[SHZ_XMTRX_XF2 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF3 ), mat[SHZ_XMTRX_XF3 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF4 ), mat[SHZ_XMTRX_XF4 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF5 ), mat[SHZ_XMTRX_XF5 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF6 ), mat[SHZ_XMTRX_XF6 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF7 ), mat[SHZ_XMTRX_XF7 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF8 ), mat[SHZ_XMTRX_XF8 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF9 ), mat[SHZ_XMTRX_XF9 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF10), mat[SHZ_XMTRX_XF10], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF11), mat[SHZ_XMTRX_XF11], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF12), mat[SHZ_XMTRX_XF12], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF13), mat[SHZ_XMTRX_XF13], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF14), mat[SHZ_XMTRX_XF14], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz_xmtrx_read_reg(SHZ_XMTRX_XF15), mat[SHZ_XMTRX_XF15], epsilon, GBL_TEST_ERROR_ABSOLUTE);
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

    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF0 ) == 100.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF1 ) == 101.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF2 ) == 102.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF3 ) == 103.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF4 ) == 104.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF5 ) == 100.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF6 ) == 101.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF7 ) == 102.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF8 ) == 103.0f);
    GBL_TEST_VERIFY(shz_xmtrx_read_reg(SHZ_XMTRX_XF9 ) == 104.0f);
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

GBL_TEST_CASE(load_4x4)
    randomize_xmtrx_();
    shz_matrix_4x4_t mat4 = {
        .left    = { -1.0f,   2.0f,  3.0f,  8.0f },
        .up      = {  4.0f,  -5.0f,  6.0f,  7.0f },
        .forward = {  7.0f,   8.0f,  9.0f, 10.0f },
        .pos     = { 11.0f, -12.0f, 13.0f, 14.0f }
    };
    shz_xmtrx_load_4x4(&mat4);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose({
                    -1.0f,   2.0f,  3.0f,  8.0f,
                     4.0f,  -5.0f,  6.0f,  7.0f,
                     7.0f,   8.0f,  9.0f, 10.0f,
                    11.0f, -12.0f, 13.0f, 14.0f })));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_4x4_unaligned)
    randomize_xmtrx_();
    alignas(4) std::array<float, 16> array = {
        -1.0f,   2.0f,  3.0f,  8.0f,
         4.0f,  -5.0f,  6.0f,  7.0f,
         7.0f,   8.0f,  9.0f, 10.0f,
        11.0f, -12.0f, 13.0f, 14.0f
    };
    shz_xmtrx_load_4x4_unaligned(array.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose(array)));
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_x)
    randomize_xmtrx_();
    shz_xmtrx_init_identity();
    shz_xmtrx_apply_rotation_x(F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { 1.0f,       0.0f,        0.0f, 0.0f,
                   0.0f, cosf(F_PI), -sinf(F_PI), 0.0f,
                   0.0f, sinf(F_PI),  cosf(F_PI), 0.0f,
                   0.0f,       0.0f,        0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_y)
    randomize_xmtrx_();
    shz_xmtrx_init_identity();
    shz_xmtrx_apply_rotation_y(F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { cosf(F_PI), 0.0f,  sinf(F_PI), 0.0f,
                         0.0f, 1.0f,        0.0f, 0.0f,
                         0.0f, 0.0f,  cosf(F_PI), 0.0f,
                  -sinf(F_PI), 0.0f,        0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_z)
    randomize_xmtrx_();
    shz_xmtrx_init_identity();
    shz_xmtrx_apply_rotation_z(F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { cosf(F_PI), -sinf(F_PI), 0.0f, 0.0f,
                   sinf(F_PI),  cosf(F_PI), 0.0f, 0.0f,
                         0.0f,        0.0f, 1.0f, 0.0f,
                         0.0f,        0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(register_accessors,
                  init_identity,
                  init_diagonal,
                  init_scale,
                  init_rotation_x,
                  init_rotation_y,
                  init_rotation_z,
                  init_translation,
                  init_symmetric_skew,
                  load_4x4,
                  load_4x4_unaligned,
                  apply_rotation_x,
                  apply_rotation_y,
                  apply_rotation_z)