#include "shz_test_scenario.h"
#include "shz_test.hpp"
#include "sh4zam/shz_xmtrx.hpp"
#include "sh4zam/shz_matrix.hpp"

#include <gimbal/gimbal_algorithms.h>
#include <bit>
#include <print>

#include <cglm/clipspace/ortho_rh_no.h>
#include <cglm/clipspace/view_rh.h>
#include <cglm/clipspace/persp_rh_no.h>
#include <cglm/mat4.h>
#include <cglm/mat3.h>
#include <cglm/mat2.h>
#include <cglm/vec4.h>
#include <cglm/affine.h>

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

static bool compare_glm(const shz::mat4x4& shzmat, const mat4& glmmat) {
    for(int r = 0; r < 4; r++) {
        for(int c = 0; c < 4; c++) {
            if(!shz_equalf(shzmat.elem2D[r][c], glmmat[r][c])) {
                return false;
            }
        }
    }
    return true;
}

static void randomize_xmtrx_() {
    for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
       shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandf());
}

static void load_asymmetric_xmtrx_() {
    shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 2.0f, 3.0f);
    shz::xmtrx::scale_3x3(2.0f, 3.0f, 4.0f);
    shz::xmtrx::add_translation(10.0f, 20.0f, 30.0f);
    shz::xmtrx::write(shz::xmtrx::reg::XF3,  0.25f);
    shz::xmtrx::write(shz::xmtrx::reg::XF7, -0.5f);
    shz::xmtrx::write(shz::xmtrx::reg::XF11, 0.75f);
}

static GBL_RESULT verify_matrix(GblTestSuite* pSelf, std::array<float, 16> mat, float epsilon = 0.01f) {
    mat = transpose(mat);
    GBL_CTX_BEGIN(pSelf);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF0 ) - mat[SHZ_XMTRX_XF0 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF1 ) - mat[SHZ_XMTRX_XF1 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF2 ) - mat[SHZ_XMTRX_XF2 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF3 ) - mat[SHZ_XMTRX_XF3 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF4 ) - mat[SHZ_XMTRX_XF4 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF5 ) - mat[SHZ_XMTRX_XF5 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF6 ) - mat[SHZ_XMTRX_XF6 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF7 ) - mat[SHZ_XMTRX_XF7 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF8 ) - mat[SHZ_XMTRX_XF8 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF9 ) - mat[SHZ_XMTRX_XF9 ]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF10) - mat[SHZ_XMTRX_XF10]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF11) - mat[SHZ_XMTRX_XF11]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF12) - mat[SHZ_XMTRX_XF12]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF13) - mat[SHZ_XMTRX_XF13]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF14) - mat[SHZ_XMTRX_XF14]) <= epsilon);
    GBL_TEST_VERIFY(std::abs(shz::xmtrx::read(shz::xmtrx::reg::XF15) - mat[SHZ_XMTRX_XF15]) <= epsilon);
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
    alignas(32) shz_mat4x4_t mat4 = {
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

#if SHZ_BACKEND == SHZ_SH4
    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::load", [](const shz_mat4x4_t* m) { shz::xmtrx::load(*m); },
        "mat_load",         [](const shz_mat4x4_t* m) { mat_load(reinterpret_cast<const matrix_t*>(m)); },
        &mat4));
#endif
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
    (benchmark)(nullptr, "shz::xmtrx::load_cols_4x4", [&] {
        shz::xmtrx::load_cols(*reinterpret_cast<const shz_vec4_t *>(array.data()),
                              *reinterpret_cast<const shz_vec4_t *>(array.data() + 4),
                              *reinterpret_cast<const shz_vec4_t *>(array.data() + 8),
                              *reinterpret_cast<const shz_vec4_t *>(array.data() + 12)); });
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
    (benchmark)(nullptr, "shz::xmtrx::load_rows_4x4", [&] {
        shz::xmtrx::load_rows(*reinterpret_cast<const shz_vec4_t *>(array.data()),
                              *reinterpret_cast<const shz_vec4_t *>(array.data() + 4),
                              *reinterpret_cast<const shz_vec4_t *>(array.data() + 8),
                              *reinterpret_cast<const shz_vec4_t *>(array.data() + 12)); });
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
    alignas(32) shz_mat4x4_t mat;
    shz::xmtrx::store(&mat);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(std::to_array(mat.elem))));

#if SHZ_BACKEND == SHZ_SH4
    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::store", [](shz_mat4x4_t* m) { shz::xmtrx::store(m); },
        "mat_store",         [](shz_mat4x4_t* m) { mat_store(reinterpret_cast<matrix_t*>(m)); },
        &mat));
#endif
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
    alignas(32) shz::mat4x4 aligned;
    shz::xmtrx::store_aligned32(&aligned);
    std::array<float, 16> array = std::to_array(aligned.elem);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(array)));

    (benchmark_cmp<void>)(
        "shz::xmtrx::store_aligned32", [&] { shz::xmtrx::store_aligned32(&aligned); },
        "shz::xmtrx::store",           [&] { shz::xmtrx::store(&aligned); });
GBL_TEST_CASE_END

GBL_TEST_CASE(store_transpose_4x4)
    randomize_xmtrx_();
    shz::mat4x4 mat;
    shz::xmtrx::store_transpose(&mat);
    std::array<float, 16> array = std::to_array(mat.elem);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, array));
GBL_TEST_CASE_END

GBL_TEST_CASE(store_transpose_unaligned_4x4)
    randomize_xmtrx_();
    std::array<float, 16> array;
    shz::xmtrx::store_transpose(&array);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, array));
GBL_TEST_CASE_END


GBL_TEST_CASE(init_identity)
    volatile mat4 transform;
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));

        (benchmark_cmp<void>)(
            "shz::xmtrx::init_identity", bench_fn(shz::xmtrx::init_identity),
            "glm_mat4_identity",         [&] {
                glm_mat4_identity(*const_cast<mat4*>(&transform));
                (void)transform;
            });
            transform[0][0] = -16.0f;
GBL_TEST_CASE_END

GBL_TEST_CASE(init_identity_safe)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
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
    (benchmark)(nullptr, "shz::xmtrx::init_zero", [&] { shz::xmtrx::init_zero(); });
GBL_TEST_CASE_END

GBL_TEST_CASE(init_one)
    randomize_xmtrx_();
    shz::xmtrx::init_one();
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f, 1.0f}));
    (benchmark)(nullptr, "shz::xmtrx::init_one", [&] { shz::xmtrx::init_one(); });
GBL_TEST_CASE_END

GBL_TEST_CASE(init_fill)
    randomize_xmtrx_();
    shz::xmtrx::init_fill(3.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 3.0f, 3.0f, 3.0f, 3.0f,
                    3.0f, 3.0f, 3.0f, 3.0f,
                    3.0f, 3.0f, 3.0f, 3.0f,
                    3.0f, 3.0f, 3.0f, 3.0f}));
    (benchmark)(nullptr, "shz::xmtrx::init_fill", [&] { shz::xmtrx::init_fill(3.0f); });
GBL_TEST_CASE_END

GBL_TEST_CASE(init_translation)
    randomize_xmtrx_();
    shz::xmtrx::init_translation(10.0f, -20.0f, 30.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 10.0f,
                    0.0f, 1.0f, 0.0f, -20.0f,
                    0.0f, 0.0f, 1.0f, 30.0f,
                    0.0f, 0.0f, 0.0f, 1.0f }));
    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::init_translation", [](float x, float y, float z) {
                shz::xmtrx::init_translation(x, y, z);
            },
            "glm_translate_make", [](float x, float y, float z) {
                static volatile mat4 m;
                volatile vec3 t = { x, y, z };
                glm_translate_make(*const_cast<mat4*>(&m), *const_cast<vec3*>(&t));
            },
            10.0f, -20.0f, 30.0f
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(init_scale)
    randomize_xmtrx_();
    shz::xmtrx::init_scale(-2.0f, -3.0f, -4.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { -2.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, -3.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, -4.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::init_scale", [](float x, float y, float z) {
                shz::xmtrx::init_scale(x, y, z);
            },
            "glm_scale_make", [](float x, float y, float z) {
                static volatile mat4 m;
                volatile vec3 s = { x, y, z };
                glm_scale_make(*const_cast<mat4*>(&m), *const_cast<vec3*>(&s));
            },
            -2.0f, -3.0f, -4.0f
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_x)
    randomize_xmtrx_();
    shz::xmtrx::init_rotation_x(shz::pi_f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, cosf(shz::pi_f), -sinf(shz::pi_f), 0.0f,
                    0.0f, sinf(shz::pi_f), cosf(shz::pi_f), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
    // Double arguments must select the radian overload.
    {
        shz::mat4x4 fromFloat, fromDouble;

        shz::xmtrx::init_rotation_x(0.6f);
        shz::xmtrx::store(&fromFloat);

        randomize_xmtrx_();
        shz::xmtrx::init_rotation_x(static_cast<double>(0.6f));
        shz::xmtrx::store(&fromDouble);

        GBL_TEST_VERIFY(fromDouble == fromFloat);
    }
    {
        volatile float angle = 0.6f;

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::init_rotation_x", [](const volatile float* r) {
                shz::xmtrx::init_rotation_x(*r);
            },
            "glm_mat4_identity + glm_rotate_x", [](const volatile float* r) {
                static volatile mat4 m;
                glm_mat4_identity(*const_cast<mat4*>(&m));
                glm_rotate_x(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_y)
    randomize_xmtrx_();
    shz::xmtrx::init_rotation_y(shz::pi_f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cosf(shz::pi_f), 0.0f, sinf(shz::pi_f), 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    -sinf(shz::pi_f), 0.0f, cosf(shz::pi_f), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}));
    // Double arguments must select the radian overload.
    {
        shz::mat4x4 fromFloat, fromDouble;

        shz::xmtrx::init_rotation_y(0.6f);
        shz::xmtrx::store(&fromFloat);

        randomize_xmtrx_();
        shz::xmtrx::init_rotation_y(static_cast<double>(0.6f));
        shz::xmtrx::store(&fromDouble);

        GBL_TEST_VERIFY(fromDouble == fromFloat);
    }
    {
        volatile float angle = 0.6f;

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::init_rotation_y", [](const volatile float* r) {
                shz::xmtrx::init_rotation_y(*r);
            },
            "glm_mat4_identity + glm_rotate_y", [](const volatile float* r) {
                static volatile mat4 m;
                glm_mat4_identity(*const_cast<mat4*>(&m));
                glm_rotate_y(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_z)
    randomize_xmtrx_();
    shz::xmtrx::init_rotation_z(shz::pi_f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  { cosf(shz::pi_f), sinf(shz::pi_f), 0.0f, 0.0f,
                    sinf(shz::pi_f), cosf(shz::pi_f), 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f }));
    // Double arguments must select the radian overload.
    {
        shz::mat4x4 fromFloat, fromDouble;

        shz::xmtrx::init_rotation_z(0.6f);
        shz::xmtrx::store(&fromFloat);

        randomize_xmtrx_();
        shz::xmtrx::init_rotation_z(static_cast<double>(0.6f));
        shz::xmtrx::store(&fromDouble);

        GBL_TEST_VERIFY(fromDouble == fromFloat);
    }
    {
        volatile float angle = 0.6f;

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::init_rotation_z", [](const volatile float* r) {
                shz::xmtrx::init_rotation_z(*r);
            },
            "glm_mat4_identity + glm_rotate_z", [](const volatile float* r) {
                static volatile mat4 m;
                glm_mat4_identity(*const_cast<mat4*>(&m));
                glm_rotate_z(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle));
    }
GBL_TEST_CASE_END

static void glm_rotate_ordered_(mat4 m, const char* order, float a0, float a1, float a2) {
    const float angles[3] = { a0, a1, a2 };
    for(int i = 0; i < 3; ++i) {
        switch(order[i]) {
        case 'x': glm_rotate_x(m, angles[i], m); break;
        case 'y': glm_rotate_y(m, angles[i], m); break;
        case 'z': glm_rotate_z(m, angles[i], m); break;
        }
    }
}

static constexpr float rotation_angles_[][3] = {
    { 0.0f,        0.0f,        0.0f        },
    { shz::pi_f_2, 0.0f,        0.0f        },
    { 0.0f,        shz::pi_f_2, 0.0f        },
    { 0.0f,        0.0f,        shz::pi_f_2 },
    { 0.3f,       -1.2f,        2.5f        },
    {-2.9f,        0.7f,       -0.4f        },
    { 1.1f,        1.3f,       -2.2f        },
};

static constexpr float rotation_start_[16] = { 2.0f,  0.5f, -1.0f, 0.0f,
                                               0.25f, 3.0f,  0.75f, 0.0f,
                                              -1.5f,  1.0f,  1.25f, 0.0f,
                                               7.0f,  8.0f,  9.0f,  1.0f };

static bool rotation_matches_glm_(const shz::mat4x4& res, const mat4 ref) {
    for(int c = 0; c < 4; ++c)
        for(int r = 0; r < 4; ++r)
            if(shz::fabsf(res.elem2D[c][r] - ref[c][r]) > 1e-3f * shz::fmaxf(1.0f, shz::fabsf(ref[c][r])))
                return false;
    return true;
}

template<typename F>
static bool check_xmtrx_rotation_(const char* order, bool init, F&& fn) {
    for(const auto& a : rotation_angles_) {
        shz::mat4x4 res;
        shz_glm_mat4 ref;

        randomize_xmtrx_();
        if(init) {
            glm_mat4_identity(ref.glm);
        } else {
            shz::xmtrx::load(rotation_start_);
            memcpy(ref.glm, rotation_start_, sizeof(rotation_start_));
        }

        fn(a[0], a[1], a[2]);
        shz::xmtrx::store(&res);
        glm_rotate_ordered_(ref.glm, order, a[0], a[1], a[2]);

        if(!rotation_matches_glm_(res, ref.glm))
            return false;
    }
    return true;
}

// A rotation by 16-bit binary angles, 0x4000 per quarter turn, against the radian rotation of each angle.
static bool check_xmtrx_rotation_u16_(void (*u16)(uint16_t), void (*radians)(float)) {
    const uint16_t angles[] = { 0x0000, 0x2000, 0x4000, 0x8000, 0xc000, 0x04d2, 0xfedc, 0xffff };

    for(uint16_t angle : angles) {
        shz::mat4x4 expected, actual;

        shz::xmtrx::init_identity();
        shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
        radians(static_cast<int16_t>(angle) * (1.0f / shz::fsca_rad_factor));
        shz::xmtrx::store(&expected);

        shz::xmtrx::init_identity();
        shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
        u16(angle);
        shz::xmtrx::store(&actual);

        if(!(actual == expected))
            return false;
    }
    return true;
}

// Radians in FSCA's 16-bit units, scaled and truncated as the radian routines' FTRC does.
static uint16_t fsca_angle_(float radians) {
    return static_cast<uint16_t>(static_cast<int32_t>(radians * shz::fsca_rad_factor));
}

template<typename F>
static bool bench_xmtrx_rotation_(const char* shzName, const char* glmName, const char* order, bool init, F&& fn) {
    static shz_glm_mat4 glmMat;
    glm_mat4_identity(glmMat.glm);
    return (benchmark_cmp<void>)(
        shzName, fn,
        glmName, [order, init](float a0, float a1, float a2) {
            if(init)
                glm_mat4_identity(glmMat.glm);
            glm_rotate_ordered_(glmMat.glm, order, a0, a1, a2);
        },
        0.3f, -1.2f, 2.5f);
}

