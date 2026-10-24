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

    \important Do not copy this file or its routines into other projects. Use the
               KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
*/

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_quat_sh4.inl.h"
#else
#   include "sw/shz_quat_sw.inl.h"
#endif

SHZ_FORCE_INLINE shz_quat_t shz_quat_init(float w, float x, float y, float z) SHZ_NOEXCEPT {
    return SHZ_INIT(shz_quat_t, { { w, { { x, y, z } } } });
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_identity(void) SHZ_NOEXCEPT {
    return shz_quat_init(1.0f, 0.0f, 0.0f, 0.0f);
}

SHZ_FORCE_INLINE bool shz_quat_equal(shz_quat_t a, shz_quat_t b) SHZ_NOEXCEPT {
    return shz_equalf(a.w, b.w) && shz_equalf(a.x, b.x) &&
           shz_equalf(a.y, b.y) && shz_equalf(a.z, b.z);
}

SHZ_INLINE shz_quat_t shz_quat_from_angles_xyz(float xangle, float yangle, float zangle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_xyz(xangle, yangle, zangle);
}

/* qA(a) * qB(b) * qC(c) for axis quaternions of the half angles comes out, for every Tait-Bryan
   order, as the same eight triple products, with only the sign of the second term of each
   component depending on the order. */
SHZ_FORCE_INLINE shz_quat_t shz_quat_from_rotation_(shz_sincos_t x, shz_sincos_t y, shz_sincos_t z,
                                                    float sw, float sx, float sy, float sz) SHZ_NOEXCEPT {
    const float cc = y.cos * z.cos;
    const float ss = y.sin * z.sin;
    const float cs = y.cos * z.sin;
    const float sc = y.sin * z.cos;

    return shz_quat_init(shz_fmaf(sw * x.sin, ss, x.cos * cc),
                         shz_fmaf(sx * x.cos, ss, x.sin * cc),
                         shz_fmaf(sy * x.sin, cs, x.cos * sc),
                         shz_fmaf(sz * x.sin, sc, x.cos * cs));
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_(shz_sincosf(xAngle * 0.5f), shz_sincosf(yAngle * 0.5f),
                                   shz_sincosf(zAngle * 0.5f), -1.0f, 1.0f, -1.0f, 1.0f);
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_(shz_sincosf(xAngle * 0.5f), shz_sincosf(yAngle * 0.5f),
                                   shz_sincosf(zAngle * 0.5f), 1.0f, -1.0f, 1.0f, -1.0f);
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_zxy(float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_(shz_sincosf(xAngle * 0.5f), shz_sincosf(yAngle * 0.5f),
                                   shz_sincosf(zAngle * 0.5f), -1.0f, -1.0f, 1.0f, 1.0f);
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_(shz_sincosf(xAngle * 0.5f), shz_sincosf(yAngle * 0.5f),
                                   shz_sincosf(zAngle * 0.5f), 1.0f, 1.0f, -1.0f, -1.0f);
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_xyz_u16(uint16_t xAngle, uint16_t yAngle, uint16_t zAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_xyz((int16_t)xAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)yAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)zAngle * (1.0f / SHZ_FSCA_RAD_FACTOR));
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_zyx_u16(uint16_t zAngle, uint16_t yAngle, uint16_t xAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_zyx((int16_t)zAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)yAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)xAngle * (1.0f / SHZ_FSCA_RAD_FACTOR));
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_zxy_u16(uint16_t zAngle, uint16_t xAngle, uint16_t yAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_zxy((int16_t)zAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)xAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)yAngle * (1.0f / SHZ_FSCA_RAD_FACTOR));
}

SHZ_INLINE shz_quat_t shz_quat_from_rotation_yxz_u16(uint16_t yAngle, uint16_t xAngle, uint16_t zAngle) SHZ_NOEXCEPT {
    return shz_quat_from_rotation_yxz((int16_t)yAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)xAngle * (1.0f / SHZ_FSCA_RAD_FACTOR),
                                      (int16_t)zAngle * (1.0f / SHZ_FSCA_RAD_FACTOR));
}

SHZ_INLINE shz_quat_t shz_quat_from_axis_angle(shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    shz_sincos_t half_alpha = shz_sincosf(angle * 0.5f);

    return shz_quat_init(half_alpha.cos,
                         half_alpha.sin * axis.x,
                         half_alpha.sin * axis.y,
                         half_alpha.sin * axis.z);
}

SHZ_INLINE float shz_quat_angle(shz_quat_t q) SHZ_NOEXCEPT {
    return 2.0f * shz_acosf(q.w);
}

SHZ_INLINE shz_vec3_t shz_quat_axis_from_angle_(shz_quat_t q, float angle) SHZ_NOEXCEPT {
    // Protect against subsequent divide-by-zero with FSRRA of (sin(0.0) == 0.0) => NaN.
    if(angle == 0.0f) {
        return shz_vec3_init(1.0f, 0.0f, 0.0f);
    } else {
        float invS = shz_invf_fsrra(shz_sinf(angle * 0.5f));
        return shz_vec3_init(q.x * invS, q.y * invS, q.z * invS);
    }
}

