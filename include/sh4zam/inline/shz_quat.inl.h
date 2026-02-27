//! \cond INTERNAL
/*! \file
    \brief Internal implementation of Quaternion API
    \ingroup quat

    This file contains the implementation of the inline functions declared
    within the Quaternion API, which optionally handle constant expression
    evaluation then forward the call on to the respective back-ends.

    \author 2025, 2026 Falco Girgis
    \author 2025 Oleg Endo

    \copyright MIT License
*/

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_quat_sh4.inl.h"
#endif

#include "sw/shz_quat_sw.inl.h"

SHZ_FORCE_INLINE shz_quat_t shz_quat_init(float w, float x, float y, float z) SHZ_NOEXCEPT {
    return shz_quat_init_sw(w, x, y, z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_identity(void) SHZ_NOEXCEPT {
    return shz_quat_identity_sw();
}

SHZ_FORCE_INLINE bool shz_quat_equal(shz_quat_t a, shz_quat_t b) SHZ_NOEXCEPT {
    return shz_quat_equal_sw(a, b);
}

SHZ_INLINE shz_quat_t shz_quat_from_angles_xyz(float xangle, float yangle, float zangle) SHZ_NOEXCEPT {
    return shz_quat_from_angles_xyz_sw(xangle, yangle, zangle);
}

SHZ_INLINE shz_quat_t shz_quat_from_axis_angle(shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    return shz_quat_from_axis_angle_sw(axis, angle);
}

SHZ_INLINE float shz_quat_angle(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_quat_angle_sw(q);
}

SHZ_INLINE shz_vec3_t shz_quat_axis(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_quat_axis_sw(q);
}

SHZ_INLINE void shz_quat_to_axis_angle(shz_quat_t q, shz_vec3_t* vec, float* angle) SHZ_NOEXCEPT {
    return shz_quat_to_axis_angle_sw(q, vec, angle);
}

SHZ_INLINE float shz_quat_angle_x(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_quat_angle_x_sw(q);
}

SHZ_INLINE float shz_quat_angle_y(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_quat_angle_y_sw(q);
}

SHZ_INLINE float shz_quat_angle_z(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_quat_angle_z_sw(q);
}

SHZ_INLINE shz_vec3_t shz_quat_to_angles_xyz(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_quat_to_angles_xyz_sw(q);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_add(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    return shz_quat_add_sw(q, p);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_scale(shz_quat_t q, float f) SHZ_NOEXCEPT {
    return shz_quat_scale_sw(q, f);
}

SHZ_FORCE_INLINE float shz_quat_magnitude_sqr(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_magnitude_sqr_sw(quat);
}

SHZ_FORCE_INLINE float shz_quat_magnitude(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_magnitude_sw(quat);
}

SHZ_FORCE_INLINE float shz_quat_magnitude_inv(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_magnitude_inv_sw(quat);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_normalize_sw(quat);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize_safe(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_normalize_safe_sw(quat);
}

SHZ_FORCE_INLINE float shz_quat_dot(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    return shz_quat_dot_sw(q1, q2);
}

SHZ_FORCE_INLINE shz_vec2_t shz_quat_dot2(shz_quat_t l, shz_quat_t r1, shz_quat_t r2) SHZ_NOEXCEPT {
    return shz_quat_dot2_sw(l, r1, r2);
}

SHZ_FORCE_INLINE shz_vec3_t shz_quat_dot3(shz_quat_t l, shz_quat_t r1, shz_quat_t r2, shz_quat_t r3) SHZ_NOEXCEPT {
    return shz_quat_dot3_sw(l, r1, r2, r3);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_conjugate(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_conjugate_sw(quat);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_inv(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_inv_sw(quat);
}

SHZ_INLINE shz_quat_t shz_quat_from_rotated_axis(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT {
    return shz_quat_from_rotated_axis_sw(v1, v2);
}

SHZ_INLINE shz_quat_t shz_quat_lerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT {
    return shz_quat_lerp_sw(a, b, t);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_nlerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT {
    return shz_quat_nlerp_sw(a, b, t);
}

SHZ_INLINE shz_quat_t shz_quat_slerp(shz_quat_t q, shz_quat_t p, float t) SHZ_NOEXCEPT {
    return shz_quat_slerp_sw(q, p, t);
}

SHZ_INLINE shz_quat_t shz_quat_mult(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_quat_mult_dc(q1, q2);
#else
    return shz_quat_mult_sw(q1, q2);
#endif
}

SHZ_INLINE shz_vec3_t shz_quat_transform_vec3(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_quat_transform_vec3_dc(q, v);
#else
    return shz_quat_transform_vec3_sw(q, v);
#endif
}

//! \endcond