GBL_TEST_CASE(init_rotation_xyz)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_xyz(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_xyz(shz::vec3(a0, a1, a2));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_xyz(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::init_rotation_xyz(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::init_rotation_xyz(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::init_rotation_xyz", "glm_mat4_identity + glm_rotate_x/y/z", "xyz", true,
                                          [](float a0, float a1, float a2) { shz::xmtrx::init_rotation_xyz(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_zyx)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_zyx(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_zyx(shz::vec3(a2, a1, a0));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_zyx(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::init_rotation_zyx(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::init_rotation_zyx(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::init_rotation_zyx", "glm_mat4_identity + glm_rotate_z/y/x", "zyx", true,
                                          [](float a0, float a1, float a2) { shz::xmtrx::init_rotation_zyx(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_zxy)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_zxy(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_zxy(shz::vec3(a1, a2, a0));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_zxy(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::init_rotation_zxy(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::init_rotation_zxy(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::init_rotation_zxy", "glm_mat4_identity + glm_rotate_z/x/y", "zxy", true,
                                          [](float a0, float a1, float a2) { shz::xmtrx::init_rotation_zxy(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_yxz)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_yxz(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_yxz(shz::vec3(a1, a0, a2));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", true, [](float a0, float a1, float a2) {
        shz::xmtrx::init_rotation_yxz(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::init_rotation_yxz(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::init_rotation_yxz(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::init_rotation_yxz", "glm_mat4_identity + glm_rotate_y/x/z", "yxz", true,
                                          [](float a0, float a1, float a2) { shz::xmtrx::init_rotation_yxz(a0, a1, a2); }));
GBL_TEST_CASE_END

static float u16_radians_(uint16_t angle) {
    return static_cast<int16_t>(angle) * (1.0f / shz::fsca_rad_factor);
}

template<typename U, typename R>
static bool check_xmtrx_init_rotation_u16_(U&& u16, R&& radians) {
    const uint16_t angles[] = { 0x0000, 0x2000, 0x4000, 0x8000, 0xc000, 0x04d2, 0xfedc, 0xffff };
    constexpr unsigned count = sizeof(angles) / sizeof(angles[0]);

    for(unsigned i = 0; i < count; ++i) {
        const uint16_t a0 = angles[i];
        const uint16_t a1 = angles[(i + 3) % count];
        const uint16_t a2 = angles[(i + 5) % count];
        shz::mat4x4 expected, actual;

        randomize_xmtrx_();
        radians(u16_radians_(a0), u16_radians_(a1), u16_radians_(a2));
        shz::xmtrx::store(&expected);

        randomize_xmtrx_();
        u16(a0, a1, a2);
        shz::xmtrx::store(&actual);

        if(!(actual == expected))
            return false;
    }
    return true;
}

GBL_TEST_CASE(init_rotation_x_u16)
    GBL_TEST_VERIFY(check_xmtrx_init_rotation_u16_(
        [](uint16_t a, uint16_t, uint16_t) { shz::xmtrx::init_rotation_x(a); },
        [](float a, float, float) { shz::xmtrx::init_rotation_x(a); }));
    (benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_x(uint16_t)", [](uint16_t angle) { shz::xmtrx::init_rotation_x(angle); },
        "shz::xmtrx::init_rotation_x(float)", [](uint16_t angle) {
            shz::xmtrx::init_rotation_x(angle * (1.0f / shz::fsca_rad_factor));
        },
        static_cast<uint16_t>(0x2000));
    {
        volatile uint16_t angle16 = 0x2000;
        volatile float    angle   = 0x2000 * (1.0f / shz::fsca_rad_factor);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::init_rotation_x(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::init_rotation_x(static_cast<uint16_t>(*a));
            },
            "glm_mat4_identity + glm_rotate_x", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_mat4_identity(*const_cast<mat4*>(&m));
                glm_rotate_x(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle16, &angle));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_y_u16)
    GBL_TEST_VERIFY(check_xmtrx_init_rotation_u16_(
        [](uint16_t a, uint16_t, uint16_t) { shz::xmtrx::init_rotation_y(a); },
        [](float a, float, float) { shz::xmtrx::init_rotation_y(a); }));
    (benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_y(uint16_t)", [](uint16_t angle) { shz::xmtrx::init_rotation_y(angle); },
        "shz::xmtrx::init_rotation_y(float)", [](uint16_t angle) {
            shz::xmtrx::init_rotation_y(angle * (1.0f / shz::fsca_rad_factor));
        },
        static_cast<uint16_t>(0x2000));
    {
        volatile uint16_t angle16 = 0x2000;
        volatile float    angle   = 0x2000 * (1.0f / shz::fsca_rad_factor);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::init_rotation_y(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::init_rotation_y(static_cast<uint16_t>(*a));
            },
            "glm_mat4_identity + glm_rotate_y", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_mat4_identity(*const_cast<mat4*>(&m));
                glm_rotate_y(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle16, &angle));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_z_u16)
    GBL_TEST_VERIFY(check_xmtrx_init_rotation_u16_(
        [](uint16_t a, uint16_t, uint16_t) { shz::xmtrx::init_rotation_z(a); },
        [](float a, float, float) { shz::xmtrx::init_rotation_z(a); }));
    (benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_z(uint16_t)", [](uint16_t angle) { shz::xmtrx::init_rotation_z(angle); },
        "shz::xmtrx::init_rotation_z(float)", [](uint16_t angle) {
            shz::xmtrx::init_rotation_z(angle * (1.0f / shz::fsca_rad_factor));
        },
        static_cast<uint16_t>(0x2000));
    {
        volatile uint16_t angle16 = 0x2000;
        volatile float    angle   = 0x2000 * (1.0f / shz::fsca_rad_factor);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::init_rotation_z(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::init_rotation_z(static_cast<uint16_t>(*a));
            },
            "glm_mat4_identity + glm_rotate_z", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_mat4_identity(*const_cast<mat4*>(&m));
                glm_rotate_z(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle16, &angle));
    }
GBL_TEST_CASE_END

template<typename U, typename R, typename S>
static bool test_xmtrx_init_rotation_u16_(const char* order, const char* shzName, const char* splitName, const char* glmName,
                                         U&& u16, R&& radians, S&& split) {
    if(!check_xmtrx_init_rotation_u16_(u16, radians))
        return false;

    if(!check_xmtrx_rotation_(order, true, [&u16](float a0, float a1, float a2) {
        u16(fsca_angle_(a0), fsca_angle_(a1), fsca_angle_(a2));
    }))
        return false;

    (benchmark_cmp<void>)(shzName, u16, splitName, split,
                          fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f));

    static shz_glm_mat4 glmMat;
    return (benchmark_cmp<void>)(
        shzName, u16,
        glmName, [order](uint16_t a0, uint16_t a1, uint16_t a2) {
            glm_mat4_identity(glmMat.glm);
            glm_rotate_ordered_(glmMat.glm, order, u16_radians_(a0), u16_radians_(a1), u16_radians_(a2));
        },
        fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f));
}

GBL_TEST_CASE(init_rotation_xyz_u16)
    GBL_TEST_VERIFY(test_xmtrx_init_rotation_u16_("xyz",
        "shz::xmtrx::init_rotation_xyz(uint16_t)",
        "init_rotation_x + rotate_y + rotate_z (uint16_t)",
        "glm_mat4_identity + glm_rotate_x/y/z",
        [](uint16_t x, uint16_t y, uint16_t z) { shz::xmtrx::init_rotation_xyz(x, y, z); },
        [](float x, float y, float z) { shz::xmtrx::init_rotation_xyz(x, y, z); },
        [](uint16_t x, uint16_t y, uint16_t z) {
            shz::xmtrx::init_rotation_x(x);
            shz::xmtrx::rotate_y(y);
            shz::xmtrx::rotate_z(z);
        }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_zyx_u16)
    GBL_TEST_VERIFY(test_xmtrx_init_rotation_u16_("zyx",
        "shz::xmtrx::init_rotation_zyx(uint16_t)",
        "init_rotation_z + rotate_y + rotate_x (uint16_t)",
        "glm_mat4_identity + glm_rotate_z/y/x",
        [](uint16_t z, uint16_t y, uint16_t x) { shz::xmtrx::init_rotation_zyx(z, y, x); },
        [](float z, float y, float x) { shz::xmtrx::init_rotation_zyx(z, y, x); },
        [](uint16_t z, uint16_t y, uint16_t x) {
            shz::xmtrx::init_rotation_z(z);
            shz::xmtrx::rotate_y(y);
            shz::xmtrx::rotate_x(x);
        }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_zxy_u16)
    GBL_TEST_VERIFY(test_xmtrx_init_rotation_u16_("zxy",
        "shz::xmtrx::init_rotation_zxy(uint16_t)",
        "init_rotation_z + rotate_x + rotate_y (uint16_t)",
        "glm_mat4_identity + glm_rotate_z/x/y",
        [](uint16_t z, uint16_t x, uint16_t y) { shz::xmtrx::init_rotation_zxy(z, x, y); },
        [](float z, float x, float y) { shz::xmtrx::init_rotation_zxy(z, x, y); },
        [](uint16_t z, uint16_t x, uint16_t y) {
            shz::xmtrx::init_rotation_z(z);
            shz::xmtrx::rotate_x(x);
            shz::xmtrx::rotate_y(y);
        }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_yxz_u16)
    GBL_TEST_VERIFY(test_xmtrx_init_rotation_u16_("yxz",
        "shz::xmtrx::init_rotation_yxz(uint16_t)",
        "init_rotation_y + rotate_x + rotate_z (uint16_t)",
        "glm_mat4_identity + glm_rotate_y/x/z",
        [](uint16_t y, uint16_t x, uint16_t z) { shz::xmtrx::init_rotation_yxz(y, x, z); },
        [](float y, float x, float z) { shz::xmtrx::init_rotation_yxz(y, x, z); },
        [](uint16_t y, uint16_t x, uint16_t z) {
            shz::xmtrx::init_rotation_y(y);
            shz::xmtrx::rotate_x(x);
            shz::xmtrx::rotate_z(z);
        }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation)

GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_dir)
    // Rotating about a standard basis axis should match the dedicated
    // init_rotation_x() routine exactly (axis-angle is a strict generalization).
    {
        randomize_xmtrx_();
        shz::xmtrx::init_rotation_dir(shz::pi_f, shz::vec3(1.0f, 0.0f, 0.0f));
        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                      { 1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, cosf(shz::pi_f), -sinf(shz::pi_f), 0.0f,
                        0.0f, sinf(shz::pi_f), cosf(shz::pi_f), 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f}));
    }

    // Rotate around a non-trivial (non-axis-aligned) normalized axis, cross-
    // checked against cglm's equivalent from-scratch construction.
    {
        randomize_xmtrx_();

        shz::vec3 axis  = shz::vec3(1.0f, 1.0f, 1.0f).direction();
        float     angle = shz::deg_to_rad(42.0f);

        shz::xmtrx::init_rotation_dir(angle, axis);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        mat4 glmResult;
        vec3 glmAxis = { axis.x, axis.y, axis.z };
        glm_rotate_make(glmResult, angle, glmAxis);

        GBL_TEST_VERIFY(compare_glm(result, glmResult));
    }

    {
        GBL_TEST_VERIFY(
            [] SHZ_NO_INLINE {
                shz::vec3 axis  = shz::vec3(1.0f, 1.0f, 1.0f).direction();
                float     angle = shz::deg_to_rad(42.0f);
                volatile mat4 m;

                bool success =
                    (benchmark_cmp<void>)(
                        "shz::xmtrx::init_rotation_dir", [](float angle, const shz::vec3& dir) {
                            shz::xmtrx::init_rotation_dir(angle, dir);
                        },
                        "glm_rotate_make", [&](float angle, const shz::vec3& dir) {
                            volatile vec3 a = { dir.x, dir.y, dir.z };
                            glm_rotate_make(const_cast<mat4&>(m), angle, const_cast<vec3&>(a));
                        },
                        angle, axis
                    );

                return success;
            }()
        );
    }
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

GBL_TEST_CASE(init_upper_triangular)
    randomize_xmtrx_();
    shz::xmtrx::init_upper_triangular(  1.0f,
                                    { 2.0f, 3.0f },
                                    { 4.0f, 5.0f, 6.0f },
                                    { 7.0f, 8.0f, 9.0f, 10.0f });
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    1.0f, 2.0f, 4.0f, 7.0f,
                                    0.0f, 3.0f, 5.0f, 8.0f,
                                    0.0f, 0.0f, 6.0f, 9.0f,
                                    0.0f, 0.0f, 0.0f, 10.0f
                                }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_lower_triangular)
    randomize_xmtrx_();
    shz::xmtrx::init_lower_triangular({ 7.0f, 8.0f, 9.0f, 10.0f },
                                    { 4.0f, 5.0f, 6.0f },
                                    { 2.0f, 3.0f },
                                      1.0f);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    7.0f, 0.0f, 0.0f, 0.0f,
                                    8.0f, 4.0f, 0.0f, 0.0f,
                                    9.0f, 5.0f, 2.0f, 0.0f,
                                   10.0f, 6.0f, 3.0f, 1.0f
                                }));
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
    (benchmark)(nullptr, "shz::xmtrx::init_screen", [&] { shz::xmtrx::init_screen(640.0f, 480.0f); });
GBL_TEST_CASE_END

GBL_TEST_CASE(init_permutation_wxyz)
    randomize_xmtrx_();
    shz::xmtrx::init_permutation_wxyz();

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f
    }));

GBL_TEST_CASE_END

GBL_TEST_CASE(init_permutation_yzwx)
    randomize_xmtrx_();
    shz::xmtrx::init_permutation_yzwx();

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f
    }));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_permutation_wzyx)

    randomize_xmtrx_();
    shz::xmtrx::init_permutation_wzyx();

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f
    }));

GBL_TEST_CASE_END

GBL_TEST_CASE(init_outer_product)
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_4x4)
    alignas(32) shz::mat4x4 in;

    in.init_translation(1.0f, 2.0f, 3.0f);
    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
    shz::xmtrx::apply(in);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    2.0f, 0.0f, 0.0f, 2.0f,
                                    0.0f, 3.0f, 0.0f, 6.0f,
                                    0.0f, 0.0f, 4.0f, 12.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    {
        alignas(32) shz::mat4x4 xmtrx;

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::apply", [](shz::mat4x4& m) { shz::xmtrx::apply(m); },
                "glm_mat4_mul",      [&](shz::mat4x4& m) { glm_mat4_mul(*reinterpret_cast<mat4*>(&xmtrx),
                                                                        *reinterpret_cast<mat4*>(&m),
                                                                        *reinterpret_cast<mat4*>(&xmtrx));
                                                                    },
                in
            )
        );
    }
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_unaligned_4x4)
    alignas(32) char buffer[sizeof(shz::mat4x4) + 4];
    auto *in = new (buffer + 4) std::array<float, 16> {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 2.0f, 3.0f, 1.0f
    };

    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
    shz::xmtrx::apply(in->data());

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    2.0f, 0.0f, 0.0f, 2.0f,
                                    0.0f, 3.0f, 0.0f, 6.0f,
                                    0.0f, 0.0f, 4.0f, 12.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    (benchmark)(nullptr, "shz::xmtrx::apply_unaligned()", [](const float* m) {
        shz::xmtrx::apply(m);
    }, in->data());
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_transpose_4x4)
    alignas(32) shz_mat4x4_t in{
        .elem = {
            2.0f, 3.0f, 1.0f, 4.0f,
            5.0f, 1.0f, 6.0f, 2.0f,
            3.0f, 7.0f, 2.0f, 5.0f,
            1.0f, 4.0f, 8.0f, 3.0f
        }
    };
    alignas(32) const float xmat[] = {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    shz::xmtrx::load(xmat);
    shz::xmtrx::apply_transpose(in);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    67.0f, 123.0f, 153.0f, 98.0f,
                                    78.0f, 138.0f, 170.0f, 112.0f,
                                    89.0f, 153.0f, 187.0f, 126.0f,
                                    100.0f, 168.0f, 204.0f, 140.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    (benchmark)(nullptr, "shz::xmtrx::apply_transpose_4x4()", [](const shz::mat4x4& m) {
        shz::xmtrx::apply_transpose(m);
    }, in);
#endif

GBL_TEST_CASE_END

GBL_TEST_CASE(apply_transpose_unaligned_4x4)
    alignas(32) char buffer[sizeof(shz::mat4x4) + 4];
    auto *in = new (buffer + 4) std::array<float, 16> {
        2.0f, 3.0f, 1.0f, 4.0f,
        5.0f, 1.0f, 6.0f, 2.0f,
        3.0f, 7.0f, 2.0f, 5.0f,
        1.0f, 4.0f, 8.0f, 3.0f
    };

    alignas(32) const float xmat[] = {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    shz::xmtrx::load(xmat);
    shz::xmtrx::apply_transpose(in->data());

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    67.0f, 123.0f, 153.0f, 98.0f,
                                    78.0f, 138.0f, 170.0f, 112.0f,
                                    89.0f, 153.0f, 187.0f, 126.0f,
                                    100.0f, 168.0f, 204.0f, 140.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    (benchmark)(nullptr, "shz::xmtrx::apply_transpose_unaligned_4x4()", [](const float* m) {
        shz::xmtrx::apply_transpose(m);
    }, in->data());
#endif

GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_4x4)
    alignas(32) shz_mat4x4_t in{
        .elem = {
            2.0f, 3.0f, 1.0f, 4.0f,
            5.0f, 1.0f, 6.0f, 2.0f,
            3.0f, 7.0f, 2.0f, 5.0f,
            1.0f, 4.0f, 8.0f, 3.0f
        }
    };
    alignas(32) const float xmat[] = {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    shz::xmtrx::load(xmat);
    shz::xmtrx::apply_reverse(in);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    25.0f, 69.0f, 113.0f, 157.0f,
                                    42.0f, 102.0f, 162.0f, 222.0f,
                                    51.0f, 119.0f, 187.0f, 255.0f,
                                    35.0f, 91.0f, 147.0f, 203.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    (benchmark)(nullptr, "shz::xmtrx::apply_reverse_4x4()", [](const shz::mat4x4& m) {
        shz::xmtrx::apply_reverse(m);
    }, in);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_unaligned_4x4)
    alignas(32) char buffer[sizeof(shz::mat4x4) + 4];
    auto *in = new (buffer + 4) std::array<float, 16> {
        2.0f, 3.0f, 1.0f, 4.0f,
        5.0f, 1.0f, 6.0f, 2.0f,
        3.0f, 7.0f, 2.0f, 5.0f,
        1.0f, 4.0f, 8.0f, 3.0f
    };

    alignas(32) const float xmat[] = {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    shz::xmtrx::load(xmat);
    shz::xmtrx::apply_reverse(in->data());

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    25.0f, 69.0f, 113.0f, 157.0f,
                                    42.0f, 102.0f, 162.0f, 222.0f,
                                    51.0f, 119.0f, 187.0f, 255.0f,
                                    35.0f, 91.0f, 147.0f, 203.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    (benchmark)(nullptr, "shz::xmtrx::apply_reverse_unaligned_4x4()", [](const float* m) {
        shz::xmtrx::apply_reverse(m);
    }, in->data());
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_transpose_4x4)
    alignas(32) shz_mat4x4_t in{
        .elem = {
            2.0f, 3.0f, 1.0f, 4.0f,
            5.0f, 1.0f, 6.0f, 2.0f,
            3.0f, 7.0f, 2.0f, 5.0f,
            1.0f, 4.0f, 8.0f, 3.0f
        }
    };
    alignas(32) const float xmat[] = {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    shz::xmtrx::load(xmat);
    shz::xmtrx::apply_reverse_transpose(in);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    27.0f, 67.0f, 107.0f, 147.0f,
                                    33.0f, 89.0f, 145.0f, 201.0f,
                                    43.0f, 111.0f, 179.0f, 247.0f,
                                    45.0f, 109.0f, 173.0f, 237.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    (benchmark)(nullptr, "shz::xmtrx::apply_reverse_transpose_4x4()", [](const shz::mat4x4& m) {
        shz::xmtrx::apply_reverse_transpose(m);
    }, in);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_transpose_unaligned_4x4)
    alignas(32) char buffer[sizeof(shz::mat4x4) + 4];
    auto *in = new (buffer + 4) std::array<float, 16> {
        2.0f, 3.0f, 1.0f, 4.0f,
        5.0f, 1.0f, 6.0f, 2.0f,
        3.0f, 7.0f, 2.0f, 5.0f,
        1.0f, 4.0f, 8.0f, 3.0f
    };

    alignas(32) const float xmat[] = {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };

    shz::xmtrx::load(xmat);
    shz::xmtrx::apply_reverse_transpose(in->data());

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    27.0f, 67.0f, 107.0f, 147.0f,
                                    33.0f, 89.0f, 145.0f, 201.0f,
                                    43.0f, 111.0f, 179.0f, 247.0f,
                                    45.0f, 109.0f, 173.0f, 237.0f
                                }));

