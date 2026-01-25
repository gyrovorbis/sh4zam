/*! \file
    \brief   C++ routines for operating on in-memory matrices.
    \ingroup matrix

    This file provides a C++ binding layer over the C API provied
    by shz_matrix.h.

    \author    2025, 2026 Falco Girgis
    \copyright MIT License

    \todo
        - Fully document
        - Operator overloading
        - full transforms (GL-style) taking a separate destination matrix?
        - Copy to unaligned dest
*/

#ifndef SHZ_MATRIX_HPP
#define SHZ_MATRIX_HPP

#include "shz_matrix.h"
#include "shz_vector.hpp"
#include "shz_quat.hpp"
#include "shz_xmtrx.hpp"

namespace shz {

    struct mat4x4: public shz_mat4x4_t {
        static constexpr size_t Rows = 4;   //!< Number of rows
        static constexpr size_t Cols = 4;   //!< Number of columns

        mat4x4() noexcept = default;

        //! Overloaded subscript operator -- allows for indexing vectors like an array.
        SHZ_FORCE_INLINE auto&& operator[](this auto&& self, size_t index) noexcept {
            return std::forward<decltype(self)>(self).elem[index];
        }

        //! Returns an iterator to the beginning of the vector -- For STL support.
        SHZ_FORCE_INLINE auto begin(this auto&& self) noexcept {
            return &self[0];
        }

        //! Returns an iterator to the end of the vector -- For STL support.
        SHZ_FORCE_INLINE auto end(this auto&& self) noexcept {
            return &self[Rows * Cols - 1];
        }

        //! Overloaded space-ship operator, for generic lexicographical comparison of vectors.
        friend constexpr auto operator<=>(const mat4x4& lhs, const mat4x4& rhs) noexcept {
            return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                          rhs.begin(), rhs.end());
        }

        //! Overloaded equality operator, for comparing vectors.
        friend constexpr auto operator==(const mat4x4& lhs, const mat4x4& rhs) noexcept {
            return lhs.col(0) == rhs.col(0) && lhs.col(1) == rhs.col(1) &&
                   lhs.col(2) == rhs.col(2) && lhs.col(3) == rhs.col(3);
        }

        //! Overloaded "less-than" operator, for comparing vectors.
        friend constexpr auto operator<(const mat4x4& lhs, const mat4x4& rhs) noexcept {
            return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
        }

        /*! \name  Initialization
            \brief Routines for fully initializing a matrix.
           @{
        */

        SHZ_FORCE_INLINE void init_identity() noexcept {
            shz_mat4x4_init_identity(this);
        }

        SHZ_FORCE_INLINE void init_identity_safe() noexcept {
            shz_mat4x4_init_identity_safe(this);
        }

        SHZ_FORCE_INLINE void init_zero() noexcept {
            shz_mat4x4_init_zero(this);
        }

        SHZ_FORCE_INLINE void init_one() noexcept {
            shz_mat4x4_init_one(this);
        }

        SHZ_FORCE_INLINE void init_fill(float value) noexcept {
            shz_mat4x4_init_fill(this, value);
        }

        SHZ_FORCE_INLINE void init_translation(float x, float y, float z) noexcept {
            shz_mat4x4_init_translation(this, x, y, z);
        }

        SHZ_FORCE_INLINE void init_scale(float x, float y, float z) noexcept {
            shz_mat4x4_init_scale(this, x, y, z);
        }

        SHZ_FORCE_INLINE void init_rotation_x(float angle) noexcept {
            shz_mat4x4_init_rotation_x(this, angle);
        }

        SHZ_FORCE_INLINE void init_rotation_y(float angle) noexcept {
            shz_mat4x4_init_rotation_y(this, angle);
        }

        SHZ_FORCE_INLINE void init_rotation_z(float angle) noexcept {
            shz_mat4x4_init_rotation_z(this, angle);
        }

        SHZ_FORCE_INLINE void init_rotation_xyz(float xAngle, float yAngle, float zAngle) noexcept {
            shz_mat4x4_init_rotation_xyz(this, xAngle, yAngle, zAngle);
        }

        SHZ_FORCE_INLINE void init_rotation_zyx(float zAngle, float yAngle, float xAngle) noexcept {
            shz_mat4x4_init_rotation_zyx(this, zAngle, yAngle, xAngle);
        }

