/*! \file
    \brief Routines for operating upon quaternions.
    \ingroup quat

    This file contains the public type(s) and interface providing the
    quaternion math API.

    \author 2025, 2026 Falco Girgis
    \author 2025 Oleg Endo

    \copyright MIT License

    \important Do not copy this file or its routines into other projects. Use the
               KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
*/

#ifndef SHZ_QUAT_H
#define SHZ_QUAT_H

#include "shz_vector.h"
#include "shz_mem.h"

/*! \defgroup quat Quaternions
    \brief    Routines for quaternion math.
*/

SHZ_DECLS_BEGIN

/*! Represents a 3D quaternion.

    Quaternions are a mathematical structure representing a rotation
    about a single arbitrary axis.
*/
typedef struct shz_quat {
    union {           //!< Convenience anonymous union between named and array-indexed components.
        struct {      //!< Anonymous structure with named components.
            float w;             //!< W or scalar component/real part of quaternion.
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
SHZ_INLINE shz_quat_t shz_quat_init(float w, float x, float y, float z) SHZ_NOEXCEPT;

//! Initializes and returns an identity quaternion.
SHZ_INLINE shz_quat_t shz_quat_identity(void) SHZ_NOEXCEPT;

//! Returns true if the two given quaternions are considered equal based on either absolute or relative tolerance.
SHZ_INLINE bool shz_quat_equal(shz_quat_t a, shz_quat_t b) SHZ_NOEXCEPT;

/*! Returns the quaternion of the rotation shz_xmtrx_init_rotation_xyz() builds, from angles in radians.

    The result equals the product of the axis quaternions in the order of the name, Qx * Qy * Qz,
    so it describes the same orientation as the matrix of the same name.

    \sa shz_quat_from_rotation_xyz_u16(), shz_xmtrx_init_rotation_xyz(), shz_xmtrx_init_rotation_quat()
*/
SHZ_INLINE shz_quat_t shz_quat_from_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

//! Returns the quaternion of the rotation shz_xmtrx_init_rotation_zyx() builds, Qz * Qy * Qx, from angles in radians.
SHZ_INLINE shz_quat_t shz_quat_from_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

//! Returns the quaternion of the rotation shz_xmtrx_init_rotation_zxy() builds, Qz * Qx * Qy, from angles in radians.
SHZ_INLINE shz_quat_t shz_quat_from_rotation_zxy(float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT;

//! Returns the quaternion of the rotation shz_xmtrx_init_rotation_yxz() builds, Qy * Qx * Qz, from angles in radians.
SHZ_INLINE shz_quat_t shz_quat_from_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

/*! Equivalent to shz_quat_from_rotation_xyz(), with 16-bit binary angles (65536 per turn, the FSCA unit).

    For games whose orientations are stored as 16-bit angles, such as N64 titles. Signed 16-bit
    angles can be passed as they are. The angles are converted to radians and forwarded to
    shz_quat_from_rotation_xyz(), so the half angles are quantized to FSCA's 1/65536 of a turn,
    which can be off by up to half a unit for odd angles.

    \sa shz_quat_from_rotation_xyz(), shz_sincosu16()
*/
SHZ_INLINE shz_quat_t shz_quat_from_rotation_xyz_u16(uint16_t xAngle, uint16_t yAngle, uint16_t zAngle) SHZ_NOEXCEPT;

//! Equivalent to shz_quat_from_rotation_zyx(), with 16-bit binary angles.
SHZ_INLINE shz_quat_t shz_quat_from_rotation_zyx_u16(uint16_t zAngle, uint16_t yAngle, uint16_t xAngle) SHZ_NOEXCEPT;

//! Equivalent to shz_quat_from_rotation_zxy(), with 16-bit binary angles.
SHZ_INLINE shz_quat_t shz_quat_from_rotation_zxy_u16(uint16_t zAngle, uint16_t xAngle, uint16_t yAngle) SHZ_NOEXCEPT;

//! Equivalent to shz_quat_from_rotation_yxz(), with 16-bit binary angles.
SHZ_INLINE shz_quat_t shz_quat_from_rotation_yxz_u16(uint16_t yAngle, uint16_t xAngle, uint16_t zAngle) SHZ_NOEXCEPT;

//! Initializes and returns a quaternion representing a rotation of \p angle radians about the given \p axis.
SHZ_INLINE shz_quat_t shz_quat_from_axis_angle(shz_vec3_t axis, float angle) SHZ_NOEXCEPT;

//! Creates a quaternion looking in the given direction with the given reference direction.
SHZ_INLINE shz_quat_t shz_quat_from_look_axis(shz_vec3_t forward, shz_vec3_t up) SHZ_NOEXCEPT;

//! Returns the quaternion representing the rotation from axis \p v1 to \p v2.
SHZ_INLINE shz_quat_t shz_quat_from_rotated_axis(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT;

//! Returns the quaternion that is linearly interpolating \p a to \p b, by a t factor of `0.0f-1.0f`.
SHZ_INLINE shz_quat_t shz_quat_lerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT;

//! Equivalent to shz_quat_lerp(), except that the resulting quaternion is normalized.
SHZ_INLINE shz_quat_t shz_quat_nlerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT;

//! Returns the quaternion that is spherically linearly interpolating \p a to \p b, by a \p t factor of `0.0f-1.0f`.
SHZ_INLINE shz_quat_t shz_quat_slerp(shz_quat_t q, shz_quat_t p, float t) SHZ_NOEXCEPT;

/*! Evaluates a smooth cubic spherical interpolation (SQUAD) at parameter \p t.

    \note
    SQUAD(q1, q2, s1, s2, t) = slerp(slerp(q1, q2, t), slerp(s1, s2, t), 2t(1-t))

    \param q1   Start quaternion.
    \param q2   End quaternion.
    \param s1   Inner control point for \p q1 (typically computed from surrounding keyframes).
    \param s2   Inner control point for \p q2 (typically computed from surrounding keyframes).
    \param t    Interpolation factor in [0, 1].

    \returns    Resulting interpolated quaternion
*/
SHZ_INLINE shz_quat_t shz_quat_squad(shz_quat_t q1, shz_quat_t q2, shz_quat_t s1, shz_quat_t s2, float t) SHZ_NOEXCEPT;

//! @}

/*! \name  Properties
    \brief Routines returning derived values from a quaternion.
    @{
*/

//! Returns the angle of rotation from the given quaternion.
SHZ_INLINE float shz_quat_angle(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns the axis of rotation from the given quaternion.
SHZ_INLINE shz_vec3_t shz_quat_axis(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns the angle of rotation the quaternion represents about the X axis in radians.
SHZ_INLINE float shz_quat_angle_x(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns the angle of rotation the quaternion represents about the Y axis in radians.
SHZ_INLINE float shz_quat_angle_y(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns the angle of rotation the quaternion represents about the Z axis in radians.
SHZ_INLINE float shz_quat_angle_z(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns both the axis and angle of rotation simultaneously (faster if both are needed) from the given quaternion.
SHZ_INLINE void shz_quat_to_axis_angle(shz_quat_t q, shz_vec3_t* vec, float* angle) SHZ_NOEXCEPT;

/*! Decomposes the given quaternion into X, Y, then Z Tait-Bryan angles.

    Returns the angles a, b, and c, in radians, such that the given quaternion is equal to
    Qx(a) * Qy(b) * Qz(c), making this the inverse of
    shz_quat_from_rotation_xyz(). The angles are returned in the same order as that routine
    takes them: x = X angle, y = Y angle, z = Z angle.

    The outer angles are in [-PI, PI] and the middle angle is in [-PI/2, PI/2].

    \note   At gimbal lock, when the middle angle is +/-PI/2, the outer angles are not unique.
            The returned angles still reconstruct the same rotation.

    \param  q   Unit quaternion to decompose.

    \returns    The X, Y, and Z angles of rotation, in radians.

    \sa shz_quat_from_rotation_xyz(), shz_quat_angle_x(), shz_quat_angle_y(), shz_quat_angle_z()
*/
SHZ_INLINE shz_vec3_t shz_quat_to_angles_xyz(shz_quat_t q) SHZ_NOEXCEPT;

/*! Decomposes the given quaternion into Z, Y, then X Tait-Bryan angles.

    Returns the angles a, b, and c, in radians, such that the given quaternion is equal to
    Qz(a) * Qy(b) * Qx(c), making this the inverse of
    shz_quat_from_rotation_zyx(). The angles are returned in the same order as that routine
    takes them: x = Z angle, y = Y angle, z = X angle.

    The outer angles are in [-PI, PI] and the middle angle is in [-PI/2, PI/2].

    \note   At gimbal lock, when the middle angle is +/-PI/2, the outer angles are not unique.
            The returned angles still reconstruct the same rotation.

    \param  q   Unit quaternion to decompose.

    \returns    The Z, Y, and X angles of rotation, in radians.

    \sa shz_quat_from_rotation_zyx(), shz_quat_to_angles_xyz()
*/
SHZ_INLINE shz_vec3_t shz_quat_to_angles_zyx(shz_quat_t q) SHZ_NOEXCEPT;

/*! Decomposes the given quaternion into Z, X, then Y Tait-Bryan angles.

    Returns the angles a, b, and c, in radians, such that the given quaternion is equal to
    Qz(a) * Qx(b) * Qy(c), making this the inverse of
    shz_quat_from_rotation_zxy(). The angles are returned in the same order as that routine
    takes them: x = Z angle, y = X angle, z = Y angle.

    The outer angles are in [-PI, PI] and the middle angle is in [-PI/2, PI/2].

    \note   At gimbal lock, when the middle angle is +/-PI/2, the outer angles are not unique.
            The returned angles still reconstruct the same rotation.

    \param  q   Unit quaternion to decompose.

    \returns    The Z, X, and Y angles of rotation, in radians.

    \sa shz_quat_from_rotation_zxy(), shz_quat_to_angles_yxz()
*/
SHZ_INLINE shz_vec3_t shz_quat_to_angles_zxy(shz_quat_t q) SHZ_NOEXCEPT;

/*! Decomposes the given quaternion into Y, X, then Z Tait-Bryan angles.

    Returns the angles a, b, and c, in radians, such that the given quaternion is equal to
    Qy(a) * Qx(b) * Qz(c), making this the inverse of
    shz_quat_from_rotation_yxz(). The angles are returned in the same order as that routine
    takes them: x = Y angle, y = X angle, z = Z angle.

    The outer angles are in [-PI, PI] and the middle angle is in [-PI/2, PI/2].

    \note   At gimbal lock, when the middle angle is +/-PI/2, the outer angles are not unique.
            The returned angles still reconstruct the same rotation.

    \param  q   Unit quaternion to decompose.

    \returns    The Y, X, and Z angles of rotation, in radians.

    \sa shz_quat_from_rotation_yxz(), shz_quat_to_angles_zxy()
*/
SHZ_INLINE shz_vec3_t shz_quat_to_angles_yxz(shz_quat_t q) SHZ_NOEXCEPT;

//! Returns the squared magnitude of the given quaternion.
SHZ_INLINE float shz_quat_magnitude_sqr(shz_quat_t quat) SHZ_NOEXCEPT;

/*! Returns the magnitude of the given quaternion.

    Safely returns `0.0f` when \p quat has a magnitude of `0.0f`.

    \param  quat    The quaternion whose magnitude is returned.
    \returns        The magnitude of \p quat.

    \sa shz_quat_magnitude_fsrra(), shz_quat_magnitude_sqr(), shz_quat_magnitude_inv()
*/
SHZ_INLINE float shz_quat_magnitude(shz_quat_t quat) SHZ_NOEXCEPT;

/*! Returns the magnitude of the given quaternion, unsafely, using FSRRA.

    Faster alternative to shz_quat_magnitude() which does not check for a
    zero-length quaternion.

    \warning Results in a division-by-zero when \p quat has a magnitude of `0.0f`.

    \param  quat    The quaternion whose magnitude is returned.
    \returns        The magnitude of \p quat.

    \sa shz_quat_magnitude()
*/
SHZ_INLINE float shz_quat_magnitude_fsrra(shz_quat_t quat) SHZ_NOEXCEPT;

/*! Returns the inverse magnitude of the given quaternion.

    Safely returns `0.0f` when \p quat has a magnitude of `0.0f`.

    \param  quat    The quaternion whose inverse magnitude is returned.
    \returns        `1.0f` divided by the magnitude of \p quat.

    \sa shz_quat_magnitude_inv_fsrra(), shz_quat_magnitude()
*/
SHZ_INLINE float shz_quat_magnitude_inv(shz_quat_t quat) SHZ_NOEXCEPT;

/*! Returns the inverse magnitude of the given quaternion, unsafely, using FSRRA.

    Faster alternative to shz_quat_magnitude_inv() which does not check for a
    zero-length quaternion.

    \warning Results in a division-by-zero when \p quat has a magnitude of `0.0f`.

    \param  quat    The quaternion whose inverse magnitude is returned.
    \returns        `1.0f` divided by the magnitude of \p quat.

    \sa shz_quat_magnitude_inv()
*/
SHZ_INLINE float shz_quat_magnitude_inv_fsrra(shz_quat_t quat) SHZ_NOEXCEPT;

//! @}

/*! \name  Modifiers
    \brief Routines for returning new quaternions derived from existing ones.
    @{
*/

/*! Returns the normalized form of the given quaternion.

    This is the fastest normalization routine, which does not check for a
    zero-length quaternion.

    \warning Results in a division-by-zero when \p quat has a magnitude of `0.0f`.

    \param  quat    The quaternion to normalize.
    \returns        The unit quaternion in the same direction as \p quat.

    \sa shz_quat_normalize_or(), shz_quat_normalize_safe()
*/
SHZ_INLINE shz_quat_t shz_quat_normalize(shz_quat_t quat) SHZ_NOEXCEPT;

/*! Returns the normalized form of the given quaternion, or an alternate value.

    Safely returns \p alt when \p quat has a magnitude of `0.0f` and cannot be normalized.

    \param  quat    The quaternion to normalize.
    \param  alt     The quaternion to return when \p quat cannot be normalized.
    \returns        The unit quaternion in the same direction as \p quat, or \p alt.

    \sa shz_quat_normalize(), shz_quat_normalize_safe()
*/
SHZ_INLINE shz_quat_t shz_quat_normalize_or(shz_quat_t quat, shz_quat_t alt) SHZ_NOEXCEPT;

/*! SAFELY returns the normalized form of the given quaternion.

    Safely returns the identity quaternion when \p quat has a magnitude of `0.0f`
    and cannot be normalized.

    \param  quat    The quaternion to normalize.
    \returns        The unit quaternion in the same direction as \p quat, or the identity.

    \sa shz_quat_normalize(), shz_quat_normalize_or(), shz_quat_identity()
*/
SHZ_INLINE shz_quat_t shz_quat_normalize_safe(shz_quat_t quat) SHZ_NOEXCEPT;

//! Returns the conjugate of the given quaternion.
SHZ_INLINE shz_quat_t shz_quat_conjugate(shz_quat_t quat) SHZ_NOEXCEPT;

/*! Returns the inverse of the given quaternion.

    The inverse is the conjugate of \p quat divided by its squared magnitude.

    \warning Results in a division-by-zero when \p quat has a magnitude of `0.0f`.

    \param  quat    The quaternion to invert.
    \returns        The inverse of \p quat.

    \sa shz_quat_conjugate()
*/
SHZ_INLINE shz_quat_t shz_quat_inv(shz_quat_t quat) SHZ_NOEXCEPT;

//! Returns the negation of the given quaternion.
SHZ_INLINE shz_quat_t shz_quat_neg(shz_quat_t quat) SHZ_NOEXCEPT;

//! @}

/*! \name  Arithmetic
    \brief Routines performing calculations with quaternions.
    @{
*/

//! Returns the quaternion produced from adding each component of the given quaternions.
SHZ_INLINE shz_quat_t shz_quat_add(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT;

//! Returns the quaternion produced from subtracting each component of quaternion \p p from quaterion \p q.
SHZ_INLINE shz_quat_t shz_quat_sub(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT;

//! Scales the components of the given quaternion by the given factor.
SHZ_INLINE shz_quat_t shz_quat_scale(shz_quat_t q, float f) SHZ_NOEXCEPT;

//! Multiplies the two quaternions, returning the result as a new quaternion.
SHZ_INLINE shz_quat_t shz_quat_mult(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT;

//! Divides quaternion \p p by quaternion \p q (multiplying by its inverse), returning the resulting quaternion.
SHZ_INLINE shz_quat_t shz_quat_div(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT;

//! Returns the dot product of the two quaternions.
SHZ_INLINE float shz_quat_dot(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT;

//! Returns the two dot products taken between the \p l quaternion and the \p r1 and \p r2 quaternions.
SHZ_INLINE shz_vec2_t shz_quat_dot2(shz_quat_t l, shz_quat_t r1, shz_quat_t r2) SHZ_NOEXCEPT;

//! Returns the two dot products taken between the \p l quaternion and the \p r1, \p r2, and \p r3 quaternions.
SHZ_INLINE shz_vec3_t shz_quat_dot3(shz_quat_t l, shz_quat_t r1, shz_quat_t r2, shz_quat_t r3) SHZ_NOEXCEPT;

//! @}

/*! \name  Miscellaneous
    \brief Other types of quaternion operations.
    @{
*/

//! Returns the angle in radians between the rotations represented by quaternions \p q and \p p.
SHZ_INLINE float shz_quat_angle_between(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT;

/*! Rotates quaternion \p from towards quaternion \p to by at most \p max_angle radians.

    \note Returns \p to if the angle between them is already less than \p max_angle.
*/
SHZ_INLINE shz_quat_t shz_quat_rotate_towards(shz_quat_t from, shz_quat_t to, float max_angle) SHZ_NOEXCEPT;

//! @}

/*! \name  Transformations
    \brief Routines for applying quaternion transforms.
    @{
*/

/*! Rotates a vector by the given quaternion.

    \note
    This routine is meant for one-off rotations of a single vector by a single
    quaternion. If you plan to rotate multiple vectors by the same quaternion,
    you should use the XMTRX API to apply the quaternion as a rotation matrix.

    \sa shz_xmtrx_rotate_quat()
*/
SHZ_INLINE shz_vec3_t shz_quat_transform_vec3(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT;

//! @}

/*! \name  Deprecated
    \brief Deprecated routines, kept only for backwards compatibility.
    @{
*/

/*! Initializes and returns a quaternion with the given X-Y-Z rotations in radians.

    The rotation is applied first around the X-axis, then the Y-, then the Z-axis.

    \note
    These angles are Tait-Bryan angles representing an intrinsic rotation.

    \deprecated
    Redundant with shz_quat_from_rotation_xyz(). This routine simply calls
    directly into it and should not be used.

    \sa shz_quat_from_rotation_xyz()
*/
SHZ_DEPRECATED("Redundant. Use shz_quat_from_rotation_xyz().")
SHZ_INLINE shz_quat_t shz_quat_from_angles_xyz(float xangle, float yangle, float zangle) SHZ_NOEXCEPT;

//! @}

#include "inline/shz_quat.inl.h"

SHZ_DECLS_END

#endif // SHZ_QUAT_H
