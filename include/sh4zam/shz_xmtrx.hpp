/*! \file
 *  \brief   C++ Active Matrix API
 *  \ingroup xmtrx
 *
 *  This file provides an API built around manipulating and performing
 *  calculations using the SH4's "current" 4x4 matrix, which is held within
 *  a secondary back-bank of 16 single-precision floating-point registers.
 *
 *  \todo
 *      - Fourier transforms
 *      - Arbitrarily-sized matrix routines
 *      - Outer products
 *
 *  \author    Falco Girgis
 *  \copyright MIT License
 */

#ifndef SHZ_XMTRX_HPP
#define SHZ_XMTRX_HPP

#include <array>

#include "shz_xmtrx.h"
#include "shz_vector.hpp"
#include "shz_quat.hpp"

namespace shz {

    struct xmtrx {
        enum class reg: uint8_t {
            XF0  = SHZ_XMTRX_XF0,
            XF1  = SHZ_XMTRX_XF1,
            XF2  = SHZ_XMTRX_XF2,
            XF3  = SHZ_XMTRX_XF3,
            XF4  = SHZ_XMTRX_XF4,
            XF5  = SHZ_XMTRX_XF5,
            XF6  = SHZ_XMTRX_XF6,
            XF7  = SHZ_XMTRX_XF7,
            XF8  = SHZ_XMTRX_XF8,
            XF9  = SHZ_XMTRX_XF9,
            XF10 = SHZ_XMTRX_XF10,
            XF11 = SHZ_XMTRX_XF11,
            XF12 = SHZ_XMTRX_XF12,
            XF13 = SHZ_XMTRX_XF13,
            XF14 = SHZ_XMTRX_XF14,
            XF15 = SHZ_XMTRX_XF15
        };

        SHZ_FORCE_INLINE static float read(reg xf) noexcept {
            return shz_xmtrx_read_reg(static_cast<shz_xmtrx_reg>(xf));
        }

        SHZ_FORCE_INLINE static void write(reg xf, float value) noexcept {
            shz_xmtrx_write_reg(static_cast<shz_xmtrx_reg>(xf), value);
        }

        SHZ_FORCE_INLINE static void load(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_load_4x4(&mat4);
        }

        SHZ_FORCE_INLINE static void load(const float cArray[16]) noexcept {
            shz_xmtrx_load_4x4_unaligned(cArray);
        }

        SHZ_FORCE_INLINE static void load(std::array<float, 16> array) noexcept {
            load(array.data());
        }

        SHZ_FORCE_INLINE static void load_transpose(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_load_4x4_transpose(&mat4);
        }

        SHZ_FORCE_INLINE static void load_transpose(const float cArray[16]) noexcept {
            shz_xmtrx_load_4x4_transpose(reinterpret_cast<const shz_mat4x4_t*>(cArray));
        }

        SHZ_FORCE_INLINE static void load_transpose(std::array<float, 16> array) noexcept {
            load_transpose(array.data());
        }

        SHZ_FORCE_INLINE static void load_wxyz(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_load_4x4_wxyz(&mat4);
        }

        SHZ_FORCE_INLINE static void load_cols(const shz_vec4_t& c1,
                                               const shz_vec4_t& c2,
                                               const shz_vec4_t& c3,
                                               const shz_vec4_t& c4) noexcept {
            shz_xmtrx_load_4x4_cols(&c1, &c2, &c3, &c4);
        }

        SHZ_FORCE_INLINE static void load_rows(const shz_vec4_t& r1,
                                               const shz_vec4_t& r2,
                                               const shz_vec4_t& r3,
                                               const shz_vec4_t& r4) noexcept {
            shz_xmtrx_load_4x4_rows(&r1, &r2, &r3, &r4);
        }

        SHZ_FORCE_INLINE static void load_apply(const shz_mat4x4_t &mat1, const shz_mat4x4_t &mat2) {
            shz_xmtrx_load_4x4_apply(&mat1, &mat2);
        }

        SHZ_FORCE_INLINE static void load_apply_store(shz_mat4x4_t* dst, const shz_mat4x4_t &mat1, const shz_mat4x4_t &mat2) {
            shz_xmtrx_load_4x4_apply_store(dst, &mat1, &mat2);
        }

        SHZ_FORCE_INLINE static void store(shz_mat4x4_t* mat) noexcept {
            shz_xmtrx_store_4x4(mat);
        }

        SHZ_FORCE_INLINE static void store(float cArray[16]) noexcept {
            shz_xmtrx_store_4x4_unaligned(cArray);
        }

        SHZ_FORCE_INLINE static void store(std::array<float, 16> array) noexcept {
            shz_xmtrx_store_4x4_unaligned(array.data());
        }

        SHZ_FORCE_INLINE static void store_transpose(shz_mat4x4_t* mat) noexcept {
            shz_xmtrx_store_4x4_transpose(mat);
        }

        SHZ_FORCE_INLINE static void init_identity() noexcept {
            shz_xmtrx_init_identity();
        }

        SHZ_FORCE_INLINE static void init_diagonal(float x, float y, float z, float w) noexcept {
            shz_xmtrx_init_diagonal(x, y, z, w);
        }

