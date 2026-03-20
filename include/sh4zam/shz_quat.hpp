/*! \file
    \brief   C++ routines for operating upon quaternions.
    \ingroup quat

    \todo
        - overload arithmetic operators

    \author    2025, 2026 Falco Girgis
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
    The SH4ZAM internal quaternion representation puts the W or angle component
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

        /*! \name  Initialization
            \brief Routines for creating and initializing quaternions.
            @{
        */

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
        
        //! C++ convenience wrapper for shz_quat_from_angles_xyz().
        SHZ_FORCE_INLINE static quat from_angles_xyz(float x, float y, float z) noexcept {
            return shz_quat_from_angles_xyz(x, y, z);
        }

        //! Initializes a quaternion which is a rotation in \p angle radians about the given \p axis.
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

        //! Returns the quaternion that is spherically linearly interpolating \p q to \p p, by a \p t factor of `0.0f-1.0f`.
        SHZ_FORCE_INLINE static quat slerp(quat q, quat p, float t) noexcept {
            return shz_quat_slerp(q, p, t);
        }

        //! Evaluates the smooth cubic spherical interpolation (SQUAD) at parameter \p t.
        SHZ_FORCE_INLINE static quat squad(quat q1, quat q2, quat s1, quat s2, float t) noexcept {
            return shz_quat_squad(q1, q2, s1, s2, t);
        }

        //! @}