#if SHZ_BACKEND == SHZ_SH4
    (benchmark)(nullptr, "shz::xmtrx::apply_reverse_transpose_unaligned_4x4()", [](const float* m) {
        shz::xmtrx::apply_reverse_transpose(m);
    }, in->data());
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(add_translation)
    // Apply against identity.
    // Expectation: Only touches translation parts of the matrix
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::add_translation(10.0f, -20.0f, 30.0f);

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
        shz::xmtrx::add_translation(20.0f, -100.0f, -60.0f);

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
        shz::xmtrx::init_identity();

        shz::xmtrx::add_translation(2.0f, 3.0f, -4.0f);
        shz::xmtrx::add_translation(5.0f, 6.0f,  7.0f);

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

        {
            vec3 t1v = { 2.0f, 3.0f, -4.0f };
            vec3 t2v = { 5.0f, 6.0f, 7.0f };
            glm_vec3_copy(t1v, t1);
            glm_vec3_copy(t2v, t2);

            glm_translate(m_glm, t1);
            glm_translate(m_glm, t2);

            GBL_TEST_VERIFY(compare_glm(res, m_glm));
        }
    }

    {
        mat4 xmtrx = GLM_MAT4_IDENTITY_INIT;

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::add_translation", [](float x, float y, float z) {
                    shz::xmtrx::add_translation(x, y, z);
                },
                "glm_translate", [&](float x, float y, float z) {
                    vec3 t = { x, y, z };
                    glm_translate(xmtrx, t);
                },
                100.0f, 200.0f, 300.0f
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(set_translation)
    // Set against identity.
    // Expectation: Same result as init_translation()/a fresh translation matrix.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::set_translation(10.0f, -20.0f, 30.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    { 1.0f, 0.0f, 0.0f, 10.0f,
                      0.0f, 1.0f, 0.0f, -20.0f,
                      0.0f, 0.0f, 1.0f, 30.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }));
    }

    // Set against pre-filled matrix.
    // Expectation: ONLY the translation column (XF12/13/14) is overwritten;
    // everything else--including the bottom-right w component--is left untouched.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_fill(32.0f);
        shz::xmtrx::set_translation(20.0f, -100.0f, -60.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        32.0f, 32.0f, 32.0f, 20.0f,
                        32.0f, 32.0f, 32.0f, -100.0f,
                        32.0f, 32.0f, 32.0f, -60.0f,
                        32.0f, 32.0f, 32.0f, 32.0f
                    }));
    }

    // Test overwrite (NOT composition) semantics.
    // Expectation: The second call fully replaces the first, unlike add_translation().
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();

        shz::xmtrx::set_translation(2.0f, 3.0f, -4.0f);
        shz::xmtrx::set_translation(5.0f, 6.0f, 7.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        1.0f, 0.0f, 0.0f, 5.0f,
                        0.0f, 1.0f, 0.0f, 6.0f,
                        0.0f, 0.0f, 1.0f, 7.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::set_translation", [](float x, float y, float z) {
                shz::xmtrx::set_translation(x, y, z);
            },
            "glm direct column write", [](float x, float y, float z) {
                static volatile mat4 m = GLM_MAT4_IDENTITY_INIT;
                m[3][0] = x;
                m[3][1] = y;
                m[3][2] = z;
                (void)m;
            },
            100.0f, 200.0f, 300.0f
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(get_translation)
    // Extract from a fresh translation matrix.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::set_translation(10.0f, -20.0f, 30.0f);

        GBL_TEST_VERIFY(shz::xmtrx::get_translation() == shz::vec3(10.0f, -20.0f, 30.0f));
    }

    // Extract from an arbitrary, fully-populated matrix.
    // Expectation: Only the translation column is read back; rotation/scale/w are ignored.
    {
        randomize_xmtrx_();
        alignas(32) shz_mat4x4_t mat4 = {
            .left    = { -1.0f,   2.0f,  3.0f,  8.0f },
            .up      = {  4.0f,  -5.0f,  6.0f,  7.0f },
            .forward = {  7.0f,   8.0f,  9.0f, 10.0f },
            .pos     = { 11.0f, -12.0f, 13.0f, 14.0f }
        };
        shz::xmtrx::load(mat4);

        GBL_TEST_VERIFY(shz::xmtrx::get_translation() == shz::vec3(11.0f, -12.0f, 13.0f));
    }

    // Extract from a matrix with a zero/negative translation.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::set_translation(0.0f, -0.0f, -5.0f);

        GBL_TEST_VERIFY(shz::xmtrx::get_translation() == shz::vec3(0.0f, 0.0f, -5.0f));
    }

    {
        ::mat4 glmMat = GLM_MAT4_IDENTITY_INIT;
        glmMat[3][0] = 100.0f;
        glmMat[3][1] = -200.0f;
        glmMat[3][2] = 300.0f;
        shz::xmtrx::load(reinterpret_cast<const float*>(glmMat));

        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)(
                "shz::xmtrx::get_translation", [] { return shz::xmtrx::get_translation(); },
                "glm_vec3(mat[3])",            [&] {
                    union {
                        shz::vec3 shz;
                        vec3      glm;
                    } v3;
                    glm_vec3(glmMat[3], v3.glm);
                    return v3.shz;
                }
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(set_scale)
    // Set against identity.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::set_scale(-2.0f, -3.0f, -4.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    { -2.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, -3.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, -4.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }));
    }

    // Set against a pre-filled matrix.
    // Expectation: The ENTIRE upper-left 3x3 block is overwritten with a clean
    // diagonal--off-diagonal terms are zeroed too, not just the diagonal set.
    // Only row 3 (w) and the translation column are left untouched.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_fill(32.0f);
        shz::xmtrx::set_scale(20.0f, -100.0f, -60.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        20.0f, 0.0f, 0.0f, 32.0f,
                        0.0f, -100.0f, 0.0f, 32.0f,
                        0.0f, 0.0f, -60.0f, 32.0f,
                        32.0f, 32.0f, 32.0f, 32.0f
                    }));
    }

    // Test overwrite (NOT composition) semantics.
    // Expectation: The second call fully replaces the first, unlike scale_3x3().
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();

        shz::xmtrx::set_scale(2.0f, 3.0f, -4.0f);
        shz::xmtrx::set_scale(5.0f, 6.0f, 7.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        5.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 6.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 7.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::set_scale", [](float x, float y, float z) {
                shz::xmtrx::set_scale(x, y, z);
            },
            "glm direct diagonal write", [](float x, float y, float z) {
                static volatile mat4 m = GLM_MAT4_IDENTITY_INIT;
                m[0][0] = x;    m[0][1] = 0.0f; m[0][2] = 0.0f;
                m[1][0] = 0.0f; m[1][1] = y;    m[1][2] = 0.0f;
                m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = z;
                (void)m;
            },
            -2.0f, -3.0f, -4.0f
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(get_scale)
    // Extract from a fresh, axis-aligned scale.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::set_scale(10.0f, -20.0f, 30.0f);

        GBL_TEST_VERIFY(shz::xmtrx::get_scale() == shz::vec3(10.0f, 20.0f, 30.0f));
    }

    // Extract from a matrix with scale composed under a rotation.
    // Expectation: Per-axis magnitude extraction recovers the correct scale
    // regardless of rotation, since rotating a column doesn't change its length.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::rotate_x(shz::deg_to_rad(30.0f));
        shz::xmtrx::rotate_y(shz::deg_to_rad(60.0f));
        shz::xmtrx::scale_3x3(3.0f, 4.0f, 5.0f);

        shz::mat4x4 mat;
        shz::xmtrx::store(&mat);

        mat4 glmMat;
        vec3 glmScale;
        memcpy(glmMat, &mat, sizeof(glmMat));
        glm_decompose_scalev(glmMat, glmScale);

        shz::vec3 scale = shz::xmtrx::get_scale();
        GBL_TEST_VERIFY(shz_equalf(scale.x, glmScale[0]));
        GBL_TEST_VERIFY(shz_equalf(scale.y, glmScale[1]));
        GBL_TEST_VERIFY(shz_equalf(scale.z, glmScale[2]));
    }

    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::set_scale(0.0f, 0.0f, 0.0f);

        shz::vec3 scale = shz::xmtrx::get_scale();

        GBL_TEST_VERIFY(scale == shz::vec3(0.0f, 0.0f, 0.0f));
    }

    {
        randomize_xmtrx_();
        shz::xmtrx::init_zero();

        shz::vec3 scale = shz::xmtrx::get_scale();

        GBL_TEST_VERIFY(scale == shz::vec3(0.0f, 0.0f, 0.0f));
    }

    {
        shz::xmtrx::init_identity();
        shz::xmtrx::rotate_x(shz::deg_to_rad(30.0f));
        shz::xmtrx::rotate_y(shz::deg_to_rad(60.0f));
        shz::xmtrx::scale_3x3(3.0f, 4.0f, 5.0f);

        shz::mat4x4 mat;
        shz::xmtrx::store(&mat);

        alignas(32) volatile mat4 glmMat;
        memcpy(*const_cast<mat4*>(&glmMat), &mat, sizeof(glmMat));

        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)(
                "shz::xmtrx::get_scale", [] {
                    return shz::xmtrx::get_scale();
                },
                "glm_decompose_scalev", [&] {
                    volatile shz_glm_vec3 s;
                    glm_decompose_scalev(*const_cast<mat4*>(&glmMat), *const_cast<vec3*>(&s.glm));
                    return s.shz;
                }
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(get_rotation)
    auto verify = [&] {
        shz::mat4x4 mat;
        shz::xmtrx::store(&mat);

        float     expectedAngle, angle;
        shz::vec3 expectedAxis,  axis;
        mat.get_rotation(&expectedAngle, &expectedAxis);
        shz::xmtrx::get_rotation(&angle, &axis);

        const auto [pairAngle, pairAxis] = shz::xmtrx::get_rotation();

        return angle == expectedAngle && axis == expectedAxis &&
               pairAngle == angle && pairAxis == axis;
    };

    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    GBL_TEST_VERIFY(verify());

    randomize_xmtrx_();
    shz::xmtrx::init_rotation_x(0.5f);
    GBL_TEST_VERIFY(verify());

    randomize_xmtrx_();
    shz::xmtrx::init_rotation(1.2f, shz::vec3(1.0f, 2.0f, 3.0f).direction());
    GBL_TEST_VERIFY(verify());

    randomize_xmtrx_();
    shz::xmtrx::init_rotation(-2.5f, shz::vec3(1.0f, 2.0f, 3.0f).direction());
    shz::xmtrx::add_translation(5.0f, -6.0f, 7.0f);
    GBL_TEST_VERIFY(verify());

    randomize_xmtrx_();
    shz::xmtrx::init_rotation(3.1f, shz::vec3(-1.0f, 2.0f, -3.0f).direction());
    GBL_TEST_VERIFY(verify());

    randomize_xmtrx_();
    shz::xmtrx::init_rotation(shz::pi_f, shz::vec3(1.0f, 2.0f, 3.0f).direction());
    GBL_TEST_VERIFY(verify());

    for(float radians : { 1.2f, 3.1f }) {
        shz::xmtrx::init_rotation(radians, shz::vec3(1.0f, 2.0f, 3.0f).direction());

        shz::mat4x4 mat;
        shz::xmtrx::store(&mat);

        alignas(32) volatile mat4 glmMat;
        memcpy(*const_cast<mat4*>(&glmMat), &mat, sizeof(glmMat));

        GBL_TEST_VERIFY(
            (benchmark_cmp<float>)(
                radians < 3.0f? "shz::xmtrx::get_rotation" : "shz::xmtrx::get_rotation [near pi]", [] {
                    float     angle;
                    shz::vec3 axis;
                    shz::xmtrx::get_rotation(&angle, &axis);
                    return angle + axis.x;
                },
                radians < 3.0f? "glm_mat4_quat + glm_quat_angle/axis" : "glm_mat4_quat + glm_quat_angle/axis [near pi]", [&] {
                    versor q;
                    vec3   axis;
                    glm_mat4_quat(*const_cast<mat4*>(&glmMat), q);
                    glm_quat_axis(q, axis);
                    return glm_quat_angle(q) + axis[0];
                }
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(scale_3x3)
    // Apply against identity.
    // Expectation: Diagonal scale cells filled in the 3x3 part.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::scale_3x3(10.0f, -20.0f, 30.0f);
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
        shz::xmtrx::scale_3x3(20.0f, -100.0f, -60.0f);

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
        shz::xmtrx::init_identity();

        shz::xmtrx::scale_3x3(0.0f, 0.0f, 0.0f);

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
        shz::xmtrx::scale_3x3(1.0f, 1.0f, 1.0f);

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
        shz::xmtrx::init_identity();

        shz::xmtrx::scale_3x3(2.0f, 3.0f, 4.0f);
        shz::xmtrx::scale_3x3(5.0f, 6.0f, 7.0f);

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

        {
            vec3 scale1v = { 2.0f, 3.0f, 4.0f };
            vec3 scale2v = { 5.0f, 6.0f, 7.0f };
            glm_vec3_copy(scale1v, scale1);
            glm_vec3_copy(scale2v, scale2);

            glm_scale(m_glm, scale1);
            glm_scale(m_glm, scale2);

            GBL_TEST_VERIFY(compare_glm(res, m_glm));
        }
    }

    {
        volatile mat4 xmtrx = GLM_MAT4_IDENTITY_INIT;

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::scale_3x3", [](float x, float y, float z) {
                    shz::xmtrx::scale_3x3(x, y, z);
                },
                "glm_scale", [&](float x, float y, float z) {
                    volatile vec3 s = { x, y, z };
                    glm_scale(*const_cast<mat4*>(&xmtrx), *const_cast<vec3*>(&s));
                },
                2.0f, 3.0f, 4.0f
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate)
    {
        alignas(32) shz_mat4x4_t m = { .elem = { 1.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 1.0f, 0.0f,
                                                 7.0f, 8.0f, 9.0f, 1.0f } };
        randomize_xmtrx_();
        shz::xmtrx::load(m);
        shz::xmtrx::rotate(0.0f, 1.0f, 0.0f, 0.0f);
        shz::xmtrx::store(&m);

        GBL_TEST_VERIFY(m.elem[12] == 7.0f && m.elem[13] == 8.0f &&
                        m.elem[14] == 9.0f && m.elem[15] == 1.0f);
        for(int c = 0; c < 3; ++c)
            for(int r = 0; r < 4; ++r)
                GBL_TEST_VERIFY(shz::fabsf(m.elem2D[c][r] - (c == r ? 1.0f : 0.0f)) <= 1e-4f);
    }

    for(const auto& [angle, axis] : { std::pair{ 1.1f,          shz::vec3(0.3f, -0.5f, 0.8f) },
                                      std::pair{ -2.5f,         shz::vec3(0.0f,  0.0f, 3.0f) },
                                      std::pair{ shz::pi_f_2,   shz::vec3(1.0f,  1.0f, 1.0f) } }) {
        alignas(32) shz_mat4x4_t in = { .elem = { 2.0f,  0.5f, -1.0f, 0.0f,
                                                  0.25f, 3.0f,  0.75f, 0.0f,
                                                 -1.5f,  1.0f,  1.25f, 0.0f,
                                                  7.0f,  8.0f,  9.0f,  1.0f } };
        shz_glm_mat4 ref;
        shz_glm_vec3 glmAxis;
        memcpy(ref.glm, in.elem, sizeof(ref.glm));
        glmAxis.shz = axis;
        glm_rotate(ref.glm, angle, glmAxis.glm);

        shz::mat4x4 out;
        randomize_xmtrx_();
        shz::xmtrx::load(in);
        shz::xmtrx::rotate(angle, axis);
        shz::xmtrx::store(&out);

        for(int i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz::fabsf(out.elem[i] - ref.glm[i / 4][i % 4]) <= 1e-3f * shz::fmaxf(1.0f, shz::fabsf(ref.glm[i / 4][i % 4])));
    }

    {
        volatile mat4  xmtrx = GLM_MAT4_IDENTITY_INIT;
        volatile float in[4] = { 1.1f, 0.3f, -0.5f, 0.8f };

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::rotate", [](const volatile float* a) {
                    shz::xmtrx::rotate(a[0], a[1], a[2], a[3]);
                },
                "glm_rotate", [&](const volatile float* a) {
                    volatile vec3 axis = { a[1], a[2], a[3] };
                    glm_rotate(*const_cast<mat4*>(&xmtrx), a[0], *const_cast<vec3*>(&axis));
                },
                &in[0]
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_dir)
    {
        alignas(32) shz_mat4x4_t m = { .elem = { 1.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 1.0f, 0.0f,
                                                 7.0f, 8.0f, 9.0f, 1.0f } };
        randomize_xmtrx_();
        shz::xmtrx::load(m);
        shz::xmtrx::rotate_dir(0.0f, 1.0f, 0.0f, 0.0f);
        shz::xmtrx::store(&m);

        GBL_TEST_VERIFY(m.elem[12] == 7.0f && m.elem[13] == 8.0f &&
                        m.elem[14] == 9.0f && m.elem[15] == 1.0f);
        for(int c = 0; c < 3; ++c)
            for(int r = 0; r < 4; ++r)
                GBL_TEST_VERIFY(shz::fabsf(m.elem2D[c][r] - (c == r ? 1.0f : 0.0f)) <= 1e-4f);
    }

    for(const auto& [angle, axis] : { std::pair{ 1.1f,        shz::vec3(0.3f, -0.5f, 0.8f).direction() },
                                      std::pair{ -2.5f,       shz::vec3(0.0f,  0.0f, 1.0f)             },
                                      std::pair{ shz::pi_f_2, shz::vec3(1.0f,  1.0f, 1.0f).direction() } }) {
        alignas(32) shz_mat4x4_t in = { .elem = { 2.0f,  0.5f, -1.0f, 0.0f,
                                                  0.25f, 3.0f,  0.75f, 0.0f,
                                                 -1.5f,  1.0f,  1.25f, 0.0f,
                                                  7.0f,  8.0f,  9.0f,  1.0f } };
        shz_glm_mat4 ref;
        shz_glm_vec3 glmAxis;
        memcpy(ref.glm, in.elem, sizeof(ref.glm));
        glmAxis.shz = axis;
        glm_rotate(ref.glm, angle, glmAxis.glm);

        shz::mat4x4 out;
        randomize_xmtrx_();
        shz::xmtrx::load(in);
        shz::xmtrx::rotate_dir(angle, axis);
        shz::xmtrx::store(&out);

        for(int i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz::fabsf(out.elem[i] - ref.glm[i / 4][i % 4]) <= 1e-3f * shz::fmaxf(1.0f, shz::fabsf(ref.glm[i / 4][i % 4])));
    }

    {
        volatile mat4 xmtrx = GLM_MAT4_IDENTITY_INIT;
        const shz::vec3 dir = shz::vec3(0.3f, -0.5f, 0.8f).direction();

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::rotate_dir", [](float angle, float x, float y, float z) {
                    shz::xmtrx::rotate_dir(angle, x, y, z);
                },
                "glm_rotate", [&](float angle, float x, float y, float z) {
                    volatile vec3 axis = { x, y, z };
                    glm_rotate(*const_cast<mat4*>(&xmtrx), angle, *const_cast<vec3*>(&axis));
                },
                1.1f, dir.x, dir.y, dir.z
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_u16)
    const uint16_t  angles[] = { 0x0000, 0x2000, 0x4000, 0x8000, 0xc000, 0x04d2, 0xfedc, 0xffff };
    const shz::vec3 axes[]   = { shz::vec3(0.3f, -0.5f, 0.8f),
                                 shz::vec3(0.0f,  0.0f, 3.0f),
                                 shz::vec3(1.0f,  1.0f, 1.0f) };

    for(uint16_t angle : angles) {
        for(const shz::vec3& axis : axes) {
            shz::mat4x4 expected, actual;

            shz::xmtrx::init_identity();
            shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
            shz::xmtrx::rotate(static_cast<int16_t>(angle) * (1.0f / shz::fsca_rad_factor), axis);
            shz::xmtrx::store(&expected);

            shz::xmtrx::init_identity();
            shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
            shz::xmtrx::rotate(angle, axis);
            shz::xmtrx::store(&actual);

            GBL_TEST_VERIFY(actual == expected);
        }
    }

    {
        const shz::vec3   dir     = shz::vec3(0.3f, -0.5f, 0.8f);
        volatile uint16_t angle16 = 0x2000;
        volatile float    in[4]   = { 0x2000 * (1.0f / shz::fsca_rad_factor), dir.x, dir.y, dir.z };

        (benchmark_cmp<void>)(
            "shz::xmtrx::rotate(uint16_t)", [](const volatile uint16_t* a, const volatile float* v) {
                shz::xmtrx::rotate(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "shz::xmtrx::rotate(float)", [](const volatile uint16_t*, const volatile float* v) {
                shz::xmtrx::rotate(v[0], v[1], v[2], v[3]);
            },
            &angle16, &in[0]);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate(uint16_t)", [](const volatile uint16_t* a, const volatile float* v) {
                shz::xmtrx::rotate(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "glm_rotate", [](const volatile uint16_t*, const volatile float* v) {
                static volatile mat4 m;
                volatile vec3        axis = { v[1], v[2], v[3] };
                glm_rotate(*const_cast<mat4*>(&m), v[0], *const_cast<vec3*>(&axis));
            },
            &angle16, &in[0]));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_dir_u16)
    const uint16_t  angles[] = { 0x0000, 0x2000, 0x4000, 0x8000, 0xc000, 0x04d2, 0xfedc, 0xffff };
    const shz::vec3 axes[]   = { shz::vec3(0.3f, -0.5f, 0.8f).direction(),
                                 shz::vec3(0.0f,  0.0f, 3.0f).direction(),
                                 shz::vec3(1.0f,  1.0f, 1.0f).direction() };

    for(uint16_t angle : angles) {
        for(const shz::vec3& axis : axes) {
            shz::mat4x4 expected, actual;

            shz::xmtrx::init_identity();
            shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
            shz::xmtrx::rotate_dir(static_cast<int16_t>(angle) * (1.0f / shz::fsca_rad_factor), axis);
            shz::xmtrx::store(&expected);

            shz::xmtrx::init_identity();
            shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
            shz::xmtrx::rotate_dir(angle, axis);
            shz::xmtrx::store(&actual);

            GBL_TEST_VERIFY(actual == expected);
        }
    }

    {
        const shz::vec3   dir     = shz::vec3(0.3f, -0.5f, 0.8f).direction();
        volatile uint16_t angle16 = 0x2000;
        volatile float    in[4]   = { 0x2000 * (1.0f / shz::fsca_rad_factor), dir.x, dir.y, dir.z };

        (benchmark_cmp<void>)(
            "shz::xmtrx::rotate_dir(uint16_t)", [](const volatile uint16_t* a, const volatile float* v) {
                shz::xmtrx::rotate_dir(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "shz::xmtrx::rotate_dir(float)", [](const volatile uint16_t*, const volatile float* v) {
                shz::xmtrx::rotate_dir(v[0], v[1], v[2], v[3]);
            },
            &angle16, &in[0]);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_dir(uint16_t)", [](const volatile uint16_t* a, const volatile float* v) {
                shz::xmtrx::rotate_dir(static_cast<uint16_t>(*a), v[1], v[2], v[3]);
            },
            "glm_rotate", [](const volatile uint16_t*, const volatile float* v) {
                static volatile mat4 m;
                volatile vec3        axis = { v[1], v[2], v[3] };
                glm_rotate(*const_cast<mat4*>(&m), v[0], *const_cast<vec3*>(&axis));
            },
            &angle16, &in[0]));
    }
GBL_TEST_CASE_END

static void xmtrx_init_rotation_quat_c_(const shz::quat& q) {
    shz_mat4x4_t mat;
    shz_mat4x4_init_rotation_quat(&mat, q);
    shz_xmtrx_load_4x4(&mat);
}

GBL_TEST_CASE(init_rotation_quat)
    const shz::quat quats[] = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.5f, 0.5f, 0.5f, 0.5f },
        { 0.7071068f, 0.0f, 0.0f, 0.7071068f },
        shz::quat::from_rotation_xyz(0.4f, -0.8f, 1.2f),
        shz::quat::from_rotation_xyz(-2.9f, 0.7f, -0.4f),
        shz::quat::from_rotation_xyz(1.1f, 1.3f, -2.2f),
    };

    for(const auto& q : quats) {
        shz::mat4x4  res, expected;
        shz_glm_mat4 ref;
        versor       v = { q.x, q.y, q.z, q.w };

        randomize_xmtrx_();
        shz::xmtrx::init_rotation_quat(q);
        shz::xmtrx::store(&res);

        randomize_xmtrx_();
        xmtrx_init_rotation_quat_c_(q);
        shz::xmtrx::store(&expected);

        glm_quat_mat4(v, ref.glm);

        GBL_TEST_VERIFY(rotation_matches_glm_(res, expected.elem2D));
        GBL_TEST_VERIFY(compare_glm(res, ref.glm));
    }

    {
        const auto q = shz::quat::from_rotation_xyz(gblRandUniform(-shz::pi_f, shz::pi_f),
                                                    gblRandUniform(-shz::pi_f, shz::pi_f),
                                                    gblRandUniform(-shz::pi_f, shz::pi_f));

        GBL_TEST_VERIFY((benchmark_cmp<void>)("shz::xmtrx::init_rotation_quat",
                                              [](const shz::quat& q) { shz::xmtrx::init_rotation_quat(q); },
                                              "glm_quat_mat4",
                                              [](const shz::quat& q) {
                                                  static volatile mat4 m;
                                                  volatile versor v = { q.x, q.y, q.z, q.w };
                                                  glm_quat_mat4(*const_cast<versor*>(&v), *const_cast<mat4*>(&m));
                                              }, q));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_quat)
    shz::xmtrx::init_identity();
    shz::xmtrx::rotate_quat({ 0.5f, 0.5f, 0.5f, 0.5f });
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

    shz::xmtrx::init_identity();
    shz::xmtrx::rotate_quat({ 0.7071068f, 0.0f, 0.0f, 0.7071068f });
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    0.0f, -1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

    {
        const auto q = shz::quat::from_rotation_xyz(gblRandUniform(-shz::pi_f, shz::pi_f),
                                                    gblRandUniform(-shz::pi_f, shz::pi_f),
                                                    gblRandUniform(-shz::pi_f, shz::pi_f));

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_quat", [](const shz::quat& q) {
                shz::xmtrx::rotate_quat(q);
            },
            "glm_quat_rotate", [](const shz::quat& q) {
                static volatile mat4 m;
                volatile versor v = { q.x, q.y, q.z, q.w };
                glm_quat_rotate(*const_cast<mat4*>(&m), *const_cast<versor*>(&v), *const_cast<mat4*>(&m));
            },
            q));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(permute_wxyz)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::permute_wxyz();

    shz::vec4 input = { 1.0f, 2.0f, 3.0f, 4.0f };
    auto output = shz::xmtrx::transform(input);

    // output should be (w,x,y,z) = (4,1,2,3)
    GBL_TEST_VERIFY(output.x == 4.0f);
    GBL_TEST_VERIFY(output.y == 1.0f);
    GBL_TEST_VERIFY(output.z == 2.0f);
    GBL_TEST_VERIFY(output.w == 3.0f);

GBL_TEST_CASE_END

GBL_TEST_CASE(permute_yzwx)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::permute_yzwx();

    shz::vec4 input = { 1.0f, 2.0f, 3.0f, 4.0f };
    auto output = shz::xmtrx::transform(input);

    // output should be (y,z,w,x) = (2,3,4,1)
    GBL_TEST_VERIFY(output.x == 2.0f);
    GBL_TEST_VERIFY(output.y == 3.0f);
    GBL_TEST_VERIFY(output.z == 4.0f);
    GBL_TEST_VERIFY(output.w == 1.0f);

GBL_TEST_CASE_END

