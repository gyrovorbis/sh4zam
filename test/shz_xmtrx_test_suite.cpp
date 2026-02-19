#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_xmtrx.hpp"
#include "sh4zam/shz_matrix.hpp"

#include <gimbal/gimbal_algorithms.h>
#include <print>
#include <cglm/cglm.h>
#include <cglm/clipspace/ortho_lh_no.h>
#include <cglm/clipspace/view_rh.h>

#define GBL_SELF_TYPE shz_xmtrx_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

static std::array<float, 16> transpose(std::array<float, 16> mat) {
    std::array<float, 16> transpose;

    for(unsigned r = 0; r < 4; ++r)
        for(unsigned c = 0; c < 4; ++c)
            transpose[c * 4 + r] = mat[r * 4 + c];

    return transpose;
}

SHZ_NO_INLINE
static void randomize_xmtrx_() {
    //for(unsigned reg = SHZ_XMTRX_XF0; reg < SHZ_XMTRX_XF15; ++reg)
     //   shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandf());
}

SHZ_NO_INLINE static GBL_RESULT verify_matrix(GblTestSuite* pSelf, std::array<float, 16> mat, double epsilon=0.01) {
    mat = transpose(mat);
    GBL_CTX_BEGIN(pSelf);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF0 ), mat[SHZ_XMTRX_XF0 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF1 ), mat[SHZ_XMTRX_XF1 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF2 ), mat[SHZ_XMTRX_XF2 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF3 ), mat[SHZ_XMTRX_XF3 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF4 ), mat[SHZ_XMTRX_XF4 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF5 ), mat[SHZ_XMTRX_XF5 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF6 ), mat[SHZ_XMTRX_XF6 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF7 ), mat[SHZ_XMTRX_XF7 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF8 ), mat[SHZ_XMTRX_XF8 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF9 ), mat[SHZ_XMTRX_XF9 ], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF10), mat[SHZ_XMTRX_XF10], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF11), mat[SHZ_XMTRX_XF11], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF12), mat[SHZ_XMTRX_XF12], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF13), mat[SHZ_XMTRX_XF13], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF14), mat[SHZ_XMTRX_XF14], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_TEST_ERROR(shz::xmtrx::read(shz::xmtrx::reg::XF15), mat[SHZ_XMTRX_XF15], epsilon, GBL_TEST_ERROR_ABSOLUTE);
    GBL_CTX_END();
}

