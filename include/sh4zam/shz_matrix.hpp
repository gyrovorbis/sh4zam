/*! \file
    \brief   C++ routines for operating on in-memory matrices.
    \ingroup matrix

    This file provides a C++ binding layer over the C API provied
    by shz_matrix.h.

    \author    2025 Falco Girgis
    \copyright MIT License
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
            return std::equal(lhs.begin(), lhs.end(),
                              rhs.begin(), rhs.end());
        }

        //! Overloaded "less-than" operator, for comparing vectors.
        friend constexpr auto operator<(const mat4x4& lhs, const mat4x4& rhs) noexcept {
            return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
        }

        SHZ_FORCE_INLINE static mat4x4 from_xmtrx() noexcept {
            mat4x4 mat;
            xmtrx::store(&mat);
            return mat;
        }

        SHZ_FORCE_INLINE void init_identity() noexcept {
            shz_mat4x4_init_identity(this);
        }

        SHZ_FORCE_INLINE void init_diagonal(float x, float y, float z, float w) noexcept {
            shz_mat4x4_init_diagonal(this, x, y, z, w);
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

        SHZ_FORCE_INLINE void init_translation(float x, float y, float z) noexcept {
            shz_mat4x4_init_translation(this, x, y, z);
        }

        SHZ_FORCE_INLINE void init_symmetric_skew(float x, float y, float z) noexcept {
            shz_mat4x4_init_symmetric_skew(this, x, y, z);
        }

        SHZ_FORCE_INLINE void apply(const shz_mat4x4_t& mat) noexcept {
            shz_mat4x4_apply(this, &mat);
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

        SHZ_FORCE_INLINE void copy(const shz_mat4x4_t& mat) noexcept {
            shz_mat4x4_copy(this, &mat);
        }

        SHZ_FORCE_INLINE vec3 transform(vec3 in) const noexcept {
            return shz_mat4x4_transform_vec3(this, in);
        }

        SHZ_FORCE_INLINE vec4 transform(vec4 in) const noexcept {
            return shz_mat4x4_transform_vec4(this, in);
        }

        SHZ_FORCE_INLINE vec3 transform_point(vec3 pt) const noexcept {
            return shz_mat4x4_transform_point3(this, pt);
        }

        SHZ_FORCE_INLINE quat to_quat() const noexcept {
            return shz_mat4x4_to_quat(this);
        }

        SHZ_FORCE_INLINE void set_rotation(quat q) noexcept {
            shz_mat4x4_set_rotation_quat(this, q);
        }

        SHZ_FORCE_INLINE float determinant() const noexcept {
            return shz_mat4x4_determinant(this);
        }

        SHZ_FORCE_INLINE void inverse(mat4x4* out) const noexcept {
            return shz_mat4x4_inverse(this, out);
        }
    };
}

#endif
