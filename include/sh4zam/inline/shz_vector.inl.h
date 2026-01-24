//! \cond INTERNAL
/*! \file
 *  \brief Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides types and mathematical routines for representing and
 *  operating on vectors.
 *
 *  \todo
 *      - shz_vec_rotate(): for one-off rotations
 *
 *  \author 2025, 2026 Falco Girgis
 *  \author 2025 Paul Cercueil
 *
 *  \copyright MIT License
 */

 #include "../shz_mem.h"

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

SHZ_FORCE_INLINE float shz_vec4_magnitude_sqr(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(vec.x, vec.y, vec.z, vec.w);
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_sqr(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr3f(vec.x, vec.y, vec.z);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf_fsrra(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_vec4_magnitude_sqr(vec));
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

SHZ_FORCE_INLINE float shz_vec4_dot(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_dot8f(vec1.x, vec1.y, vec1.z, vec1.w,
                     vec2.x, vec2.y, vec2.z, vec2.w);
}

SHZ_FORCE_INLINE float shz_vec3_dot(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_dot6f(vec1.x, vec1.y, vec1.z,
                     vec2.x, vec2.y, vec2.z);
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
    return shz_vec2_from_sincos(shz_sincosf(radians));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_sincos(shz_sincosf(azimuth), shz_sincosf(elevation));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle_deg(float degrees) SHZ_NOEXCEPT {
    return shz_vec2_from_sincos(shz_sincosf_deg(degrees));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles_deg(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_sincos(shz_sincosf_deg(azimuth), shz_sincosf_deg(elevation));
}


SHZ_FORCE_INLINE shz_vec2_t shz_vec2_rotate(shz_vec2_t vec, float radians) SHZ_NOEXCEPT {
#if 0 // GOD DAMNIT, SO CLOSE TO BEATING YOUR ASS, GCC!!
    //shz_sincos_t sincos = shz_sincosf(radians);
    shz_vec2_t res;

    register float vx asm("fr4") = vec.x;
    register float vy asm("fr5") = vec.y;
    register float rd asm("fr6") = radians * SHZ_FSCA_RAD_FACTOR;
    register float r2 asm("fr3");
    register float r1 asm("fr11");

    asm (R"(
    .align 2
        ftrc    fr6, fpul

        fsca    fpul, dr0
        fldi0   fr6
        fldi0   fr7
        fmov    fr0, fr9

        fipr    fv4, fv0
        fmov    fr1, fr8
        fneg    fr9

        fipr    fv4, fv8

    )"
    : [r1] "=f" (r1), [r2] "=f" (r2), [rd] "+&f" (rd)
    : [vx] "f" (vx), [vy] "f" (vy)
    : "fr0", "fr1", "fr7", "fr8", "fr9", "fpul");
    SHZ_MEMORY_BARRIER_SOFT();
    res.y = r2;
    SHZ_MEMORY_BARRIER_SOFT();
    res.x = r1;

    return res;
#else
    shz_sincos_t sincos = shz_sincosf(radians);

    return shz_vec2_init(vec.x * sincos.cos - vec.y * sincos.sin,
                         vec.x * sincos.sin + vec.y * sincos.cos);
#endif
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
    register float ax asm("fr8")  = a.x;
    register float bx asm("fr9")  = b.x;
    register float cx asm("fr10") = c.x;

    float dotPQ = shz_dot6f(ax, bx, cx, b.y * c.z, -c.y * a.z, -a.y * b.z);
    float dotRS = shz_dot6f(ax, bx, cx, c.y * b.z,  a.y * c.z,  b.y * a.z);

    return -(dotPQ + dotRS);
}

//! \todo The FIPR-y can still be better pipelined and optimized here.
SHZ_INLINE shz_vec3_t shz_vec3_barycenter(shz_vec3_t p,
                                          shz_vec3_t a,
                                          shz_vec3_t b,
                                          shz_vec3_t c) SHZ_NOEXCEPT {
    shz_vec3_t result;

    shz_vec3_t v0 = shz_vec3_sub(b, a);
    shz_vec3_t v1 = shz_vec3_sub(c, a);
    shz_vec3_t v2 = shz_vec3_sub(p, a);

    register float rx asm ("fr8")  = v0.x;
    register float ry asm ("fr9")  = v0.y;
    register float rz asm ("fr10") = v0.z;

    float d00 = shz_mag_sqr3f(rx, ry, rz);
    float d01 = shz_dot6f(rx, ry, rz, v1.x, v1.y, v1.z);
    float d02 = shz_dot6f(rx, ry, rz, v2.x, v2.y, v2.z);

    // Now hold V1 constant within the FV8 regs.
    rx = v1.x;
    ry = v1.y;
    rz = v1.z;

    float d11 = shz_mag_sqr3f(rx, ry, rz);
    float d12 = shz_dot6f(rx, ry, rz, v2.x, v2.y, v2.z);

    float denom = (d00 * d11) - (d01 * d01);

    /* Protect against divide-by-zero only in debug builds for now,
       cuz ain't nobody got the cycles to spare.*/
    assert(denom > 0.0f);

    /* We can mathematically prove that "denom" is guaranteed to be
       positive here, so we can abuse the fast reciprocal square root
       approximation instruction (FSRRA) to do a fast inversion. */
    float inv_denom = shz_invf_fsrra(denom);

    result.y = ((d11 * d02) - (d01 * d12)) * inv_denom;
    result.z = ((d00 * d12) - (d01 * d02)) * inv_denom;
    result.x = 1.0f - (result.z + result.y);

    return result;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_dot2(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2) SHZ_NOEXCEPT {
    shz_vec2_t res;

    register float lx asm("fr0") = l.x;
    register float ly asm("fr1") = l.y;
    register float lz asm("fr2") = 0.0f;
    register float lw asm("fr3") = 0.0f;

    register float r1x asm("fr4") = r1.x;
    register float r1y asm("fr5") = r1.y;
    register float r1w asm("fr7");

    register float r2x asm("fr8")  = r2.x;
    register float r2y asm("fr9")  = r2.y;
    register float r2w asm("fr10");

    asm("fipr   fv0, fv4\n"
        : "=f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y));

    asm("fipr   fv0, fv8\n"
        : "=f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y));

    //SHZ_MEMORY_BARRIER_HARD();
    res.x = r1w;
    //SHZ_MEMORY_BARRIER_HARD();
    res.y = r2w;

    return res;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_dot3(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2, shz_vec2_t r3) SHZ_NOEXCEPT {
    shz_vec3_t res;

    register float r1x asm("fr12") = r1.x;
    register float r1y asm("fr13") = r1.y;
    register float r1w asm("fr15");

    register float lx asm("fr8")  = l.x;
    register float ly asm("fr9")  = l.y;
    register float lz asm("fr10") = 0.0f;
    register float lw asm("fr11") = 0.0f;

    asm("fipr   fv8, fv12\n"
        : "=f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y));

    register float r2x asm("fr0")  = r2.x;
    register float r2y asm("fr1")  = r2.y;
    register float r2w asm("fr3");

    asm("fipr   fv8, fv0\n"
        : "=f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y));

    register float r3x asm("fr4")  = r3.x;
    register float r3y asm("fr5")  = r3.y;
    register float r3w asm("fr7");

    res.x = r1w;

    asm("fipr   fv8, fv4\n"
        : "=f" (r3w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r3x), "f" (r3y));

    SHZ_MEMORY_BARRIER_HARD();
    res.y = r2w;
    SHZ_MEMORY_BARRIER_HARD();
    res.z = r3w;

    return res;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec3_dot2(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2) SHZ_NOEXCEPT {
    shz_vec2_t res;

    register float lx asm("fr8")  = l.x;
    register float ly asm("fr9")  = l.y;
    register float lz asm("fr10") = l.z;
    register float lw asm("fr11") = 0.0f;

    register float r1x asm("fr12") = r1.x;
    register float r1y asm("fr13") = r1.y;
    register float r1z asm("fr14") = r1.z;
    register float r1w asm("fr15");

    register float r2x asm("fr0") = r2.x;
    register float r2y asm("fr1") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3");

    asm("fipr   fv8, fv12\n"
        : "=f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    asm("fipr   fv8, fv0\n"
        : "=f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    //SHZ_MEMORY_BARRIER_HARD();
    res.x = r1w;
    SHZ_MEMORY_BARRIER_HARD();
    res.y = r2w;

    return res;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_dot3(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT {
    shz_vec3_t res;

    register float lx asm("fr8")  = l.x;
    register float ly asm("fr9")  = l.y;
    register float lz asm("fr10") = l.z;
    register float lw asm("fr11") = 0.0f;

    register float r1x asm("fr12") = r1.x;
    register float r1y asm("fr13") = r1.y;
    register float r1z asm("fr14") = r1.z;
    register float r1w asm("fr15");

    asm("fipr   fv8, fv12\n"
        : "=f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    register float r2x asm("fr0") = r2.x;
    register float r2y asm("fr1") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3");

    asm("fipr   fv8, fv0\n"
        : "=f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    register float r3x asm("fr4") = r3.x;
    register float r3y asm("fr5") = r3.y;
    register float r3z asm("fr6") = r3.z;
    register float r3w asm("fr7");

    //SHZ_MEMORY_BARRIER_HARD();

    res.x = r1w;

    asm("fipr   fv8, fv4\n"
        : "=f" (r3w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r3x), "f" (r3y), "f" (r3z));

    SHZ_MEMORY_BARRIER_HARD();
    res.y = r2w;
    SHZ_MEMORY_BARRIER_HARD();
    res.z = r3w;

    return res;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec4_dot2(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2) SHZ_NOEXCEPT {
    shz_vec2_t res;

    register float lx asm("fr8")  = l.x;
    register float ly asm("fr9")  = l.y;
    register float lz asm("fr10") = l.z;
    register float lw asm("fr11") = l.w;

    register float r1x asm("fr12") = r1.x;
    register float r1y asm("fr13") = r1.y;
    register float r1z asm("fr14") = r1.z;
    register float r1w asm("fr15") = r1.w;

    register float r2x asm("fr0") = r2.x;
    register float r2y asm("fr1") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3") = r2.w;

    asm("fipr   fv8, fv12\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    asm("fipr   fv8, fv0\n"
        : "+f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    //SHZ_MEMORY_BARRIER_HARD();
    res.x = r1w;
    SHZ_MEMORY_BARRIER_HARD();
    res.y = r2w;

    return res;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec4_dot3(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2, shz_vec4_t r3) SHZ_NOEXCEPT {
    shz_vec3_t res;

    register float lx asm("fr8")  = l.x;
    register float ly asm("fr9")  = l.y;
    register float lz asm("fr10") = l.z;
    register float lw asm("fr11") = l.w;

    register float r1x asm("fr12") = r1.x;
    register float r1y asm("fr13") = r1.y;
    register float r1z asm("fr14") = r1.z;
    register float r1w asm("fr15") = r1.w;

    asm("fipr   fv8, fv12\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    register float r2x asm("fr0") = r2.x;
    register float r2y asm("fr1") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3") = r2.w;

    asm("fipr   fv8, fv0\n"
        : "+f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    register float r3x asm("fr4") = r3.x;
    register float r3y asm("fr5") = r3.y;
    register float r3z asm("fr6") = r3.z;
    register float r3w asm("fr7") = r3.w;

    SHZ_MEMORY_BARRIER_HARD();

    res.x = r1w;

    asm("fipr   fv8, fv4\n"
        : "+f" (r3w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r3x), "f" (r3y), "f" (r3z));

    SHZ_MEMORY_BARRIER_HARD();
    res.y = r2w;
    SHZ_MEMORY_BARRIER_HARD();
    res.z = r3w;

    return res;
}


 //! \endcond
 