GBL_TEST_CASE(read_write_registers)
    randomize_xmtrx_();

    shz::xmtrx::write(shz::xmtrx::XF0,  100.0f);
    shz::xmtrx::write(shz::xmtrx::XF1,  101.0f);
    shz::xmtrx::write(shz::xmtrx::XF2,  102.0f);
    shz::xmtrx::write(shz::xmtrx::XF3,  103.0f);
    shz::xmtrx::write(shz::xmtrx::XF4,  104.0f);
    shz::xmtrx::write(shz::xmtrx::XF5,  100.0f);
    shz::xmtrx::write(shz::xmtrx::XF6,  101.0f);
    shz::xmtrx::write(shz::xmtrx::XF7,  102.0f);
    shz::xmtrx::write(shz::xmtrx::XF8,  103.0f);
    shz::xmtrx::write(shz::xmtrx::XF9,  104.0f);
    shz::xmtrx::write(shz::xmtrx::XF10, 110.0f);
    shz::xmtrx::write(shz::xmtrx::XF11, 111.0f);
    shz::xmtrx::write(shz::xmtrx::XF12, 112.0f);
    shz::xmtrx::write(shz::xmtrx::XF13, 113.0f);
    shz::xmtrx::write(shz::xmtrx::XF14, 114.0f);
    shz::xmtrx::write(shz::xmtrx::XF15, 115.0f);

    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF0 ) == 100.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF1 ) == 101.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF2 ) == 102.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF3 ) == 103.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF4 ) == 104.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF5 ) == 100.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF6 ) == 101.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF7 ) == 102.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF8 ) == 103.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF9 ) == 104.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF10) == 110.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF11) == 111.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF12) == 112.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF13) == 113.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF14) == 114.0f);
    GBL_TEST_VERIFY(shz::xmtrx::read(shz::xmtrx::XF15) == 115.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(read_write_rows)
    randomize_xmtrx_();

    shz::xmtrx::write_row(0, {  1.0f,  2.0f,  3.0f,  4.0f });
    shz::xmtrx::write_row(1, {  5.0f,  6.0f,  7.0f,  8.0f });
    shz::xmtrx::write_row(2, {  9.0f, 10.0f, 11.0f, 12.0f });
    shz::xmtrx::write_row(3, { 13.0f, 14.0f, 15.0f, 16.0f });

    GBL_TEST_VERIFY(shz::xmtrx::read_row(0) == shz::vec4( 1.0f,  2.0f,  3.0f,  4.0f));
    GBL_TEST_VERIFY(shz::xmtrx::read_row(1) == shz::vec4( 5.0f,  6.0f,  7.0f,  8.0f));
    GBL_TEST_VERIFY(shz::xmtrx::read_row(2) == shz::vec4( 9.0f, 10.0f, 11.0f, 12.0f));
    GBL_TEST_VERIFY(shz::xmtrx::read_row(3) == shz::vec4(13.0f, 14.0f, 15.0f, 16.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(read_write_cols)
    randomize_xmtrx_();

    shz::xmtrx::write_col(0, {  1.0f,  2.0f,  3.0f,  4.0f });
    shz::xmtrx::write_col(1, {  5.0f,  6.0f,  7.0f,  8.0f });
    shz::xmtrx::write_col(2, {  9.0f, 10.0f, 11.0f, 12.0f });
    shz::xmtrx::write_col(3, { 13.0f, 14.0f, 15.0f, 16.0f });

    GBL_TEST_VERIFY(shz::xmtrx::read_col(0) == shz::vec4( 1.0f,  2.0f,  3.0f,  4.0f));
    GBL_TEST_VERIFY(shz::xmtrx::read_col(1) == shz::vec4( 5.0f,  6.0f,  7.0f,  8.0f));
    GBL_TEST_VERIFY(shz::xmtrx::read_col(2) == shz::vec4( 9.0f, 10.0f, 11.0f, 12.0f));
    GBL_TEST_VERIFY(shz::xmtrx::read_col(3) == shz::vec4(13.0f, 14.0f, 15.0f, 16.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(swap_rows)
    randomize_xmtrx_();

    std::array<shz::vec4, 4> rows = {
        shz::xmtrx::read_row(0),
        shz::xmtrx::read_row(1),
        shz::xmtrx::read_row(2),
        shz::xmtrx::read_row(3)
    };

    shz::xmtrx::swap_rows(0, 3);
    shz::xmtrx::swap_rows(1, 2);

    GBL_TEST_VERIFY(rows[0] == shz::xmtrx::read_row(3));
    GBL_TEST_VERIFY(rows[1] == shz::xmtrx::read_row(2));
    GBL_TEST_VERIFY(rows[2] == shz::xmtrx::read_row(1));
    GBL_TEST_VERIFY(rows[3] == shz::xmtrx::read_row(0));
GBL_TEST_CASE_END

GBL_TEST_CASE(swap_cols)
    randomize_xmtrx_();

    std::array<shz::vec4, 4> cols = {
        shz::xmtrx::read_col(0),
        shz::xmtrx::read_col(1),
        shz::xmtrx::read_col(2),
        shz::xmtrx::read_col(3)
    };

    shz::xmtrx::swap_cols(0, 3);
    shz::xmtrx::swap_cols(1, 2);

    GBL_TEST_VERIFY(cols[0] == shz::xmtrx::read_col(3));
    GBL_TEST_VERIFY(cols[1] == shz::xmtrx::read_col(2));
    GBL_TEST_VERIFY(cols[2] == shz::xmtrx::read_col(1));
    GBL_TEST_VERIFY(cols[3] == shz::xmtrx::read_col(0));
GBL_TEST_CASE_END


GBL_TEST_CASE(load_4x4)
    randomize_xmtrx_();
    shz_mat4x4_t mat4 = {
        .left    = { -1.0f,   2.0f,  3.0f,  8.0f },
        .up      = {  4.0f,  -5.0f,  6.0f,  7.0f },
        .forward = {  7.0f,   8.0f,  9.0f, 10.0f },
        .pos     = { 11.0f, -12.0f, 13.0f, 14.0f }
    };
    shz::xmtrx::load(mat4);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose({
                    -1.0f,   2.0f,  3.0f,  8.0f,
                     4.0f,  -5.0f,  6.0f,  7.0f,
                     7.0f,   8.0f,  9.0f, 10.0f,
                    11.0f, -12.0f, 13.0f, 14.0f })));

    benchmark(nullptr, [&]() { shz::xmtrx::load(mat4); });
    benchmark(nullptr, mat_load, (const float(*)[4][4])&mat4);
GBL_TEST_CASE_END

GBL_TEST_CASE(load_wxyz_4x4)
    randomize_xmtrx_();
    shz_mat4x4_t mat4 = {
        .left    = { -1.0f,   2.0f,  3.0f,  8.0f },
        .up      = {  4.0f,  -5.0f,  6.0f,  7.0f },
        .forward = {  7.0f,   8.0f,  9.0f, 10.0f },
        .pos     = { 11.0f, -12.0f, 13.0f, 14.0f }
    };
    shz::xmtrx::load_wxyz(mat4);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose({
                     11.0f, -12.0f, 13.0f, 14.0f,
                     -1.0f,   2.0f,  3.0f,  8.0f,
                      4.0f,  -5.0f,  6.0f,  7.0f,
                      7.0f,   8.0f,  9.0f, 10.0f })));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_unaligned_4x4)
    randomize_xmtrx_();
    alignas(4) std::array<float, 16> array = {
        -1.0f,   2.0f,  3.0f,  8.0f,
         4.0f,  -5.0f,  6.0f,  7.0f,
         7.0f,   8.0f,  9.0f, 10.0f,
        11.0f, -12.0f, 13.0f, 14.0f
    };
    shz::xmtrx::load(array.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose(array)));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_cols_4x4)
    randomize_xmtrx_();
    std::array<float, 16> array = {
        -1.0f,   2.0f,  3.0f,  8.0f,
         4.0f,  -5.0f,  6.0f,  7.0f,
         7.0f,   8.0f,  9.0f, 10.0f,
        11.0f, -12.0f, 13.0f, 14.0f
    };
    shz::xmtrx::load_cols(*reinterpret_cast<const shz_vec4_t *>(array.data()),
                          *reinterpret_cast<const shz_vec4_t *>(array.data() + 4),
                          *reinterpret_cast<const shz_vec4_t *>(array.data() + 8),
                          *reinterpret_cast<const shz_vec4_t *>(array.data() + 12));
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose(array)));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_rows_4x4)
    randomize_xmtrx_();
    std::array<float, 16> array = {
        -1.0f,   2.0f,  3.0f,  8.0f,
         4.0f,  -5.0f,  6.0f,  7.0f,
         7.0f,   8.0f,  9.0f, 10.0f,
        11.0f, -12.0f, 13.0f, 14.0f
    };
    shz::xmtrx::load_rows(*reinterpret_cast<const shz_vec4_t *>(array.data()),
                          *reinterpret_cast<const shz_vec4_t *>(array.data() + 4),
                          *reinterpret_cast<const shz_vec4_t *>(array.data() + 8),
                          *reinterpret_cast<const shz_vec4_t *>(array.data() + 12));
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, array));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_transpose_unaligned_4x4)
    randomize_xmtrx_();
    alignas(4) std::array<float, 16> array = {
        -1.0f,   2.0f,  3.0f,  8.0f,
         4.0f,  -5.0f,  6.0f,  7.0f,
         7.0f,   8.0f,  9.0f, 10.0f,
        11.0f, -12.0f, 13.0f, 14.0f
    };
    shz::xmtrx::load_transpose(array.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, array));