        SHZ_FORCE_INLINE void init_rotation_zxy(float zAngle, float xAngle, float yAngle) noexcept {
            shz_mat4x4_init_rotation_zxy(this, zAngle, xAngle, yAngle);
        }

        SHZ_FORCE_INLINE void init_rotation_yxz(float yAngle, float xAngle, float zAngle) noexcept {
            shz_mat4x4_init_rotation_yxz(this, yAngle, xAngle, zAngle);
        }

        //! C++ wrapper for shz_mat4x4_init_rotation().
        SHZ_FORCE_INLINE void init_rotation(float angle, float x, float y, float z) noexcept {
            shz_mat4x4_init_rotation(this, angle, x, y, z);
        }

        SHZ_FORCE_INLINE void init_rotation(quat q) noexcept {
            shz_mat4x4_init_rotation_quat(this, q);
        }

        SHZ_FORCE_INLINE void init_diagonal(float x, float y, float z, float w) noexcept {
            shz_mat4x4_init_diagonal(this, x, y, z, w);
        }

        SHZ_FORCE_INLINE void init_upper_diagonal(float col1, vec2 col2, vec3 col3, vec4 col4) noexcept{
            shz_mat4x4_init_upper_diagonal(this, col1, col2, col3, col4);
        }

        SHZ_FORCE_INLINE void init_lower_diagonal(vec4 col1, vec3 col2, vec2 col3, float col4) noexcept {
            shz_mat4x4_init_lower_diagonal(this, col1, col2, col3, col4);
        }

        SHZ_FORCE_INLINE void init_symmetric_skew(float x, float y, float z) noexcept {
            shz_mat4x4_init_symmetric_skew(this, x, y, z);
        }

        SHZ_FORCE_INLINE void init_outer_product(vec4 v1, vec4 v2) noexcept {
            shz_mat4x4_init_outer_product(this, v1, v2);
        }

        SHZ_FORCE_INLINE void init_permutation_wxyz() noexcept {
            shz_mat4x4_init_permutation_wxyz(this);
        }

        SHZ_FORCE_INLINE void init_permutation_yzwx() noexcept {
            shz_mat4x4_init_permutation_yzwx(this);
        }

        SHZ_FORCE_INLINE void init_screen(float width, float height) noexcept {
            shz_mat4x4_init_screen(this, width, height);
        }

        //! @}

        /*! \name  Getting
            \brief Routines for getting specific values within a matrix
            @{
        */

        //! C++ wrapper for shz_mat4x4_row().
        SHZ_FORCE_INLINE vec4 row(size_t index) const noexcept {
            return shz_mat4x4_row(this, index);
        }

        //! C++ wrapper for shz_mat4x4_col().
        SHZ_FORCE_INLINE vec4 col(size_t index) const noexcept {
            return shz_mat4x4_col(this, index);
        }

        //! @}

        /*! \name  Setting
            \brief Routines for setting specific values within a matrix
            @{
        */

        SHZ_FORCE_INLINE void set_row(size_t index, vec4 values) noexcept {
            shz_mat4x4_set_row(this, index, values);
        }

        SHZ_FORCE_INLINE void set_col(size_t index, vec4 values) noexcept {
            shz_mat4x4_set_col(this, index, values);
        }

        SHZ_FORCE_INLINE void swap_rows(size_t row1, size_t row2) noexcept {
            shz_mat4x4_swap_rows(this, row1, row2);
        }

        SHZ_FORCE_INLINE void swap_cols(size_t col1, size_t col2) noexcept {
            shz_mat4x4_swap_cols(this, col1, col2);
        }

        SHZ_FORCE_INLINE void set_translation(float x, float y, float z) noexcept {
            shz_mat4x4_set_translation(this, x, y, z);
        }

        SHZ_FORCE_INLINE void set_rotation(quat rot) noexcept {
            shz_mat4x4_set_rotation_quat(this, rot);
        }

        SHZ_FORCE_INLINE void set_diagonal(float x, float y, float z, float w) noexcept {
            shz_mat4x4_set_diagonal(this, x, y, z, w);
        }

        //! @}

        /*! \name  Applying
            \brief Routines for multiplying and accumulating onto the given matrix.
            @{
        */

        SHZ_FORCE_INLINE void apply(const shz_mat4x4_t& mat) noexcept {
            shz_mat4x4_apply(this, &mat);
        }

        SHZ_FORCE_INLINE void apply(const float mat[16]) noexcept {
            shz_mat4x4_apply_unaligned(this, mat);
        }

