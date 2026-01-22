/*! \file
    \brief Routines for operating upon quaternions.
    \ingroup quat

    This file contains the public type(s) and interface providing the
    quaternion math API.

    \todo
        - shz_quat_rotate_towards()
        - shz_quat_to_angles_xyz()
        - shz_quat_angle_between()

    \author 2025 Falco Girgis
    \author 2025 Oleg Endo

    \copyright MIT License
*/

#ifndef SHZ_QUAT_H
#define SHZ_QUAT_H

#include "shz_vector.h"
#include "shz_mem.h"

/*! \defgroup quat Quaternions
    \brief    Routines for quaternion math.
*/

//! Minimum epsilon below which shz_quat_slerp() performs no interpolation.
#define SHZ_QUAT_SLERP_PHI_EPSILON FLT_EPSILON

SHZ_DECLS_BEGIN

/*! Represents a 3D quaternion.

    Quaternions are a mathematical structure representing a rotation
    about a single arbitrary axis.
*/
typedef struct shz_quat {
    union {           //!< Convenience anonymous union between named and array-indexed components.
        struct {      //!< Anonymous structure with named components.
            float w;             //!< Scalar component/real part of quaternion.
            union {   //!< Convenience anonymous union for vector components.
                struct {
                    float x;     //!< X component of axis of rotation (imaginary)
                    float y;     //!< Y component of axis of rotation (imaginary)
                    float z;     //!< Z component of axis of rotation (imaginary)
                };
                shz_vec3_t axis; //!< 3D vector representing axis of rotation
            };
        };
        float e[4];              //!< Array-indexed components.
    };
} shz_quat_t;

//! Alternate shz_quat_t C typedef for those who hate POSIX style.
typedef shz_quat_t shz_quat;

/*! \name  Initialization
    \brief Routines for creating and initializing quaternions.
    @{
*/

//! Initializes and returns a new quaternion with the given components, in WXYZ order.
SHZ_FORCE_INLINE shz_quat_t shz_quat_init(float w, float x, float y, float z) SHZ_NOEXCEPT;

//! Initializes and returns an identity quaternion.
SHZ_FORCE_INLINE shz_quat_t shz_quat_identity(void) SHZ_NOEXCEPT;

/*! Initializes and returns a quaternion with the given X-Y-Z rotations in radians.

    The rotation is applied first around the X-axis, then the Y-, then the Z-axis.

    \note
    These angles are Tait-Bryan angles representing an intrinsic rotation.

*/
SHZ_INLINE shz_quat_t shz_quat_from_angles_xyz(float xangle, float yangle, float zangle) SHZ_NOEXCEPT;

//! Initializes and returns a quaternion representing a rotation of \p angle radians about the given \p axis.
SHZ_INLINE shz_quat_t shz_quat_from_axis_angle(shz_vec3_t axis, float angle) SHZ_NOEXCEPT;

//! Creates a quaternion looking in the given direction with the given reference direction.
shz_quat_t shz_quat_from_look_axis(shz_vec3_t forward, shz_vec3_t up) SHZ_NOEXCEPT;