        SHZ_FORCE_INLINE static void init_scale(float x, float y, float z) noexcept {
            shz_xmtrx_init_scale(x, y, z);
        }

        SHZ_FORCE_INLINE static void init_rotation_x(float x) noexcept {
            shz_xmtrx_init_rotation_x(x);
        }

        SHZ_FORCE_INLINE static void init_rotation_y(float y) noexcept {
            shz_xmtrx_init_rotation_y(y);
        }

        SHZ_FORCE_INLINE static void init_rotation_z(float z) noexcept {
            shz_xmtrx_init_rotation_z(z);
        }

        SHZ_FORCE_INLINE static void init_translation(float x, float y, float z) noexcept {
            shz_xmtrx_init_translation(x, y, z);
        }

        SHZ_FORCE_INLINE static void init_symmetric_skew(float x, float y, float z) noexcept {
            shz_xmtrx_init_symmetric_skew(x, y, z);
        }

        SHZ_FORCE_INLINE static void apply_symmetric_skew(float x, float y, float z) noexcept {
            shz_xmtrx_apply_symmetric_skew(x, y, z);
        }

        SHZ_FORCE_INLINE static void add_symmetric_skew(float x, float y, float z) noexcept {
            shz_xmtrx_add_symmetric_skew(x, y, z);
        }

        SHZ_FORCE_INLINE static void add_diagonal(float x, float y, float z, float w) noexcept {
            shz_xmtrx_add_diagonal(x, y, z, w);
        }

        SHZ_FORCE_INLINE static void apply(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_apply_4x4(&mat4);
        }

        SHZ_FORCE_INLINE static void apply(const float cArray[16]) noexcept {
            shz_xmtrx_apply_4x4_unaligned(cArray);
        }

        SHZ_FORCE_INLINE static void apply(const std::array<float, 16> array) noexcept {
            shz_xmtrx_apply_4x4_unaligned(array.data());
        }

        SHZ_FORCE_INLINE static void set_translation(float x, float y, float z) noexcept {
            shz_xmtrx_set_translation(x, y, z);
        }

        SHZ_FORCE_INLINE static void apply_translation(float x, float y, float z) noexcept {
            shz_xmtrx_apply_translation(x, y, z);
        }

        SHZ_FORCE_INLINE static void apply_scale(float x, float y, float z) noexcept {
            shz_xmtrx_apply_scale(x, y, z);
        }

        SHZ_FORCE_INLINE static void apply_rotation_x(float x) noexcept {
            shz_xmtrx_apply_rotation_x(x);
        }

        SHZ_FORCE_INLINE static void apply_rotation_y(float y) noexcept {
            shz_xmtrx_apply_rotation_x(y);
        }

        SHZ_FORCE_INLINE static void apply_rotation_z(float z) noexcept {
            shz_xmtrx_apply_rotation_z(z);
        }

        SHZ_FORCE_INLINE static void init_rotation_xyz(float x, float y, float z) noexcept {
            shz_xmtrx_init_rotation_xyz(x, y, z);
        }

        SHZ_FORCE_INLINE static void init_rotation_zyx(float z, float y, float x) noexcept {
            shz_xmtrx_init_rotation_zyx(z, y, x);
        }

        SHZ_FORCE_INLINE static void init_rotation_yxz(float y, float x, float z) noexcept {
            shz_xmtrx_init_rotation_yxz(y, x, z);
        }

        SHZ_FORCE_INLINE static void apply_rotation_xyz(float x, float y, float z) noexcept {
            shz_xmtrx_init_rotation_xyz(x, y, z);
        }

        SHZ_FORCE_INLINE static void apply_rotation_zyx(float z, float y, float x) noexcept {
            shz_xmtrx_apply_rotation_zyx(z, y, x);
        }

        SHZ_FORCE_INLINE static void apply_rotation_yxz(float y, float x, float z) noexcept {
            shz_xmtrx_apply_rotation_yxz(y, x, z);
        }

        SHZ_FORCE_INLINE static void init_rotation(shz_vec3_t axis, float angle) noexcept {
            shz_xmtrx_init_rotation(axis, angle);
        }

        SHZ_FORCE_INLINE static void apply_rotation(shz_vec3_t axis, float angle) noexcept {
            shz_xmtrx_init_rotation(axis, angle);
        }

        SHZ_FORCE_INLINE static float determinant() noexcept {
            return shz_xmtrx_determinant();
        }

        SHZ_FORCE_INLINE static quat to_quat() noexcept {
            return shz_xmtrx_to_quat();
        }

        //SHZ_FORCE_INLINE static void invert_orthonormal() noexcept {
        //    shz_xmtrx_invert_orthonormal();
        //}

        SHZ_FORCE_INLINE static void transpose() noexcept {
            shz_xmtrx_transpose();
        }

        SHZ_FORCE_INLINE static vec4 transform(shz_vec4_t in) noexcept {
            return shz_xmtrx_transform_vec4(in);
        }

        SHZ_FORCE_INLINE static vec3 transform(shz_vec3_t in) noexcept {
            return shz_xmtrx_transform_vec3(in);
        }

        SHZ_FORCE_INLINE static vec2 transform(shz_vec2_t in) noexcept {
            return shz_xmtrx_transform_vec2(in);
        }
    };
}

#endif