GBL_TEST_CASE(permute_wzyx)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::permute_wzyx();

    shz::vec4 input = { 1.0f, 2.0f, 3.0f, 4.0f };
    auto output = shz::xmtrx::transform(input);

    // output should be (w,z,y,x) = (4,3,2,1)
    GBL_TEST_VERIFY(output.x == 4.0f);
    GBL_TEST_VERIFY(output.y == 3.0f);
    GBL_TEST_VERIFY(output.z == 2.0f);
    GBL_TEST_VERIFY(output.w == 1.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_4x4)
    alignas(32) std::array<float, 16> matrix1 = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    alignas(32) std::array<float, 16> matrix2 = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 2.0f, 3.0f, 1.0f
    };
    alignas(8) std::array<float, 16> out;

    const shz_mat4x4_t* mat1 = reinterpret_cast<const shz_mat4x4_t*>(matrix1.data());
    const shz_mat4x4_t* mat2 = reinterpret_cast<const shz_mat4x4_t*>(matrix2.data());

    benchmark(nullptr, [&] {
        shz::xmtrx::load_apply(*mat1, *mat2);
    });

    shz::xmtrx::store(out.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    1.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 2.0f,
                                    0.0f, 0.0f, 1.0f, 3.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_unaligned_4x4)
    alignas(32) char buffer1[sizeof(shz::mat4x4) + 4];
    alignas(32) char buffer2[sizeof(shz::mat4x4) + 4];

    auto* matrix1 = new (buffer1 + 4) std::array<float, 16> {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    auto* matrix2 = new (buffer2 + 4) std::array<float, 16> {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 2.0f, 3.0f, 1.0f
    };
    alignas(8) std::array<float, 16> out;

    benchmark(nullptr, [&] {
        shz::xmtrx::load_apply(matrix1->data(), matrix2->data());
    });

    shz::xmtrx::store(out.data());
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    1.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 2.0f,
                                    0.0f, 0.0f, 1.0f, 3.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

GBL_TEST_CASE_END

GBL_TEST_CASE(apply_store_4x4)
    alignas(32) shz::mat4x4 in;
    alignas(32) shz::mat4x4 out;

    in.init_translation(1.0f, 2.0f, 3.0f);
    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);

    (benchmark)(nullptr, "shz::xmtrx::apply_store",
                [](shz::mat4x4* out, const shz::mat4x4& in) {
                    shz::xmtrx::apply_store(out, in);
                },
                &out, in);

    shz::xmtrx::load(out);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    2.0f, 0.0f, 0.0f, 2.0f,
                                    0.0f, 3.0f, 0.0f, 6.0f,
                                    0.0f, 0.0f, 4.0f, 12.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f
                                }));
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_store_unaligned_4x4)
    alignas(32) char in[sizeof(shz_mat4x4_t)];
    alignas(32) char out[sizeof(shz_mat4x4_t)];

    shz::mat4x4* i = reinterpret_cast<shz::mat4x4*>(in);
    shz::mat4x4* o = reinterpret_cast<shz::mat4x4*>(out);

    i->init_translation(1.0f, 2.0f, 3.0f);
    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);

    (benchmark)(nullptr, "shz::xmtrx::apply_store_unaligned",
                [](float of_[16], const float if_[16]) {
                    shz::xmtrx::apply_store(of_, if_);
                },
                reinterpret_cast<float*>(o), reinterpret_cast<const float*>(i));

    shz::xmtrx::load(*o);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    2.0f, 0.0f, 0.0f, 2.0f,
                                    0.0f, 3.0f, 0.0f, 6.0f,
                                    0.0f, 0.0f, 4.0f, 12.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f
                                }));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_store_4x4)
   shz_glm_mat4 shzRes, glmRes;
   shz_glm_mat4 mat1 = { .shz = shz::mat4x4({
        .left    = shz::vec4( -13.0f,  2.0f,  3.0f, -0.0001f ),
        .up      = shz::vec4(  4.0f,  -5.0f,  6.0f, 12.3232f ),
        .forward = shz::vec4(  7.0f,   8.0f,  9.0f, 882.023f ),
        .pos     = shz::vec4( 11.0f, -12.0f, 13.0f, -0.0435f )
   })};
   shz_glm_mat4 mat2 = { .shz = shz_mat4x4({
        .left    = shz::vec4( 11.0f,    2.5f, -3.333f, -4.0343f ),
        .up      = shz::vec4( -46.0f,  -5.0f,  0.777f, 9999.34f ),
        .forward = shz::vec4( -75.0f, 0.008f, -99.44f, 0.23233f ),
        .pos     = shz::vec4( 11.0f,   12.0f, 13.888f, -345.88f )
   })};

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::load_apply_store_4x4()", [&] { shz::xmtrx::load_apply_store(&shzRes.shz, mat1.shz, mat2.shz); },
        "glm_mat4_mul",                       [&] { glm_mat4_mul(mat1.glm, mat2.glm, glmRes.glm); }));

#if 0
    for(unsigned i = 0; i < 4; ++i)
        for(unsigned j = 0; j < 4; ++j)
            std::println("[{}][{}]: {} vs {}", i, j, shzRes.shz.elem2D[i][j], glmRes.shz.elem2D[i][j]);
#endif

    GBL_TEST_VERIFY(static_cast<const shz::mat4x4&>(shzRes.shz) ==
                    static_cast<const shz::mat4x4&>(glmRes.shz));
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
    vec3 sc = { 2.0f, 2.0f, 2.0f };
    vec3 tl = { 1.0f, 2.0f, 3.0f };

    mat4 glmRes;
    shz::mat4x4 shzRes;

    shz::xmtrx::init_identity();
    shz::xmtrx::scale(2.0f, 2.0f, 2.0f);
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                            {
                                2.0f, 0.0f, 0.0f, 2.0f,
                                0.0f, 2.0f, 0.0f, 4.0f,
                                0.0f, 0.0f, 2.0f, 6.0f,
                                0.0f, 0.0f, 0.0f, 1.0f }));


    shz::xmtrx::store(&shzRes);

    glm_mat4_identity(glmRes);
    glm_scale(glmRes, sc);
    glm_translate(glmRes, tl);

    GBL_TEST_VERIFY(compare_glm(shzRes, glmRes));

#if SHZ_BACKEND == SHZ_SH4
    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::translate", [](float x, float y, float z) {
                shz::xmtrx::translate(x, y, z);
            },
            "mat_translate", bench_fn(mat_translate),
            100.0f, 200.0f, 300.0f
        )
    );
#endif
{
    shz_glm_mat4 xmtrx;
    shz_glm_vec3 axis = { .shz = {1.0f, 2.0f, 3.0f} };
        (benchmark_cmp<void>)(
            "shz::xmtrx::translate", [](shz_glm_vec3 axis) { shz::xmtrx::scale(axis.shz); },
            "glm_translate",        [&](shz_glm_vec3 axis) {
                                            glm_translate(xmtrx.glm, axis.glm);
                                     }, axis
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(translate_reverse)
    // Against identity.
    // Expectation: Order doesn't matter against identity, so this matches translate().
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::translate_reverse(10.0f, -20.0f, 30.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    { 1.0f, 0.0f, 0.0f, 10.0f,
                      0.0f, 1.0f, 0.0f, -20.0f,
                      0.0f, 0.0f, 1.0f, 30.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }));
    }

    // Test pre-multiply (NOT post-multiply) order versus a non-identity matrix.
    // Expectation: XMTRX = T(x,y,z) * XMTRX_old, so the translation column is
    // NOT scaled by the existing matrix--unlike translate(), which composes
    // as XMTRX_old * T and therefore DOES scale it (see the "translate" test case).
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::scale(2.0f, 2.0f, 2.0f);
        shz::xmtrx::translate_reverse(1.0f, 2.0f, 3.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        2.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 2.0f, 0.0f, 2.0f,
                        0.0f, 0.0f, 2.0f, 3.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }));

        // Cross-check versus cglm doing the pre-multiply by hand.
        shz::mat4x4 shzRes;
        shz::xmtrx::store(&shzRes);

        mat4 glmScale, glmTranslate, glmRes;
        vec3 sc = { 2.0f, 2.0f, 2.0f };
        vec3 tl = { 1.0f, 2.0f, 3.0f };

        glm_scale_make(glmScale, sc);
        glm_translate_make(glmTranslate, tl);
        glm_mat4_mul(glmTranslate, glmScale, glmRes);

        GBL_TEST_VERIFY(compare_glm(shzRes, glmRes));
    }

    {
        volatile mat4 xmtrx = GLM_MAT4_IDENTITY_INIT;

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::translate_reverse", [](float x, float y, float z) {
                    shz::xmtrx::translate_reverse(x, y, z);
                },
                "glm translate_reverse", [&](float x, float y, float z) {
                    volatile mat4 t;
                    volatile vec3 v = { x, y, z };
                    glm_translate_make(*const_cast<mat4*>(&t), *const_cast<vec3*>(&v));
                    glm_mat4_mul(*const_cast<mat4*>(&t), *const_cast<mat4*>(&xmtrx), *const_cast<mat4*>(&xmtrx));
                },
                100.0f, 200.0f, 300.0f
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(scale_reverse)
    // Against identity.
    // Expectation: Order doesn't matter against identity, so this matches scale().
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::scale_reverse(-2.0f, -3.0f, -4.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    { -2.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, -3.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, -4.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }));
    }

    // Test pre-multiply (NOT post-multiply) order versus a non-identity matrix.
    // Expectation: XMTRX = S(x,y,z) * XMTRX_old, so the EXISTING translation column
    // gets scaled--unlike scale(), which composes as XMTRX_old * S and therefore
    // leaves a prior translation untouched (see the "scale" test case).
    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();
        shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
        shz::xmtrx::scale_reverse(2.0f, 2.0f, 2.0f);

        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        2.0f, 0.0f, 0.0f, 2.0f,
                        0.0f, 2.0f, 0.0f, 4.0f,
                        0.0f, 0.0f, 2.0f, 6.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    }));

        // Cross-check versus cglm doing the pre-multiply by hand.
        shz::mat4x4 shzRes;
        shz::xmtrx::store(&shzRes);

        mat4 glmTranslate, glmScale, glmRes;
        vec3 tl = { 1.0f, 2.0f, 3.0f };
        vec3 sc = { 2.0f, 2.0f, 2.0f };

        glm_translate_make(glmTranslate, tl);
        glm_scale_make(glmScale, sc);
        glm_mat4_mul(glmScale, glmTranslate, glmRes);

        GBL_TEST_VERIFY(compare_glm(shzRes, glmRes));
    }

    {
        volatile mat4 xmtrx = GLM_MAT4_IDENTITY_INIT;

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::scale_reverse", [](float x, float y, float z) {
                    shz::xmtrx::scale_reverse(x, y, z);
                },
                "glm scale_reverse", [&](float x, float y, float z) {
                    volatile mat4 s;
                    volatile vec3 v = { x, y, z };
                    glm_scale_make(*const_cast<mat4*>(&s), *const_cast<vec3*>(&v));
                    glm_mat4_mul(*const_cast<mat4*>(&s), *const_cast<mat4*>(&xmtrx), *const_cast<mat4*>(&xmtrx));
                },
                2.0f, 3.0f, 4.0f
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(scale)
    vec3 sc = { 2.0f, 2.0f, 2.0f };
    vec3 tl = { 1.0f, 2.0f, 3.0f };

    mat4 glmScaleRes;
    shz::mat4x4 shzScaleRes;

    shz::xmtrx::init_identity();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    shz::xmtrx::scale(2.0f, 2.0f, 2.0f);

    shz::xmtrx::store(&shzScaleRes);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                            {
                                2.0f, 0.0f, 0.0f, 1.0f,
                                0.0f, 2.0f, 0.0f, 2.0f,
                                0.0f, 0.0f, 2.0f, 3.0f,
                                0.0f, 0.0f, 0.0f, 1.0f }));

    glm_mat4_identity(glmScaleRes);
    glm_translate(glmScaleRes, tl);
    glm_scale(glmScaleRes, sc);

    GBL_TEST_VERIFY(compare_glm(shzScaleRes, glmScaleRes));

    {
        mat4 xmtrx;
        shz_glm_vec3 axis = { .shz = {1.0f, 2.0f, 3.0f} };

        (benchmark_cmp<void>)(
            "shz::xmtrx::scale", [](shz_glm_vec3 axis)  { shz::xmtrx::scale(axis.shz); },
            "glm_scale",        [&](shz_glm_vec3 axis) {
                                        glm_scale(xmtrx, axis.glm);
                                }, axis
        );

#if SHZ_BACKEND == SHZ_SH4
        (benchmark_cmp<void>)(
            "shz::xmtrx::scale", [](shz::vec3 axis) { shz::xmtrx::scale(axis); },
            "mat_scale",         [](shz::vec3 axis) { mat_scale(axis.x, axis.y, axis.z);
                                                    }, axis.shz
        );
#endif
    }

GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_x)
    constexpr float rotation_factor = 0.785398f;
    vec3 tl = { 1.0f, 2.0f, 3.0f };

    shz::mat4x4 shzRotRes;
    mat4 glmRotRes;
    shz::xmtrx::init_identity();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    shz::xmtrx::rotate_x(rotation_factor);
    shz::xmtrx::store(&shzRotRes);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                            {
                                1.0f, 0.0f,       0.0f,      1.0f,
                                0.0f, 0.707107f, -0.707107f, 2.0f,
                                0.0f, 0.707107f,  0.707107f, 3.0f,
                                0.0f, 0.0f,       0.0f,      1.0f }));

    glm_mat4_identity(glmRotRes);
    glm_translate(glmRotRes, tl);
    glm_rotate_x(glmRotRes, rotation_factor, glmRotRes);

    GBL_TEST_VERIFY(compare_glm(shzRotRes, glmRotRes));

    // Double arguments must select the radian overload.
    shz::mat4x4 shzRotDouble;
    shz::xmtrx::init_identity();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    shz::xmtrx::rotate_x(static_cast<double>(rotation_factor));
    shz::xmtrx::store(&shzRotDouble);

    GBL_TEST_VERIFY(shzRotDouble == shzRotRes);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::rotate_x", [](float angle) {
                shz::xmtrx::rotate_x(angle);
            },
            "glm_rotate_x", [&](float angle) {
                glm_rotate_x(glmRotRes, angle, glmRotRes);
            },
            rotation_factor
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_y)
    constexpr float rotation_factor = 0.785398f;
    vec3 tl = { 1.0f, 2.0f, 3.0f };

    shz::mat4x4 shzRotRes;
    mat4 glmRotRes;
    shz::xmtrx::init_identity();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    shz::xmtrx::rotate_y(rotation_factor);
    shz::xmtrx::store(&shzRotRes);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                            0.707107f,  0.0f,  0.707107f,  1.0f,
                            0.0f,       1.0f,  0.0f,      2.0f,
                        -0.707107f,  0.0f,  0.707107f, 3.0f,
                            0.0f,       0.0f,  0.0f,      1.0f }));

    glm_mat4_identity(glmRotRes);
    glm_translate(glmRotRes, tl);
    glm_rotate_y(glmRotRes, rotation_factor, glmRotRes);

    GBL_TEST_VERIFY(compare_glm(shzRotRes, glmRotRes));

    // Double arguments must select the radian overload.
    shz::mat4x4 shzRotDouble;
    shz::xmtrx::init_identity();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    shz::xmtrx::rotate_y(static_cast<double>(rotation_factor));
    shz::xmtrx::store(&shzRotDouble);

    GBL_TEST_VERIFY(shzRotDouble == shzRotRes);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::rotate_y", [](float angle) {
                shz::xmtrx::rotate_y(angle);
            },
            "glm_rotate_y", [&](float angle) {
                glm_rotate_y(glmRotRes, angle, glmRotRes);
            },
            rotation_factor
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_z)
    constexpr float rotation_factor = 0.785398f;
    vec3 tl = { 1.0f, 2.0f, 3.0f };

    shz::mat4x4 shzRotRes;
    mat4 glmRotRes;
    shz::xmtrx::init_identity();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    shz::xmtrx::rotate_z(rotation_factor);
    shz::xmtrx::store(&shzRotRes);

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                    {
                        0.707107f, -0.707107f, 0.0f, 1.0f,
                        0.707107f,  0.707107f, 0.0f, 2.0f,
                        0.0f,      0.0f,     1.0f, 3.0f,
                        0.0f,      0.0f,     0.0f, 1.0f }));

    glm_mat4_identity(glmRotRes);
    glm_translate(glmRotRes, tl);
    glm_rotate_z(glmRotRes, rotation_factor, glmRotRes);

    GBL_TEST_VERIFY(compare_glm(shzRotRes, glmRotRes));

    // Double arguments must select the radian overload.
    shz::mat4x4 shzRotDouble;
    shz::xmtrx::init_identity();
    shz::xmtrx::translate(1.0f, 2.0f, 3.0f);
    shz::xmtrx::rotate_z(static_cast<double>(rotation_factor));
    shz::xmtrx::store(&shzRotDouble);

    GBL_TEST_VERIFY(shzRotDouble == shzRotRes);

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::rotate_z", [](float angle) {
                shz::xmtrx::rotate_z(angle);
            },
            "glm_rotate_z", [&](float angle) {
                glm_rotate_z(glmRotRes, angle, glmRotRes);
            },
            rotation_factor
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(ortho)
    shz::xmtrx::init_identity();
    shz::xmtrx::ortho(0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1000.0f);
    union {
        alignas(16) mat4 cmat;
        shz::mat4x4 shzcmat;
    } cunion;
    glm_ortho_rh_no(0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1000.0f, cunion.cmat);
    shz::mat4x4 shzmat;
    shz::xmtrx::store(&shzmat);
#if 0
    for(unsigned i = 0; i < 4; ++i)
        for(unsigned j = 0; j < 4; ++j)
            std::println("[{}][{}]: {} vs {}", i, j, shzmat.elem2D[i][j], cunion.shzcmat.elem2D[i][j]);
#endif
    GBL_TEST_VERIFY(shzmat == cunion.shzcmat);
GBL_TEST_CASE_END

GBL_TEST_CASE(frustum)
    shz::xmtrx::init_identity();
    shz::xmtrx::frustum(0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1000.0f);
    union {
        alignas(16) mat4 cmat;
        shz::mat4x4 shzcmat;
    } cunion;
    glm_frustum_rh_no(0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1000.0f, cunion.cmat);
    shz::mat4x4 shzmat;
    shz::xmtrx::store(&shzmat);
#if 0
    for(unsigned i = 0; i < 4; ++i)
        for(unsigned j = 0; j < 4; ++j)
            std::println("[{}][{}]: {} vs {}", i, j, shzmat.elem2D[i][j], cunion.shzcmat.elem2D[i][j]);
#endif
    GBL_TEST_VERIFY(shzmat == cunion.shzcmat);
GBL_TEST_CASE_END

GBL_TEST_CASE(perspective)
#if 1
    GBL_TEST_SKIP("Not equivalent to any CGLM reference. Figure out why.");
#else
    union {
        alignas(16) mat4 cmat;
        shz::mat4x4 shzcmat;
    } cunion;

    shz::xmtrx::init_identity();
    shz::xmtrx::perspective(shz::deg_to_rad(45.0f), 640.0f / 480.0f, 0.0f);
    glm_perspective_infinite_rh_no(shz::deg_to_rad(45.0f), 640.0f / 480.0f, 0.0f, cunion.cmat);
    shz::mat4x4 shzmat;
    shz::xmtrx::store(&shzmat);
#if 0
    for(unsigned i = 0; i < 4; ++i)
        for(unsigned j = 0; j < 4; ++j)
            std::println("[{}][{}]: {} vs {}", i, j, shzmat.elem2D[i][j], cunion.shzcmat.elem2D[i][j]);
#endif
    GBL_TEST_VERIFY(shzmat == cunion.shzcmat);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(screen)
    shz::xmtrx::init_identity();
    shz::xmtrx::screen(640.0f, 480.0f);

    shz::mat4x4 applied_screen_mat;
    shz::xmtrx::store(&applied_screen_mat);

    // Ensure screen computes
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                { 320.0f, -0.0f,   0.0f, 320.0f,
                 -0.0f,   -240.0f, 0.0f, 240.0f,
                  0.0f,   -0.0f,   1.0f, 0.0f,
                  0.0f,   -0.0f,   0.0f, 1.0f }));

    // Check bruces balls
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::permute_wxyz();
    shz::xmtrx::screen(640.0f, 480.0f);

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
    (benchmark)(nullptr, "shz::xmtrx::screen", [&] { shz::xmtrx::screen(640.0f, 480.0f); });
GBL_TEST_CASE_END