#ifdef SHZ_CPP23

        /*! \name  Component Accessors
            \brief Routines for iterating and accessing components.
            @{
        */

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

        //! @}

        /*! \name  Relational Operators
            \brief Overloaded comparison operators
            @{
        */

        //! Overloaded space-ship operator for auto-generating lexicographical comparison operators.
        friend auto operator<=>(quat lhs, quat rhs) noexcept {
            return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

        //! Returns true if \p lhs is lexicographically less than \p rhs.
        friend auto operator<(quat lhs, quat rhs) noexcept {
            return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
        }
#endif
        //! Overloaded comparison operator, checks for quaternion equality.
        friend auto operator==(quat lhs, quat rhs) noexcept {
            return shz_quat_equal(lhs, rhs);
        }

        //! @}

        /*! \name  Properties
            \brief Routines for accessing or extracting values.
            @{
        */

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

        //! @}

        /*! \name  Conversions
            \brief Routines for converting to other types.
            @{
        */

        //! Returns the tait-bryan rotation angles about the X, Y, then Z axes which are represented by the given quaternion.
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

        //! @}

        /*! \name  Modifiers
            \brief Routines for applying modifiers to an existing quaternion.
            @{
        */

        //! Returns the given quaternion as a unit quaternion.
        SHZ_FORCE_INLINE quat normalized() const noexcept {
            return shz_quat_normalize(*this);
        }

        //! Normalizes the given quaternion.
        SHZ_FORCE_INLINE void normalize() noexcept {
            *this = normalized();
        }

        //! Returns a safely normalized quaternion from the given quaternion, protecting against division-by-zero.
        SHZ_FORCE_INLINE quat normalized_safe() const noexcept {
            return shz_quat_normalize_safe(*this);
        }

        //! Safely normalizes the given quaternion by protecting against division-by-zero.
        SHZ_FORCE_INLINE void normalize_safe() noexcept {
            *this = normalized_safe();
        }

        //! Returns a quaternion that is the conjugate of the given quaternion.
        SHZ_FORCE_INLINE quat conjugated() const noexcept {
            return shz_quat_conjugate(*this);
        }

        //! Conjugates the given quaternion.
        SHZ_FORCE_INLINE void conjugate() noexcept {
            *this = conjugated();
        }

        //! Returns the inverse of the given quaternion.
        SHZ_FORCE_INLINE quat inverse() const noexcept {
            return shz_quat_inv(*this);
        }

        //! Inverts the given quaternion.
        SHZ_FORCE_INLINE void invert() noexcept {
            *this = inverse();
        }

        //! Returns a new quaternion whose components are the negated values of the given quaternion.
        SHZ_FORCE_INLINE quat negated() const noexcept {
            return shz_quat_neg(*this);
        }

        //! Negates the components of the given quaternion.
        SHZ_FORCE_INLINE void negate() noexcept {
            *this = negated();
        }

        //! @}

        /*! \name  Arithmetic
            \brief Routines performing calculations with quaternions.
            @{
        */

        //! Returns a new quaternion from adding the given quaterion to \p rhs.
        SHZ_FORCE_INLINE quat add(quat rhs) const noexcept {
            return shz_quat_add(*this, rhs);
        }

        //! Returns a new quaternion from adding \p rhs from the given quaternion.
        SHZ_FORCE_INLINE quat sub(quat rhs) const noexcept {
            return shz_quat_sub(*this, rhs);
        }

        //! Returns a new quaternion from scaling the given quaterion by \p s.
        SHZ_FORCE_INLINE quat scaled(float s) const noexcept {
            return shz_quat_scale(*this, s);
        }

        //! Multiplies each component of the given quaternion by \p s.
        SHZ_FORCE_INLINE void scale(float s) noexcept {
            *this = scaled(s);
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

        //! Returns a new quaterion from dividing the given quaternion by another (or multiplying the given quaternion by the inverse of another).
        SHZ_FORCE_INLINE quat div(quat rhs) const noexcept {
            return shz_quat_div(*this, rhs);
        }

        //! @}

        /*! \name  Miscellaneous
            \brief Other types of quaternion operations.
            @{
        */

        //! Returns the angle in radians between the rotations represented by quaternions \p q and \p p.
        SHZ_FORCE_INLINE float angle_between(quat p) const noexcept {
            return shz_quat_angle_between(*this, p);
        }

        //! Rotates quaternion \p from towards quaternion \p to by at most \p max_angle radians.
        SHZ_FORCE_INLINE quat rotate_towards(shz_quat_t to, float max_angle) const noexcept {
            return shz_quat_rotate_towards(*this, to, max_angle);
        }

        //! @}

        /*! \name  Transformations
            \brief Routines for applying quaternion transforms.
            @{
        */

        //! Returns a new shz::vec3 from transforming \p in by the given quaternion.
        SHZ_FORCE_INLINE vec3 transform(vec3 in) const noexcept {
            return shz_quat_transform_vec3(*this, in);
        }

        //! @}

        //! Overloaded unary negation operator, returns the negation of the given quaternion.
        SHZ_FORCE_INLINE quat operator-() const noexcept {
            return negated();
        }

        //! Adds and accumulates \p rhs onto the given quaternion.
        SHZ_FORCE_INLINE quat operator+=(quat rhs) noexcept {
            return *this = add(rhs);
        }

        //! Subtracts \p rhs from the given quaternion.
        SHZ_FORCE_INLINE quat operator-=(quat rhs) noexcept {
            return *this = sub(rhs);
        }

        //! Multiplies and accumulates \p rhs into the given quaternion.
        SHZ_FORCE_INLINE quat operator*=(quat rhs) noexcept {
            return *this = mult(rhs);
        }

        //! Divides the given quaternion by \p rhs.
        SHZ_FORCE_INLINE quat operator/=(quat rhs) noexcept {
            return *this = div(rhs);
        }

        //! Multiplies and accumulates each component of the given quaternion by \p rhs.
        SHZ_FORCE_INLINE quat operator*=(float rhs) noexcept {
            scale(rhs);
            return *this;
        }

        //! Divides each component of the given quaternion by \p rhs.
        SHZ_FORCE_INLINE quat operator/=(float rhs) noexcept {
            scale(shz_invf(rhs));
            return *this;
        }
    };

    //! Alternate C++ alias for quat, for those who like POSIX style.
    using quat_t = quat;

    //! Overloaded operator for adding two quaternions and returning the result.
    SHZ_FORCE_INLINE quat operator+(quat lhs, quat rhs) noexcept {
        return lhs.add(rhs);
    }

    //! Overloaded operator for subtracting two quaternions and returning the result.
    SHZ_FORCE_INLINE quat operator-(quat lhs, quat rhs) noexcept {
        return lhs.sub(rhs);
    }

    //! Overloaded operator for multiplying two quaternions and returning the result.
    SHZ_FORCE_INLINE quat operator*(quat lhs, quat rhs) noexcept {
        return lhs.mult(rhs);
    }

    //! Overloaded operator for dividing \p lhs by \p rhs (or multiplying by the reciprocal of \p rhs) and returning the result.
    SHZ_FORCE_INLINE quat operator/(quat lhs, quat rhs) noexcept {
        return lhs.div(rhs);
    }

    //! Overloaded operator for scaling each component of \p lhs by \p rhs and returning the result.
    SHZ_FORCE_INLINE quat operator*(quat lhs, float rhs) noexcept {
        return lhs.scaled(rhs);
    }

    //! Overloaded operator for scaling each component of \p rhs by \p lhs and returning the result.
    SHZ_FORCE_INLINE quat operator*(float lhs, quat rhs) noexcept {
        return rhs.scaled(lhs);
    }

    //! Overloaded operator for dividing each element of \p lhs by \p rhs and returning the result.
    SHZ_FORCE_INLINE quat operator/(quat lhs, float rhs) noexcept {
        return lhs.scaled(shz_invf(rhs));
    }

    //! Overloaded operator for dividing each component of \p rhs by \p lhs.
    SHZ_FORCE_INLINE quat operator/(float lhs, quat rhs) noexcept {
        return rhs.scaled(shz_invf(lhs));
    }

    //! Overloaded operator for transforming/rotating a vec3, \p rhs, by a quaternion, \p lhs.
    SHZ_FORCE_INLINE vec3 operator*(quat lhs, vec3 rhs) noexcept {
        return lhs.transform(rhs);
    }
}

#endif