SHZ_INLINE shz_vec3_t shz_quat_axis(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_quat_axis_from_angle_(q, shz_quat_angle(q));
}

SHZ_INLINE void shz_quat_to_axis_angle(shz_quat_t q, shz_vec3_t* vec, float* angle) SHZ_NOEXCEPT {
    *angle = shz_quat_angle(q);
    *vec   = shz_quat_axis_from_angle_(q, *angle);
}

SHZ_INLINE float shz_quat_angle_x(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_atan2f(2.0f * ((q.w * q.x) - (q.y * q.z)),
                     shz_dot8f(q.w, -q.x, -q.y, q.z, q.w, q.x, q.y, q.z));
}

SHZ_INLINE float shz_quat_angle_y(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_asinf(shz_clampf(2.0f * ((q.x * q.z) + (q.w * q.y)), -1.0f, 1.0f));
}

SHZ_INLINE float shz_quat_angle_z(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_atan2f(2.0f * ((q.w * q.z) - (q.x * q.y)),
                      shz_dot8f(q.w, q.x, -q.y, -q.z, q.w, q.x, q.y, q.z));
}

SHZ_INLINE shz_vec3_t shz_quat_to_angles_xyz(shz_quat_t q) SHZ_NOEXCEPT {
    shz_vec2_t   xy = shz_quat_dot2(q, shz_quat_init(q.w, -q.x, -q.y, q.z),
                                       shz_quat_init(q.w, -q.x, q.y, -q.z));
    float        x  = shz_atan2f(2.0f * ((q.w * q.x) - (q.y * q.z)), xy.x);
    shz_sincos_t sc = shz_sincosf(x);

    return shz_vec3_init(x,
                         shz_quat_angle_y(q),
                         shz_atan2f(shz_fmaf(sc.cos, 2.0f * ((q.x * q.y) + (q.w * q.z)),
                                             sc.sin * 2.0f * ((q.x * q.z) - (q.w * q.y))),
                                    shz_fmaf(sc.cos, xy.y,
                                             sc.sin * 2.0f * ((q.y * q.z) + (q.w * q.x)))));
}

SHZ_INLINE shz_vec3_t shz_quat_to_angles_zyx(shz_quat_t q) SHZ_NOEXCEPT {
    shz_vec3_t a = shz_quat_to_angles_xyz(shz_quat_conjugate(q));

    return shz_vec3_init(-a.z, -a.y, -a.x);
}

SHZ_INLINE shz_vec3_t shz_quat_to_angles_zxy(shz_quat_t q) SHZ_NOEXCEPT {
    shz_vec3_t a = shz_quat_to_angles_xyz(shz_quat_init(q.w, q.y, q.x, q.z));

    return shz_vec3_init(a.z, a.y, a.x);
}