GBL_TEST_CASE(lookat)
    volatile float in[9] = { 1.0f, 2.0f, 5.0f, 0.0f, 0.5f, -1.0f, 0.0f, 1.0f, 0.0f };

    auto test = [](shz::vec3 eye, shz::vec3 center, shz::vec3 up) {
        shz::mat4x4 shzMat, cglmMat;

        shz::xmtrx::init_identity();
        shz::xmtrx::lookat(eye, center, up);
        shz::xmtrx::store(&shzMat);

        glm_lookat_rh(*reinterpret_cast<vec3*>(&eye),
                      *reinterpret_cast<vec3*>(&center),
                      *reinterpret_cast<vec3*>(&up),
                      *reinterpret_cast<mat4*>(&cglmMat));

        for(unsigned i = 0; i < 4; ++i) {
            for(unsigned j = 0; j < 4; ++j) {
                if(!shz_equalf(shzMat.elem2D[i][j], cglmMat.elem2D[i][j]))
                    std::print("[{}][{}]: {} != {} ", j, i, shzMat.elem2D[i][j], cglmMat.elem2D[i][j]);
            }
        }

        return shzMat == cglmMat;
    };

    auto test_projection = [](shz::vec3 eye, shz::vec3 center, shz::vec3 up) {
        union {
            alignas(32) mat4 glm;
            shz::mat4x4      shz;
        } proj, look, expected;
        alignas(32) shz::mat4x4 actual;

        shz::xmtrx::init_identity();
        shz::xmtrx::perspective(shz::deg_to_rad(60.0f), 640.0f / 480.0f, 0.5f);
        shz::xmtrx::store(&proj.shz);
        shz::xmtrx::lookat(eye, center, up);
        shz::xmtrx::store(&actual);

        glm_lookat_rh(*reinterpret_cast<vec3*>(&eye),
                      *reinterpret_cast<vec3*>(&center),
                      *reinterpret_cast<vec3*>(&up),
                      look.glm);
        glm_mat4_mul(proj.glm, look.glm, expected.glm);

        for(unsigned i = 0; i < 4; ++i) {
            for(unsigned j = 0; j < 4; ++j) {
                if(!shz_equalf(actual.elem2D[i][j], expected.shz.elem2D[i][j]))
                    std::print("[{}][{}]: {} != {} ", j, i, actual.elem2D[i][j], expected.shz.elem2D[i][j]);
            }
        }

        return actual == expected.shz;
    };

    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }));
    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
    GBL_TEST_VERIFY(test({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 2.0f, 0.0f }));
    GBL_TEST_VERIFY(test({ 5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));

    GBL_TEST_VERIFY(test_projection({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
    GBL_TEST_VERIFY(test_projection({ 5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
    GBL_TEST_VERIFY(test_projection({ 1.0f, 2.0f, 5.0f }, { 0.0f, 0.5f, -1.0f }, { 0.0f, 1.0f, 0.0f }));
    GBL_TEST_VERIFY(test_projection({ -3.0f, 4.0f, -2.0f }, { 1.0f, 0.0f, 2.0f }, { 0.0f, 0.0f, 1.0f }));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::lookat", [](const volatile float* v) {
            shz::xmtrx::lookat({ v[0], v[1], v[2] }, { v[3], v[4], v[5] }, { v[6], v[7], v[8] });
        },
        "glm_lookat_rh + glm_mat4_mul", [](const volatile float* v) {
            static volatile mat4 m;
            alignas(16) mat4     look;
            vec3 eye    = { v[0], v[1], v[2] };
            vec3 center = { v[3], v[4], v[5] };
            vec3 up     = { v[6], v[7], v[8] };
            glm_lookat_rh(eye, center, up, look);
            glm_mat4_mul(*const_cast<mat4*>(&m), look, *const_cast<mat4*>(&m));
        },
        &in[0]));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_3x4)
    randomize_xmtrx_();
    alignas(32) shz_mat3x4_t mat = {
        .left    = { -1.0f,   2.0f,  3.0f },
        .up      = {  4.0f,  -5.0f,  6.0f },
        .forward = {  7.0f,   8.0f,  9.0f },
        .pos     = { 11.0f, -12.0f, 13.0f }
    };
    shz::xmtrx::load(mat);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose({
                    -1.0f,   2.0f,  3.0f, 0.0f,
                     4.0f,  -5.0f,  6.0f, 0.0f,
                     7.0f,   8.0f,  9.0f, 0.0f,
                    11.0f, -12.0f, 13.0f, 1.0f })));

    (benchmark)(nullptr, "shz::xmtrx::load_3x4", [&] { shz::xmtrx::load(mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(load_transpose_3x4)
    randomize_xmtrx_();
    alignas(32) shz_mat3x4_t mat = { .elem = {
        -1.0f,   2.0f,  3.0f,   4.0f,
         5.0f,  -6.0f,  7.0f,   8.0f,
         9.0f,  10.0f, 11.0f, -12.0f
    }};

    shz::xmtrx::load_transpose(mat);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
                    -1.0f,   2.0f,  3.0f,   4.0f,
                     5.0f,  -6.0f,  7.0f,   8.0f,
                     9.0f,  10.0f, 11.0f, -12.0f,
                     0.0f,   0.0f,  0.0f,   1.0f }));

    (benchmark)(nullptr, "shz::xmtrx::load_transpose_3x4",
                [&] { shz::xmtrx::load_transpose(mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(load_rows_3x4)
    randomize_xmtrx_();

    alignas(32)
    shz::vec4 r1 = { -1.0f,   2.0f,  3.0f,  8.0f };
    shz::vec4 r2 = {  4.0f,  -5.0f,  6.0f,  7.0f };
    shz::vec4 r3 = {  7.0f,   8.0f,  9.0f, 10.0f };

    shz::xmtrx::load_rows(r1, r2, r3);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
                    -1.0f,   2.0f,  3.0f,  8.0f,
                     4.0f,  -5.0f,  6.0f,  7.0f,
                     7.0f,   8.0f,  9.0f, 10.0f,
                     0.0f,   0.0f,  0.0f,  1.0f }));

    (benchmark)(nullptr, "shz::xmtrx::load_rows_3x4",
                [&] { shz::xmtrx::load_rows(r1, r2, r3); });
GBL_TEST_CASE_END

GBL_TEST_CASE(load_cols_3x4)
    randomize_xmtrx_();
    shz::vec3 c1 = { -1.0f,   2.0f,  3.0f };
    shz::vec3 c2 = {  4.0f,  -5.0f,  6.0f };
    shz::vec3 c3 = {  7.0f,   8.0f,  9.0f };
    shz::vec3 c4 = { 10.0f, -11.0f, 12.0f };

    shz::xmtrx::load_cols(c1, c2, c3, c4);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
                    -1.0f,   4.0f,  7.0f,  10.0f,
                     2.0f,  -5.0f,  8.0f, -11.0f,
                     3.0f,   6.0f,  9.0f,  12.0f,
                     0.0f,   0.0f,  0.0f,   1.0f }));

    {
        randomize_xmtrx_();
        shz::vec3 packed[4] = { c1, c2, c3, c4 };

        shz::xmtrx::load_cols(packed[0], packed[1], packed[2], packed[3]);
        GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
                        -1.0f,   4.0f,  7.0f,  10.0f,
                         2.0f,  -5.0f,  8.0f, -11.0f,
                         3.0f,   6.0f,  9.0f,  12.0f,
                         0.0f,   0.0f,  0.0f,   1.0f }));
    }

    {
        randomize_xmtrx_();
        shz::vec4 r1 = { c1.x, c2.x, c3.x, c4.x };
        shz::vec4 r2 = { c1.y, c2.y, c3.y, c4.y };
        shz::vec4 r3 = { c1.z, c2.z, c3.z, c4.z };
        shz::mat4x4 fromRows, fromCols;

        shz::xmtrx::load_rows(r1, r2, r3);
        shz::xmtrx::store(&fromRows);
        shz::xmtrx::load_cols(c1, c2, c3, c4);
        shz::xmtrx::store(&fromCols);

        for(unsigned i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(fromCols.elem[i] == fromRows.elem[i]);
    }

    (benchmark)(nullptr, "shz::xmtrx::load_cols_3x4",
                [&] { shz::xmtrx::load_cols(c1, c2, c3, c4); });
GBL_TEST_CASE_END

GBL_TEST_CASE(store_3x4)
    randomize_xmtrx_();
    alignas(32) shz_mat3x4_t mat;
    shz::mat4x4 mat4;
    shz::xmtrx::store(&mat);
    shz::xmtrx::load(mat);
    shz::xmtrx::store(&mat4);
    std::array<float, 16> array = std::to_array(mat4.elem);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, transpose(array)));

    (benchmark)(nullptr, "shz::xmtrx::store_3x4", [&] { shz::xmtrx::store(&mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(store_transpose_3x4)
    randomize_xmtrx_();
    shz_mat3x4_t src = { .elem = {
        -1.0f,   2.0f,  3.0f,   4.0f,
         5.0f,  -6.0f,  7.0f,   8.0f,
         9.0f,  10.0f, 11.0f, -12.0f
    }};

    shz::xmtrx::load_transpose(src);

    alignas(32) shz_mat3x4_t dst;
    shz::xmtrx::store_transpose(&dst);

    for(unsigned i = 0; i < 12; ++i)
        GBL_TEST_VERIFY(shz_equalf(dst.elem[i], src.elem[i]));

    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
                    -1.0f,   2.0f,  3.0f,   4.0f,
                     5.0f,  -6.0f,  7.0f,   8.0f,
                     9.0f,  10.0f, 11.0f, -12.0f,
                     0.0f,   0.0f,  0.0f,   1.0f }));

    (benchmark)(nullptr, "shz::xmtrx::store_transpose_3x4",
                [&] { shz::xmtrx::store_transpose(&dst); });
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_3x4)
    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz_mat3x4_t mat = {
        .left    = { -1.0f,   2.0f,  3.0f },
        .up      = {  4.0f,  -5.0f,  6.0f },
        .forward = {  7.0f,   8.0f,  9.0f },
        .pos     = { 11.0f, -12.0f, 13.0f }
    };
    shz::xmtrx::apply(mat);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose({
                    -1.0f,   2.0f,  3.0f, 0.0f,
                     4.0f,  -5.0f,  6.0f, 0.0f,
                     7.0f,   8.0f,  9.0f, 0.0f,
                    11.0f, -12.0f, 13.0f, 1.0f })));

    (benchmark)(nullptr, "shz::xmtrx::apply_3x4", [&] { shz::xmtrx::apply(mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_transpose_3x4)
    randomize_xmtrx_();
    alignas(32) shz_mat3x4_t mat = { .elem = {
        -1.0f,   2.0f,  3.0f,   4.0f,
         5.0f,  -6.0f,  7.0f,   8.0f,
         9.0f,  10.0f, 11.0f, -12.0f
    }};

    shz::xmtrx::init_identity();
    shz::xmtrx::apply_transpose(mat);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
                    -1.0f,   2.0f,  3.0f,   4.0f,
                     5.0f,  -6.0f,  7.0f,   8.0f,
                     9.0f,  10.0f, 11.0f, -12.0f,
                     0.0f,   0.0f,  0.0f,   1.0f }));

    {
        alignas(32) shz_mat4x4_t wide = { .elem = {
            -1.0f,   2.0f,  3.0f,   4.0f,
             5.0f,  -6.0f,  7.0f,   8.0f,
             9.0f,  10.0f, 11.0f, -12.0f,
             0.0f,   0.0f,  0.0f,   1.0f
        }};

        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_transpose(mat);
        shz::mat4x4 narrow;
        shz::xmtrx::store(&narrow);

        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_transpose(wide);
        shz::mat4x4 reference;
        shz::xmtrx::store(&reference);

        for(unsigned i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz_equalf(narrow.elem[i], reference.elem[i]));
    }

    (benchmark)(nullptr, "shz::xmtrx::apply_transpose_3x4",
                [&] { shz::xmtrx::apply_transpose(mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_3x4)
    shz_mat3x4_t mat = {
        .left    = { -1.0f,   2.0f,  3.0f },
        .up      = {  4.0f,  -5.0f,  6.0f },
        .forward = {  7.0f,   8.0f,  9.0f },
        .pos     = { 11.0f, -12.0f, 13.0f }
    };

    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::apply_reverse(mat);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                  transpose({
                    -1.0f,   2.0f,  3.0f, 0.0f,
                     4.0f,  -5.0f,  6.0f, 0.0f,
                     7.0f,   8.0f,  9.0f, 0.0f,
                    11.0f, -12.0f, 13.0f, 1.0f })));

    {
        alignas(32) shz_mat4x4_t wide = { .elem = {
            -1.0f,   2.0f,  3.0f, 0.0f,
             4.0f,  -5.0f,  6.0f, 0.0f,
             7.0f,   8.0f,  9.0f, 0.0f,
            11.0f, -12.0f, 13.0f, 1.0f
        }};
        shz::mat4x4 narrow, reference, forward;

        randomize_xmtrx_();
        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_reverse(mat);
        shz::xmtrx::store(&narrow);

        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_reverse(wide);
        shz::xmtrx::store(&reference);

        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply(mat);
        shz::xmtrx::store(&forward);

        for(unsigned i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz_equalf(narrow.elem[i], reference.elem[i]));

        GBL_TEST_VERIFY(!shz_equalf(narrow.elem[1], forward.elem[1]));
    }

    (benchmark)(nullptr, "shz::xmtrx::apply_reverse_3x4", [&] { shz::xmtrx::apply_reverse(mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_reverse_transpose_3x4)
    alignas(32) shz_mat3x4_t mat = { .elem = {
        -1.0f,   2.0f,  3.0f,   4.0f,
         5.0f,  -6.0f,  7.0f,   8.0f,
         9.0f,  10.0f, 11.0f, -12.0f
    }};

    randomize_xmtrx_();
    shz::xmtrx::init_identity();
    shz::xmtrx::apply_reverse_transpose(mat);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME, {
                    -1.0f,   2.0f,  3.0f,   4.0f,
                     5.0f,  -6.0f,  7.0f,   8.0f,
                     9.0f,  10.0f, 11.0f, -12.0f,
                     0.0f,   0.0f,  0.0f,   1.0f }));

    {
        alignas(32) shz_mat4x4_t wide = { .elem = {
            -1.0f,   2.0f,  3.0f,   4.0f,
             5.0f,  -6.0f,  7.0f,   8.0f,
             9.0f,  10.0f, 11.0f, -12.0f,
             0.0f,   0.0f,  0.0f,   1.0f
        }};
        shz::mat4x4 narrow, reference, forward;

        randomize_xmtrx_();
        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_reverse_transpose(mat);
        shz::xmtrx::store(&narrow);

        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_reverse_transpose(wide);
        shz::xmtrx::store(&reference);

        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::xmtrx::apply_transpose(mat);
        shz::xmtrx::store(&forward);

        for(unsigned i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz_equalf(narrow.elem[i], reference.elem[i]));

        GBL_TEST_VERIFY(!shz_equalf(narrow.elem[1], forward.elem[1]));
    }

    (benchmark)(nullptr, "shz::xmtrx::apply_reverse_transpose_3x4", [&] { shz::xmtrx::apply_reverse_transpose(mat); });
GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_3x4)
    shz_mat3x4_t mat1 = {
        .left    = { 1.0f, 0.0f, 0.0f },
        .up      = { 0.0f, 1.0f, 0.0f },
        .forward = { 0.0f, 0.0f, 1.0f },
        .pos     = { 0.0f, 0.0f, 0.0f }
    };
    shz_mat3x4_t mat2 = {
        .left    = { 1.0f, 0.0f, 0.0f },
        .up      = { 0.0f, 1.0f, 0.0f },
        .forward = { 0.0f, 0.0f, 1.0f },
        .pos     = { 1.0f, 2.0f, 3.0f }
    };

    randomize_xmtrx_();
    shz::xmtrx::load_apply(mat1, mat2);
    GBL_TEST_CALL(verify_matrix(GBL_SELF_TYPE_NAME,
                                {
                                    1.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 2.0f,
                                    0.0f, 0.0f, 1.0f, 3.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f }));

    {
        alignas(32) shz_mat3x4_t a = {
            .left    = { -13.0f,   2.0f,  3.0f },
            .up      = {  4.0f,  -5.0f,  6.0f },
            .forward = {  7.0f,   8.0f,  9.0f },
            .pos     = { 11.0f, -12.0f, 13.0f }
        };
        alignas(32) shz_mat3x4_t b = {
            .left    = { 11.0f,    2.5f, -3.333f },
            .up      = { -46.0f,  -5.0f,  0.777f },
            .forward = { -75.0f, 0.008f, -99.44f },
            .pos     = { 11.0f,   12.0f, 13.888f }
        };
        shz::mat4x4 fused, separate;

        randomize_xmtrx_();
        shz::xmtrx::load_apply(a, b);
        shz::xmtrx::store(&fused);

        randomize_xmtrx_();
        shz::xmtrx::load(a);
        shz::xmtrx::apply(b);
        shz::xmtrx::store(&separate);

        for(unsigned i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz_equalf(fused.elem[i], separate.elem[i]));

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::load_apply_3x4", [&] {
                shz::xmtrx::load_apply(a, b);
            },
            "shz::xmtrx::load + apply 3x4", [&] {
                shz::xmtrx::load(a);
                shz::xmtrx::apply(b);
            }));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_store_3x4)
   shz_mat3x4_t shzRes, q2Res;
   shz_mat3x4_t mat1 = {
        .left    = { -13.0f,   2.0f,  3.0f },
        .up      = {  4.0f,  -5.0f,  6.0f },
        .forward = {  7.0f,   8.0f,  9.0f },
        .pos     = { 11.0f, -12.0f, 13.0f }
   };
   shz_mat3x4_t mat2 = {
        .left    = { 11.0f,    2.5f, -3.333f },
        .up      = { -46.0f,  -5.0f,  0.777f },
        .forward = { -75.0f, 0.008f, -99.44f },
        .pos     = { 11.0f,   12.0f, 13.888f }
   };

   GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::load_apply_store_3x4", [&] { shz::xmtrx::load_apply_store(&shzRes, mat1, mat2); },
        "R_ConcatTransforms", [&] {
        q2Res.elem2D[0][0] = mat1.elem2D[0][0] * mat2.elem2D[0][0] + mat1.elem2D[1][0] * mat2.elem2D[0][1] + mat1.elem2D[2][0] * mat2.elem2D[0][2];
        q2Res.elem2D[1][0] = mat1.elem2D[0][0] * mat2.elem2D[1][0] + mat1.elem2D[1][0] * mat2.elem2D[1][1] + mat1.elem2D[2][0] * mat2.elem2D[1][2];
        q2Res.elem2D[2][0] = mat1.elem2D[0][0] * mat2.elem2D[2][0] + mat1.elem2D[1][0] * mat2.elem2D[2][1] + mat1.elem2D[2][0] * mat2.elem2D[2][2];
        q2Res.elem2D[3][0] = mat1.elem2D[0][0] * mat2.elem2D[3][0] + mat1.elem2D[1][0] * mat2.elem2D[3][1] + mat1.elem2D[2][0] * mat2.elem2D[3][2] + mat1.elem2D[3][0];
        q2Res.elem2D[0][1] = mat1.elem2D[0][1] * mat2.elem2D[0][0] + mat1.elem2D[1][1] * mat2.elem2D[0][1] + mat1.elem2D[2][1] * mat2.elem2D[0][2];
        q2Res.elem2D[1][1] = mat1.elem2D[0][1] * mat2.elem2D[1][0] + mat1.elem2D[1][1] * mat2.elem2D[1][1] + mat1.elem2D[2][1] * mat2.elem2D[1][2];
        q2Res.elem2D[2][1] = mat1.elem2D[0][1] * mat2.elem2D[2][0] + mat1.elem2D[1][1] * mat2.elem2D[2][1] + mat1.elem2D[2][1] * mat2.elem2D[2][2];
        q2Res.elem2D[3][1] = mat1.elem2D[0][1] * mat2.elem2D[3][0] + mat1.elem2D[1][1] * mat2.elem2D[3][1] + mat1.elem2D[2][1] * mat2.elem2D[3][2] + mat1.elem2D[3][1];
        q2Res.elem2D[0][2] = mat1.elem2D[0][2] * mat2.elem2D[0][0] + mat1.elem2D[1][2] * mat2.elem2D[0][1] + mat1.elem2D[2][2] * mat2.elem2D[0][2];
        q2Res.elem2D[1][2] = mat1.elem2D[0][2] * mat2.elem2D[1][0] + mat1.elem2D[1][2] * mat2.elem2D[1][1] + mat1.elem2D[2][2] * mat2.elem2D[1][2];
        q2Res.elem2D[2][2] = mat1.elem2D[0][2] * mat2.elem2D[2][0] + mat1.elem2D[1][2] * mat2.elem2D[2][1] + mat1.elem2D[2][2] * mat2.elem2D[2][2];
        q2Res.elem2D[3][2] = mat1.elem2D[0][2] * mat2.elem2D[3][0] + mat1.elem2D[1][2] * mat2.elem2D[3][1] + mat1.elem2D[2][2] * mat2.elem2D[3][2] + mat1.elem2D[3][2];
    }));

#if 0
   for(unsigned i = 0; i < 4; ++i)
       for(unsigned j = 0; j < 3; ++j)
            std::println("[{}][{}]: {} vs {}", i, j, shzRes.elem2D[i][j], q2Res.elem2D[i][j]);
#endif

    for(unsigned i = 0; i < 4; ++i)
       for(unsigned j = 0; j < 3; ++j)
            GBL_TEST_VERIFY(shz_equalf(shzRes.elem2D[i][j], q2Res.elem2D[i][j]));
GBL_TEST_CASE_END

GBL_TEST_CASE(load_store_2x2)
    shz_mat2x2_t m2 = { .elem = { 1.0f, 2.0f, -3.0f, 4.0f } };
    alignas(32) shz_mat4x4_t wide = { .elem = { 1.0f,  2.0f, 0.0f, 0.0f,
                                               -3.0f,  4.0f, 0.0f, 0.0f,
                                                0.0f,  0.0f, 1.0f, 0.0f,
                                                0.0f,  0.0f, 0.0f, 1.0f } };
    shz::mat4x4 got, ref;

    randomize_xmtrx_();
    shz::xmtrx::load(m2);
    shz::xmtrx::store(&got);

    randomize_xmtrx_();
    shz::xmtrx::load(wide);
    shz::xmtrx::store(&ref);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(got.elem[i] == ref.elem[i]);

    shz_mat2x2_t out;
    randomize_xmtrx_();
    shz::xmtrx::load(wide);
    shz::xmtrx::store(&out);
    for(unsigned i = 0; i < 4; ++i)
        GBL_TEST_VERIFY(out.elem[i] == m2.elem[i]);

    (benchmark)(nullptr, "shz::xmtrx::load_2x2",  [&] { shz::xmtrx::load(m2); });
    (benchmark)(nullptr, "shz::xmtrx::store_2x2", [&] { shz::xmtrx::store(&out); });
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_self)
    alignas(32) shz_mat4x4_t in = { .elem = { 2.0f, 3.0f, 1.0f, 4.0f,
                                              5.0f, 1.0f, 6.0f, 2.0f,
                                              3.0f, 7.0f, 2.0f, 5.0f,
                                              1.0f, 4.0f, 8.0f, 3.0f } };
    shz::mat4x4 got, ref;

    randomize_xmtrx_();
    shz::xmtrx::load(in);
    shz::xmtrx::apply_self();
    shz::xmtrx::store(&got);

    randomize_xmtrx_();
    shz::xmtrx::load(in);
    shz::xmtrx::apply(in);
    shz::xmtrx::store(&ref);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(shz_equalf(got.elem[i], ref.elem[i]));

    (benchmark)(nullptr, "shz::xmtrx::apply_self", [&] { shz::xmtrx::apply_self(); });
GBL_TEST_CASE_END

