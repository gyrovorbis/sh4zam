//! \cond INTERNAL
/*! \file
    \brief Software implementation of Quaternion API
    \ingroup quat

    This file contains the generic implementation routines for
    quaternion math, which are platform-independent, shared,
    and may be run anywhere. They're offered as part of the
    SW back-end.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_QUAT_SW_INL_H
#define SHZ_QUAT_SW_INL_H

SHZ_FORCE_INLINE shz_quat_t shz_quat_init_sw(float w, float x, float y, float z) SHZ_NOEXCEPT {
    return (shz_quat_t) { .w = w, .x = x, .y = y, .z = z };
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_identity_sw(void) SHZ_NOEXCEPT {
    return shz_quat_init(1.0f, 0.0f, 0.0f, 0.0f);
}

SHZ_FORCE_INLINE bool shz_quat_equal_sw(shz_quat_t a, shz_quat_t b) SHZ_NOEXCEPT {
    return shz_equalf(a.w, b.w) && shz_equalf(a.x, b.x) &&
           shz_equalf(a.y, b.y) && shz_equalf(a.z, b.z);
}

SHZ_INLINE shz_quat_t shz_quat_from_angles_xyz_sw(float xangle, float yangle, float zangle) SHZ_NOEXCEPT {
    shz_sincos_t scx = shz_sincosf(xangle * 0.5f);
    shz_sincos_t scy = shz_sincosf(yangle * 0.5f);
    shz_sincos_t scz = shz_sincosf(zangle * 0.5f);

    // \todo Some shz_vec2_dot4() kind of pattern?
    return shz_quat_init(
        ((scy.cos * scx.cos) * scz.sin) - ((scy.sin * scx.sin) * scz.cos),
        ((scx.sin * scy.cos) * scz.cos) + ((scy.sin * scx.cos) * scz.sin),
        ((scy.sin * scx.cos) * scz.cos) - ((scx.sin * scy.cos) * scz.sin),
        ((scy.cos * scx.cos) * scz.cos) + ((scy.sin * scx.sin) * scz.sin)
    );
}

SHZ_INLINE shz_quat_t shz_quat_from_axis_angle_sw(shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    shz_sincos_t half_alpha = shz_sincosf(angle * 0.5f);

    return shz_quat_init(half_alpha.cos,
                         half_alpha.sin * axis.x,
                         half_alpha.sin * axis.y,
                         half_alpha.sin * axis.z);
}

SHZ_INLINE float shz_quat_angle_sw(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_acosf(q.w);
}

SHZ_INLINE shz_vec3_t shz_quat_axis_sw(shz_quat_t q) SHZ_NOEXCEPT {
    float angle = shz_quat_angle(q);
    float invS = shz_invf_fsrra(shz_sinf(angle));
    return shz_vec3_init(q.x * invS, q.y * invS, q.z * invS);
}

SHZ_INLINE void shz_quat_to_axis_angle_sw(shz_quat_t q, shz_vec3_t* vec, float* angle) SHZ_NOEXCEPT {
    *angle = shz_quat_angle(q);
    float invS = shz_invf_fsrra(shz_sinf(*angle));
    *vec = shz_vec3_init(q.x * invS, q.y * invS, q.z * invS);
}

SHZ_INLINE float shz_quat_angle_x_sw(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_atan2f(-2.0f * ((q.y * q.z) + (q.w * q.x)),
                     shz_dot8f(q.w, -q.x, -q.y, q.z, q.w, q.x, q.y, q.z));
}

SHZ_INLINE float shz_quat_angle_y_sw(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_asinf(shz_clampf(2.0f * ((q.x * q.z) - (q.w * q.y)), -1.0f, 1.0f));
}

SHZ_INLINE float shz_quat_angle_z_sw(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_atan2f(2.0f * ((q.x * q.y) + (q.w * q.z)),
                      shz_dot8f(q.w, q.x, -q.y, -q.z, q.w, q.x, q.y, q.z));
}

SHZ_INLINE shz_vec3_t shz_quat_to_angles_xyz_sw(shz_quat_t q) SHZ_NOEXCEPT {
    shz_vec2_t xz = shz_quat_dot2(q, shz_quat_init(q.w, q.x, -q.y, -q.z),
                                     shz_quat_init(q.w, -q.x, -q.y, q.z));

    return shz_vec3_init(xz.x, shz_quat_angle_y(q), xz.y);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_add_sw(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    return shz_quat_init(q.w + p.w, q.x + p.x, q.y + p.y, q.z + p.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_scale_sw(shz_quat_t q, float f) SHZ_NOEXCEPT {
    return shz_quat_init(q.w * f, q.x * f, q.y * f, q.z * f);
}

SHZ_FORCE_INLINE float shz_quat_magnitude_sqr_sw(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(quat.w, quat.x, quat.y, quat.z);
}

SHZ_FORCE_INLINE float shz_quat_magnitude_sw(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE float shz_quat_magnitude_inv_sw(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize_sw(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_scale(quat, shz_quat_magnitude_inv(quat));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize_safe_sw(shz_quat_t quat) SHZ_NOEXCEPT {
    float mag = shz_quat_magnitude_sqr(quat);

    return (mag != 0.0f)?
        shz_quat_scale(quat, shz_inv_sqrtf_fsrra(mag)) :
        shz_quat_identity();
}

SHZ_FORCE_INLINE float shz_quat_dot_sw(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    return shz_dot8f(q1.x, q1.y, q1.z, q1.w,
                     q2.x, q2.y, q2.z, q2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_quat_dot2_sw(shz_quat_t l, shz_quat_t r1, shz_quat_t r2) SHZ_NOEXCEPT {
    return shz_vec4_dot2(shz_vec3_vec4( l.axis,  l.w),
                         shz_vec3_vec4(r1.axis, r1.w),
                         shz_vec3_vec4(r2.axis, r2.w));
}

SHZ_FORCE_INLINE shz_vec3_t shz_quat_dot3_sw(shz_quat_t l, shz_quat_t r1, shz_quat_t r2, shz_quat_t r3) SHZ_NOEXCEPT {
    return shz_vec4_dot3(shz_vec3_vec4( l.axis,  l.w),
                         shz_vec3_vec4(r1.axis, r1.w),
                         shz_vec3_vec4(r2.axis, r2.w),
                         shz_vec3_vec4(r3.axis, r3.w));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_conjugate_sw(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_init(quat.w, -quat.x, -quat.y, -quat.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_inv_sw(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_scale(shz_quat_conjugate(quat), shz_quat_magnitude_inv(quat));
}

SHZ_INLINE shz_quat_t shz_quat_from_rotated_axis_sw(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT {
    shz_vec3_t a = shz_vec3_cross(v1, v2);
    float      m = shz_sqrtf_fsrra(shz_vec3_magnitude_sqr(v1) * shz_vec3_magnitude_sqr(v2));

    return shz_quat_normalize(shz_quat_init(m, a.x, a.y, a.z));
}

// \todo Some kind of shz_vec3_dot4() pattern.
SHZ_INLINE shz_quat_t shz_quat_lerp_sw(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT {
	if(shz_quat_dot(a, b) < 0.0f) {
        return shz_quat_init(t * (b.w + a.w) - a.w,
                             t * (b.x + a.x) - a.x,
                             t * (b.y + a.y) - a.y,
                             t * (b.z + a.z) - a.z);
	} else {
        return shz_quat_init(t * (b.w - a.w) + a.w,
                             t * (b.x - a.x) + a.x,
                             t * (b.y - a.y) + a.y,
                             t * (b.z - a.z) + a.z);
    }
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_nlerp_sw(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT {
    return shz_quat_normalize(shz_quat_lerp(a, b, t));
}

SHZ_INLINE shz_quat_t shz_quat_slerp_sw(shz_quat_t q, shz_quat_t p, float t) SHZ_NOEXCEPT {
	shz_quat_t q1 = q;

    float c = shz_quat_dot(q1, p);
    if(c < 0.0f) {
        c = -c;
        q1 = shz_quat_inv(q1);
    }

    float phi = shz_acosf(c);
    // Check for a minimum epsilon, below which we do no interpolation.
    if(phi > SHZ_QUAT_SLERP_PHI_EPSILON) {
        /* The output of acosf() is in the range of [0 : PI],
           giving us a sine that is guaranteed to be a positive value. */
        float s = shz_invf_fsrra(shz_sinf(phi));
        /* Add the two vectors, which have been scaled by their respective ratios. */
        return shz_quat_add(shz_quat_scale(q1, shz_sinf((1.0f - t) * phi) * s),
                            shz_quat_scale(p,  shz_sinf(t * phi) * s));
    }

	return q1;
}

SHZ_INLINE shz_quat_t shz_quat_mult_sw(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    shz_quat_t r;

    assert(false);

    return r;
}

SHZ_INLINE shz_vec3_t shz_quat_transform_vec3_sw(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec2_t dot_qqv = shz_vec3_dot2(q.axis,
                                       q.axis, v);

    shz_vec3_t cross_qv = shz_vec3_cross(q.axis, v);

    return shz_vec3_dot3(shz_vec3_init(2.0f * dot_qqv.y, 
                                      (q.w * q.w) - dot_qqv.x,
                                      2.0f * q.w),
                         shz_vec3_init(q.x, v.x, cross_qv.x),
                         shz_vec3_init(q.y, v.y, cross_qv.y),
                         shz_vec3_init(q.z, v.z, cross_qv.z));
}

//! \endcond

#endif