        SHZ_FORCE_INLINE void apply_transpose(const shz_mat4x4_t& mat) noexcept {
            shz_mat4x4_apply_transpose(this, &mat);
        }

        SHZ_FORCE_INLINE void apply_transpose(const float mat[16]) noexcept {
            shz_mat4x4_apply_transpose_unaligned(this, mat);
        }

        SHZ_FORCE_INLINE void apply_scale(float x, float y, float z) noexcept {
            shz_mat4x4_apply_scale(this, x, y, z);
        }

        SHZ_FORCE_INLINE void apply_translation(float x, float y, float z) noexcept {
            shz_mat4x4_apply_translation(this, x, y, z);
        }

        SHZ_FORCE_INLINE void apply_rotation_x(float angle) noexcept {
            shz_mat4x4_apply_rotation_x(this, angle);
        }

        SHZ_FORCE_INLINE void apply_rotation_y(float angle) noexcept {
            shz_mat4x4_apply_rotation_y(this, angle);
        }

        SHZ_FORCE_INLINE void apply_rotation_z(float angle) noexcept {
            shz_mat4x4_apply_rotation_z(this, angle);
        }

        SHZ_FORCE_INLINE void apply_rotation_xyz(float xAngle, float yAngle, float zAngle) noexcept {
            shz_mat4x4_apply_rotation_xyz(this, xAngle, yAngle, zAngle);
        }

        SHZ_FORCE_INLINE void apply_rotation_zyx(float zAngle, float yAngle, float xAngle) noexcept {
            shz_mat4x4_apply_rotation_zyx(this, zAngle, yAngle, xAngle);
        }

        SHZ_FORCE_INLINE void apply_rotation_zxy(float zAngle, float xAngle, float yAngle) noexcept {
            shz_mat4x4_apply_rotation_zxy(this, zAngle, xAngle, yAngle);
        }

        SHZ_FORCE_INLINE void apply_rotation_yxz(float yAngle, float xAngle, float zAngle) noexcept {
            shz_mat4x4_apply_rotation_yxz(this, yAngle, xAngle, zAngle);
        }

        SHZ_FORCE_INLINE void apply_rotation(float angle, float x, float y, float z) noexcept {
            shz_mat4x4_apply_rotation(this, angle, x, y, z);
        }

        SHZ_FORCE_INLINE void apply_rotation(quat q) noexcept {
            shz_mat4x4_apply_rotation_quat(this, q);
        }

        SHZ_FORCE_INLINE void apply_lookat(vec3 pos, vec3 target, vec3 up) noexcept {
            shz_mat4x4_apply_lookat(this, pos, target, up);
        }

        SHZ_FORCE_INLINE void apply_perspective(float fov, float aspect, float near_z) noexcept {
            shz_mat4x4_apply_perspective(this, fov, aspect, near_z);
        }

        SHZ_FORCE_INLINE void apply_screen(float width, float height) noexcept {
            shz_mat4x4_apply_screen(this, width, height);
        }

        SHZ_FORCE_INLINE void apply_symmetric_skew(float x, float y, float z) noexcept {
            shz_mat4x4_apply_symmetric_skew(this, x, y, z);
        }

        SHZ_FORCE_INLINE void apply_permutation_wxyz() noexcept {
            shz_mat4x4_apply_permutation_wxyz(this);
        }

        SHZ_FORCE_INLINE void apply_permutation_yzwx() noexcept {
            shz_mat4x4_apply_permutation_yzwx(this);
        }

        SHZ_FORCE_INLINE void apply_self() noexcept {
            shz_mat4x4_apply_self(this);
        }

        //! @}

        /*! \name  GL Transformations
            \brief OpenGL-style 4x4 matrix transforms.
            @{
        */

        SHZ_FORCE_INLINE void translate(float x, float y, float z) noexcept {
            shz_mat4x4_translate(this, x, y, z);
        }

        SHZ_FORCE_INLINE void scale(float x, float y, float z) noexcept {
            shz_mat4x4_scale(this, x, y, z);
        }

        SHZ_FORCE_INLINE void rotate_x(float radians) noexcept {
            shz_mat4x4_rotate_x(this, radians);
        }

        SHZ_FORCE_INLINE void rotate_y(float radians) noexcept {
            shz_mat4x4_rotate_y(this, radians);
        }

        SHZ_FORCE_INLINE void rotate_z(float radians) noexcept {
            shz_mat4x4_rotate_z(this, radians);
        }