GBL_TEST_CASE(trace)
    alignas(32) shz_mat4x4_t in = { .elem = { 2.0f, 3.0f,  1.0f, 4.0f,
                                              5.0f, -1.5f, 6.0f, 2.0f,
                                              3.0f, 7.0f,  0.25f, 5.0f,
                                              1.0f, 4.0f,  8.0f, -3.0f } };
    mat4 ref;
    mat3 ref3;
    mat2 ref2 = { { in.elem[0], in.elem[1] }, { in.elem[4], in.elem[5] } };

    memcpy(ref, in.elem, sizeof(ref));
    glm_mat4_pick3(ref, ref3);

    randomize_xmtrx_();
    shz::xmtrx::load(in);

    GBL_TEST_VERIFY(shz::xmtrx::trace<2>() == 2.0f + -1.5f);
    GBL_TEST_VERIFY(shz::xmtrx::trace<3>() == 2.0f + -1.5f + 0.25f);
    GBL_TEST_VERIFY(shz::xmtrx::trace<4>() == 2.0f + -1.5f + 0.25f + -3.0f);
    GBL_TEST_VERIFY((shz::xmtrx::trace<4, 4>() == glm_mat4_trace(ref)));
    GBL_TEST_VERIFY(shz::xmtrx::trace<3>() == glm_mat3_trace(ref3));
    GBL_TEST_VERIFY(shz::xmtrx::trace<2>() == glm_mat2_trace(ref2));

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(shz::xmtrx::read(static_cast<shz::xmtrx::reg>(i)) == in.elem[i]);

    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::xmtrx::trace<4>", [] { return shz::xmtrx::trace<4>(); },
                               "glm_mat4_trace",       [&] { return glm_mat4_trace(ref); })
    );
    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::xmtrx::trace<3>", [] { return shz::xmtrx::trace<3>(); },
                               "glm_mat3_trace",       [&] { return glm_mat3_trace(ref3); })
    );
    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::xmtrx::trace<2>", [] { return shz::xmtrx::trace<2>(); },
                               "glm_mat2_trace",       [&] { return glm_mat2_trace(ref2); })
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(invert_rigid)
    struct rigid { float rx, ry, rz, tx, ty, tz, mirror; };
    constexpr rigid cases[] = {
        {  0.0f,  0.0f,  0.0f,   0.0f,  0.0f,  0.0f,  1.0f },
        {  0.4f, -0.8f,  1.2f,   3.0f, -2.0f,  5.0f,  1.0f },
        { -2.1f,  1.3f,  0.7f, -10.0f,  0.5f, -7.5f,  1.0f },
        {  0.9f,  0.2f, -1.6f,   4.0f,  6.0f, -1.0f, -1.0f }
    };

    alignas(32) shz::mat4x4 in, got;
    mat4 ref;

    for(const auto& c : cases) {
        randomize_xmtrx_();
        shz::xmtrx::init_rotation_xyz(c.rx, c.ry, c.rz);
        shz::xmtrx::scale(c.mirror, 1.0f, 1.0f);
        shz::xmtrx::translate(c.tx, c.ty, c.tz);
        shz::xmtrx::store(&in);

        shz::xmtrx::invert_rigid();
        shz::xmtrx::store(&got);

        memcpy(ref, in.elem, sizeof(ref));
        glm_inv_tr(ref);

        for(unsigned i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz_equalf(got.elem[i], ref[i / 4][i % 4]));

        GBL_TEST_VERIFY(got.elem[3] == 0.0f && got.elem[7] == 0.0f && got.elem[11] == 0.0f && got.elem[15] == 1.0f);
    }

    shz::xmtrx::load(in);
    memcpy(ref, in.elem, sizeof(ref));

    (benchmark_cmp<void>)(
        "shz::xmtrx::invert_rigid", bench_fn(shz::xmtrx::invert_rigid),
        "glm_inv_tr",               [&] { glm_inv_tr(ref); });
GBL_TEST_CASE_END

static void init_add_sub_operands_(shz_mat4x4_t& lhs, shz_mat4x4_t& rhs) {
    for(unsigned i = 0; i < 16; ++i) {
        lhs.elem[i] = 32.0f - i;
        rhs.elem[i] = 2.0f * i + 1.0f;
    }
}

GBL_TEST_CASE(add_4x4)
    alignas(32) shz_mat4x4_t lhs, rhs, sum;
    init_add_sub_operands_(lhs, rhs);
    shz::xmtrx::load(lhs);
    shz::xmtrx::add(rhs);
    shz::xmtrx::store(&sum);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(sum.elem[i] == lhs.elem[i] + rhs.elem[i]);

    shz::xmtrx::load(lhs);
    (benchmark_cmp<void>)(
        "shz::xmtrx::add",  [&] { shz::xmtrx::add(rhs); },
        "4 x glm_vec4_add", [&] {
            glm_vec4_add(lhs.elem2D[0], rhs.elem2D[0], lhs.elem2D[0]);
            glm_vec4_add(lhs.elem2D[1], rhs.elem2D[1], lhs.elem2D[1]);
            glm_vec4_add(lhs.elem2D[2], rhs.elem2D[2], lhs.elem2D[2]);
            glm_vec4_add(lhs.elem2D[3], rhs.elem2D[3], lhs.elem2D[3]);
        });
GBL_TEST_CASE_END

GBL_TEST_CASE(sub_4x4)
    alignas(32) shz_mat4x4_t lhs, rhs, difference;
    init_add_sub_operands_(lhs, rhs);
    shz::xmtrx::load(lhs);
    shz::xmtrx::sub(rhs);
    shz::xmtrx::store(&difference);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(difference.elem[i] == lhs.elem[i] - rhs.elem[i]);

    shz::xmtrx::load(lhs);
    (benchmark_cmp<void>)(
        "shz::xmtrx::sub",  [&] { shz::xmtrx::sub(rhs); },
        "4 x glm_vec4_sub", [&] {
            glm_vec4_sub(lhs.elem2D[0], rhs.elem2D[0], lhs.elem2D[0]);
            glm_vec4_sub(lhs.elem2D[1], rhs.elem2D[1], lhs.elem2D[1]);
            glm_vec4_sub(lhs.elem2D[2], rhs.elem2D[2], lhs.elem2D[2]);
            glm_vec4_sub(lhs.elem2D[3], rhs.elem2D[3], lhs.elem2D[3]);
        });
GBL_TEST_CASE_END

GBL_TEST_CASE(negate)
    alignas(32) shz_mat4x4_t in = { .elem = { 2.0f,     -3.0f,  0.0f,     -0.0f,
                                              5.5f,     -1.5f,  1.0e30f,  -1.0e-30f,
                                              INFINITY, -INFINITY, 0.25f, 7.0f,
                                              -1.0f,    4.0f,   -8.0f,    123.456f } };
    alignas(32) shz_mat4x4_t got;
    mat4 ref;

    randomize_xmtrx_();
    shz::xmtrx::load(in);
    shz::xmtrx::negate();
    shz::xmtrx::store(&got);

    for(unsigned i = 0; i < 16; ++i) {
        const uint32_t gotBits = std::bit_cast<uint32_t>(got.elem[i]);
        const uint32_t inBits  = std::bit_cast<uint32_t>(in.elem[i]);
        GBL_TEST_VERIFY(gotBits == (inBits ^ 0x80000000u));
    }

    shz::xmtrx::negate();
    shz::xmtrx::store(&got);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(!memcmp(&got.elem[i], &in.elem[i], sizeof(float)));

    shz::xmtrx::load(in);
    memcpy(ref, in.elem, sizeof(ref));

    (benchmark_cmp<void>)(
        "shz::xmtrx::negate", bench_fn(shz::xmtrx::negate),
        "glm_mat4_scale",     [&] { glm_mat4_scale(ref, -1.0f); });
GBL_TEST_CASE_END

GBL_TEST_CASE(load_3x3)
    shz_mat3x3_t m3 = { .elem = { 1.0f, 2.0f, 3.0f, 4.0f, -5.0f, 6.0f, 7.0f, 8.0f, -9.0f } };
    alignas(32) shz_mat4x4_t wide = { .elem = { 1.0f, 2.0f,  3.0f, 0.0f,
                                                4.0f, -5.0f, 6.0f, 0.0f,
                                                7.0f, 8.0f, -9.0f, 0.0f,
                                                0.0f, 0.0f,  0.0f, 1.0f } };
    shz::mat4x4 got, ref;

    randomize_xmtrx_();
    shz::xmtrx::load(m3);
    shz::xmtrx::store(&got);

    randomize_xmtrx_();
    shz::xmtrx::load(wide);
    shz::xmtrx::store(&ref);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(got.elem[i] == ref.elem[i]);

    (benchmark)(nullptr, "shz::xmtrx::load_3x3", [&] { shz::xmtrx::load(m3); });
GBL_TEST_CASE_END

GBL_TEST_CASE(load_transpose_3x3)
    shz_mat3x3_t m3 = { .elem = { 1.0f, 2.0f, 3.0f, 4.0f, -5.0f, 6.0f, 7.0f, 8.0f, -9.0f } };
    shz_mat3x3_t tr = { .elem = { 1.0f, 4.0f, 7.0f, 2.0f, -5.0f, 8.0f, 3.0f, 6.0f, -9.0f } };
    shz::mat4x4 got, ref;

    randomize_xmtrx_();
    shz::xmtrx::load_transpose(m3);
    shz::xmtrx::store(&got);

    randomize_xmtrx_();
    shz::xmtrx::load(tr);
    shz::xmtrx::store(&ref);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(got.elem[i] == ref.elem[i]);

    (benchmark)(nullptr, "shz::xmtrx::load_transpose_3x3", [&] { shz::xmtrx::load_transpose(m3); });
GBL_TEST_CASE_END

GBL_TEST_CASE(store_3x3)
    alignas(32) shz_mat4x4_t wide = { .elem = { 1.0f, 2.0f,  3.0f, 0.0f,
                                                4.0f, -5.0f, 6.0f, 0.0f,
                                                7.0f, 8.0f, -9.0f, 0.0f,
                                                0.0f, 0.0f,  0.0f, 1.0f } };
    alignas(32) shz_mat3x3_t out;

    randomize_xmtrx_();
    shz::xmtrx::load(wide);
    shz::xmtrx::store(&out);

    const float expect[9] = { 1.0f, 2.0f, 3.0f, 4.0f, -5.0f, 6.0f, 7.0f, 8.0f, -9.0f };
    for(unsigned i = 0; i < 9; ++i)
        GBL_TEST_VERIFY(out.elem[i] == expect[i]);

    (benchmark)(nullptr, "shz::xmtrx::store_3x3", [&] { shz::xmtrx::store(&out); });
GBL_TEST_CASE_END

GBL_TEST_CASE(store_transpose_3x3)
    alignas(32) shz_mat4x4_t wide = { .elem = { 1.0f, 2.0f,  3.0f, 0.0f,
                                                4.0f, -5.0f, 6.0f, 0.0f,
                                                7.0f, 8.0f, -9.0f, 0.0f,
                                                0.0f, 0.0f,  0.0f, 1.0f } };
    alignas(32) shz_mat3x3_t out;

    randomize_xmtrx_();
    shz::xmtrx::load(wide);
    shz::xmtrx::store_transpose(&out);

    const float expect[9] = { 1.0f, 4.0f, 7.0f, 2.0f, -5.0f, 8.0f, 3.0f, 6.0f, -9.0f };
    for(unsigned i = 0; i < 9; ++i)
        GBL_TEST_VERIFY(out.elem[i] == expect[i]);

    (benchmark)(nullptr, "shz::xmtrx::store_transpose_3x3", [&] { shz::xmtrx::store_transpose(&out); });
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_3x3)
    shz_mat3x3_t m3 = { .elem = { 1.0f, 2.0f, 3.0f, 4.0f, -5.0f, 6.0f, 7.0f, 8.0f, -9.0f } };
    alignas(32) shz_mat4x4_t wide = { .elem = { 1.0f, 2.0f,  3.0f, 0.0f,
                                                4.0f, -5.0f, 6.0f, 0.0f,
                                                7.0f, 8.0f, -9.0f, 0.0f,
                                                0.0f, 0.0f,  0.0f, 1.0f } };
    shz::mat4x4 got, ref;

    randomize_xmtrx_();
    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
    shz::xmtrx::apply(m3);
    shz::xmtrx::store(&got);

    randomize_xmtrx_();
    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
    shz::xmtrx::apply(wide);
    shz::xmtrx::store(&ref);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(shz_equalf(got.elem[i], ref.elem[i]));

    (benchmark)(nullptr, "shz::xmtrx::apply_3x3", [&] { shz::xmtrx::apply(m3); });
GBL_TEST_CASE_END

GBL_TEST_CASE(apply_transpose_3x3)
    shz_mat3x3_t m3 = { .elem = { 1.0f, 2.0f, 3.0f, 4.0f, -5.0f, 6.0f, 7.0f, 8.0f, -9.0f } };
    shz_mat3x3_t tr = { .elem = { 1.0f, 4.0f, 7.0f, 2.0f, -5.0f, 8.0f, 3.0f, 6.0f, -9.0f } };
    shz::mat4x4 got, ref;

    randomize_xmtrx_();
    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
    shz::xmtrx::apply_transpose(m3);
    shz::xmtrx::store(&got);

    randomize_xmtrx_();
    shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
    shz::xmtrx::apply(tr);
    shz::xmtrx::store(&ref);

    for(unsigned i = 0; i < 16; ++i)
        GBL_TEST_VERIFY(shz_equalf(got.elem[i], ref.elem[i]));

    (benchmark)(nullptr, "shz::xmtrx::apply_transpose_3x3", [&] { shz::xmtrx::apply_transpose(m3); });
GBL_TEST_CASE_END

GBL_TEST_CASE(load_apply_store_3x3)
   union shz_glm_mat3x3_t {
       shz_mat3x3_t shz;
       mat3         glm;
   };

   shz_glm_mat3x3_t shzRes, glmRes;
   shz_glm_mat3x3_t mat1 = { .shz = {
        .left    = { -13.0f,   2.0f,  3.0f },
        .up      = {  4.0f,  -5.0f,  6.0f },
        .forward = {  7.0f,   8.0f,  9.0f }
   }};
   shz_glm_mat3x3_t mat2 = { .shz = {
        .left    = { 11.0f,    2.5f, -3.333f },
        .up      = { -46.0f,  -5.0f,  0.777f },
        .forward = { -75.0f, 0.008f, -99.44f }
   }};

    GBL_TEST_VERIFY(
        (benchmark_cmp<void>)(
            "shz::xmtrx::load_apply_store[3x3]", [&] {
                shz::xmtrx::load_apply_store(&shzRes.shz, mat1.shz, mat2.shz);
            },
            "glm_mat3_mul", [&] {
                glm_mat3_mul(mat1.glm, mat2.glm, glmRes.glm);
            }
        )
    );

#if 0
   for(unsigned i = 0; i < 3; ++i)
       for(unsigned j = 0; j < 3; ++j)
            std::println("[{}][{}]: {} vs {}", i, j, shzRes.shz.elem2D[i][j], glmRes.shz.elem2D[i][j]);
#endif

    for(unsigned i = 0; i < 3; ++i)
       for(unsigned j = 0; j < 3; ++j)
            GBL_TEST_VERIFY(shz_equalf(shzRes.shz.elem2D[i][j], glmRes.shz.elem2D[i][j]));
GBL_TEST_CASE_END