SHZ_INLINE shz_vec3_t shz_quat_to_angles_yxz(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_vec3_neg(shz_quat_to_angles_xyz(shz_quat_init(q.w, -q.y, -q.x, -q.z)));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_add(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    return shz_quat_init(q.w + p.w, q.x + p.x, q.y + p.y, q.z + p.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_sub(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    return shz_quat_init(q.w - p.w, q.x - p.x, q.y - p.y, q.z - p.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_scale(shz_quat_t q, float f) SHZ_NOEXCEPT {
    return shz_quat_init(q.w * f, q.x * f, q.y * f, q.z * f);
}

SHZ_FORCE_INLINE float shz_quat_magnitude_sqr(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(quat.w, quat.x, quat.y, quat.z);
}

SHZ_FORCE_INLINE float shz_quat_magnitude(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE float shz_quat_magnitude_fsrra(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE float shz_quat_magnitude_inv(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE float shz_quat_magnitude_inv_fsrra(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_scale(quat, shz_quat_magnitude_inv_fsrra(quat));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize_or(shz_quat_t quat, shz_quat_t alt) SHZ_NOEXCEPT {
    const float sqr_mag = shz_quat_magnitude_sqr(quat);

    return (sqr_mag == 0.0f)?
        alt : shz_quat_scale(quat, shz_inv_sqrtf_fsrra(sqr_mag));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize_safe(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_normalize_or(quat, shz_quat_identity());
}

SHZ_FORCE_INLINE float shz_quat_dot(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    return shz_dot8f(q1.x, q1.y, q1.z, q1.w,
                     q2.x, q2.y, q2.z, q2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_quat_dot2(shz_quat_t l, shz_quat_t r1, shz_quat_t r2) SHZ_NOEXCEPT {
    return shz_vec4_dot2(shz_vec3_vec4( l.axis,  l.w),
                         shz_vec3_vec4(r1.axis, r1.w),
                         shz_vec3_vec4(r2.axis, r2.w));
}

SHZ_FORCE_INLINE shz_vec3_t shz_quat_dot3(shz_quat_t l, shz_quat_t r1, shz_quat_t r2, shz_quat_t r3) SHZ_NOEXCEPT {
    return shz_vec4_dot3(shz_vec3_vec4( l.axis,  l.w),
                         shz_vec3_vec4(r1.axis, r1.w),
                         shz_vec3_vec4(r2.axis, r2.w),
                         shz_vec3_vec4(r3.axis, r3.w));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_conjugate(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_init(quat.w, -quat.x, -quat.y, -quat.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_inv(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_scale(shz_quat_conjugate(quat), shz_invf_fsrra(shz_quat_magnitude_sqr(quat)));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_neg(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_init(-quat.w, -quat.x, -quat.y, -quat.z);
}

SHZ_INLINE shz_quat_t shz_quat_from_rotated_axis(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT {
    shz_vec3_t a = shz_vec3_cross(v1, v2);
    float      m = shz_sqrtf_fsrra(shz_vec3_magnitude_sqr(v1) * shz_vec3_magnitude_sqr(v2)) +
                   shz_vec3_dot(v1, v2);

    return shz_quat_normalize(shz_quat_init(m, a.x, a.y, a.z));
}

SHZ_INLINE shz_quat_t shz_quat_lerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT {
    if (shz_quat_dot(a, b) < 0.0f)
        b = shz_quat_neg(b);

    return shz_quat_init(
        shz_lerpf(a.w, b.w, t),
        shz_lerpf(a.x, b.x, t),
        shz_lerpf(a.y, b.y, t),
        shz_lerpf(a.z, b.z, t)
    );
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_nlerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT {
    return shz_quat_normalize(shz_quat_lerp(a, b, t));
}

SHZ_INLINE shz_quat_t shz_quat_slerp(shz_quat_t q, shz_quat_t p, float t) SHZ_NOEXCEPT {
    shz_quat_t q1;
    float c, phi;

    /* Negate one of the inputs when the dot product is negative in order
       to avoid taking the long path around the sphere. */
    if((c = shz_quat_dot(q, p)) < 0.0f) {
        c = -c;
        q1 = shz_quat_neg(q);
    } else
        q1 = q;

    // Check whether the inputs are closer than a certain threshold.
    const float SHZ_QUAT_DOT_THRESHOLD = 0.9995f;
    if(c > SHZ_QUAT_DOT_THRESHOLD) {
        // LERP and normalize to mitigate numerical instability and division by zero.
        return shz_quat_normalize(shz_quat_init(shz_lerpf(q1.w, p.w, t),
                                                shz_lerpf(q1.x, p.x, t),
                                                shz_lerpf(q1.y, p.y, t),
                                                shz_lerpf(q1.z, p.z, t)));
    }

    // Check for a minimum epsilon, below which we do no interpolation.
    const float SHZ_QUAT_SLERP_PHI_EPSILON = SHZ_FLT_EPSILON;
    if((phi = shz_acosf(c)) > SHZ_QUAT_SLERP_PHI_EPSILON) {
        /* The output of acosf() is in the range of [0 : PI],
           giving us a sine that is guaranteed to be a positive value. */
        float s = shz_invf_fsrra(shz_sinf(phi));

        // Add the two vectors, which have been scaled by their respective ratios.
        return shz_quat_add(shz_quat_scale(q1, shz_sinf((1.0f - t) * phi) * s),
                            shz_quat_scale(p,  shz_sinf(t * phi) * s));
    }

	return q1;
}

SHZ_INLINE float shz_quat_angle_between(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    float d = shz_fabsf(shz_quat_dot(q, p));

    // Clamp to handle floating-point overshoot.
    return 2.0f * shz_acosf(shz_fminf(d, 1.0f));
}

SHZ_INLINE shz_quat_t shz_quat_squad(shz_quat_t q1, shz_quat_t q2, shz_quat_t s1, shz_quat_t s2, float t) SHZ_NOEXCEPT {
    return shz_quat_slerp(shz_quat_slerp(q1, q2, t),
                          shz_quat_slerp(s1, s2, t),
                          2.0f * t * (1.0f - t));
}

SHZ_INLINE shz_quat_t shz_quat_rotate_towards(shz_quat_t from, shz_quat_t to, float max_angle) SHZ_NOEXCEPT {
    float angle = shz_quat_angle_between(from, to);

    if(angle <= max_angle)
        return to;

    return shz_quat_slerp(from, to, max_angle * shz_invf_fsrra(angle));
}

SHZ_INLINE shz_quat_t shz_quat_mult(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_quat_mult_sh4(q1, q2);
#else
    return shz_quat_mult_sw(q1, q2);
#endif
}

SHZ_INLINE shz_quat_t shz_quat_div(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    return shz_quat_mult(q, shz_quat_inv(p));
}

SHZ_INLINE shz_vec3_t shz_quat_transform_vec3(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_quat_transform_vec3_sh4(q, v);
#else
    return shz_quat_transform_vec3_sw(q, v);
#endif
}

//! \endcond

#include "../shz_xmtrx.h"
