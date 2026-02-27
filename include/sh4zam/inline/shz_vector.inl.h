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

#if SHZ_BACKEND == SHZ_Sh4
#   include "sh4/shz_vector_sh4.inl.h"
#endif

#include "sw/shz_vector_sw.inl.h"

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_init(float x, float y) SHZ_NOEXCEPT {
    return shz_vec2_init_sw(x, y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_init(float x, float y, float z) SHZ_NOEXCEPT {
    return shz_vec3_init_sw(x, y, z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_init(float x, float y, float z, float w) SHZ_NOEXCEPT {
    return shz_vec4_init_sw(x, y, z, w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_fill(float v) SHZ_NOEXCEPT {
    return shz_vec2_fill_sw(v);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_fill(float v) SHZ_NOEXCEPT {
    return shz_vec3_fill_sw(v);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_fill(float v) SHZ_NOEXCEPT {
    return shz_vec4_fill_sw(v);
}

 SHZ_FORCE_INLINE shz_vec2_t shz_vec2_swizzle(shz_vec2_t vec, unsigned x_idx, unsigned y_idx) SHZ_NOEXCEPT {
    return shz_vec2_swizzle_sw(vec, x_idx, y_idx);
 }

 SHZ_FORCE_INLINE shz_vec3_t shz_vec3_swizzle(shz_vec3_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx) SHZ_NOEXCEPT {
    return shz_vec3_swizzle_sw(vec, x_idx, y_idx, z_idx);
 }

 SHZ_FORCE_INLINE shz_vec4_t shz_vec4_swizzle(shz_vec4_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx, unsigned w_idx) SHZ_NOEXCEPT {
    return shz_vec4_swizzle_sw(vec, x_idx, y_idx, z_idx, w_idx);
 }

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_vec3(shz_vec2_t vec, float z) SHZ_NOEXCEPT {
    return shz_vec2_vec3_sw(vec, z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec2_vec4(shz_vec2_t vec, float z, float w) SHZ_NOEXCEPT {
    return shz_vec2_vec4_sw(vec, z, w);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec3_vec4(shz_vec3_t vec, float w) SHZ_NOEXCEPT {
    return shz_vec3_vec4_sw(vec, w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_add_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_add_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_add_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_sub_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_sub_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_sub_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_mul_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_mul(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_mul_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_mul_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_div_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_div(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_div_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_div_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_scale(shz_vec2_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec2_scale_sw(vec, factor);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_scale(shz_vec3_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec3_scale_sw(vec, factor);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_scale(shz_vec4_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec4_scale_sw(vec, factor);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_sqr(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_magnitude_sqr_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_sqr(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_magnitude_sqr_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_sqr(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_magnitude_sqr_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_magnitude_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec3_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_magnitude_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec4_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_magnitude_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_inv(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_magnitude_inv_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_inv(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_magnitude_inv_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_inv(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_magnitude_inv_sw(vec);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_normalize_sw(vec);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_normalize_sw(vec);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_normalize_sw(vec);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize_safe(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_normalize_safe_sw(vec);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize_safe(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_normalize_safe_sw(vec);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize_safe(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_normalize_safe_sw(vec);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_abs(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_abs_sw(vec);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_abs(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_abs_sw(vec);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_abs(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_abs_sw(vec);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_neg(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_neg_sw(vec);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_neg(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_neg_sw(vec);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_neg(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_neg_sw(vec);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_inv(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_inv_sw(vec);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_inv(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_inv_sw(vec);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_inv(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_inv_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec2_max(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_max_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec3_max(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_max_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec4_max(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_max_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec2_min(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_min_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec3_min(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_min_sw(vec);
}

SHZ_FORCE_INLINE float shz_vec4_min(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec4_min_sw(vec);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_clamp(shz_vec2_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec2_clamp_sw(vec, min, max);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_clamp(shz_vec3_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec3_clamp_sw(vec, min, max);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_clamp(shz_vec4_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec4_clamp_sw(vec, min, max);
}

SHZ_FORCE_INLINE bool shz_vec2_equal(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT {
    return shz_vec2_equal_sw(a, b);
}

SHZ_FORCE_INLINE bool shz_vec3_equal(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT {
    return shz_vec3_equal_sw(a, b);
}

SHZ_FORCE_INLINE bool shz_vec4_equal(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT {
    return shz_vec4_equal_sw(a, b);
}

SHZ_FORCE_INLINE float shz_vec2_dot(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_dot_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec3_dot(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_dot_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec4_dot(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_dot_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec2_distance(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_distance_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec3_distance(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_distance_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec4_distance(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_distance_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec2_distance_sqr(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_distance_sqr_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec3_distance_sqr(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_distance_sqr_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec4_distance_sqr(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_distance_sqr_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_lerp(shz_vec2_t a, shz_vec2_t b, float t) SHZ_NOEXCEPT {
    return shz_vec2_lerp_sw(a, b, t);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_lerp(shz_vec3_t a, shz_vec3_t b, float t) SHZ_NOEXCEPT {
    return shz_vec3_lerp_sw(a, b, t);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_lerp(shz_vec4_t a, shz_vec4_t b, float t) SHZ_NOEXCEPT {
    return shz_vec4_lerp_sw(a, b, t);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_reflect(shz_vec2_t incidence, shz_vec2_t normal) SHZ_NOEXCEPT {
    return shz_vec2_reflect_sw(incidence, normal);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_reflect(shz_vec3_t incidence, shz_vec3_t normal) SHZ_NOEXCEPT {
    return shz_vec3_reflect_sw(incidence, normal);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_reflect(shz_vec4_t incidence, shz_vec4_t normal) SHZ_NOEXCEPT {
    return shz_vec4_reflect_sw(incidence, normal);
}

SHZ_FORCE_INLINE float shz_vec2_cross(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_cross_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cross(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_cross_sw(vec1, vec2);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT {
    return shz_vec2_project_sw(vec, onto);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT {
    return shz_vec3_project_sw(vec, onto);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT {
    return shz_vec4_project_sw(vec, onto);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project_safe(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT {
    return shz_vec2_project_safe_sw(vec, onto);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project_safe(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT {
    return shz_vec3_project_safe_sw(vec, onto);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project_safe(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT  {
    return shz_vec4_project_safe_sw(vec, onto);
}

SHZ_FORCE_INLINE float shz_vec2_angle_between(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_angle_between_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec3_angle_between(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_angle_between_sw(vec1, vec2);
}

SHZ_FORCE_INLINE float shz_vec2_angle(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_angle_sw(vec);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_angles(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_angles_sw(vec);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_sincos(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return shz_vec2_from_sincos_sw(sincos);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_sincos(shz_sincos_t azimuth, shz_sincos_t elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_sincos_sw(azimuth, elevation);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle(float radians) SHZ_NOEXCEPT {
    return shz_vec2_from_angle_sw(radians);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_angles_sw(azimuth, elevation);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle_deg(float degrees) SHZ_NOEXCEPT {
    return shz_vec2_from_angle_deg_sw(degrees);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles_deg(float azimuth, float elevation) SHZ_NOEXCEPT {
    return shz_vec3_from_angles_deg_sw(azimuth, elevation);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_rotate(shz_vec2_t vec, float radians) SHZ_NOEXCEPT {
    return shz_vec2_rotate_sw(vec, radians);
}

SHZ_INLINE shz_vec2_t shz_vec2_refract(shz_vec2_t incidence, shz_vec2_t normal, float eta) SHZ_NOEXCEPT {
    return shz_vec2_refract_sw(incidence, normal, eta);
}

SHZ_INLINE shz_vec3_t shz_vec3_refract(shz_vec3_t incidence, shz_vec3_t normal, float eta) SHZ_NOEXCEPT {
    return shz_vec3_refract_sw(incidence, normal, eta);
}

SHZ_INLINE shz_vec4_t shz_vec4_refract(shz_vec4_t incidence, shz_vec4_t normal, float eta) SHZ_NOEXCEPT {
    return shz_vec4_refract_sw(incidence, normal, eta);
}

SHZ_INLINE float shz_vec3_triple(shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec3_triple_dc(a, b, c);
#else
    return shz_vec3_triple_sw(a, b, c);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_move(shz_vec2_t vec, shz_vec2_t target, float max_distance) SHZ_NOEXCEPT {
    return shz_vec2_move_sw(vec, target, max_distance);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_move(shz_vec3_t vec, shz_vec3_t target, float max_distance) SHZ_NOEXCEPT {
    return shz_vec3_move_sw(vec, target, max_distance);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_move(shz_vec4_t vec, shz_vec4_t target, float max_distance) SHZ_NOEXCEPT {
    return shz_vec4_move_sw(vec, target, max_distance);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_barycenter(shz_vec3_t p,
                                                shz_vec3_t a,
                                                shz_vec3_t b,
                                                shz_vec3_t c) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec3_barycenter_dc(p, a, b, c);
#else
    return shz_vec3_barycenter_sw(p, a, b, c);
#endif
}

SHZ_INLINE shz_vec3_t shz_vec3_perp(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_perp_sw(vec);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_reject(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT {
    return shz_vec3_reject_sw(v1, v2);
}

SHZ_INLINE void shz_vec3_orthonormalize(shz_vec3_t   in1, shz_vec3_t   in2,
                                        shz_vec3_t* out1, shz_vec3_t* out2) SHZ_NOEXCEPT {
    return shz_vec3_orthonormalize_sw(in1, in2, out1, out2);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cubic_hermite(shz_vec3_t vec1, shz_vec3_t tangent1, shz_vec3_t vec2, shz_vec3_t tangent2, float t) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec3_cubic_hermite_dc(vec1, tangent1, vec2, tangent2, t);
#else
    return shz_vec3_cubic_hermite_sw(vec1, tangent1, vec2, tangent2, t);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_dot2(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec2_dot2_dc(l, r1, r2);
#else
    return shz_vec2_dot2_sw(l, r1, r2);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_dot3(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2, shz_vec2_t r3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec2_dot3_dc(l, r1, r2, r3);
#else
    return shz_vec2_dot3_sw(l, r1, r2, r3);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec3_dot2(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec3_dot2_dc(l, r1, r2);
#else
    return shz_vec3_dot2_sw(l, r1, r2);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_dot3(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec3_dot3_dc(l, r1, r2, r3);
#else
    return shz_vec3_dot3_sw(l, r1, r2, r3);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec4_dot2(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec4_dot2_dc(l, r1, r2);
#else
    return shz_vec4_dot2_sw(l, r1, r2);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec4_dot3(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2, shz_vec4_t r3) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_DREAMCAST
    return shz_vec4_dot3_dc(l, r1, r2, r3);
#else
    return shz_vec4_dot3_sw(l, r1, r2, r3);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_stepv(shz_vec2_t vec, shz_vec2_t edge) SHZ_NOEXCEPT {
    return shz_vec2_stepv_sw(vec, edge);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_stepv(shz_vec3_t vec, shz_vec3_t edge) SHZ_NOEXCEPT {
    return shz_vec3_stepv_sw(vec, edge);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_stepv(shz_vec4_t vec, shz_vec4_t edge) SHZ_NOEXCEPT {
    return shz_vec4_stepv_sw(vec, edge);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_step(shz_vec2_t vec, float edge) SHZ_NOEXCEPT {
    return shz_vec2_step_sw(vec, edge);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_step(shz_vec3_t vec, float edge) SHZ_NOEXCEPT {
    return shz_vec3_step_sw(vec, edge);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_step(shz_vec4_t vec, float edge) SHZ_NOEXCEPT {
    return shz_vec4_step_sw(vec, edge);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstepv(shz_vec2_t vec, shz_vec2_t edge0, shz_vec2_t edge1) SHZ_NOEXCEPT {
    return shz_vec2_smoothstepv_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstepv(shz_vec3_t vec, shz_vec3_t edge0, shz_vec3_t edge1) SHZ_NOEXCEPT {
    return shz_vec3_smoothstepv_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstepv(shz_vec4_t vec, shz_vec4_t edge0, shz_vec4_t edge1) SHZ_NOEXCEPT {
    return shz_vec4_smoothstepv_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstep(shz_vec2_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec2_smoothstep_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstep(shz_vec3_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec3_smoothstep_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstep(shz_vec4_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec4_smoothstep_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstepv_safe(shz_vec2_t vec, shz_vec2_t edge0, shz_vec2_t edge1) SHZ_NOEXCEPT {
    return shz_vec2_smoothstepv_safe_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstepv_safe(shz_vec3_t vec, shz_vec3_t edge0, shz_vec3_t edge1) SHZ_NOEXCEPT {
    return shz_vec3_smoothstepv_safe_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstepv_safe(shz_vec4_t vec, shz_vec4_t edge0, shz_vec4_t edge1) SHZ_NOEXCEPT {
    return shz_vec4_smoothstepv_safe_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_smoothstep_safe(shz_vec2_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec2_smoothstep_safe_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_smoothstep_safe(shz_vec3_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec3_smoothstep_safe_sw(vec, edge0, edge1);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_smoothstep_safe(shz_vec4_t vec, float edge0, float edge1) SHZ_NOEXCEPT {
    return shz_vec4_smoothstep_safe_sw(vec, edge0, edge1);
}

 //! \endcond
 