GBL_TEST_CASE(blend_4x4)
    // Blend a single joint matrix onto a known starting matrix.
    // Expectation: XMTRX[i] = XMTRX_old[i] + joint[i] * weight, for all 16
    // elements--a straight per-element weighted accumulate, matching the
    // skeletal-skinning blend semantics (see shz_xmtrx_blend_4x4_sw()).
    {
        randomize_xmtrx_();
        shz::xmtrx::init_fill(2.0f);

        alignas(32) shz_mat4x4_t joint = { .elem = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        }};

        shz::xmtrx::blend(joint, 0.5f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int i = 0; i < 16; ++i)
            GBL_TEST_VERIFY(shz_equalf(result.elem[i], 2.0f + joint.elem[i] * 0.5f));
    }

    // Accumulate multiple weighted joints from zero--the standard skeletal
    // skinning pattern: start the working matrix at zero and blend() in each
    // influencing bone's weighted contribution.
    {
        randomize_xmtrx_();
        shz::xmtrx::init_zero();

        alignas(32) shz_mat4x4_t joint1 = { .elem = {
              1.0f,   0.0f,   0.0f,  0.0f,
              0.0f,   1.0f,   0.0f,  0.0f,
              0.0f,   0.0f,   1.0f,  0.0f,
             10.0f,  20.0f,  30.0f,  1.0f
        }};
        alignas(32) shz_mat4x4_t joint2 = { .elem = {
              2.0f,   0.0f,   0.0f,  0.0f,
              0.0f,   2.0f,   0.0f,  0.0f,
              0.0f,   0.0f,   2.0f,  0.0f,
             -5.0f,  -6.0f,  -7.0f,  1.0f
        }};

        shz::xmtrx::blend(joint1, 0.25f);
        shz::xmtrx::blend(joint2, 0.75f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int i = 0; i < 16; ++i) {
            float expected = joint1.elem[i] * 0.25f + joint2.elem[i] * 0.75f;
            GBL_TEST_VERIFY(shz_equalf(result.elem[i], expected));
        }
    }

    {
        alignas(32) shz_mat4x4_t joint = { .elem = {
             1.0f,  2.0f,  3.0f,  4.0f,
             5.0f,  6.0f,  7.0f,  8.0f,
             9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        }};

        alignas(32) std::array<float, 16> accum{};

        shz::xmtrx::init_zero();

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::blend", [](const shz_mat4x4_t& j, float w) {
                    shz::xmtrx::blend(j, w);
                },
                "naive per-element accum", [&](const shz_mat4x4_t& j, float w) {
                    for(int i = 0; i < 16; ++i)
                        accum[i] += j.elem[i] * w;
                },
                joint, 0.5f
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(blend_3x4)
    {
        randomize_xmtrx_();

        alignas(32) shz_mat4x4_t start;
        for(int i = 0; i < 16; ++i)
            start.elem[i] = 100.0f + static_cast<float>(i);

        alignas(32) shz_mat3x4_t joint = {
            .left    = {  1.0f,  -2.0f,   3.0f },
            .up      = { -4.0f,   5.0f,  -6.0f },
            .forward = {  7.0f,  -8.0f,   9.0f },
            .pos     = { 10.0f, -11.0f,  12.0f }
        };

        shz::xmtrx::load(start);
        shz::xmtrx::blend(joint, 0.5f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int c = 0; c < 4; ++c)
            for(int r = 0; r < 4; ++r) {
                float expected = start.elem2D[c][r];
                if(r < 3)
                    expected += joint.elem2D[c][r] * 0.5f;
                GBL_TEST_VERIFY(result.elem2D[c][r] == expected);
            }
    }

    {
        randomize_xmtrx_();
        shz::xmtrx::init_zero();

        alignas(32) shz_mat3x4_t joint1 = {
            .left    = {  1.0f,   0.0f,   0.0f },
            .up      = {  0.0f,   1.0f,   0.0f },
            .forward = {  0.0f,   0.0f,   1.0f },
            .pos     = { 10.0f,  20.0f,  30.0f }
        };
        alignas(32) shz_mat3x4_t joint2 = {
            .left    = {  2.0f,   0.0f,   0.0f },
            .up      = {  0.0f,   2.0f,   0.0f },
            .forward = {  0.0f,   0.0f,   2.0f },
            .pos     = { -5.0f,  -6.0f,  -7.0f }
        };

        shz::xmtrx::blend(joint1, 0.25f);
        shz::xmtrx::blend(joint2, 0.75f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int c = 0; c < 4; ++c) {
            for(int r = 0; r < 3; ++r) {
                float expected = joint1.elem2D[c][r] * 0.25f + joint2.elem2D[c][r] * 0.75f;
                GBL_TEST_VERIFY(shz_equalf(result.elem2D[c][r], expected));
            }
            GBL_TEST_VERIFY(result.elem2D[c][3] == 0.0f);
        }
    }

    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();

        alignas(32) shz_mat3x4_t joint = {
            .left    = {  1.0f,   2.0f,   3.0f },
            .up      = {  4.0f,   5.0f,   6.0f },
            .forward = {  7.0f,   8.0f,   9.0f },
            .pos     = { 10.0f,  11.0f,  12.0f }
        };

        shz::xmtrx::blend(joint, 0.0f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int c = 0; c < 4; ++c)
            for(int r = 0; r < 4; ++r)
                GBL_TEST_VERIFY(result.elem2D[c][r] == (c == r ? 1.0f : 0.0f));
    }

    {
        alignas(32) shz_mat3x4_t joint = {
            .left    = {  1.0f,   2.0f,   3.0f },
            .up      = {  4.0f,   5.0f,   6.0f },
            .forward = {  7.0f,   8.0f,   9.0f },
            .pos     = { 10.0f,  11.0f,  12.0f }
        };

        alignas(32) std::array<float, 16> accum{};

        shz::xmtrx::init_zero();

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::blend_3x4", [](const shz_mat3x4_t& j, float w) {
                    shz::xmtrx::blend(j, w);
                },
                "naive per-element accum", [&](const shz_mat3x4_t& j, float w) {
                    for(int c = 0; c < 4; ++c)
                        for(int r = 0; r < 3; ++r)
                            accum[c * 4 + r] += j.elem2D[c][r] * w;
                },
                joint, 0.5f
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(blend_4x3)
    {
        randomize_xmtrx_();

        alignas(32) shz_mat4x4_t start;
        for(int i = 0; i < 16; ++i)
            start.elem[i] = 100.0f + static_cast<float>(i);

        const shz_mat4x3_t src = {
            .left    = {  1.0f,  -2.0f,   3.0f,  -4.0f },
            .up      = {  5.0f,  -6.0f,   7.0f,  -8.0f },
            .forward = {  9.0f, -10.0f,  11.0f, -12.0f }
        };

#if SHZ_BACKEND == SHZ_SH4
        alignas(8) float storage[13];
        memcpy(&storage[1], &src, sizeof(src));
        const shz_mat4x3_t& joint = *reinterpret_cast<const shz_mat4x3_t*>(&storage[1]);

        GBL_TEST_VERIFY(reinterpret_cast<uintptr_t>(&joint) % 8 == 4);
#else
        const shz_mat4x3_t& joint = src;
#endif

        shz::xmtrx::load(start);
        shz::xmtrx::blend(joint, 0.5f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int c = 0; c < 4; ++c)
            for(int r = 0; r < 4; ++r) {
                float expected = start.elem2D[c][r];
                if(c < 3)
                    expected += joint.elem2D[c][r] * 0.5f;
                GBL_TEST_VERIFY(result.elem2D[c][r] == expected);
            }
    }

    {
        randomize_xmtrx_();
        shz::xmtrx::init_zero();

        alignas(32) shz_mat4x3_t joint1 = {
            .left    = {  1.0f,   0.0f,   0.0f,  10.0f },
            .up      = {  0.0f,   1.0f,   0.0f,  20.0f },
            .forward = {  0.0f,   0.0f,   1.0f,  30.0f }
        };
        alignas(32) shz_mat4x3_t joint2 = {
            .left    = {  2.0f,   0.0f,   0.0f,  -5.0f },
            .up      = {  0.0f,   2.0f,   0.0f,  -6.0f },
            .forward = {  0.0f,   0.0f,   2.0f,  -7.0f }
        };

        shz::xmtrx::blend(joint1, 0.25f);
        shz::xmtrx::blend(joint2, 0.75f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int c = 0; c < 3; ++c)
            for(int r = 0; r < 4; ++r) {
                float expected = joint1.elem2D[c][r] * 0.25f + joint2.elem2D[c][r] * 0.75f;
                GBL_TEST_VERIFY(shz_equalf(result.elem2D[c][r], expected));
            }
        for(int r = 0; r < 4; ++r)
            GBL_TEST_VERIFY(result.elem2D[3][r] == 0.0f);
    }

    {
        randomize_xmtrx_();
        shz::xmtrx::init_identity();

        alignas(32) shz_mat4x3_t joint = {
            .left    = {  1.0f,   2.0f,   3.0f,   4.0f },
            .up      = {  5.0f,   6.0f,   7.0f,   8.0f },
            .forward = {  9.0f,  10.0f,  11.0f,  12.0f }
        };

        shz::xmtrx::blend(joint, 0.0f);

        shz::mat4x4 result;
        shz::xmtrx::store(&result);

        for(int c = 0; c < 4; ++c)
            for(int r = 0; r < 4; ++r)
                GBL_TEST_VERIFY(result.elem2D[c][r] == (c == r ? 1.0f : 0.0f));
    }

    {
        alignas(32) shz_mat4x3_t joint = {
            .left    = {  1.0f,   2.0f,   3.0f,   4.0f },
            .up      = {  5.0f,   6.0f,   7.0f,   8.0f },
            .forward = {  9.0f,  10.0f,  11.0f,  12.0f }
        };

        alignas(32) std::array<float, 16> accum{};

        shz::xmtrx::init_zero();

        GBL_TEST_VERIFY(
            (benchmark_cmp<void>)(
                "shz::xmtrx::blend_4x3", [](const shz_mat4x3_t& j, float w) {
                    shz::xmtrx::blend(j, w);
                },
                "naive per-element accum", [&](const shz_mat4x3_t& j, float w) {
                    for(int i = 0; i < 12; ++i)
                        accum[i] += j.elem[i] * w;
                },
                joint, 0.5f
            )
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec4)
    shz::vec4 v{
        3.0f, 2.0f, 1.0f, 0.0f
    };
    union shz_glm_vec4_t {
        shz::vec4 shz;
        vec4 glm;
    };

    // Rotated
    {
        shz::vec4 expected = { 2.356913f, 2.772574f, 0.870512f, 0.000000f };
        shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
        shz::vec4 vout = shz::xmtrx::transform(v);
        float dist = vout.distance(expected);
        GBL_TEST_VERIFY(dist <= 0.001f);
    }

    // Scaled
    {
        shz::vec4 expected = { 6.0f, 6.0f, 4.0f, 0.0f };
        shz::xmtrx::init_scale(2.0f, 3.0f, 4.0f);
        shz::vec4 vout = shz::xmtrx::transform(v);
        GBL_TEST_VERIFY(vout == expected);
    }

    // Rotate + Scale
    {
        shz::vec4 expected = { 4.713826f, 5.545148f, 1.741025f, 0.000000f };
        shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
        shz::xmtrx::scale_3x3(2.0f, 2.0f, 2.0f);
        shz::vec4 vout = shz::xmtrx::transform(v);
        float dist = vout.distance(expected);
        GBL_TEST_VERIFY(dist <= 0.001f);
    }

    // Rotate + Scale + Translate
    // Expectation: Only rotation and scale applied. Translation ignored.
    {
        shz::vec4 expected = { 4.713826f, 5.545148f, 1.741025f, 0.000000f };
        shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
        shz::xmtrx::scale_3x3(2.0f, 2.0f, 2.0f);
        shz::xmtrx::add_translation(10.0f, 20.0f, 30.0f);
        shz::vec4 vout = shz::xmtrx::transform(v);
        float dist = vout.distance(expected);
        GBL_TEST_VERIFY(dist <= 0.001f);

        vec4 v_glm = { 3.0f, 2.0f, 1.0f, 0.0f };
        mat4 gmat;
        glm_mat4_identity(gmat);
        vec3 tl = { 10.0f, 20.0f, 30.0f };
        glm_translate(gmat, tl);
        vec3 sc = { 2.0f, 2.0f, 2.0f };
        glm_scale(gmat, sc);
        vec3 axis = { 1.0f, 1.0f, 1.0f };
        glm_rotate(gmat, glm_rad(42.0f), axis);

        shz_glm_vec4_t vdest;
        glm_mat4_mulv(gmat, v_glm, vdest.glm);

        dist = vout.distance(vdest.shz);
        GBL_TEST_VERIFY(dist <= 0.001f);
    }

    // Rotate + Scale + Translate
    // Expectation: Translation applied.
    {
        v.w = 1.0f;
        shz::vec4 expected = { 14.713826f, 25.545149f, 31.741024f, 1.000000f };
        shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
        shz::xmtrx::scale_3x3(2.0f, 2.0f, 2.0f);
        shz::xmtrx::add_translation(10.0f, 20.0f, 30.0f);
        shz::vec4 vout = shz::xmtrx::transform(v);
        float dist = vout.distance(expected);
        GBL_TEST_VERIFY(dist <= 0.001f);

        vec4 v_glm = { 3.0f, 2.0f, 1.0f, 1.0f };
        mat4 gmat;
        glm_mat4_identity(gmat);
        vec3 tl = { 10.0f, 20.0f, 30.0f };
        glm_translate(gmat, tl);
        vec3 sc = { 2.0f, 2.0f, 2.0f };
        glm_scale(gmat, sc);
        vec3 axis = { 1.0f, 1.0f, 1.0f };
        glm_rotate(gmat, glm_rad(42.0f), axis);

        shz_glm_vec4_t vdest;
        glm_mat4_mulv(gmat, v_glm, vdest.glm);

        dist = vout.distance(vdest.shz);
        GBL_TEST_VERIFY(dist <= 0.001f);
    }

    {
        volatile mat4 xmtrx;
        shz_glm_vec4 axis = { .shz = {1.0f, 2.0f, 3.0f, 4.0f} };

        (benchmark_cmp<shz::vec4>)(
            "shz::xmtrx::transform", [](const shz_glm_vec4& axis) { return shz::xmtrx::transform(axis.shz); },
            "glm_mat4_mulv",         [&](const shz_glm_vec4& axis) {
                shz_glm_vec4 dst;
                glm_mat4_mulv(*const_cast<mat4*>(&xmtrx), axis.glm, dst.glm);
                return dst.shz;
            }, axis
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec3)

    shz::vec3 expected = { 11.286173f, 10.454851f, 14.258974f };
    shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
    shz::xmtrx::scale_3x3(2.0f, 2.0f, 2.0f);
    shz::xmtrx::add_translation(10.0f, 20.0f, 30.0f);

    shz::vec3 v = { 5.0f, 6.0f, 7.0f };
    shz::vec3 vout = shz::xmtrx::transform(v);

    float dist = vout.distance(expected);
    GBL_TEST_VERIFY(dist <= 0.001f);

GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec2)

    shz::vec2 expected = { 4.679756f, 14.664473f };
    shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
    shz::xmtrx::scale_3x3(2.0f, 2.0f, 2.0f);
    shz::xmtrx::add_translation(10.0f, 20.0f, 30.0f);

    shz::vec2 v = { 5.0f, 6.0f };
    shz::vec2 vout = shz::xmtrx::transform(v);

    float dist = vout.distance(expected);
    GBL_TEST_VERIFY(dist <= 0.001f);

GBL_TEST_CASE_END

GBL_TEST_CASE(transform_point3)

    // Rotate + Scale + Translate
    // Expectation: Translation applied.
    shz::vec3 expected = { 33.442780f, 19.434233f, 67.122986f };
    shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
    shz::xmtrx::scale_3x3(2.0f, 2.0f, 2.0f);
    shz::xmtrx::add_translation(10.0f, 20.0f, 30.0f);

    shz::vec3 p = { 4.0f, 5.0f, 21.0f };
    shz::vec3 vout = shz::xmtrx::transform_point(p);

    float dist = vout.distance(expected);
    GBL_TEST_VERIFY(dist <= 0.005f);
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_point2)

    // Rotate + Scale + Translate
    // Expectation: Translation applied.
    shz::vec2 expected = { 13.623529f, 32.063099f };
    shz::xmtrx::init_rotation(shz::deg_to_rad(42.0f), 1.0f, 1.0f, 1.0f);
    shz::xmtrx::scale_3x3(2.0f, 2.0f, 2.0f);
    shz::xmtrx::add_translation(10.0f, 20.0f, 30.0f);

    shz::vec2 p = { 4.0f, 5.0f };
    shz::vec2 vout = shz::xmtrx::transform_point(p);

    float dist = vout.distance(expected);
    GBL_TEST_VERIFY(dist <= 0.001f);

GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec4_transpose)
    shz::mat4x4 xmtrx_before, xmtrx_after;

    load_asymmetric_xmtrx_();

    shz::xmtrx::store(&xmtrx_before);
    const shz::vec4 v   = { 5.0f, 6.0f, 7.0f, 8.0f };
    const shz::vec4 out = shz::xmtrx::transform_transpose(v);
    shz::xmtrx::store(&xmtrx_after);

    // XMTRX is left exactly as it was.
    GBL_TEST_VERIFY(xmtrx_before == xmtrx_after);

    for(unsigned c = 0; c < 4; ++c) {
        // Each component is one column of XMTRX dotted with the vector.
        const float dot = xmtrx_before.col(c).x * v.x +
                          xmtrx_before.col(c).y * v.y +
                          xmtrx_before.col(c).z * v.z +
                          xmtrx_before.col(c).w * v.w;

        GBL_TEST_VERIFY(shz_equalf(out[c], dot));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec4>)(
            "shz_xmtrx_transform_vec4_transpose", [](const shz::vec4& vec) {
                return shz::xmtrx::transform_transpose(vec);
            },
            "transpose + transform + transpose", [](const shz::vec4& vec) {
                shz::xmtrx::transpose();
                shz::vec4 res = shz::xmtrx::transform(vec);
                shz::xmtrx::transpose();
                return res;
            },
            v
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec3_transpose)
   shz::mat4x4 xmtrx_before, xmtrx_after;

    load_asymmetric_xmtrx_();

    shz::xmtrx::store(&xmtrx_before);
    const shz::vec3 v   = { 5.0f, 6.0f, 7.0f };
    const shz::vec3 out = shz::xmtrx::transform_transpose(v);
    shz::xmtrx::store(&xmtrx_after);

    // XMTRX is left exactly as it was.
    GBL_TEST_VERIFY(xmtrx_before == xmtrx_after);

    for(unsigned c = 0; c < 3; ++c) {
        // Each component is one of the first three columns dotted with the vector.
        const float dot = xmtrx_before.col(c).x * v.x +
                          xmtrx_before.col(c).y * v.y +
                          xmtrx_before.col(c).z * v.z;

        GBL_TEST_VERIFY(shz_equalf(out[c], dot));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec3>)(
            "shz_xmtrx_transform_vec3_transpose", [](const shz::vec3& vec) {
                return shz::xmtrx::transform_transpose(vec);
            },
            "transpose + transform + transpose", [](const shz::vec3& vec) {
                shz::xmtrx::transpose();
                shz::vec3 res = shz::xmtrx::transform(vec);
                shz::xmtrx::transpose();
                return res;
            },
            v
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_point3_transpose)
    shz::mat4x4 xmtrx_before, xmtrx_after;

    load_asymmetric_xmtrx_();

    shz::xmtrx::store(&xmtrx_before);
    const shz::vec3 p   = { 5.0f, 6.0f, 7.0f };
    const shz::vec3 out = shz::xmtrx::transform_point_transpose(p);
    shz::xmtrx::store(&xmtrx_after);

    GBL_TEST_VERIFY(xmtrx_before == xmtrx_after);

    for(unsigned c = 0; c < 3; ++c) {
        const float dot = xmtrx_before.col(c).x * p.x +
                          xmtrx_before.col(c).y * p.y +
                          xmtrx_before.col(c).z * p.z +
                          xmtrx_before.col(c).w;

        GBL_TEST_VERIFY(shz_equalf(out[c], dot));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec3>)(
            "shz_xmtrx_transform_point3_transpose", [](const shz::vec3& pt) {
                return shz::xmtrx::transform_point_transpose(pt);
            },
            "transpose + transform_point + transpose", [](const shz::vec3& pt) {
                shz::xmtrx::transpose();
                shz::vec3 res = shz::xmtrx::transform_point(pt);
                shz::xmtrx::transpose();
                return res;
            },
            p
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_point2_transpose)
    shz::mat4x4 xmtrx_before, xmtrx_after;

    load_asymmetric_xmtrx_();

    shz::xmtrx::store(&xmtrx_before);
    const shz::vec2 p   = { 5.0f, 6.0f };
    const shz::vec2 out = shz::xmtrx::transform_point_transpose(p);
    shz::xmtrx::store(&xmtrx_after);

    GBL_TEST_VERIFY(xmtrx_before == xmtrx_after);

    for(unsigned c = 0; c < 2; ++c) {
        const float dot = xmtrx_before.col(c).x * p.x +
                          xmtrx_before.col(c).y * p.y +
                          xmtrx_before.col(c).w;

        GBL_TEST_VERIFY(shz_equalf(out[c], dot));
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec2>)(
            "shz_xmtrx_transform_point2_transpose", [](const shz::vec2& pt) {
                return shz::xmtrx::transform_point_transpose(pt);
            },
            "transpose + transform_point + transpose", [](const shz::vec2& pt) {
                shz::xmtrx::transpose();
                shz::vec2 res = shz::xmtrx::transform_point(pt);
                shz::xmtrx::transpose();
                return res;
            },
            p
        )
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_x_u16)
    GBL_TEST_VERIFY(check_xmtrx_rotation_u16_(shz::xmtrx::rotate_x, shz::xmtrx::rotate_x));
    (benchmark_cmp<void>)(
        "shz::xmtrx::rotate_x(uint16_t)", [](uint16_t angle) { shz::xmtrx::rotate_x(angle); },
        "shz::xmtrx::rotate_x(float)", [](uint16_t angle) {
            shz::xmtrx::rotate_x(angle * (1.0f / shz::fsca_rad_factor));
        },
        static_cast<uint16_t>(0x2000));
    {
        volatile uint16_t angle16 = 0x2000;
        volatile float    angle   = 0x2000 * (1.0f / shz::fsca_rad_factor);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_x(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::rotate_x(static_cast<uint16_t>(*a));
            },
            "glm_rotate_x", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_rotate_x(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle16, &angle));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_y_u16)
    GBL_TEST_VERIFY(check_xmtrx_rotation_u16_(shz::xmtrx::rotate_y, shz::xmtrx::rotate_y));
    (benchmark_cmp<void>)(
        "shz::xmtrx::rotate_y(uint16_t)", [](uint16_t angle) { shz::xmtrx::rotate_y(angle); },
        "shz::xmtrx::rotate_y(float)", [](uint16_t angle) {
            shz::xmtrx::rotate_y(angle * (1.0f / shz::fsca_rad_factor));
        },
        static_cast<uint16_t>(0x2000));
    {
        volatile uint16_t angle16 = 0x2000;
        volatile float    angle   = 0x2000 * (1.0f / shz::fsca_rad_factor);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_y(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::rotate_y(static_cast<uint16_t>(*a));
            },
            "glm_rotate_y", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_rotate_y(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle16, &angle));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_z_u16)
    GBL_TEST_VERIFY(check_xmtrx_rotation_u16_(shz::xmtrx::rotate_z, shz::xmtrx::rotate_z));
    (benchmark_cmp<void>)(
        "shz::xmtrx::rotate_z(uint16_t)", [](uint16_t angle) { shz::xmtrx::rotate_z(angle); },
        "shz::xmtrx::rotate_z(float)", [](uint16_t angle) {
            shz::xmtrx::rotate_z(angle * (1.0f / shz::fsca_rad_factor));
        },
        static_cast<uint16_t>(0x2000));
    {
        volatile uint16_t angle16 = 0x2000;
        volatile float    angle   = 0x2000 * (1.0f / shz::fsca_rad_factor);

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_z(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::rotate_z(static_cast<uint16_t>(*a));
            },
            "glm_rotate_z", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_rotate_z(*const_cast<mat4*>(&m), *r, *const_cast<mat4*>(&m));
            },
            &angle16, &angle));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_x_yz)
    shz::mat4x4    out;
    mat4           glmMat;
    volatile float angle = 0.6f;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        randomize_xmtrx_();
        shz::xmtrx::init_rotation_x_yz(-sinf(theta), cosf(theta));
        shz::xmtrx::store(&out);

        glm_mat4_identity(glmMat);
        glm_rotate_x(glmMat, theta, glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_x_yz", [](float y, float z) {
            shz::xmtrx::init_rotation_x_yz(y, z);
        },
        "C init_rotation_x_yz", [&](float y, float z) {
            glm_mat4_identity(glmMat);
            glmMat[1][1] = z;
            glmMat[1][2] = -y;
            glmMat[2][1] = y;
            glmMat[2][2] = z;
        },
        -sinf(0.6f), cosf(0.6f)));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_x_yz", [](float y, float z, const volatile float*) {
            shz::xmtrx::init_rotation_x_yz(y, z);
        },
        "glm_mat4_identity + glm_rotate_x", [&](float, float, const volatile float* r) {
            glm_mat4_identity(glmMat);
            glm_rotate_x(glmMat, *r, glmMat);
        },
        -sinf(0.6f), cosf(0.6f), &angle));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_y_xz)
    shz::mat4x4    out;
    mat4           glmMat;
    volatile float angle = 0.6f;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        randomize_xmtrx_();
        shz::xmtrx::init_rotation_y_xz(sinf(theta), cosf(theta));
        shz::xmtrx::store(&out);

        glm_mat4_identity(glmMat);
        glm_rotate_y(glmMat, theta, glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_y_xz", [](float x, float z) {
            shz::xmtrx::init_rotation_y_xz(x, z);
        },
        "C init_rotation_y_xz", [&](float x, float z) {
            glm_mat4_identity(glmMat);
            glmMat[0][0] = z;
            glmMat[0][2] = -x;
            glmMat[2][0] = x;
            glmMat[2][2] = z;
        },
        sinf(0.6f), cosf(0.6f)));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_y_xz", [](float x, float z, const volatile float*) {
            shz::xmtrx::init_rotation_y_xz(x, z);
        },
        "glm_mat4_identity + glm_rotate_y", [&](float, float, const volatile float* r) {
            glm_mat4_identity(glmMat);
            glm_rotate_y(glmMat, *r, glmMat);
        },
        sinf(0.6f), cosf(0.6f), &angle));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_rotation_z_xy)
    shz::mat4x4    out;
    mat4           glmMat;
    volatile float angle = 0.6f;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        randomize_xmtrx_();
        shz::xmtrx::init_rotation_z_xy(cosf(theta), sinf(theta));
        shz::xmtrx::store(&out);

        glm_mat4_identity(glmMat);
        glm_rotate_z(glmMat, theta, glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_z_xy", [](float x, float y) {
            shz::xmtrx::init_rotation_z_xy(x, y);
        },
        "C init_rotation_z_xy", [&](float x, float y) {
            glm_mat4_identity(glmMat);
            glmMat[0][0] = x;
            glmMat[0][1] = y;
            glmMat[1][0] = -y;
            glmMat[1][1] = x;
        },
        cosf(0.6f), sinf(0.6f)));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_rotation_z_xy", [](float x, float y, const volatile float*) {
            shz::xmtrx::init_rotation_z_xy(x, y);
        },
        "glm_mat4_identity + glm_rotate_z", [&](float, float, const volatile float* r) {
            glm_mat4_identity(glmMat);
            glm_rotate_z(glmMat, *r, glmMat);
        },
        cosf(0.6f), sinf(0.6f), &angle));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_x_yz)
    shz::mat4x4 before, out;
    mat4        glmMat, glmRot;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
            shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
        shz::xmtrx::store(&before);
        shz::xmtrx::rotate_x_yz(-sinf(theta), cosf(theta));
        shz::xmtrx::store(&out);

        memcpy(glmMat, &before, sizeof(mat4));
        glm_rotate_x(glmMat, theta, glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    glm_mat4_identity(glmMat);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::rotate_x_yz", [](float y, float z) {
            shz::xmtrx::rotate_x_yz(y, z);
        },
        "glm_mul_rot", [&](float y, float z) {
            glm_mat4_identity(glmRot);
            glmRot[1][1] = z;
            glmRot[1][2] = -y;
            glmRot[2][1] = y;
            glmRot[2][2] = z;
            glm_mul_rot(glmMat, glmRot, glmMat);
        },
        -sinf(0.6f), cosf(0.6f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_y_xz)
    shz::mat4x4 before, out;
    mat4        glmMat, glmRot;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
            shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
        shz::xmtrx::store(&before);
        shz::xmtrx::rotate_y_xz(sinf(theta), cosf(theta));
        shz::xmtrx::store(&out);

        memcpy(glmMat, &before, sizeof(mat4));
        glm_rotate_y(glmMat, theta, glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    glm_mat4_identity(glmMat);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::rotate_y_xz", [](float x, float z) {
            shz::xmtrx::rotate_y_xz(x, z);
        },
        "glm_mul_rot", [&](float x, float z) {
            glm_mat4_identity(glmRot);
            glmRot[0][0] = z;
            glmRot[0][2] = -x;
            glmRot[2][0] = x;
            glmRot[2][2] = z;
            glm_mul_rot(glmMat, glmRot, glmMat);
        },
        sinf(0.6f), cosf(0.6f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_z_xy)
    shz::mat4x4 before, out;
    mat4        glmMat, glmRot;

    for(float theta : { 0.0f, 0.6f, -1.9f, 2.5f, 3.1f, -shz::pi_f * 0.5f }) {
        for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
            shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
        shz::xmtrx::store(&before);
        shz::xmtrx::rotate_z_xy(cosf(theta), sinf(theta));
        shz::xmtrx::store(&out);

        memcpy(glmMat, &before, sizeof(mat4));
        glm_rotate_z(glmMat, theta, glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    glm_mat4_identity(glmMat);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::rotate_z_xy", [](float x, float y) {
            shz::xmtrx::rotate_z_xy(x, y);
        },
        "glm_mul_rot", [&](float x, float y) {
            glm_mat4_identity(glmRot);
            glmRot[0][0] = x;
            glmRot[0][1] = y;
            glmRot[1][0] = -y;
            glmRot[1][1] = x;
            glm_mul_rot(glmMat, glmRot, glmMat);
        },
        cosf(0.6f), sinf(0.6f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_billboard_x)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 9.0f, 2.0f, 3.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    shz::mat4x4            out;
    mat4                   glmMat;

    for(const auto& c : cases) {
        randomize_xmtrx_();
        shz::xmtrx::init_billboard_x(c[0], c[1]);
        shz::xmtrx::store(&out);

        vec2 d = { c[1].y - c[0].y, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        glm_mat4_identity(glmMat);
        glm_rotate_x(glmMat, atan2f(-d[0], d[1]), glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    randomize_xmtrx_();
    shz::xmtrx::init_billboard_x(onAxis[0], onAxis[1]);
    shz::xmtrx::store(&out);
    glm_mat4_identity(glmMat);
    GBL_TEST_VERIFY(compare_glm(out, glmMat));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_billboard_x", [](const volatile float* v) {
            shz::xmtrx::init_billboard_x(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + rotation", [&](const volatile float* v) {
            vec2 d = { v[4] - v[1], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(glmMat);
            glmMat[1][1] = d[1];
            glmMat[1][2] = -d[0];
            glmMat[2][1] = d[0];
            glmMat[2][2] = d[1];
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_billboard_y)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 9.0f, 3.0f } };
    alignas(32) static struct {
        mat4           glmMat;
        volatile float in[6];
    } bench = { {}, { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f } };
    auto& [glmMat, in] = bench;
    shz::mat4x4            out;

    for(const auto& c : cases) {
        randomize_xmtrx_();
        shz::xmtrx::init_billboard_y(c[0], c[1]);
        shz::xmtrx::store(&out);

        vec2 d = { c[1].x - c[0].x, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        glm_mat4_identity(glmMat);
        glm_rotate_y(glmMat, atan2f(d[0], d[1]), glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    randomize_xmtrx_();
    shz::xmtrx::init_billboard_y(onAxis[0], onAxis[1]);
    shz::xmtrx::store(&out);
    glm_mat4_identity(glmMat);
    GBL_TEST_VERIFY(compare_glm(out, glmMat));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_billboard_y", [](const volatile float* v) {
            shz::xmtrx::init_billboard_y(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + rotation", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(glmMat);
            glmMat[0][0] = d[1];
            glmMat[0][2] = -d[0];
            glmMat[2][0] = d[0];
            glmMat[2][2] = d[1];
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(init_billboard_z)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 2.0f, 9.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    shz::mat4x4            out;
    mat4                   glmMat;

    for(const auto& c : cases) {
        randomize_xmtrx_();
        shz::xmtrx::init_billboard_z(c[0], c[1]);
        shz::xmtrx::store(&out);

        vec2 d = { c[1].x - c[0].x, c[1].y - c[0].y };
        glm_vec2_normalize(d);
        glm_mat4_identity(glmMat);
        glm_rotate_z(glmMat, atan2f(d[1], d[0]), glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    randomize_xmtrx_();
    shz::xmtrx::init_billboard_z(onAxis[0], onAxis[1]);
    shz::xmtrx::store(&out);
    glm_mat4_identity(glmMat);
    GBL_TEST_VERIFY(compare_glm(out, glmMat));

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::init_billboard_z", [](const volatile float* v) {
            shz::xmtrx::init_billboard_z(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + rotation", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[4] - v[1] };
            glm_vec2_normalize(d);
            glm_mat4_identity(glmMat);
            glmMat[0][0] = d[0];
            glmMat[0][1] = d[1];
            glmMat[1][0] = -d[1];
            glmMat[1][1] = d[0];
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(billboard_x)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 9.0f, 2.0f, 3.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    shz::mat4x4            before, out;
    mat4                   glmMat, glmRot;

    for(const auto& c : cases) {
        for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
            shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
        shz::xmtrx::store(&before);
        shz::xmtrx::billboard_x(c[0], c[1]);
        shz::xmtrx::store(&out);

        vec2 d = { c[1].y - c[0].y, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        memcpy(glmMat, &before, sizeof(mat4));
        glm_rotate_x(glmMat, atan2f(-d[0], d[1]), glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
        shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
    shz::xmtrx::store(&before);
    shz::xmtrx::billboard_x(onAxis[0], onAxis[1]);
    shz::xmtrx::store(&out);
    memcpy(glmMat, &before, sizeof(mat4));
    GBL_TEST_VERIFY(compare_glm(out, glmMat));

    glm_mat4_identity(glmMat);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::billboard_x", [](const volatile float* v) {
            shz::xmtrx::billboard_x(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + glm_mul_rot", [&](const volatile float* v) {
            vec2 d = { v[4] - v[1], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(glmRot);
            glmRot[1][1] = d[1];
            glmRot[1][2] = -d[0];
            glmRot[2][1] = d[0];
            glmRot[2][2] = d[1];
            glm_mul_rot(glmMat, glmRot, glmMat);
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(billboard_y)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 9.0f, 3.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    shz::mat4x4            before, out;
    mat4                   glmMat, glmRot;

    for(const auto& c : cases) {
        for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
            shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
        shz::xmtrx::store(&before);
        shz::xmtrx::billboard_y(c[0], c[1]);
        shz::xmtrx::store(&out);

        vec2 d = { c[1].x - c[0].x, c[1].z - c[0].z };
        glm_vec2_normalize(d);
        memcpy(glmMat, &before, sizeof(mat4));
        glm_rotate_y(glmMat, atan2f(d[0], d[1]), glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
        shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
    shz::xmtrx::store(&before);
    shz::xmtrx::billboard_y(onAxis[0], onAxis[1]);
    shz::xmtrx::store(&out);
    memcpy(glmMat, &before, sizeof(mat4));
    GBL_TEST_VERIFY(compare_glm(out, glmMat));

    glm_mat4_identity(glmMat);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::billboard_y", [](const volatile float* v) {
            shz::xmtrx::billboard_y(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + glm_mul_rot", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[5] - v[2] };
            glm_vec2_normalize(d);
            glm_mat4_identity(glmRot);
            glmRot[0][0] = d[1];
            glmRot[0][2] = -d[0];
            glmRot[2][0] = d[0];
            glmRot[2][2] = d[1];
            glm_mul_rot(glmMat, glmRot, glmMat);
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(billboard_z)
    static const shz::vec3 cases[][2] = {
        { {  1.0f, 2.0f,  3.0f }, {  4.0f, -1.0f,  7.0f } },
        { {  0.0f, 0.0f,  0.0f }, { -3.0f,  5.0f, -2.0f } },
        { { -2.0f, 1.0f,  4.0f }, {  5.0f,  9.0f, -6.0f } },
        { { 10.0f, 0.5f, -5.0f }, { 10.5f,  3.0f, -5.5f } },
        { {  0.0f, 0.0f,  0.0f }, {  0.0f, -1.0f, -1.0f } },
    };
    static const shz::vec3 onAxis[2]  = { { 1.0f, 2.0f, 3.0f }, { 1.0f, 2.0f, 9.0f } };
    static volatile float  in[6]      = { 1.0f, 2.0f, 3.0f, 4.0f, -1.0f, 7.0f };
    shz::mat4x4            before, out;
    mat4                   glmMat, glmRot;

    for(const auto& c : cases) {
        for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
            shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
        shz::xmtrx::store(&before);
        shz::xmtrx::billboard_z(c[0], c[1]);
        shz::xmtrx::store(&out);

        vec2 d = { c[1].x - c[0].x, c[1].y - c[0].y };
        glm_vec2_normalize(d);
        memcpy(glmMat, &before, sizeof(mat4));
        glm_rotate_z(glmMat, atan2f(d[1], d[0]), glmMat);

        GBL_TEST_VERIFY(compare_glm(out, glmMat));
    }

    for(unsigned reg = SHZ_XMTRX_XF0; reg <= SHZ_XMTRX_XF15; ++reg)
        shz::xmtrx::write(static_cast<shz::xmtrx::reg>(reg), gblRandUniform(-10.0f, 10.0f));
    shz::xmtrx::store(&before);
    shz::xmtrx::billboard_z(onAxis[0], onAxis[1]);
    shz::xmtrx::store(&out);
    memcpy(glmMat, &before, sizeof(mat4));
    GBL_TEST_VERIFY(compare_glm(out, glmMat));

    glm_mat4_identity(glmMat);

    GBL_TEST_VERIFY((benchmark_cmp<void>)(
        "shz::xmtrx::billboard_z", [](const volatile float* v) {
            shz::xmtrx::billboard_z(shz::vec3(v[0], v[1], v[2]), shz::vec3(v[3], v[4], v[5]));
        },
        "glm_vec2_normalize + glm_mul_rot", [&](const volatile float* v) {
            vec2 d = { v[3] - v[0], v[4] - v[1] };
            glm_vec2_normalize(d);
            glm_mat4_identity(glmRot);
            glmRot[0][0] = d[0];
            glmRot[0][1] = d[1];
            glmRot[1][0] = -d[1];
            glmRot[1][1] = d[0];
            glm_mul_rot(glmMat, glmRot, glmMat);
        },
        in));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_xyz)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_xyz(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_xyz(shz::vec3(a0, a1, a2));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_xyz(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::rotate_xyz(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::rotate_xyz(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::rotate_xyz", "glm_rotate_x/y/z", "xyz", false,
                                          [](float a0, float a1, float a2) { shz::xmtrx::rotate_xyz(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zyx)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zyx(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zyx(shz::vec3(a2, a1, a0));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zyx(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::rotate_zyx(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::rotate_zyx(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::rotate_zyx", "glm_rotate_z/y/x", "zyx", false,
                                          [](float a0, float a1, float a2) { shz::xmtrx::rotate_zyx(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zxy)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zxy(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zxy(shz::vec3(a1, a2, a0));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zxy(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::rotate_zxy(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::rotate_zxy(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::rotate_zxy", "glm_rotate_z/x/y", "zxy", false,
                                          [](float a0, float a1, float a2) { shz::xmtrx::rotate_zxy(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_yxz)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_yxz(a0, a1, a2);
    }));
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_yxz(shz::vec3(a1, a0, a2));
    }));
    // Double arguments must select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_yxz(static_cast<double>(a0), static_cast<double>(a1), static_cast<double>(a2));
    }));
    // So must a double beside a float, and an integer beside floating-point arguments.
    static_assert(noexcept(shz::xmtrx::rotate_yxz(0.3, -1.2f, 2.5)));
    static_assert(noexcept(shz::xmtrx::rotate_yxz(0.3, -1, 2.5L)));
    GBL_TEST_VERIFY(bench_xmtrx_rotation_("shz::xmtrx::rotate_yxz", "glm_rotate_y/x/z", "yxz", false,
                                          [](float a0, float a1, float a2) { shz::xmtrx::rotate_yxz(a0, a1, a2); }));
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_xyz_u16)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_xyz(fsca_angle_(a0), fsca_angle_(a1), fsca_angle_(a2));
    }));
    // A double beside uint16_t angles must not select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("xyz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_xyz(static_cast<double>(fsca_angle_(a0)), fsca_angle_(a1), fsca_angle_(a2));
    }));
    (benchmark_cmp<void>)(
        "shz::xmtrx::rotate_xyz(uint16_t)", [](uint16_t x, uint16_t y, uint16_t z) { shz::xmtrx::rotate_xyz(x, y, z); },
        "rotate_x + rotate_y + rotate_z (uint16_t)", [](uint16_t x, uint16_t y, uint16_t z) {
            shz::xmtrx::rotate_x(x);
            shz::xmtrx::rotate_y(y);
            shz::xmtrx::rotate_z(z);
        },
        fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f));
    {
        volatile uint16_t angles16[3] = { fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f) };
        volatile float    angles[3]   = { 0.3f, -1.2f, 2.5f };

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_xyz(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::rotate_xyz(static_cast<uint16_t>(a[0]), static_cast<uint16_t>(a[1]), static_cast<uint16_t>(a[2]));
            },
            "glm_rotate_x/y/z", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_rotate_ordered_(*const_cast<mat4*>(&m), "xyz", r[0], r[1], r[2]);
            },
            angles16, angles));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zyx_u16)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zyx(fsca_angle_(a0), fsca_angle_(a1), fsca_angle_(a2));
    }));
    // A double beside uint16_t angles must not select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zyx", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zyx(static_cast<double>(fsca_angle_(a0)), fsca_angle_(a1), fsca_angle_(a2));
    }));
    (benchmark_cmp<void>)(
        "shz::xmtrx::rotate_zyx(uint16_t)", [](uint16_t z, uint16_t y, uint16_t x) { shz::xmtrx::rotate_zyx(z, y, x); },
        "rotate_z + rotate_y + rotate_x (uint16_t)", [](uint16_t z, uint16_t y, uint16_t x) {
            shz::xmtrx::rotate_z(z);
            shz::xmtrx::rotate_y(y);
            shz::xmtrx::rotate_x(x);
        },
        fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f));
    {
        volatile uint16_t angles16[3] = { fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f) };
        volatile float    angles[3]   = { 0.3f, -1.2f, 2.5f };

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_zyx(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::rotate_zyx(static_cast<uint16_t>(a[0]), static_cast<uint16_t>(a[1]), static_cast<uint16_t>(a[2]));
            },
            "glm_rotate_z/y/x", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_rotate_ordered_(*const_cast<mat4*>(&m), "zyx", r[0], r[1], r[2]);
            },
            angles16, angles));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_zxy_u16)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zxy(fsca_angle_(a0), fsca_angle_(a1), fsca_angle_(a2));
    }));
    // A double beside uint16_t angles must not select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("zxy", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_zxy(static_cast<double>(fsca_angle_(a0)), fsca_angle_(a1), fsca_angle_(a2));
    }));
    (benchmark_cmp<void>)(
        "shz::xmtrx::rotate_zxy(uint16_t)", [](uint16_t z, uint16_t x, uint16_t y) { shz::xmtrx::rotate_zxy(z, x, y); },
        "rotate_z + rotate_x + rotate_y (uint16_t)", [](uint16_t z, uint16_t x, uint16_t y) {
            shz::xmtrx::rotate_z(z);
            shz::xmtrx::rotate_x(x);
            shz::xmtrx::rotate_y(y);
        },
        fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f));
    {
        volatile uint16_t angles16[3] = { fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f) };
        volatile float    angles[3]   = { 0.3f, -1.2f, 2.5f };

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_zxy(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::rotate_zxy(static_cast<uint16_t>(a[0]), static_cast<uint16_t>(a[1]), static_cast<uint16_t>(a[2]));
            },
            "glm_rotate_z/x/y", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_rotate_ordered_(*const_cast<mat4*>(&m), "zxy", r[0], r[1], r[2]);
            },
            angles16, angles));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(rotate_yxz_u16)
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_yxz(fsca_angle_(a0), fsca_angle_(a1), fsca_angle_(a2));
    }));
    // A double beside uint16_t angles must not select the radian overload.
    GBL_TEST_VERIFY(check_xmtrx_rotation_("yxz", false, [](float a0, float a1, float a2) {
        shz::xmtrx::rotate_yxz(static_cast<double>(fsca_angle_(a0)), fsca_angle_(a1), fsca_angle_(a2));
    }));
    (benchmark_cmp<void>)(
        "shz::xmtrx::rotate_yxz(uint16_t)", [](uint16_t y, uint16_t x, uint16_t z) { shz::xmtrx::rotate_yxz(y, x, z); },
        "rotate_y + rotate_x + rotate_z (uint16_t)", [](uint16_t y, uint16_t x, uint16_t z) {
            shz::xmtrx::rotate_y(y);
            shz::xmtrx::rotate_x(x);
            shz::xmtrx::rotate_z(z);
        },
        fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f));
    {
        volatile uint16_t angles16[3] = { fsca_angle_(0.3f), fsca_angle_(-1.2f), fsca_angle_(2.5f) };
        volatile float    angles[3]   = { 0.3f, -1.2f, 2.5f };

        GBL_TEST_VERIFY((benchmark_cmp<void>)(
            "shz::xmtrx::rotate_yxz(uint16_t)", [](const volatile uint16_t* a, const volatile float*) {
                shz::xmtrx::rotate_yxz(static_cast<uint16_t>(a[0]), static_cast<uint16_t>(a[1]), static_cast<uint16_t>(a[2]));
            },
            "glm_rotate_y/x/z", [](const volatile uint16_t*, const volatile float* r) {
                static volatile mat4 m;
                glm_rotate_ordered_(*const_cast<mat4*>(&m), "yxz", r[0], r[1], r[2]);
            },
            angles16, angles));
    }
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
                  init_rotation_zxy,
                  init_rotation_yxz,
                  init_rotation_x_u16,
                  init_rotation_y_u16,
                  init_rotation_z_u16,
                  init_rotation_xyz_u16,
                  init_rotation_zyx_u16,
                  init_rotation_zxy_u16,
                  init_rotation_yxz_u16,
                  init_rotation,
                  init_rotation_dir,
                  init_diagonal,
                  init_upper_triangular,
                  init_lower_triangular,
                  init_symmetric_skew,
                  init_outer_product,
                  init_screen,
                  init_permutation_wxyz,
                  init_permutation_yzwx,
                  init_permutation_wzyx,
                  apply_4x4,
                  apply_unaligned_4x4,
                  apply_transpose_4x4,
                  apply_transpose_unaligned_4x4,
                  apply_reverse_4x4,
                  apply_reverse_unaligned_4x4,
                  apply_reverse_transpose_4x4,
                  apply_reverse_transpose_unaligned_4x4,
                  add_translation,
                  get_translation,
                  set_translation,
                  set_scale,
                  get_scale,
                  get_rotation,
                  scale_3x3,
                  rotate,
                  rotate_dir,
                  rotate_u16,
                  rotate_dir_u16,
                  init_rotation_quat,
                  rotate_quat,
                  permute_wxyz,
                  permute_yzwx,
                  permute_wzyx,
                  load_apply_4x4,
                  load_apply_unaligned_4x4,
                  apply_store_4x4,
                  apply_store_unaligned_4x4,
                  load_apply_store_4x4,
                  load_apply_store_unaligned_4x4,
                  translate,
                  translate_reverse,
                  scale,
                  scale_reverse,
                  rotate_x,
                  rotate_y,
                  rotate_z,
                  rotate_x_u16,
                  rotate_y_u16,
                  rotate_z_u16,
                  init_rotation_x_yz,
                  init_rotation_y_xz,
                  init_rotation_z_xy,
                  rotate_x_yz,
                  rotate_y_xz,
                  rotate_z_xy,
                  init_billboard_x,
                  init_billboard_y,
                  init_billboard_z,
                  billboard_x,
                  billboard_y,
                  billboard_z,
                  rotate_xyz,
                  rotate_zyx,
                  rotate_zxy,
                  rotate_yxz,
                  rotate_xyz_u16,
                  rotate_zyx_u16,
                  rotate_zxy_u16,
                  rotate_yxz_u16,
                  ortho,
                  frustum,
                  perspective,
                  screen,
                  lookat,
                  load_3x4,
                  load_transpose_3x4,
                  load_rows_3x4,
                  load_cols_3x4,
                  store_3x4,
                  store_transpose_3x4,
                  apply_3x4,
                  apply_transpose_3x4,
                  apply_reverse_3x4,
                  apply_reverse_transpose_3x4,
                  load_apply_3x4,
                  load_apply_store_3x4,
                  load_store_2x2,
                  apply_self,
                  trace,
                  invert_rigid,
                  add_4x4,
                  sub_4x4,
                  negate,
                  load_3x3,
                  load_transpose_3x3,
                  store_3x3,
                  store_transpose_3x3,
                  apply_3x3,
                  apply_transpose_3x3,
                  load_apply_store_3x3,
                  blend_4x4,
                  blend_3x4,
                  blend_4x3,
                  transform_vec4,
                  transform_vec3,
                  transform_vec2,
                  transform_point3,
                  transform_point2,
                  transform_vec4_transpose,
                  transform_vec3_transpose,
                  transform_point3_transpose,
                  transform_point2_transpose)