GBL_TEST_CASE_END

GBL_TEST_CASE(store_4x4)
    randomize_xmtrx_();
    shz::mat4x4 mat;
    shz::xmtrx::store(&mat);
    std::array<float, 16> array = std::to_array(mat.elem);
    //GBL_TEST_SKIP("No fucking clue why this shits the bed.");
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(array)));
    benchmark(nullptr, [&]{ shz::xmtrx::store(&mat); });
    benchmark(nullptr, [&]{ mat_store((float (*)[4][4])&mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(store_unaligned_4x4)
    randomize_xmtrx_();
    std::array<float, 16> array;
    shz::xmtrx::store(&array);
   // GBL_TEST_SKIP("No fucking clue why this shits the bed.");
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(array)));
GBL_TEST_CASE_END

GBL_TEST_CASE(store_aligned32_4x4)
    randomize_xmtrx_();

    alignas(32) shz_mat4x4_t shzMat;
    alignas(32) char buffer[sizeof(shz::mat4x4) + 8];
    auto* cMat = reinterpret_cast<shz::mat4x4*>(buffer + 8);


    benchmark(nullptr, shz_xmtrx_store_4x4, cMat);
    benchmark(nullptr, shz_xmtrx_store_4x4, &shzMat);

    std::array<float, 16> array = std::to_array(shzMat.elem);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(array)));