        SHZ_FORCE_INLINE void rotate_xyz(float xRadians, float yRadians, float zRadians) noexcept {
            shz_mat4x4_rotate_xyz(this, xRadians, yRadians, zRadians);
        }

        SHZ_FORCE_INLINE void rotate_zyx(float zRadians, float yRadians, float xRadians) noexcept {
            shz_mat4x4_rotate_zyx(this, zRadians, yRadians, xRadians);
        }

        SHZ_FORCE_INLINE void rotate_zxy(float zRadians, float xRadians, float yRadians) noexcept {
            shz_mat4x4_rotate_zxy(this, zRadians, xRadians, yRadians);
        }

        SHZ_FORCE_INLINE void rotate_yxz(float yRadians, float xRadians, float zRadians) noexcept {
            shz_mat4x4_rotate_yxz(this, yRadians, xRadians, zRadians);
        }

        SHZ_FORCE_INLINE void rotate(float radians, float xAxis, float yAxis, float zAxis) noexcept {
            shz_mat4x4_rotate(this, radians, xAxis, yAxis, zAxis);
        }

        //! @}

        /*! \name  Transforming
            \brief Routines for transforming vectors and points by a matrix.
            @{
        */

        SHZ_FORCE_INLINE static void mult(mat4x4* dst, const mat4x4& lhs, const mat4x4& rhs) noexcept {
            shz_mat4x4_mult(dst, &lhs, &rhs);
        }

        SHZ_FORCE_INLINE static void mult(mat4x4* dst, const mat4x4& lhs, const float rhs[16]) noexcept {
            shz_mat4x4_mult_unaligned(dst, &lhs, rhs);
        }

        SHZ_FORCE_INLINE vec2 transform(vec2 in) const noexcept {
            return shz_mat4x4_transform_vec2(this, in);
        }

        SHZ_FORCE_INLINE vec3 transform(vec3 in) const noexcept {
            return shz_mat4x4_transform_vec3(this, in);
        }

        SHZ_FORCE_INLINE vec4 transform(vec4 in) const noexcept {
            return shz_mat4x4_transform_vec4(this, in);
        }

        SHZ_FORCE_INLINE vec2 transform_point(vec2 pt) const noexcept {
            return shz_mat4x4_transform_point2(this, pt);
        }

        SHZ_FORCE_INLINE vec3 transform_point(vec3 pt) const noexcept {
            return shz_mat4x4_transform_point3(this, pt);
        }

        SHZ_FORCE_INLINE vec2 transform_transpose(vec2 in) const noexcept {
            return shz_mat4x4_transform_vec2_transpose(this, in);
        }

        SHZ_FORCE_INLINE vec3 transform_transpose(vec3 in) const noexcept {
            return shz_mat4x4_transform_vec3_transpose(this, in);
        }

        SHZ_FORCE_INLINE vec4 transform_transpose(vec4 in) const noexcept {
            return shz_mat4x4_transform_vec4_transpose(this, in);
        }

        SHZ_FORCE_INLINE vec2 transform_point_transpose(vec2 pt) const noexcept {
            return shz_mat4x4_transform_point2_transpose(this, pt);
        }

        SHZ_FORCE_INLINE vec3 transform_point_transpose(vec3 pt) const noexcept {
            return shz_mat4x4_transform_point3_transpose(this, pt);
        }

        //! @}

        /*! \name  Miscellaneous
            \brief Other matrix-related operations and routines
            @{
        */

        SHZ_FORCE_INLINE static void copy(shz_mat4x4_t* lhs, const shz_mat4x4_t& rhs) noexcept {
            shz_mat4x4_copy(lhs, &rhs);
        }

        SHZ_FORCE_INLINE static void copy(shz_mat4x4_t* lhs, const float rhs[16]) noexcept {
            shz_mat4x4_copy_unaligned(lhs, rhs);
        }

        SHZ_FORCE_INLINE quat to_quat() const noexcept {
            return shz_mat4x4_to_quat(this);
        }

        SHZ_FORCE_INLINE float determinant() const noexcept {
            return shz_mat4x4_determinant(this);
        }

        SHZ_FORCE_INLINE void inverse(mat4x4* out) const noexcept {
            return shz_mat4x4_inverse(this, out);
        }

        //! @}
    };

    //! Alternate mat4x4 C++ alias for those who like POSIX style.
    using mat4x4_t = mat4x4;
}

#endif
