//! \cond INTERNAL
/*! \file
    \brief Software implementation of Vector API
    \ingroup vector

    This file contains the generic implementation routines for
    vector math, which are platform-independent, shared, and may
    be run anywhere. They're offered as part of the SW back-end.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_VECTOR_SW_INL_H
#define SHZ_VECTOR_SW_INL_H

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_init_sw(float x, float y) SHZ_NOEXCEPT {
    return (shz_vec2_t){ .x = x, .y = y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_init_sw(float x, float y, float z) SHZ_NOEXCEPT {
    return (shz_vec3_t){ .x = x, .y = y, .z = z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_init_sw(float x, float y, float z, float w) SHZ_NOEXCEPT {
    return (shz_vec4_t){ .x = x, .y = y, .z = z, .w = w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_fill_sw(float v) SHZ_NOEXCEPT {
    return shz_vec2_init(v, v);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_fill_sw(float v) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_fill(v), v);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_fill_sw(float v) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_fill(v), v);
}

 SHZ_FORCE_INLINE shz_vec2_t shz_vec2_swizzle_sw(shz_vec2_t vec, unsigned x_idx, unsigned y_idx) SHZ_NOEXCEPT {
    return shz_vec2_init(vec.e[x_idx], vec.e[y_idx]);
 }

 SHZ_FORCE_INLINE shz_vec3_t shz_vec3_swizzle_sw(shz_vec3_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx) SHZ_NOEXCEPT {
    return shz_vec3_init(vec.e[x_idx], vec.e[y_idx], vec.e[z_idx]);
 }

 SHZ_FORCE_INLINE shz_vec4_t shz_vec4_swizzle_sw(shz_vec4_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx, unsigned w_idx) SHZ_NOEXCEPT {
    return shz_vec4_init(vec.e[x_idx], vec.e[y_idx], vec.e[z_idx], vec.e[w_idx]);
 }

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_vec3_sw(shz_vec2_t vec, float z) SHZ_NOEXCEPT {
    return (shz_vec3_t){ .xy = vec, .z = z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec2_vec4_sw(shz_vec2_t vec, float z, float w) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec2_vec3(vec, z), w);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec3_vec4_sw(shz_vec3_t vec, float w) SHZ_NOEXCEPT {
    return (shz_vec4_t){ .xyz = vec, .w = w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x + vec2.x, vec1.y + vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_add_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_add(vec1.xy, vec2.xy), vec1.z + vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add_sw(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_add(vec1.xyz, vec2.xyz), vec1.w + vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x - vec2.x, vec1.y - vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sub_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_sub(vec1.xy, vec2.xy), vec1.z - vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub_sw(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_sub(vec1.xyz, vec2.xyz), vec1.w - vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x * vec2.x, vec1.y * vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_mul_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_mul(vec1.xy, vec2.xy), vec1.z * vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul_sw(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_mul(vec1.xyz, vec2.xyz), vec1.w * vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_divf(vec1.x, vec2.x), shz_divf(vec1.y, vec2.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_div_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_div(vec1.xy, vec2.xy), shz_divf(vec1.z, vec2.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div_sw(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_div(vec1.xyz, vec2.xyz), shz_divf(vec1.w, vec2.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_scale_sw(shz_vec2_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec2_init(vec.x * factor, vec.y * factor);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_scale_sw(shz_vec3_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_scale(vec.xy, factor), vec.z * factor);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_scale_sw(shz_vec4_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_scale(vec.xyz, factor), vec.w * factor);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_sqr_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return vec.x * vec.x + vec.y * vec.y;
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_sqr_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(vec.x, vec.y, vec.z, vec.w);
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_sqr_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr3f(vec.x, vec.y, vec.z);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_vec4_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_inv_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_inv_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_inv_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_vec4_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_scale(vec, shz_vec2_magnitude_inv(vec));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_scale(vec, shz_vec3_magnitude_inv(vec));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_scale(vec, shz_vec4_magnitude_inv(vec));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize_safe_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec2_magnitude_sqr(vec);

    return (mag > 0.0f)? shz_vec2_scale(vec, shz_inv_sqrtf_fsrra(mag)) :
                         shz_vec2_fill(0.0f);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize_safe_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec3_magnitude_sqr(vec);

    return (mag > 0.0f)? shz_vec3_scale(vec, shz_inv_sqrtf_fsrra(mag)) :
                         shz_vec3_fill(0.0f);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize_safe_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec4_magnitude_sqr(vec);

    return (mag > 0.0f)? shz_vec4_scale(vec, shz_inv_sqrtf_fsrra(mag)) :
                         shz_vec4_fill(0.0f);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_abs_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_fabsf(vec.x), shz_fabsf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_abs_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_abs(vec.xy), shz_fabsf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_abs_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_abs(vec.xyz), shz_fabsf(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_neg_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(-vec.x, -vec.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_neg_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_neg(vec.xy), -vec.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_neg_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_neg(vec.xyz), -vec.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_inv_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_invf(vec.x), shz_invf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_inv_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_inv(vec.xy), shz_invf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_inv_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_inv(vec.xyz), shz_invf(vec.w));
}

SHZ_FORCE_INLINE float shz_vec2_max_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_fmaxf(vec.x, vec.y);
}

SHZ_FORCE_INLINE float shz_vec3_max_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_fmaxf(shz_vec2_max(vec.xy), vec.z);
}

SHZ_FORCE_INLINE float shz_vec4_max_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_fmaxf(shz_vec3_max(vec.xyz), vec.w);
}

SHZ_FORCE_INLINE float shz_vec2_min_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_fminf(vec.x, vec.y);
}

SHZ_FORCE_INLINE float shz_vec3_min_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_fminf(shz_vec2_min(vec.xy), vec.z);
}

SHZ_FORCE_INLINE float shz_vec4_min_sw(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_fminf(shz_vec3_min(vec.xyz), vec.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_clamp_sw(shz_vec2_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_clampf(vec.x, min, max), shz_clampf(vec.y, min, max));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_clamp_sw(shz_vec3_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_clamp(vec.xy, min, max), shz_clampf(vec.z, min, max));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_clamp_sw(shz_vec4_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_clamp(vec.xyz, min, max), shz_clampf(vec.w, min, max));
}

SHZ_FORCE_INLINE bool shz_vec2_equal_sw(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT {
    return shz_equalf(a.x, b.x) && shz_equalf(a.y, b.y);
}

SHZ_FORCE_INLINE bool shz_vec3_equal_sw(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT {
    return shz_vec2_equal(a.xy, b.xy) && shz_equalf(a.z, b.z);
}

SHZ_FORCE_INLINE bool shz_vec4_equal_sw(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    return shz_vec3_equal(a.xyz, b.xyz) && shz_equalf(a.w, b.w);
}

SHZ_FORCE_INLINE float shz_vec2_dot_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

SHZ_FORCE_INLINE float shz_vec4_dot_sw(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_dot8f(vec1.x, vec1.y, vec1.z, vec1.w,
                     vec2.x, vec2.y, vec2.z, vec2.w);
}

SHZ_FORCE_INLINE float shz_vec3_dot_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_dot6f(vec1.x, vec1.y, vec1.z,
                     vec2.x, vec2.y, vec2.z);
}

SHZ_FORCE_INLINE float shz_vec2_distance_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_magnitude(shz_vec2_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_distance_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_magnitude(shz_vec3_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec4_distance_sw(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_magnitude(shz_vec4_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec2_distance_sqr_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_magnitude_sqr(shz_vec2_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_distance_sqr_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_magnitude_sqr(shz_vec3_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec4_distance_sqr_sw(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_magnitude_sqr(shz_vec4_sub(vec1, vec2));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_lerp_sw(shz_vec2_t a, shz_vec2_t b, float t) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_lerpf(a.x, b.x, t), shz_lerpf(a.y, b.y, t));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_lerp_sw(shz_vec3_t a, shz_vec3_t b, float t) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_lerp(a.xy, b.xy, t), shz_lerpf(a.z, b.z, t));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_lerp_sw(shz_vec4_t a, shz_vec4_t b, float t) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_lerp(a.xyz, b.xyz, t), shz_lerpf(a.w, b.w, t));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_reflect_sw(shz_vec2_t incidence, shz_vec2_t normal) SHZ_NOEXCEPT {
    return shz_vec2_add(incidence, shz_vec2_scale(normal, shz_vec2_dot(normal, shz_vec2_scale(incidence, -2.0))));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_reflect_sw(shz_vec3_t incidence, shz_vec3_t normal) SHZ_NOEXCEPT {
    return shz_vec3_add(incidence, shz_vec3_scale(normal, shz_vec3_dot(normal, shz_vec3_scale(incidence, -2.0))));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_reflect_sw(shz_vec4_t incidence, shz_vec4_t normal) SHZ_NOEXCEPT {
    return shz_vec4_add(incidence, shz_vec4_scale(normal, shz_vec4_dot(normal, shz_vec4_scale(incidence, -2.0))));
}

SHZ_FORCE_INLINE float shz_vec2_cross_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cross_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_init(vec1.y * vec2.z - vec1.z * vec2.y,
                         vec1.z * vec2.x - vec1.x * vec2.z,
                         vec1.x * vec2.y - vec1.y * vec2.x);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project_sw(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT {
    return shz_vec2_scale(shz_vec2_normalize(vec), shz_vec2_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project_sw(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT {
    return shz_vec3_scale(shz_vec3_normalize(vec), shz_vec3_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project_sw(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT {
    return shz_vec4_scale(shz_vec4_normalize(vec), shz_vec4_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project_safe_sw(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT {
    return shz_vec2_scale(shz_vec2_normalize_safe(vec), shz_vec2_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project_safe_sw(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT {
    return shz_vec3_scale(shz_vec3_normalize_safe(vec), shz_vec3_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project_safe_sw(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT  {
    return shz_vec4_scale(shz_vec4_normalize_safe(vec), shz_vec4_dot(vec, onto));
}

SHZ_FORCE_INLINE float shz_vec2_angle_between_sw(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_atan2f(shz_vec2_dot(vec1, vec2), shz_vec2_cross(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_angle_between_sw(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_atan2f(shz_vec3_dot(vec1, vec2), shz_vec3_magnitude(shz_vec3_cross(vec1, vec2)));
}

SHZ_FORCE_INLINE float shz_vec2_angle_sw(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_atan2f(vec.y, vec.x);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_angles_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_atan2f(shz_vec2_magnitude(shz_vec2_init(vec.y, vec.z)), vec.x),
                         shz_atan2f(shz_vec2_magnitude(shz_vec2_init(vec.z, vec.x)), vec.y),
                         shz_atan2f(shz_vec2_magnitude(shz_vec2_init(vec.x, vec.y)), vec.z));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_sincos_sw(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_vec2_init(sincos.sin, sincos.cos);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_sincos_sw(shz_sincos_t azimuth, shz_sincos_t elevation) SHZ_NOEXCEPT {
    return shz_vec3_init(azimuth.cos * elevation.cos, azimuth.sin * elevation.cos, elevation.sin);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle_sw(float radians) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(radians);
    return shz_vec2_init(sc.cos, sc.sin);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles_sw(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_sincos(shz_sincosf(azimuth), shz_sincosf(elevation));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle_deg_sw(float degrees) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf_deg(degrees);
    return shz_vec2_init(sc.cos, sc.sin);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles_deg_sw(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_sincos(shz_sincosf_deg(azimuth), shz_sincosf_deg(elevation));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_rotate_sw(shz_vec2_t vec, float radians) SHZ_NOEXCEPT {
    shz_sincos_t sincos = shz_sincosf(radians);

    return shz_vec2_init(vec.x * sincos.cos - vec.y * sincos.sin,
                         vec.x * sincos.sin + vec.y * sincos.cos);
}

SHZ_INLINE shz_vec2_t shz_vec2_refract_sw(shz_vec2_t incidence, shz_vec2_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec2_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k <= 0.0f)
        return (k == 0.0f)? shz_vec2_scale(incidence, eta) :
                            shz_vec2_fill(0.0f);
    else
        return shz_vec2_sub(shz_vec2_scale(incidence, eta),
                            shz_vec2_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
}

SHZ_INLINE shz_vec3_t shz_vec3_refract_sw(shz_vec3_t incidence, shz_vec3_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec3_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k <= 0.0f)
        return (k == 0.0f)? shz_vec3_scale(incidence, eta) :
                            shz_vec3_fill(0.0f);
    else
        return shz_vec3_sub(shz_vec3_scale(incidence, eta),
                            shz_vec3_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
}

SHZ_INLINE shz_vec4_t shz_vec4_refract_sw(shz_vec4_t incidence, shz_vec4_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec4_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k <= 0.0f)
        return (k == 0.0f)? shz_vec4_scale(incidence, eta) :
                            shz_vec4_fill(0.0f);
    else
        return shz_vec4_sub(shz_vec4_scale(incidence, eta),
                            shz_vec4_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
}

SHZ_INLINE float shz_vec3_triple_sw(shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT {
    assert(false);
    return 0.0f;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_move_sw(shz_vec2_t vec, shz_vec2_t target, float max_distance) SHZ_NOEXCEPT {
    shz_vec2_t delta   = shz_vec2_sub(target, vec);
    float      sqrdist = shz_vec2_magnitude_sqr(delta);

    if((max_distance >= 0.0f) && (sqrdist <= max_distance * max_distance))
        return target;

    float scaled_dist = shz_inv_sqrtf(sqrdist) * max_distance;

    return shz_vec2_add(vec, shz_vec2_scale(delta, scaled_dist));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_move_sw(shz_vec3_t vec, shz_vec3_t target, float max_distance) SHZ_NOEXCEPT {
    shz_vec3_t delta   = shz_vec3_sub(target, vec);
    float      sqrdist = shz_vec3_magnitude_sqr(delta);

    if((max_distance >= 0.0f) && (sqrdist <= max_distance * max_distance))
        return target;

    float scaled_dist = shz_inv_sqrtf(sqrdist) * max_distance;

    return shz_vec3_add(vec, shz_vec3_scale(delta, scaled_dist));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_move_sw(shz_vec4_t vec, shz_vec4_t target, float max_distance) SHZ_NOEXCEPT {
    shz_vec4_t delta   = shz_vec4_sub(target, vec);
    float      sqrdist = shz_vec4_magnitude_sqr(delta);

    if((max_distance >= 0.0f) && (sqrdist <= max_distance * max_distance))
        return target;

    float scaled_dist = shz_inv_sqrtf(sqrdist) * max_distance;

    return shz_vec4_add(vec, shz_vec4_scale(delta, scaled_dist));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_barycenter_sw(shz_vec3_t p,
                                                   shz_vec3_t a,
                                                   shz_vec3_t b,
                                                   shz_vec3_t c) SHZ_NOEXCEPT {
    shz_vec3_t result;
    
    assert(false);

    return result;
}

SHZ_INLINE shz_vec3_t shz_vec3_perp_sw(shz_vec3_t vec) SHZ_NOEXCEPT {
    shz_vec3_t cardinal_axis = shz_vec3_init(1.0f, 0.0f, 0.0f);
    float min = shz_fabsf(vec.x);
    float maybe_min;

    if ((maybe_min = shz_fabsf(vec.y)) < min) {
        min = maybe_min;
        cardinal_axis = shz_vec3_init(0.0f, 1.0f, 0.0f);
    }

    if((maybe_min = shz_fabsf(vec.z)) < min)
        cardinal_axis = shz_vec3_init(0.0f, 0.0f, 1.0f);

    return shz_vec3_cross(vec, cardinal_axis);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_reject_sw(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT {
    float v1dv2 = shz_vec3_dot(v1, v2);
    float v2dv2 = shz_vec3_magnitude_sqr(v2);
    float   mag = shz_divf_fsrra(v1dv2, v2dv2);

    return shz_vec3_sub(v1, shz_vec3_scale(v2, mag));
}

SHZ_INLINE void shz_vec3_orthonormalize_sw(shz_vec3_t   in1, shz_vec3_t   in2,
                                          shz_vec3_t* out1, shz_vec3_t* out2) SHZ_NOEXCEPT {
    shz_vec3_t tmp;

    *out1 = shz_vec3_normalize_safe(in1);
      tmp = shz_vec3_normalize_safe(shz_vec3_cross(*out1, in2));
    *out2 = shz_vec3_cross(tmp, *out1);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cubic_hermite_sw(shz_vec3_t vec1, shz_vec3_t tangent1, shz_vec3_t vec2, shz_vec3_t tangent2, float t) SHZ_NOEXCEPT {
    float t2 = t * t;
    float t3 = t2 * t;

    // Hermite basis functions
    float h00 =  2.0f*t3 - 3.0f*t2 + 1.0f;
    float h10 =  1.0f*t3 - 2.0f*t2 + t;
    float h01 = -2.0f*t3 + 3.0f*t2;
    float h11 =  1.0f*t3 - 1.0f*t2;

    // Interpolated result: P(t) = h00*p0 + h10*v0 + h01*p1 + h11*v1
    return shz_vec4_dot3(shz_vec4_init(h00, h10, h01, h11),
                         shz_vec4_init(vec1.x, tangent1.x, vec2.x, tangent2.x),
                         shz_vec4_init(vec1.y, tangent1.y, vec2.y, tangent2.y),
                         shz_vec4_init(vec1.z, tangent1.z, vec2.z, tangent2.z));
}


SHZ_FORCE_INLINE shz_vec2_t shz_vec2_dot2_sw(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2) SHZ_NOEXCEPT {
    shz_vec2_t res = shz_vec2_init(shz_vec2_dot(l, r1),
                                   shz_vec2_dot(l, r2));

    return res;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_dot3_sw(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2, shz_vec2_t r3) SHZ_NOEXCEPT {
    shz_vec3_t res = shz_vec3_init(shz_vec2_dot(l, r1),
                                   shz_vec2_dot(l, r2),
                                   shz_vec2_dot(l, r3));

    return res;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec3_dot2_sw(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2) SHZ_NOEXCEPT {
    shz_vec2_t res = shz_vec2_init(shz_vec3_dot(l, r1),
                                   shz_vec3_dot(l, r2));
    return res;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_dot3_sw(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT {
    shz_vec3_t res = shz_vec3_init(shz_vec3_dot(l, r1),
                                   shz_vec3_dot(l, r2),
                                   shz_vec3_dot(l, r3));
    return res;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec4_dot2_sw(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2) SHZ_NOEXCEPT {
    shz_vec2_t res = shz_vec2_init(shz_vec4_dot(l, r1),
                                   shz_vec4_dot(l, r2));
    return res;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec4_dot3_sw(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2, shz_vec4_t r3) SHZ_NOEXCEPT {
    shz_vec3_t res = shz_vec3_init(shz_vec4_dot(l, r1),
                                   shz_vec4_dot(l, r2),
                                   shz_vec4_dot(l, r3));
    return res;
}
//! \endcond

#endif