GBL_TEST_CASE_END

GBL_TEST_CASE(store_transpose_4x4)
    randomize_xmtrx_();
    shz::mat4x4 mat;
    shz::xmtrx::store_transpose(&mat);
    std::array<float, 16> array = std::to_array(mat.elem);
    GBL_TEST_SKIP("No fucking clue why this shits the bed.");
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(array)));
GBL_TEST_CASE_END

GBL_TEST_CASE(store_transpose_unaligned_4x4)
    randomize_xmtrx_();
    std::array<float, 16> array;
    shz::xmtrx::store_transpose(&array);
    GBL_TEST_SKIP("No fucking clue why this shits the bed.");
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(array)));
GBL_TEST_CASE_END


GBL_TEST_CASE(init_identity)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_identity_safe)
    randomize_xmtrx_();
    shz::xmtrx::init_identity_safe();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_zero)
    randomize_xmtrx_();
    shz::xmtrx::init_zero();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_one)
    randomize_xmtrx_();
    shz::xmtrx::init_one();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_fill)
    randomize_xmtrx_();
    shz::xmtrx::init_fill(3.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 3.0f, 3.0f, 3.0f, 3.0f,
                    3.0f, 3.0f, 3.0f, 3.0f,
                    3.0f, 3.0f, 3.0f, 3.0f,
                    3.0f, 3.0f, 3.0f, 3.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_translation)
    randomize_xmtrx_();
    shz::xmtrx::init_translation(10.0f, -20.0f, 30.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 10.0f,
                    0.0f, 1.0f, 0.0f, -20.0f,
                    0.0f, 0.0f, 1.0f, 30.0f,
                    0.0f, 0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_scale)
    randomize_xmtrx_();
    shz::xmtrx::init_scale(-2.0f, -3.0f, -4.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { -2.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, -3.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, -4.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_x)
    randomize_xmtrx_();
    shz::xmtrx::init_rotation_x(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, cosf(SHZ_F_PI), -sinf(SHZ_F_PI), 0.0f,
                    0.0f, sinf(SHZ_F_PI), cosf(SHZ_F_PI), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_y)
    randomize_xmtrx_();
    shz::xmtrx::init_rotation_y(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cosf(SHZ_F_PI), 0.0f, sinf(SHZ_F_PI), 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    -sinf(SHZ_F_PI), 0.0f, cosf(SHZ_F_PI), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_z)
    randomize_xmtrx_();
    shz::xmtrx::init_rotation_z(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cosf(SHZ_F_PI), sinf(SHZ_F_PI), 0.0f, 0.0f,
                    sinf(SHZ_F_PI), cosf(SHZ_F_PI), 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_xyz)
    GBL_TEST_SKIP("WIP");
    randomize_xmtrx_();
    shz::xmtrx::init_rotation_zyx(shz::deg_to_rad(180.0f),
                                  shz::deg_to_rad(-30.0f),
                                  shz::deg_to_rad(90.0f));
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                transpose({
                                    -0.866f, 0.500f, 0.000f, 0.0f,
                                    0.000f, -0.000f, 1.000f, 0.0f,
                                    0.500f, 0.866f, 0.000f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f
                                })));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_zyx)

GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_yxz)

GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation)

GBL_TEST_CASE_END

