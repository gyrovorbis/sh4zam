/*! \file
    \brief   C++ routines for operating upon quaternions.
    \ingroup quat

    \todo
        - overload arithmetic operators

    \author    2025 Falco Girgis
    \copyright MIT License
*/

#ifndef SHZ_QUAT_HPP
#define SHZ_QUAT_HPP

#include <compare>
#include <tuple>

#include "shz_quat.h"
#include "shz_vector.hpp"

namespace shz {

    /*! C++ structure representing a quaternion.

    A quaternion represents a rotation about an arbitrary axis in 3D space.

    \warning
    The SH4ZAM internal quaternion representatino puts the W or angle component
    first, followed by the X, Y, Z components for the axis!

    \note
    shz::quat is the C++ extension of shz_quat_t, which adds member functions,
    convenience operators, and still retains backwards compatibility with the
    C API.

    \sa shz_quat_t, shz::mat4x4, shz::vec3
    */
    struct quat: public shz_quat_t {
        //! Minimum epsilon for bothering to interpolate in shz::quat::slerp().
        constexpr static float slerp_phi_epsilon = SHZ_QUAT_SLERP_PHI_EPSILON;

        //! Default constructor: does nothing.
        quat() noexcept = default;

        //! Value constructor: initializes a quaternion with the given values for each component.
        SHZ_FORCE_INLINE quat(float w, float x, float y, float z) noexcept:
            shz_quat_t({w, x, y, z}) {}

        //! C Converting constructor: constructs a C++ shz::quat from a C shz_quat_t.
        SHZ_FORCE_INLINE quat(shz_quat_t q) noexcept:
            shz_quat_t(q) {}

        //! Returns an identity quaternion.
        SHZ_FORCE_INLINE static quat identity() noexcept {
            return shz_quat_identity();
        }
        
        //! C++ convenience wrapper for shz_quat_From_angles_xyz().
        SHZ_FORCE_INLINE static quat from_angles_xyz(float x, float y, float z) noexcept {
            return shz_quat_from_angles_xyz(x, y, z);
        }

        //! initializes a quaternion which is a rotation in \p angle degrees about the given \p axis.
        SHZ_FORCE_INLINE static quat from_axis_angle(vec3 axis, float angle) noexcept {
            return shz_quat_from_axis_angle(axis, angle);
        }

        //! Creates a quaternion looking in the given direction with the given reference direction.
        SHZ_FORCE_INLINE static quat from_look_axis(vec3 forward, vec3 up) noexcept {
            return shz_quat_from_look_axis(forward, up);
        }

        //! Returns the quaternion representing the rotation from the \p start to the \p end axes.
        SHZ_FORCE_INLINE static quat from_rotated_axis(vec3 start, vec3 end) noexcept {
            return shz_quat_from_rotated_axis(start, end);
        }

        //! Returns the quaternion that is linearly interpolating \p q to \p p, by a t factor of `0.0f-1.0f`.
        SHZ_FORCE_INLINE static quat lerp(quat q, quat p, float t) noexcept {
            return shz_quat_lerp(q, p, t);
        }

        //! Equivalent to lerp(), except that the resulting quaternion is normalized.
        SHZ_FORCE_INLINE static quat nlerp(quat q, quat p, float t) noexcept {
            return shz_quat_nlerp(q, p, t);
        }

        /*! Returns the quaternion that is spherically linearly interpolating \p q to \p p, by a \p t factor of `0.0f-1.0f`.

            \warning The returned quaternion is not guaranteed to be normalized due to floating-point error.
            Callers should normalize the result before reuse, especially when performing repeated interpolations.
        */
        SHZ_FORCE_INLINE static quat slerp(quat q, quat p, float t) noexcept {
            return shz_quat_slerp(q, p, t);
        }

#ifdef SHZ_CPP23
        //! Overloaded subscript operator for indexing into the quaternion like an array.
        SHZ_FORCE_INLINE auto&& operator[](this auto&& self, size_t index) noexcept {
            return std::forward<decltype(self)>(self).e[index];
        }

        //! Returns an iterator to the first element within the quaternion -- For STL support.
        SHZ_FORCE_INLINE auto begin(this auto&& self) noexcept {
            return &self[0];
        }

        //! Returns an iterator to the end of the quaternion -- For STL support.
        SHZ_FORCE_INLINE auto end(this auto&& self) noexcept {
            return &self[4];
        }

        //! Overloaded space-ship operator for auto-generating lexicographical comparison operators.
        friend auto operator<=>(quat lhs, quat rhs) noexcept {
            return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
        }

        //! Returns true if \p lhs is lexicographically less than \p rhs.
        friend auto operator<(quat lhs, quat rhs) noexcept {
            return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }
#endif
        //! Overloaded comparison operator, checks for quaternion equality.
        friend auto operator==(quat lhs, quat rhs) noexcept {
            return shz_quat_equal(lhs, rhs);
        }

        //! Returns the angle of rotation represented by the given quaternion.
        SHZ_FORCE_INLINE float angle() const noexcept {
            return shz_quat_angle(*this);
        }