//! Returns the quaternion representing the rotation from axis \p v1 to \p v2.
SHZ_INLINE shz_quat_t shz_quat_from_rotated_axis(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT;

//! Returns the quaternion that is linearly interpolating \p a to \p b, by a t factor of `0.0f-1.0f`.
SHZ_INLINE shz_quat_t shz_quat_lerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT;

//! Equivalent to shz_quat_lerp(), except that the resulting quaternion is normalized.
SHZ_FORCE_INLINE shz_quat_t shz_quat_nlerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT;

/*! Returns the quaternion that is spherically linearly interpolating \p a to \p b, by a \p t factor of `0.0f-1.0f`.

    \warning The returned quaternion is not guaranteed to be normalized due to a floating-point error.
             Callers should normalize the result before reuse, especially when performing repeated interpolations.
*/
SHZ_INLINE shz_quat_t shz_quat_slerp(shz_quat_t q, shz_quat_t p, float t) SHZ_NOEXCEPT;

//! @}

/*! \name  Properties
    \brief Routines returning derived values from a quaternion.
    @{
*/

//! Returns the angle of rotation from the given quaternion.
SHZ_INLINE float shz_quat_angle(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns the axis of rotation from the given quaternion.
SHZ_INLINE shz_vec3_t shz_quat_axis(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns both the axis and angle of rotation from the given quaternion.
SHZ_INLINE void shz_quat_axis_angle(shz_quat_t q, shz_vec3_t* vec, float* angle) SHZ_NOEXCEPT;

//! Returns the squared magnitude of the given quaternion.
SHZ_FORCE_INLINE float shz_quat_magnitude_sqr(shz_quat_t quat) SHZ_NOEXCEPT;

//! Returns the magnitude of the given quaternion.
SHZ_FORCE_INLINE float shz_quat_magnitude(shz_quat_t quat) SHZ_NOEXCEPT;

//! Returns the inverse magnitude of the given quaternion.
SHZ_FORCE_INLINE float shz_quat_magnitude_inv(shz_quat_t quat) SHZ_NOEXCEPT;

//! @}

/*! \name  Modifiers
    \brief Routines for returning new quaternions derived from existing ones.
    @{
*/

//! Returns the normalized form of the given quaternion.
SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize(shz_quat_t quat) SHZ_NOEXCEPT;

/*! SAFELY returns the normalized form of the given quaternion.

    Returns the identity quaternion when magnitude is 0.0f.
*/
SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize_safe(shz_quat_t quat) SHZ_NOEXCEPT;

//! Returns the conjugate of the given quaternion.
SHZ_FORCE_INLINE shz_quat_t shz_quat_conjugate(shz_quat_t quat) SHZ_NOEXCEPT;

//! Returns the inverse of the given quaternion
SHZ_FORCE_INLINE shz_quat_t shz_quat_inverse(shz_quat_t quat) SHZ_NOEXCEPT;

//! @}

/*! \name  Arithmetic
    \brief Routines performing calculations with quaternions.
    @{
*/

//! Returns the quaternion produced from adding each component of the given quaternions.
SHZ_FORCE_INLINE shz_quat_t shz_quat_add(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT;

//! Scales the components of the given quaternion by the given factor.
SHZ_FORCE_INLINE shz_quat_t shz_quat_scale(shz_quat_t q, float f) SHZ_NOEXCEPT;

//! Multiplies the two quaternions, returning the result as a new quaternion.
SHZ_INLINE shz_quat_t shz_quat_mult(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT;

//! Returns the dot product of the two quaternions.
SHZ_FORCE_INLINE float shz_quat_dot(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT;

//! Returns the two dot products taken between the \p l quaternion and the \p r1 and \p r2 quaternions.
SHZ_FORCE_INLINE shz_vec2_t shz_quat_dot2(shz_quat_t l, shz_quat_t r1, shz_quat_t r2) SHZ_NOEXCEPT;

//! Returns the two dot products taken between the \p l quaternion and the \p r1, \p r2, and \p r3 quaternions.
SHZ_FORCE_INLINE shz_vec3_t shz_quat_dot3(shz_quat_t l, shz_quat_t r1, shz_quat_t r2, shz_quat_t r3) SHZ_NOEXCEPT;

//! @}

/*! Rotates a vector by the given quaternion.

    \note
    This routine is meant for one-off rotations of a single vector by a single
    quaternion. If you plan to rotate multiple vectors by the same quaternion,
    you should use the XMTRX API to apply the quaternion as a rotation matrix.

    \sa shz_xmtrx_apply_rotation_quat()
*/
SHZ_INLINE shz_vec3_t shz_quat_transform_vec3(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT;

#include "inline/shz_quat.inl.h"

SHZ_DECLS_END

#endif // SHZ_QUAT_H