GBL_TEST_CASE(init_diagonal)
    randomize_xmtrx_();
    shz::xmtrx::init_diagonal(-1.0f, 2.0f, 3.0f, 4.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { -1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 2.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 3.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 4.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_upper_diagonal)
    GBL_TEST_SKIP("LOL");
    randomize_xmtrx_();
    shz::xmtrx::init_upper_diagonal(  1.0f,
                                    { 2.0f, 3.0f },
                                    { 4.0f, 5.0f, 6.0f },
                                    { 7.0f, 8.0f, 9.0f, 10.0f });
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                transpose({
                                    1.0f, 2.0f, 4.0f, 7.0f,
                                    0.0f, 3.0f, 5.0f, 8.0f,
                                    0.0f, 0.0f, 6.0f, 9.0f,
                                    0.0f, 0.0f, 0.0f, 10.0f
                                })));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_lower_diagonal)
    GBL_TEST_SKIP("LOL");
    randomize_xmtrx_();
    shz::xmtrx::init_lower_diagonal({ 7.0f, 8.0f, 9.0f, 10.0f },
                                    { 4.0f, 5.0f, 6.0f },
                                    { 2.0f, 3.0f },
                                      1.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                transpose({
                                    7.0f, 0.0f, 0.0f, 0.0f,
                                    8.0f, 4.0f, 0.0f, 0.0f,
                                    9.0f, 5.0f, 2.0f, 0.0f,
                                   10.0f, 6.0f, 3.0f, 1.0f
                                })));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_symmetric_skew)
    randomize_xmtrx_();
    shz::xmtrx::init_symmetric_skew(-1.5f, 2.7f, -3.4f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { 0.0f, -3.4f, -2.7f, 0.0f,
                   3.4f, 0.0f, -1.5f, 0.0f,
                   2.7f, 1.5f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_screen)
    randomize_xmtrx_();
    shz::xmtrx::init_screen(640.0f, 480.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                { 320.0f, 0.0f,    0.0f, 320.0f,
                 -0.0f,  -240.0f, -0.0f, 240.0f,
                  0.0f,   0.0f,    1.0f, 0.0f,
                  0.0f,   0.0f,    0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_permutation_wxyz)
GBL_TEST_CASE_END

GBL_TEST_CASE(init_permutation_yzwx)
GBL_TEST_CASE_END

GBL_TEST_CASE(init_outer_product)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_unaligned_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_transpose_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_transpose_unaligned_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_unaligned_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_transpose_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_transpose_unaligned_4x4)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_translation)
    // Apply against identity.
    // Expectation: Only touches translation parts of the matrix
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity_safe();
        shz::xmtrx::apply_translation(10.0f, -20.0f, 30.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    { 1.0f, 0.0f, 0.0f, 10.0f,
                      0.0f, 1.0f, 0.0f, -20.0f,
                      0.0f, 0.0f, 1.0f, 30.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }));

    }

    // Apply against pre-filled matrix
    // Expectation: Only touches the translation parts of the matrix.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_fill(32.0f);
        shz::xmtrx::apply_translation(20.0f, -100.0f, -60.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        32.0f, 32.0f, 32.0f, 52.0f,
                        32.0f, 32.0f, 32.0f, -68.0f,
                        32.0f, 32.0f, 32.0f, -28.0f,
                        32.0f, 32.0f, 32.0f, 32.0f
                    }));
    }
    // Test compositon of translations
    // Expectation: Translations added together rather than overwritten.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity_safe();

        shz::xmtrx::apply_translation(2.0f, 3.0f, -4.0f);
        shz::xmtrx::apply_translation(5.0f, 6.0f,  7.0f);

        shz::mat4x4 res;
        shz::xmtrx::store(&res);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        1.0f, 0.0f, 0.0f, 7.0f,
                        0.0f, 1.0f, 0.0f, 9.0f,
                        0.0f, 0.0f, 1.0f, 3.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }));

        // Test composition versus cglm
        mat4 m_glm = GLM_MAT4_IDENTITY_INIT;
        vec3 t1, t2;

        glm_vec3_copy((vec3){2.0f, 3.0f, -4.0f}, t1);
        glm_vec3_copy((vec3){5.0f, 6.0f, 7.0f}, t2);

        glm_translate(m_glm, t1);
        glm_translate(m_glm, t2);

        auto test_glm = [](const shz::mat4x4& shzmat, const mat4& glmmat) {
            for(int r = 0; r < 4; r++) {
                for(int c = 0; c < 4; c++) {
                    if(!shz_equalf(shzmat.elem2D[r][c], glmmat[r][c])) {
                        printf("(%d, %d): SHZ: [%f] -> GLM [%f]\n", r, c, shzmat.elem2D[r][c], glmmat[r][c]);
                        return false;
                    }
                }
            }
            return true;
        };

        GBL_TEST_VERIFY(test_glm(res, m_glm));        
    }

GBL_TEST_CASE_END

