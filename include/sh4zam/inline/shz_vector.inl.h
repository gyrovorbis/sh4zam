//! \cond INTERNAL
/*! \file
 *  \brief Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides the back-end inlined bridge implementation
 *  of the vector API routines, handling any constant expression
 *  evaluation, before forwarding the function calls on to the
 *  respective back-ends to perform the operation.
 *
 *  \todo
 *      - shz_vec_rotate(): for one-off rotations
 *
 *  \author 2025, 2026 Falco Girgis
 *  \author 2025 Paul Cercueil
 *
 *  \copyright MIT License
 */

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_vector_sh4.inl.h"
#else
#   include "sw/shz_vector_sw.inl.h"
#endif

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_init(float x, float y) SHZ_NOEXCEPT {
    return (shz_vec2_t){ .x = x, .y = y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_init(float x, float y, float z) SHZ_NOEXCEPT {
    return (shz_vec3_t){ .x = x, .y = y, .z = z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_init(float x, float y, float z, float w) SHZ_NOEXCEPT {
    return (shz_vec4_t){ .x = x, .y = y, .z = z, .w = w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_fill(float v) SHZ_NOEXCEPT {
    return shz_vec2_init(v, v);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_fill(float v) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_fill(v), v);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_fill(float v) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_fill(v), v);
}

 SHZ_FORCE_INLINE shz_vec2_t shz_vec2_swizzle(shz_vec2_t vec, unsigned x_idx, unsigned y_idx) SHZ_NOEXCEPT {
    return shz_vec2_init(vec.e[x_idx], vec.e[y_idx]);
 }

 SHZ_FORCE_INLINE shz_vec3_t shz_vec3_swizzle(shz_vec3_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx) SHZ_NOEXCEPT {
    return shz_vec3_init(vec.e[x_idx], vec.e[y_idx], vec.e[z_idx]);
 }

 SHZ_FORCE_INLINE shz_vec4_t shz_vec4_swizzle(shz_vec4_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx, unsigned w_idx) SHZ_NOEXCEPT {
    return shz_vec4_init(vec.e[x_idx], vec.e[y_idx], vec.e[z_idx], vec.e[w_idx]);
 }

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_vec3(shz_vec2_t vec, float z) SHZ_NOEXCEPT {
    return (shz_vec3_t){ .xy = vec, .z = z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec2_vec4(shz_vec2_t vec, float z, float w) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec2_vec3(vec, z), w);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec3_vec4(shz_vec3_t vec, float w) SHZ_NOEXCEPT {
    return (shz_vec4_t){ .xyz = vec, .w = w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x + vec2.x, vec1.y + vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_add(vec1.xy, vec2.xy), vec1.z + vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_add(vec1.xyz, vec2.xyz), vec1.w + vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x - vec2.x, vec1.y - vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_sub(vec1.xy, vec2.xy), vec1.z - vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_sub(vec1.xyz, vec2.xyz), vec1.w - vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x * vec2.x, vec1.y * vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_mul(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_mul(vec1.xy, vec2.xy), vec1.z * vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_mul(vec1.xyz, vec2.xyz), vec1.w * vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_divf(vec1.x, vec2.x), shz_divf(vec1.y, vec2.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_div(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_div(vec1.xy, vec2.xy), shz_divf(vec1.z, vec2.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_div(vec1.xyz, vec2.xyz), shz_divf(vec1.w, vec2.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_scale(shz_vec2_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec2_init(vec.x * factor, vec.y * factor);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_scale(shz_vec3_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_scale(vec.xy, factor), vec.z * factor);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_scale(shz_vec4_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_scale(vec.xyz, factor), vec.w * factor);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_sqr(shz_vec2_t vec) SHZ_NOEXCEPT {
    return vec.x * vec.x + vec.y * vec.y;
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_sqr(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr3f(vec.x, vec.y, vec.z);
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_sqr(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(vec.x, vec.y, vec.z, vec.w);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_vec4_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_inv(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_inv(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_inv(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf_fsrra(shz_vec4_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_scale(vec, shz_vec2_magnitude_inv(vec));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_scale(vec, shz_vec3_magnitude_inv(vec));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_scale(vec, shz_vec4_magnitude_inv(vec));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize_safe(shz_vec2_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec2_magnitude_sqr(vec);

    return (mag > 0.0f)? shz_vec2_scale(vec, shz_inv_sqrtf_fsrra(mag)) :
                         shz_vec2_fill(0.0f);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize_safe(shz_vec3_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec3_magnitude_sqr(vec);

    return (mag > 0.0f)? shz_vec3_scale(vec, shz_inv_sqrtf_fsrra(mag)) :
                         shz_vec3_fill(0.0f);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize_safe(shz_vec4_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec4_magnitude_sqr(vec);

    return (mag > 0.0f)? shz_vec4_scale(vec, shz_inv_sqrtf_fsrra(mag)) :
                         shz_vec4_fill(0.0f);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_abs(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_fabsf(vec.x), shz_fabsf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_abs(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_abs(vec.xy), shz_fabsf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_abs(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_abs(vec.xyz), shz_fabsf(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_neg(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(-vec.x, -vec.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_neg(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_neg(vec.xy), -vec.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_neg(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_neg(vec.xyz), -vec.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_inv(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_invf(vec.x), shz_invf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_inv(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_inv(vec.xy), shz_invf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_inv(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_inv(vec.xyz), shz_invf(vec.w));
}

SHZ_FORCE_INLINE float shz_vec2_max(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_fmaxf(vec.x, vec.y);
}

SHZ_FORCE_INLINE float shz_vec3_max(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_fmaxf(shz_vec2_max(vec.xy), vec.z);
}

SHZ_FORCE_INLINE float shz_vec4_max(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_fmaxf(shz_vec3_max(vec.xyz), vec.w);
}

SHZ_FORCE_INLINE float shz_vec2_min(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_fminf(vec.x, vec.y);
}

SHZ_FORCE_INLINE float shz_vec3_min(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_fminf(shz_vec2_min(vec.xy), vec.z);
}

SHZ_FORCE_INLINE float shz_vec4_min(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_fminf(shz_vec3_min(vec.xyz), vec.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_clamp(shz_vec2_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_clampf(vec.x, min, max), shz_clampf(vec.y, min, max));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_clamp(shz_vec3_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_clamp(vec.xy, min, max), shz_clampf(vec.z, min, max));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_clamp(shz_vec4_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_clamp(vec.xyz, min, max), shz_clampf(vec.w, min, max));
}

SHZ_FORCE_INLINE bool shz_vec2_equal(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT {
    return shz_equalf(a.x, b.x) && shz_equalf(a.y, b.y);
}

SHZ_FORCE_INLINE bool shz_vec3_equal(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT {
    return shz_vec2_equal(a.xy, b.xy) && shz_equalf(a.z, b.z);
}

SHZ_FORCE_INLINE bool shz_vec4_equal(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    return shz_vec3_equal(a.xyz, b.xyz) && shz_equalf(a.w, b.w);
}

SHZ_FORCE_INLINE float shz_vec2_dot(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

SHZ_FORCE_INLINE float shz_vec3_dot(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_dot6f(vec1.x, vec1.y, vec1.z,
                     vec2.x, vec2.y, vec2.z);
}

SHZ_FORCE_INLINE float shz_vec4_dot(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_dot8f(vec1.x, vec1.y, vec1.z, vec1.w,
                     vec2.x, vec2.y, vec2.z, vec2.w);
}

SHZ_FORCE_INLINE float shz_vec2_distance(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_magnitude(shz_vec2_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_distance(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_magnitude(shz_vec3_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec4_distance(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_magnitude(shz_vec4_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec2_distance_sqr(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_magnitude_sqr(shz_vec2_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_distance_sqr(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_magnitude_sqr(shz_vec3_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec4_distance_sqr(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_magnitude_sqr(shz_vec4_sub(vec1, vec2));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_lerp(shz_vec2_t a, shz_vec2_t b, float t) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_lerpf(a.x, b.x, t), shz_lerpf(a.y, b.y, t));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_lerp(shz_vec3_t a, shz_vec3_t b, float t) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_lerp(a.xy, b.xy, t), shz_lerpf(a.z, b.z, t));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_lerp(shz_vec4_t a, shz_vec4_t b, float t) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_lerp(a.xyz, b.xyz, t), shz_lerpf(a.w, b.w, t));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_reflect(shz_vec2_t incidence, shz_vec2_t normal) SHZ_NOEXCEPT {
    return shz_vec2_add(incidence, shz_vec2_scale(normal, shz_vec2_dot(normal, shz_vec2_scale(incidence, -2.0))));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_reflect(shz_vec3_t incidence, shz_vec3_t normal) SHZ_NOEXCEPT {
    return shz_vec3_add(incidence, shz_vec3_scale(normal, shz_vec3_dot(normal, shz_vec3_scale(incidence, -2.0))));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_reflect(shz_vec4_t incidence, shz_vec4_t normal) SHZ_NOEXCEPT {
    return shz_vec4_add(incidence, shz_vec4_scale(normal, shz_vec4_dot(normal, shz_vec4_scale(incidence, -2.0))));
}

SHZ_FORCE_INLINE float shz_vec2_cross(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cross(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_init(vec1.y * vec2.z - vec1.z * vec2.y,
                         vec1.z * vec2.x - vec1.x * vec2.z,
                         vec1.x * vec2.y - vec1.y * vec2.x);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT {
    return shz_vec2_scale(shz_vec2_normalize(vec), shz_vec2_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT {
    return shz_vec3_scale(shz_vec3_normalize(vec), shz_vec3_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT {
    return shz_vec4_scale(shz_vec4_normalize(vec), shz_vec4_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project_safe(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT {
    return shz_vec2_scale(shz_vec2_normalize_safe(vec), shz_vec2_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project_safe(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT {
    return shz_vec3_scale(shz_vec3_normalize_safe(vec), shz_vec3_dot(vec, onto));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project_safe(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT  {
    return shz_vec4_scale(shz_vec4_normalize_safe(vec), shz_vec4_dot(vec, onto));
}

SHZ_FORCE_INLINE float shz_vec2_angle_between(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_atan2f(shz_vec2_dot(vec1, vec2), shz_vec2_cross(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_angle_between(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_atan2f(shz_vec3_dot(vec1, vec2), shz_vec3_magnitude(shz_vec3_cross(vec1, vec2)));
}

SHZ_FORCE_INLINE float shz_vec2_angle(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_atan2f(vec.y, vec.x);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_angles(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_atan2f(shz_vec2_magnitude(shz_vec2_init(vec.y, vec.z)), vec.x),
                         shz_atan2f(shz_vec2_magnitude(shz_vec2_init(vec.z, vec.x)), vec.y),
                         shz_atan2f(shz_vec2_magnitude(shz_vec2_init(vec.x, vec.y)), vec.z));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_sincos(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_vec2_init(sincos.sin, sincos.cos);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_sincos(shz_sincos_t azimuth, shz_sincos_t elevation) SHZ_NOEXCEPT {
    return shz_vec3_init(azimuth.cos * elevation.cos, azimuth.sin * elevation.cos, elevation.sin);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle(float radians) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(radians);
    return shz_vec2_init(sc.cos, sc.sin);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_sincos(shz_sincosf(azimuth), shz_sincosf(elevation));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle_deg(float degrees) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf_deg(degrees);
    return shz_vec2_init(sc.cos, sc.sin);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles_deg(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_sincos(shz_sincosf_deg(azimuth), shz_sincosf_deg(elevation));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_rotate(shz_vec2_t vec, float radians) SHZ_NOEXCEPT {
    shz_sincos_t sincos = shz_sincosf(radians);

    return shz_vec2_init(vec.x * sincos.cos - vec.y * sincos.sin,
                         vec.x * sincos.sin + vec.y * sincos.cos);
}

SHZ_INLINE shz_vec2_t shz_vec2_refract(shz_vec2_t incidence, shz_vec2_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec2_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k <= 0.0f)
        return (k == 0.0f)? shz_vec2_scale(incidence, eta) :
                            shz_vec2_fill(0.0f);
    else
        return shz_vec2_sub(shz_vec2_scale(incidence, eta),
                            shz_vec2_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
}

SHZ_INLINE shz_vec3_t shz_vec3_refract(shz_vec3_t incidence, shz_vec3_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec3_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k <= 0.0f)
        return (k == 0.0f)? shz_vec3_scale(incidence, eta) :
                            shz_vec3_fill(0.0f);
    else
        return shz_vec3_sub(shz_vec3_scale(incidence, eta),
                            shz_vec3_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
}

SHZ_INLINE shz_vec4_t shz_vec4_refract(shz_vec4_t incidence, shz_vec4_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec4_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k <= 0.0f)
        return (k == 0.0f)? shz_vec4_scale(incidence, eta) :
                            shz_vec4_fill(0.0f);
    else
        return shz_vec4_sub(shz_vec4_scale(incidence, eta),
                            shz_vec4_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
}

SHZ_INLINE float shz_vec3_triple(shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec3_triple_sh4(a, b, c);
#else
    return shz_vec3_triple_sw(a, b, c);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_move(shz_vec2_t vec, shz_vec2_t target, float max_distance) SHZ_NOEXCEPT {
    shz_vec2_t delta   = shz_vec2_sub(target, vec);
    float      sqrdist = shz_vec2_magnitude_sqr(delta);

    if((max_distance >= 0.0f) && (sqrdist <= max_distance * max_distance))
        return target;

    float scaled_dist = shz_inv_sqrtf(sqrdist) * max_distance;

    return shz_vec2_add(vec, shz_vec2_scale(delta, scaled_dist));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_move(shz_vec3_t vec, shz_vec3_t target, float max_distance) SHZ_NOEXCEPT {
    shz_vec3_t delta   = shz_vec3_sub(target, vec);
    float      sqrdist = shz_vec3_magnitude_sqr(delta);

    if((max_distance >= 0.0f) && (sqrdist <= max_distance * max_distance))
        return target;

    float scaled_dist = shz_inv_sqrtf(sqrdist) * max_distance;

    return shz_vec3_add(vec, shz_vec3_scale(delta, scaled_dist));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_move(shz_vec4_t vec, shz_vec4_t target, float max_distance) SHZ_NOEXCEPT {
    shz_vec4_t delta   = shz_vec4_sub(target, vec);
    float      sqrdist = shz_vec4_magnitude_sqr(delta);

    if((max_distance >= 0.0f) && (sqrdist <= max_distance * max_distance))
        return target;

    float scaled_dist = shz_inv_sqrtf(sqrdist) * max_distance;

    return shz_vec4_add(vec, shz_vec4_scale(delta, scaled_dist));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_barycenter(shz_vec3_t p,
                                                shz_vec3_t a,
                                                shz_vec3_t b,
                                                shz_vec3_t c) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec3_barycenter_sh4(p, a, b, c);
#else
    return shz_vec3_barycenter_sw(p, a, b, c);
#endif
}

SHZ_INLINE shz_vec3_t shz_vec3_perp(shz_vec3_t vec) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_reject(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT {
    float v1dv2 = shz_vec3_dot(v1, v2);
    float v2dv2 = shz_vec3_magnitude_sqr(v2);
    float   mag = shz_divf_fsrra(v1dv2, v2dv2);

    return shz_vec3_sub(v1, shz_vec3_scale(v2, mag));
}

SHZ_INLINE void shz_vec3_orthonormalize(shz_vec3_t   in1, shz_vec3_t   in2,
                                        shz_vec3_t* out1, shz_vec3_t* out2) SHZ_NOEXCEPT {
    shz_vec3_t tmp;

    *out1 = shz_vec3_normalize_safe(in1);
      tmp = shz_vec3_normalize_safe(shz_vec3_cross(*out1, in2));
    *out2 = shz_vec3_cross(tmp, *out1);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cubic_hermite(shz_vec3_t vec1, shz_vec3_t tangent1, shz_vec3_t vec2, shz_vec3_t tangent2, float t) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec3_cubic_hermite_sh4(vec1, tangent1, vec2, tangent2, t);
#else
    return shz_vec3_cubic_hermite_sw(vec1, tangent1, vec2, tangent2, t);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_dot2(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec2_dot2_sh4(l, r1, r2);
#else
    return shz_vec2_dot2_sw(l, r1, r2);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_dot3(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2, shz_vec2_t r3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec2_dot3_sh4(l, r1, r2, r3);
#else
    return shz_vec2_dot3_sw(l, r1, r2, r3);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec3_dot2(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec3_dot2_sh4(l, r1, r2);
#else
    return shz_vec3_dot2_sw(l, r1, r2);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_dot3(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec3_dot3_sh4(l, r1, r2, r3);
#else
    return shz_vec3_dot3_sw(l, r1, r2, r3);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec4_dot2(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec4_dot2_sh4(l, r1, r2);
#else
    return shz_vec4_dot2_sw(l, r1, r2);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec4_dot3(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2, shz_vec4_t r3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_vec4_dot3_sh4(l, r1, r2, r3);
#else
    return shz_vec4_dot3_sw(l, r1, r2, r3);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_stepv(shz_vec2_t vec, shz_vec2_t edge) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_stepf(vec.x, edge.x),
                         shz_stepf(vec.y, edge.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_stepv(shz_vec3_t vec, shz_vec3_t edge) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_stepf(vec.x, edge.x),
                         shz_stepf(vec.y, edge.y),
                         shz_stepf(vec.z, edge.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_stepv(shz_vec4_t vec, shz_vec4_t edge) SHZ_NOEXCEPT {
    return shz_vec4_init(shz_stepf(vec.x, edge.x),
                         shz_stepf(vec.y, edge.y),
                         shz_stepf(vec.z, edge.z),
                         shz_stepf(vec.w, edge.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_step(shz_vec2_t vec, float edge) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_stepf(vec.x, edge),
                         shz_stepf(vec.y, edge));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_step(shz_vec3_t vec, float edge) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_stepf(vec.x, edge),
                         shz_stepf(vec.y, edge),
                         shz_stepf(vec.z, edge));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_step(shz_vec4_t vec, float edge) SHZ_NOEXCEPT {
    return shz_vec4_init(shz_stepf(vec.x, edge),
                         shz_stepf(vec.y, edge),
                         shz_stepf(vec.z, edge),
                         shz_stepf(vec.w, edge));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstepv(shz_vec2_t vec, shz_vec2_t edge0, shz_vec2_t edge1) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_smoothstepf(vec.x, edge0.x, edge1.x),
                         shz_smoothstepf(vec.y, edge0.y, edge1.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstepv(shz_vec3_t vec, shz_vec3_t edge0, shz_vec3_t edge1) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_smoothstepf(vec.x, edge0.x, edge1.x),
                         shz_smoothstepf(vec.y, edge0.y, edge1.y),
                         shz_smoothstepf(vec.z, edge0.z, edge1.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstepv(shz_vec4_t vec, shz_vec4_t edge0, shz_vec4_t edge1) SHZ_NOEXCEPT {
    return shz_vec4_init(shz_smoothstepf(vec.x, edge0.x, edge1.x),
                         shz_smoothstepf(vec.y, edge0.y, edge1.y),
                         shz_smoothstepf(vec.z, edge0.z, edge1.z),
                         shz_smoothstepf(vec.w, edge0.w, edge1.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstep(shz_vec2_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_smoothstepf(vec.x, edge0, edge1),
                         shz_smoothstepf(vec.y, edge0, edge1));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstep(shz_vec3_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_smoothstepf(vec.x, edge0, edge1),
                         shz_smoothstepf(vec.y, edge0, edge1),
                         shz_smoothstepf(vec.z, edge0, edge1));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstep(shz_vec4_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec4_init(shz_smoothstepf(vec.x, edge0, edge1),
                         shz_smoothstepf(vec.y, edge0, edge1),
                         shz_smoothstepf(vec.z, edge0, edge1),
                         shz_smoothstepf(vec.w, edge0, edge1));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstepv_safe(shz_vec2_t vec, shz_vec2_t edge0, shz_vec2_t edge1) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_smoothstepf_safe(vec.x, edge0.x, edge1.x),
                         shz_smoothstepf_safe(vec.y, edge0.y, edge1.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstepv_safe(shz_vec3_t vec, shz_vec3_t edge0, shz_vec3_t edge1) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_smoothstepf_safe(vec.x, edge0.x, edge1.x),
                         shz_smoothstepf_safe(vec.y, edge0.y, edge1.y),
                         shz_smoothstepf_safe(vec.z, edge0.z, edge1.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstepv_safe(shz_vec4_t vec, shz_vec4_t edge0, shz_vec4_t edge1) SHZ_NOEXCEPT {
    return shz_vec4_init(shz_smoothstepf_safe(vec.x, edge0.x, edge1.x),
                         shz_smoothstepf_safe(vec.y, edge0.y, edge1.y),
                         shz_smoothstepf_safe(vec.z, edge0.z, edge1.z),
                         shz_smoothstepf_safe(vec.w, edge0.w, edge1.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstep_safe(shz_vec2_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_smoothstepf_safe(vec.x, edge0, edge1),
                         shz_smoothstepf_safe(vec.y, edge0, edge1));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstep_safe(shz_vec3_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_smoothstepf_safe(vec.x, edge0, edge1),
                         shz_smoothstepf_safe(vec.y, edge0, edge1),
                         shz_smoothstepf_safe(vec.z, edge0, edge1));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstep_safe(shz_vec4_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec4_init(shz_smoothstepf_safe(vec.x, edge0, edge1),
                         shz_smoothstepf_safe(vec.y, edge0, edge1),
                         shz_smoothstepf_safe(vec.z, edge0, edge1),
                         shz_smoothstepf_safe(vec.w, edge0, edge1));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_floor(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_floorf(vec.x), shz_floorf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_floor(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_floor(vec.xy), shz_floorf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_floor(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_floor(vec.xyz), shz_floorf(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_ceil(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_ceilf(vec.x), shz_ceilf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_ceil(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_ceil(vec.xy), shz_ceilf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_ceil(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_ceil(vec.xyz), shz_ceilf(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_round(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_roundf(vec.x), shz_roundf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_round(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_round(vec.xy), shz_roundf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_round(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_round(vec.xyz), shz_roundf(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_fract(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_fractf(vec.x), shz_fractf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_fract(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_fract(vec.xy), shz_fractf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_fract(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_fract(vec.xyz), shz_fractf(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sign(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_signf(vec.x), shz_signf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sign(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_sign(vec.xy), shz_signf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sign(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_sign(vec.xyz), shz_signf(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_saturate(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_saturatef(vec.x), shz_saturatef(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_saturate(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_saturate(vec.xy), shz_saturatef(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_saturate(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_saturate(vec.xyz), shz_saturatef(vec.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_minv(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_fminf(a.x, b.x), shz_fminf(a.y, b.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_minv(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_minv(a.xy, b.xy), shz_fminf(a.z, b.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_minv(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_minv(a.xyz, b.xyz), shz_fminf(a.w, b.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_maxv(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_fmaxf(a.x, b.x), shz_fmaxf(a.y, b.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_maxv(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_maxv(a.xy, b.xy), shz_fmaxf(a.z, b.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_maxv(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_maxv(a.xyz, b.xyz), shz_fmaxf(a.w, b.w));
}

 //! \endcond
 