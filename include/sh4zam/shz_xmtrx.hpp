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

    /*! Static structure around the 4x4 XMTRX FP register back-bank.

        This structure provides the C++ bindings to the XMTRX API as a series
        of static member functions wrapping the C API.

        \sa xmtrx
    */
    struct xmtrx {

        //! FP back-bank registers comprising XMTRX.
        enum reg {
            XF0  = SHZ_XMTRX_XF0,   //!< FP register XF0
            XF1  = SHZ_XMTRX_XF1,   //!< FP register XF1
            XF2  = SHZ_XMTRX_XF2,   //!< FP register XF2
            XF3  = SHZ_XMTRX_XF3,   //!< FP register XF3
            XF4  = SHZ_XMTRX_XF4,   //!< FP register XF4
            XF5  = SHZ_XMTRX_XF5,   //!< FP register XF5
            XF6  = SHZ_XMTRX_XF6,   //!< FP register XF6
            XF7  = SHZ_XMTRX_XF7,   //!< FP register XF7
            XF8  = SHZ_XMTRX_XF8,   //!< FP register XF8
            XF9  = SHZ_XMTRX_XF9,   //!< FP register XF9
            XF10 = SHZ_XMTRX_XF10,  //!< FP register XF10
            XF11 = SHZ_XMTRX_XF11,  //!< FP register XF11
            XF12 = SHZ_XMTRX_XF12,  //!< FP register XF12
            XF13 = SHZ_XMTRX_XF13,  //!< FP register XF13
            XF14 = SHZ_XMTRX_XF14,  //!< FP register XF14
            XF15 = SHZ_XMTRX_XF15   //!< FP register XF15
        };


    /*! \name  Accessors
        \brief Setting and retrieving individual XMTRX register values.
        @{
    */

        //! C++ wrapper around shz_xmtrx_read().
        SHZ_FORCE_INLINE static float read(reg xf) noexcept {
            return shz_xmtrx_read(static_cast<shz_xmtrx_reg>(xf));
        }

        //! C++ wrapper around shz_xmtrx_write().
        SHZ_FORCE_INLINE static void write(reg xf, float value) noexcept {
            shz_xmtrx_write(static_cast<shz_xmtrx_reg>(xf), value);
        }

    //! @}

    /*! \name  Loading
        \brief Routines for loading XMTRX contents from memory.
        @{
    */

        //! C++ wrapper around shz_xmtrx_load_4x4().
        SHZ_FORCE_INLINE static void load(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_load_4x4(&mat4);
        }

        //! C++ wrapper around shz_xmtrx_load_unaligned_4x4().
        SHZ_FORCE_INLINE static void load(const float cArray[16]) noexcept {
            shz_xmtrx_load_unaligned_4x4(cArray);
        }

        //! C++ wrapper around shz_xmtrx_load_unaligned_4x4().
        SHZ_FORCE_INLINE static void load(std::array<float, 16> array) noexcept {
            load(array.data());
        }

        //! C++ wrapper around shz_xmtrx_load_transpose_4x4().
        SHZ_FORCE_INLINE static void load_transpose(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_load_transpose_4x4(&mat4);
        }

        //! C++ wrapper around shz_xmtrx_load_transpose_unaligned_4x4().
        SHZ_FORCE_INLINE static void load_transpose(const float cArray[16]) noexcept {
            shz_xmtrx_load_transpose_unaligned_4x4(cArray);
        }

        //! C++ wrapper around shz_xmtrx_load_transpose_unaligned_4x4().
        SHZ_FORCE_INLINE static void load_transpose(std::array<float, 16> array) noexcept {
            load_transpose(array.data());
        }

        //! C++ wrapper around shz_xmtrx_load_wxyz_4x4().
        SHZ_FORCE_INLINE static void load_wxyz(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_load_wxyz_4x4(&mat4);
        }

        //! C++ wrapper around shz_xmtrx_load_cols_4x4().
        SHZ_FORCE_INLINE static void load_cols(const shz_vec4_t& c1,
                                               const shz_vec4_t& c2,
                                               const shz_vec4_t& c3,
                                               const shz_vec4_t& c4) noexcept {
            shz_xmtrx_load_cols_4x4(&c1, &c2, &c3, &c4);
        }

        //! C++ wrapper around shz_xmtrx_load_rows_4x4().
        SHZ_FORCE_INLINE static void load_rows(const shz_vec4_t& r1,
                                               const shz_vec4_t& r2,
                                               const shz_vec4_t& r3,
                                               const shz_vec4_t& r4) noexcept {
            shz_xmtrx_load_rows_4x4(&r1, &r2, &r3, &r4);
        }

    //! @}

    /*! \name  Storing
        \brief Routines for saving XMTRX contents to memory.
        @{
    */

        //! C++ wrapper around shz_xmtrx_store_4x4().
        SHZ_FORCE_INLINE static void store(shz_mat4x4_t* mat) noexcept {
            shz_xmtrx_store_4x4(mat);
        }

        //! C++ wrapper around shz_xmtrx_store_unaligned_4x4().
        SHZ_FORCE_INLINE static void store(float cArray[16]) noexcept {
            shz_xmtrx_store_unaligned_4x4(cArray);
        }

        //! C++ wrapper around shz_xmtrx_store_unaligned_4x4().
        SHZ_FORCE_INLINE static void store(std::array<float, 16> array) noexcept {
            shz_xmtrx_store_unaligned_4x4(array.data());
        }

        //! C++ wrapper around shz_xmtrx_store_transpose_4x4().
        SHZ_FORCE_INLINE static void store_transpose(shz_mat4x4_t* mat) noexcept {
            shz_xmtrx_store_transpose_4x4(mat);
        }

        //! C++ wrapper around shz_xmtrx_store_transpose_unaligned_4x4().
        SHZ_FORCE_INLINE static void store_transpose(float cArray[16]) noexcept {
            shz_xmtrx_store_transpose_4x4(reinterpret_cast<shz_mat4x4_t*>(cArray));
        }

        //! C++ wrapper around shz_xmtrx_store_transpose_unaligned_4x4().
        SHZ_FORCE_INLINE static void store_transpose(std::array<float, 16> array) noexcept {
            store_transpose(array.data());
        }

    //! @}

    /*! \name  Initialization
        \brief Routines used to initialize the entirety of XMTRX.
        @{
    */

        //! C++ wrapper around shz_xmtrx_init_identity().
        SHZ_FORCE_INLINE static void init_identity() noexcept {
            shz_xmtrx_init_identity();
        }

        //! C++ wrapper around shz_xmtrx_init_translation().
        SHZ_FORCE_INLINE static void init_translation(float x, float y, float z) noexcept {
            shz_xmtrx_init_translation(x, y, z);
        }

        //! C++ wrapper around shz_xmtrx_init_scale().
        SHZ_FORCE_INLINE static void init_scale(float x, float y, float z) noexcept {
            shz_xmtrx_init_scale(x, y, z);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation_x().
        SHZ_FORCE_INLINE static void init_rotation_x(float x) noexcept {
            shz_xmtrx_init_rotation_x(x);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation_y().
        SHZ_FORCE_INLINE static void init_rotation_y(float y) noexcept {
            shz_xmtrx_init_rotation_y(y);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation_z().
        SHZ_FORCE_INLINE static void init_rotation_z(float z) noexcept {
            shz_xmtrx_init_rotation_z(z);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation_xyz().
        SHZ_FORCE_INLINE static void init_rotation_xyz(float x, float y, float z) noexcept {
            shz_xmtrx_init_rotation_xyz(x, y, z);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation_zyx().
        SHZ_FORCE_INLINE static void init_rotation_zyx(float z, float y, float x) noexcept {
            shz_xmtrx_init_rotation_zyx(z, y, x);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation_yxz().
        SHZ_FORCE_INLINE static void init_rotation_yxz(float y, float x, float z) noexcept {
            shz_xmtrx_init_rotation_yxz(y, x, z);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation().
        SHZ_FORCE_INLINE static void init_rotation(shz_vec3_t axis, float angle) noexcept {
            shz_xmtrx_init_rotation(axis, angle);
        }

        //! C++ wrapper around shz_xmtrx_init_diagonal().
        SHZ_FORCE_INLINE static void init_diagonal(float x, float y, float z, float w) noexcept {
            shz_xmtrx_init_diagonal(x, y, z, w);
        }

        //! C++ wrapper around shz_xmtrx_init_symmetric_skew().
        SHZ_FORCE_INLINE static void init_symmetric_skew(float x, float y, float z) noexcept {
            shz_xmtrx_init_symmetric_skew(x, y, z);
        }

    //! @}

    /*! \name  Apply Operation
        \brief Updates only relevant values of XMTRX based on the given transform.
        @{
    */

        //! C++ wrapper around shz_xmtrx_apply_4x4().
        SHZ_FORCE_INLINE static void apply(const shz_mat4x4_t& mat4) noexcept {
            shz_xmtrx_apply_4x4(&mat4);
        }

        //! C++ wrapper around shz_xmtrx_apply_unaligned_4x4().
        SHZ_FORCE_INLINE static void apply(const float cArray[16]) noexcept {
            shz_xmtrx_apply_unaligned_4x4(cArray);
        }

        //! C++ wrapper around shz_xmtrx_apply_unaligned_4x4().
        SHZ_FORCE_INLINE static void apply(const std::array<float, 16> array) noexcept {
            shz_xmtrx_apply_unaligned_4x4(array.data());
        }

        //! C++ wrapper around shz_xmtrx_apply_translation().
        SHZ_FORCE_INLINE static void apply_translation(float x, float y, float z) noexcept {
            shz_xmtrx_apply_translation(x, y, z);
        }

        //! C++ wrapper around shz_xmtrx_apply_scale().
        SHZ_FORCE_INLINE static void apply_scale(float x, float y, float z) noexcept {
            shz_xmtrx_apply_scale(x, y, z);
        }

        //! C++ wrapper around shz_xmtrx_apply_rotation_x().
        SHZ_FORCE_INLINE static void apply_rotation_x(float x) noexcept {
            shz_xmtrx_apply_rotation_x(x);
        }

        //! C++ wrapper around shz_xmtrx_apply_rotation_y().
        SHZ_FORCE_INLINE static void apply_rotation_y(float y) noexcept {
            shz_xmtrx_apply_rotation_y(y);
        }

        //! C++ wrapper around shz_xmtrx_apply_rotation_z().
        SHZ_FORCE_INLINE static void apply_rotation_z(float z) noexcept {
            shz_xmtrx_apply_rotation_z(z);
        }

        // C++ wrapper around shz_xmtrx_apply_rotation().
        SHZ_FORCE_INLINE static void apply_rotation(shz_vec3_t axis, float angle) noexcept {
            shz_xmtrx_init_rotation(axis, angle);
        }

        //! C++ wrapper around shz_xmtrx_apply_rotation_quat().
        SHZ_FORCE_INLINE static void apply_rotation_quat(shz_quat_t quat) noexcept {
            shz_xmtrx_apply_rotation_quat(quat);
        }

        //! C++ wrapper around shz_xmtrx_init_rotation_xyz().
        SHZ_FORCE_INLINE static void apply_rotation_xyz(float x, float y, float z) noexcept {
            shz_xmtrx_init_rotation_xyz(x, y, z);
        }

        //! C++ wrapper around shz_xmtrx_apply_rotation_zyx().
        SHZ_FORCE_INLINE static void apply_rotation_zyx(float z, float y, float x) noexcept {
            shz_xmtrx_apply_rotation_zyx(z, y, x);
        }

        //! C++ wrapper around shz_xmtrx_apply_rotation_yxz().
        SHZ_FORCE_INLINE static void apply_rotation_yxz(float y, float x, float z) noexcept {
            shz_xmtrx_apply_rotation_yxz(y, x, z);
        }

        //! C++ wrapper around shz_xmtrx_apply_symmetric_skew().
        SHZ_FORCE_INLINE static void apply_symmetric_skew(float x, float y, float z) noexcept {
            shz_xmtrx_apply_symmetric_skew(x, y, z);
        }

    //! @}

    /*! \name  Compound Operations
        \brief Multiple operations combined into one pipelined transaction. 
        @{
    */

        //! C++ wrapper around shz_xmtrx_load_apply_4x4().
        SHZ_FORCE_INLINE static void load_apply(const shz_mat4x4_t &mat1, const shz_mat4x4_t &mat2) {
            shz_xmtrx_load_apply_4x4(&mat1, &mat2);
        }

        //! C++ wrapper around shz_xmtrx_load_apply_store_4x4().
        SHZ_FORCE_INLINE static void load_apply_store(shz_mat4x4_t* dst, const shz_mat4x4_t &mat1, const shz_mat4x4_t &mat2) {
            shz_xmtrx_load_apply_store_4x4(dst, &mat1, &mat2);
        }

    //! @}

    /*! \name  Vector Transformations
        \brief Transforming vectors against XMTRX.
        @{
    */

        //! C++ wrapper around shz_xmtrx_transform_vec4().
        SHZ_FORCE_INLINE static vec4 transform(shz_vec4_t in) noexcept {
            return shz_xmtrx_transform_vec4(in);
        }

        //! C++ wrapper around shz_xmtrx_transform_vec3().
        SHZ_FORCE_INLINE static vec3 transform(shz_vec3_t in) noexcept {
            return shz_xmtrx_transform_vec3(in);
        }

        //! C++ wrapper around shz_xmtrx_transform_vec2().
        SHZ_FORCE_INLINE static vec2 transform(shz_vec2_t in) noexcept {
            return shz_xmtrx_transform_vec2(in);
        }

    //! @}

    /*! \name  Setters
        \brief Sets the values of related XMTRX components.
        @{
    */

        //! C++ wrapper around shz_xmtrx_set_translation().
        SHZ_FORCE_INLINE static void set_translation(float x, float y, float z) noexcept {
            shz_xmtrx_set_translation(x, y, z);
        }

    //! @}

    /*! \name  Modifiers
        \brief Specialized routines that modify only the values of related XMTRX components.
        @{
    */

        //! C++ wrapper around shz_xmtrx_add_diagonal().
        SHZ_FORCE_INLINE static void add_diagonal(float x, float y, float z, float w) noexcept {
            shz_xmtrx_add_diagonal(x, y, z, w);
        }

        //! C++ wrapper around shz_xmtrx_add_symmetric_skew().
        SHZ_FORCE_INLINE static void add_symmetric_skew(float x, float y, float z) noexcept {
            shz_xmtrx_add_symmetric_skew(x, y, z);
        }

    //! @}

    /*! \name  Miscellaneous
        \brief Random operations and conversions on XMTRX.
        @{
    */

        //! C++ wrapper around shz_xmtrx_transpose().
        SHZ_FORCE_INLINE static void transpose() noexcept {
            shz_xmtrx_transpose();
        }

        //! C++ wrapper around shz_xmtrx_to_quat().
        SHZ_FORCE_INLINE static quat to_quat() noexcept {
            return shz_xmtrx_to_quat();
        }

        //! C++ wrapper around shz_xmtrx_determinant().
        SHZ_FORCE_INLINE static float determinant() noexcept {
            return shz_xmtrx_determinant();
        }

    //! @}

    };
}

#endif