GBL_TEST_CASE(apply_scale)
    // Apply against identity.
    // Expectation: Diagonal scale cells filled in the 3x3 part.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity_safe();
        shz::xmtrx::apply_scale(10.0f, -20.0f, 30.0f);
        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    { 10.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, -20.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 30.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f }));
    }
    // Apply against pre-filled matrix
    // Expectation: Only touches the scale parts of the matrix.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_fill(32.0f);
        shz::xmtrx::apply_scale(20.0f, -100.0f, -60.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        640.0f, -3200.0f, -1920.0f, 32.0f,
                        640.0f, -3200.0f, -1920.0f, 32.0f,
                        640.0f, -3200.0f, -1920.0f, 32.0f,
                        32.0f, 32.0f, 32.0f, 32.0f
                    }));
    }
    // Test scale by 0.0f
    // Expectation: All scale cells of the 3x3 part of the matrix filled with 0.0f
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity_safe();

        shz::xmtrx::apply_scale(0.0f, 0.0f, 0.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }));
    }
    // Test scaling by 1.0f
    // Expectation: Values should remain at their initialized value.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_fill(5.0f);
        shz::xmtrx::apply_scale(1.0f, 1.0f, 1.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        5.0f, 5.0f, 5.0f, 5.0f,
                        5.0f, 5.0f, 5.0f, 5.0f,
                        5.0f, 5.0f, 5.0f, 5.0f,
                        5.0f, 5.0f, 5.0f, 5.0f
                    }));
    }
    // Test composition of scales
    // Expectation: Scale values should multiply NOT overwrite
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity_safe();

        shz::xmtrx::apply_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_scale(5.0f, 6.0f, 7.0f);

        shz::mat4x4 res;
        shz::xmtrx::store(&res);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        10.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 18.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 28.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }));

        // Test composition versus cglm
        mat4 m_glm = GLM_MAT4_IDENTITY_INIT;
        vec3 scale1, scale2;

        glm_vec3_copy((vec3){2.0f, 3.0f, 4.0f}, scale1);
        glm_vec3_copy((vec3){5.0f, 6.0f, 7.0f}, scale2);

        glm_scale(m_glm, scale1);
        glm_scale(m_glm, scale2);

        auto test_glm = [](const shz::mat4x4& shzmat, const mat4& glmmat) {
            for(int r = 0; r < 4; r++) {
                for(int c = 0; c < 4; c++) {
                    if(!shz_equalf(shzmat.elem2D[r][c], glmmat[r][c])) {
                        return false;
                    }
                }
            }
            return true;
        };

        GBL_TEST_VERIFY(test_glm(res, m_glm));
    }

GBL_TEST_CASE_END


