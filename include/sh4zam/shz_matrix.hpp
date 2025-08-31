/*! \file
    \brief   C++ routines for operating on in-memory matrices.
    \ingroup matrix

    This file provides a C++ binding layer over the C API provied
    by shz_matrix.h.

    \author    Falco Girgis
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
        mat4x4() noexcept = default;

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
            shz_mat4x4_init_rotation_xyz(this, zAngle, yAngle, xAngle);
        }

        SHZ_FORCE_INLINE void init_rotation_yxz(float yAngle, float xAngle, float zAngle) noexcept {
            shz_mat4x4_init_rotation_xyz(this, yAngle, xAngle, zAngle);
        }

        SHZ_FORCE_INLINE void init_rotation(vec3 axis, float angle) noexcept {
            shz_mat4x4_init_rotation(this, axis, angle);
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
            shz_mat4x4_apply_rotation_xyz(this, zAngle, yAngle, xAngle);
        }

        SHZ_FORCE_INLINE void apply_rotation_yxz(float yAngle, float xAngle, float zAngle) noexcept {
            shz_mat4x4_apply_rotation_xyz(this, yAngle, xAngle, zAngle);
        }

        SHZ_FORCE_INLINE void apply_rotation(vec3 axis, float angle) noexcept {
            shz_mat4x4_apply_rotation(this, axis, angle);
        }

        SHZ_FORCE_INLINE void apply_rotation(quat q) noexcept {
            shz_mat4x4_apply_rotation_quat(this, q);
        }

        SHZ_FORCE_INLINE void copy(const shz_mat4x4_t& mat) noexcept {
            shz_mat4x4_copy(this, &mat);
        }

        SHZ_FORCE_INLINE vec3 transform(vec3 in) const noexcept {
            return shz_mat4x4_trans_vec3(this, in);
        }

        SHZ_FORCE_INLINE vec4 transform(vec4 in) const noexcept {
            return shz_mat4x4_trans_vec4(this, in);
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
    };
}

#endif