        //! Returns the axis of rotation represented by the given quaternion.
        SHZ_FORCE_INLINE vec3 axis() const noexcept {
            return shz_quat_axis(*this);
        }

        //! Returns the angle of rotation about the X axis represented by the given quaternion.
        SHZ_FORCE_INLINE float angle_x() const noexcept {
            return shz_quat_angle_x(*this);
        }

        //! Returns the angle of rotation about the Y axis represented by the given quaternion.
        SHZ_FORCE_INLINE float angle_y() const noexcept {
            return shz_quat_angle_y(*this);
        }

        //! Returns the angle of rotation about the Z axis represented by the given quaternion.
        SHZ_FORCE_INLINE float angle_z() const noexcept {
            return shz_quat_angle_z(*this);
        }

        SHZ_FORCE_INLINE vec3 to_angles_xyz() const noexcept {
            return shz_quat_to_angles_xyz(*this);
        }

        //! Returns both the axis and angle of rotation through the pointer arguments.
        SHZ_FORCE_INLINE void to_axis_angle(shz_vec3_t* axis, float* angle) const noexcept {
            shz_quat_to_axis_angle(*this, axis, angle);
        }

        //! Returns both the axis and angle of rotation as a std::pair.
        SHZ_FORCE_INLINE auto to_axis_angle() const noexcept -> std::pair<vec3, float> {
            std::pair<vec3, float> aa;
            shz_quat_to_axis_angle(*this, &std::get<0>(aa), &std::get<1>(aa));
            return aa;
        }

        //! Returns the magnitude of the quaternion squared.
        SHZ_FORCE_INLINE float magnitude_sqr() const noexcept {
            return shz_quat_magnitude_sqr(*this);
        }

        //! Returns the magnitude of the quaternion.
        SHZ_FORCE_INLINE float magnitude() const noexcept {
            return shz_quat_magnitude(*this);
        }

        //! Returns the inverse of the magnitude of the quaternion.
        SHZ_FORCE_INLINE float magnitude_inv() const noexcept {
            return shz_quat_magnitude_inv(*this);
        }

        //! Normalizes the given quaternion.
        SHZ_FORCE_INLINE void normalize() noexcept {
            *this = shz_quat_normalize(*this);
        }

        //! Returns the given quaternion as a unit quaternion.
        SHZ_FORCE_INLINE quat normalized() const noexcept {
            return shz_quat_normalize(*this);
        }

        //! Safely normalizes the given quaternion by protecting against division-by-zero.
        SHZ_FORCE_INLINE void normalize_safe() noexcept {
            *this = shz_quat_normalize_safe(*this);
        }

        //! Returns a safely normalized quaternion from the given quaternion, protecting against division-by-zero.
        SHZ_FORCE_INLINE quat normalized_safe() const noexcept {
            return shz_quat_normalize_safe(*this);
        }

        //! Conjugates the given quaternion.
        SHZ_FORCE_INLINE void conjugate() noexcept {
            *this = shz_quat_conjugate(*this);
        }

        //! Returns a quaternion that is the conjugate of the given quaternion.
        SHZ_FORCE_INLINE quat conjugated() const noexcept {
            return shz_quat_conjugate(*this);
        }

        //! Inverts the given quaternion.
        SHZ_FORCE_INLINE void invert() noexcept {
            *this = shz_quat_inv(*this);
        }

        //! Returns the inverse of the given quaternion.
        SHZ_FORCE_INLINE quat inverse() const noexcept {
            return shz_quat_inv(*this);
        }

        //! Returns a new quaternion from adding the given quaterion to \p rhs.
        SHZ_FORCE_INLINE quat add(quat rhs) const noexcept {
            return shz_quat_add(*this, rhs);
        }

        //! Returns a new quaternion from scaling the given quaterion by \p s.
        SHZ_FORCE_INLINE quat scale(float s) const noexcept {
            return shz_quat_scale(*this, s);
        }

        //! Returns the dot product between the given quaternion and another.
        SHZ_FORCE_INLINE float dot(quat other) const noexcept {
            return shz_quat_dot(*this, other);
        }

        //! Returns the dot product of the given quaternion against two others.
        SHZ_FORCE_INLINE vec2 dot(quat q1, quat q2) const noexcept {
            return shz_quat_dot2(*this, q1, q2);
        }

        //! Returns the dot product of the given quaternion against three others.
        SHZ_FORCE_INLINE vec3 dot(quat q1, quat q2, quat q3) const noexcept {
            return shz_quat_dot3(*this, q1, q2, q3);
        }

        //! Returns a new quaterion from multiplying the given quaternion by another.
        SHZ_FORCE_INLINE quat mult(quat rhs) const noexcept {
            return shz_quat_mult(*this, rhs);
        }

        //! Returns a new shz::vec3 from transforming \p in by the given quaternion.
        SHZ_FORCE_INLINE vec3 transform(vec3 in) const noexcept {
            return shz_quat_transform_vec3(*this, in);
        }
    };

    //! Alternate C++ alias for quat, for those who like POSIX style.
    using quat_t = quat;
}

#endif