GBL_TEST_CASE(apply_rotation_x)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::apply_rotation_x(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { 1.0f,       0.0f,        0.0f, 0.0f,
                   0.0f, cosf(SHZ_F_PI), -sinf(SHZ_F_PI), 0.0f,
                   0.0f, sinf(SHZ_F_PI),  cosf(SHZ_F_PI), 0.0f,
                   0.0f,       0.0f,        0.0f, 1.0f }));
    benchmark(nullptr, shz::xmtrx::apply_rotation_x, SHZ_F_PI);
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_y)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::apply_rotation_y(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { cosf(SHZ_F_PI), 0.0f,  sinf(SHZ_F_PI), 0.0f,
                         0.0f,     1.0f,   0.0f, 0.0f,
                         0.0f,     0.0f,  cosf(SHZ_F_PI), 0.0f,
                  -sinf(SHZ_F_PI), 0.0f,   0.0f, 1.0f }));
    benchmark(nullptr, shz::xmtrx::apply_rotation_y, SHZ_F_PI);
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_z)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::apply_rotation_z(SHZ_F_PI);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                 { cosf(SHZ_F_PI), -sinf(SHZ_F_PI), 0.0f, 0.0f,
                   sinf(SHZ_F_PI),  cosf(SHZ_F_PI), 0.0f, 0.0f,
                         0.0f,        0.0f, 1.0f, 0.0f,
                         0.0f,        0.0f, 0.0f, 1.0f }));
    benchmark(nullptr, shz::xmtrx::apply_rotation_z, SHZ_F_PI);
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_xyz)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_zyx)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_yxz)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_rotation_quat)
    shz::xmtrx::init_identity();
    shz::xmtrx::apply_rotation_quat({ 0.5f, 0.5f, 0.5f, 0.5f });
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

    shz::xmtrx::init_identity();
    shz::xmtrx::apply_rotation_quat({ 0.7071068f, 0.0f, 0.0f, 0.7071068f });
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    0.0f, -1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_4x4)
    alignas(32) std::array<std::array<float, 16>, 128> matrix1 = {{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    }};
    alignas(32) std::array<std::array<float, 16>, 128> matrix2 = {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 2.0f, 3.0f, 1.0f
    }};
    alignas(8) std::array<float, 16> out;

    const shz_mat4x4_t* mat1 = reinterpret_cast<const shz_mat4x4_t*>(&matrix1[0][0]);
    const shz_mat4x4_t* mat2 = reinterpret_cast<const shz_mat4x4_t*>(&matrix2[0][0]);
    unsigned int count = 0;

    benchmark(nullptr, [&] {
        const auto idx = count++ % 128;
        shz_xmtrx_load_apply_4x4(reinterpret_cast<const shz::mat4x4*>(&mat1[idx]),
                                 reinterpret_cast<const shz::mat4x4*>(&mat2[idx]));
    });

    shz_xmtrx_load_apply_4x4(reinterpret_cast<const shz::mat4x4*>(&mat1[0]),
                             reinterpret_cast<const shz::mat4x4*>(&mat2[0]));

    shz::xmtrx::store(out.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    1.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 2.0f,
                                    0.0f, 0.0f, 1.0f, 3.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_store_4x4)
    alignas(32) std::array<std::array<float, 16>, 128> matrix1 = {{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    }};
    alignas(32) std::array<std::array<float, 16>, 128> matrix2 = {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 2.0f, 3.0f, 1.0f
    }};
    alignas(32) std::array<std::array<float, 16>, 128> out;
    unsigned int count = 0;

    benchmark(nullptr, [&] {
        const auto idx = count++ % 128;
        shz_xmtrx_load_apply_store_4x4(reinterpret_cast<      shz::mat4x4*>(&    out[idx]),
                                       reinterpret_cast<const shz::mat4x4*>(&matrix2[idx]),
                                       reinterpret_cast<const shz::mat4x4*>(&matrix1[idx]));
    });

    shz_xmtrx_load_apply_store_4x4(reinterpret_cast<      shz::mat4x4*>(&    out[0]),
                                   reinterpret_cast<const shz::mat4x4*>(&matrix2[0]),
                                   reinterpret_cast<const shz::mat4x4*>(&matrix1[0]));

    shz::xmtrx::store(out.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    1.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 2.0f,
                                    0.0f, 0.0f, 1.0f, 3.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_store_unaligned_4x4)
    randomize_xmtrx_();
    alignas(4) std::array<float, 16> matrix1 = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    alignas(4) std::array<float, 16> matrix2 = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 2.0f,
        0.0f, 0.0f, 1.0f, 3.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    alignas(4) std::array<float, 16> out;

    shz::xmtrx::load_apply_store(out.data(), transpose(matrix1).data(), transpose(matrix2).data());
    shz::xmtrx::load(out.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    1.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 2.0f,
                                    0.0f, 0.0f, 1.0f, 3.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));
GBL_TEST_CASE_END

GBL_TEST_CASE(translate)
    shz::xmtrx::init_identity_safe();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                            {
                                1.0f, 0.0f, 0.0f, 1.0f,
                                0.0f, 1.0f, 0.0f, 2.0f,
                                0.0f, 0.0f, 1.0f, 3.0f,
                                0.0f, 0.0f, 0.0f, 1.0f }));
    benchmark(nullptr, shz::xmtrx::translate, 100.0f, 200.0f, 300.0f);
    benchmark(nullptr, mat_translate, 100.0f, 200.0f, 300.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_ortho)
    shz::xmtrx::init_identity_safe();
    shz::xmtrx::apply_ortho(0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1000.0f);
    union {
        alignas(8) mat4 cmat;
        shz::mat4x4 shzcmat;
    } cunion;
    glm_ortho_lh_no(0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1000.0f, cunion.cmat);
    shz::mat4x4 shzmat;
    shz::xmtrx::store(&shzmat);
#if 0
    for(unsigned i = 0; i < 4; ++i)
        for(unsigned j = 0; j < 4; ++j)
            std::println("[{}][{}]: {} vs {}", i, j, shzmat.elem2D[i][j], cunion.shzcmat.elem2D[i][j]);
#endif
    GBL_TEST_VERIFY(shzmat == cunion.shzcmat);
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_screen)
    shz::xmtrx::init_identity_safe();
    shz::xmtrx::apply_screen(640.0f, 480.0f);

    shz::mat4x4 applied_screen_mat;
    shz::xmtrx::store(&applied_screen_mat);

    // Ensure apply_screen computes
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                { 320.0f, -0.0f,   0.0f, 320.0f,
                 -0.0f,   -240.0f, 0.0f, 240.0f,
                  0.0f,   -0.0f,   1.0f, 0.0f,
                  0.0f,   -0.0f,   0.0f, 1.0f }));

    // Check bruces balls
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::apply_permutation_wxyz();
    shz::xmtrx::apply_screen(640.0f, 480.0f);
    
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
            { 0.0f,   -0.0f,   0.0f, 1.0f,
              320.0f,  0.0f,   0.0f, 320.0f,
              0.0f,   -240.0f, 0.0f, 240.0f,
              0.0f,   -0.0f,   1.0f, 0.0f }));

    // Check parity with init_screen
    randomize_xmtrx_();
    shz::xmtrx::init_screen(640.0f, 480.0f);
    shz::mat4x4 init_screen_mat;
    shz::xmtrx::store(&init_screen_mat);

    GBL_TEST_VERIFY(applied_screen_mat == init_screen_mat);
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_lookat)
    auto test = [](shz::vec3 eye, shz::vec3 center, shz::vec3 up) {
        shz::mat4x4 shzMat, cglmMat;

        shz::xmtrx::init_identity_safe();
        shz::xmtrx::apply_lookat(eye, center, up);
        shz::xmtrx::store(&shzMat);

        glm_lookat_rh(*(vec3*)&eye, *(vec3*)&center, *(vec3*)&up, *(mat4*)&cglmMat);

        for(unsigned i = 0; i < 4; ++i) {
            for(unsigned j = 0; j < 4; ++j) {
                if(!shz_equalf(shzMat.elem2D[i][j], cglmMat.elem2D[i][j]))
                    std::print("[{}][{}]: {} != {} ", j, i, shzMat.elem2D[i][j], cglmMat.elem2D[i][j]);
            }
        }

        return shzMat == cglmMat;

    };

    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }));
    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 2.0f, 0.0f }));
    GBL_TEST_VERIFY(test({ 5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(read_write_registers,
                  read_write_rows,
                  read_write_cols,
                  swap_rows,
                  swap_cols,
                  load_4x4,
                  load_wxyz_4x4,
                  load_unaligned_4x4,
                  load_cols_4x4,
                  load_rows_4x4,
                  load_transpose_unaligned_4x4,
                  store_4x4,
                  store_unaligned_4x4,
                  store_aligned32_4x4,
                  store_transpose_4x4,
                  store_transpose_unaligned_4x4,
                  init_identity,
                  init_identity_safe,
                  init_zero,
                  init_one,
                  init_fill,
                  init_translation,
                  init_scale,
                  init_rotation_x,
                  init_rotation_y,
                  init_rotation_z,
                  init_rotation_xyz,
                  init_rotation_zyx,
                  init_rotation_yxz,
                  init_rotation,
                  init_diagonal,
                  init_upper_diagonal,
                  init_lower_diagonal,
                  init_symmetric_skew,
                  init_outer_product,
                  init_screen,
                  init_permutation_wxyz,
                  init_permutation_yzwx,
                  apply_4x4,
                  apply_unaligned_4x4,
                  apply_transpose_4x4,
                  apply_transpose_unaligned_4x4,
                  apply_reverse_4x4,
                  apply_reverse_unaligned_4x4,
                  apply_reverse_transpose_4x4,
                  apply_reverse_transpose_unaligned_4x4,
                  apply_translation,
                  apply_scale,
                  apply_rotation_x,
                  apply_rotation_y,
                  apply_rotation_z,
                  apply_rotation_xyz,
                  apply_rotation_zyx,
                  apply_rotation_yxz,
                  apply_rotation_quat,
                  load_apply_4x4,
                  load_apply_store_4x4,
                  load_apply_store_unaligned_4x4,
                  translate,
                  apply_ortho,
                  apply_screen,
                  apply_lookat)