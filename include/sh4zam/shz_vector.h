/*! \file
 *  \brief Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides types and mathematical functions for representing and
 *  operating on vectors.
 *
 *  \author Falco Girgis
 *  \author Paul Cercueil
 */
#ifndef SHZ_VECTOR_H
#define SHZ_VECTOR_H

#include <math.h>
#include <float.h>

#ifdef __cplusplus
#   include <concepts>
#endif

#include "shz_scalar.h"
#include "shz_trig.h"

/** \defgroup vector Vector
 *  \brief           API for vector math.
 *
 *  The Vector API provides structures representing commonly-used vector types
 *  as well as routines implementing various mathematical operations on them.
 */

SHZ_DECLS_BEGIN

/*! 2D Vector type
 *
 *  Structure for holding coordinates of a 2-dimensional vector.
 *
 * \sa shz_vec3_t, shz_vec4_t
 */
typedef struct shz_vec2 {
    union {
        struct {
            float x; //!< X coordinate
            float y; //!< Y coordinate
        };
        float e[2];  //!< <X, Y> coordinates as an array
    };
} shz_vec2_t;

/*! 3D Vector type
 *
 *  Structure for holding coordinates of a 3-dimensional vector.
 *
 * \sa shz_vec2_t, shz_vec4_t
 */
typedef struct shz_vec3 {
    union {
        struct {
            union {
                struct {
                    float x;     //!< X coordinate
                    float y;     //!< Y coordinate
                };
                shz_vec2_t vec2; //!< Inner 2D vector containing <X, Y> coords
            };
            float z;             //!< Z coordinate
        };
        float e[3];              //!< <X, Y, Z> coordinates as an array
    };
} shz_vec3_t;

/*! 4D Vector type
 *
 *  Structure for holding coordinates of a 4-dimensional vector.
 *
 *  \sa shz_vec2_t, shz_vec3_t
 */
typedef struct shz_vec4 {
    union {
        struct {
            union {
                struct {
                    union {
                        struct {
                            float x;     //!< X coordinate
                            float y;     //!< Y coordinate
                        };
                        shz_vec2_t vec2; //!< Inner 2D vector containing <X, Y> coords
                    };
                    float z;             //!< Z coordinate
                };
                shz_vec3_t vec3;         //!< Inner 3D vector containing <X, Y, Z> coords
            };
            float w;                     //!< W coordinate
        };
        float e[4];                      //!< <X, Y, Z, W> coordinates as an array
    };
} shz_vec4_t;

/* Simple arithmetic */

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return (shz_vec2_t) { vec1.x + vec2.x, vec1.y + vec2.y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return (shz_vec3_t) { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return (shz_vec4_t) { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return (shz_vec2_t) { vec1.x - vec2.x, vec1.y - vec2.y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return (shz_vec3_t) { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return (shz_vec4_t) { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return (shz_vec2_t) { vec1.x * vec2.x, vec1.y * vec2.y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_mul(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return (shz_vec3_t) { vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return (shz_vec4_t) { vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return (shz_vec2_t) { vec1.x * shz_fast_invf(vec2.x), vec1.y * shz_fast_invf(vec2.y) };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_div(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return (shz_vec3_t) { vec1.x * shz_fast_invf(vec2.x), vec1.y * shz_fast_invf(vec2.y), vec1.z * shz_fast_invf(vec2.z) };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return (shz_vec4_t) { vec1.x * shz_fast_invf(vec2.x), vec1.y * shz_fast_invf(vec2.y), vec1.z * shz_fast_invf(vec2.z), vec1.w * shz_fast_invf(vec2.w) };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_scale(shz_vec2_t vec, float factor) SHZ_NOEXCEPT {
    return (shz_vec2_t) { vec.x * factor, vec.y * factor };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_scale(shz_vec3_t vec, float factor) SHZ_NOEXCEPT {
    return (shz_vec3_t) { vec.x * factor, vec.y * factor, vec.z * factor };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_scale(shz_vec4_t vec, float factor) SHZ_NOEXCEPT {
    return (shz_vec4_t) { vec.x * factor, vec.y * factor, vec.z * factor, vec.w * factor };
}

/* 3D math */
SHZ_FORCE_INLINE float shz_vec2_dot(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

SHZ_FORCE_INLINE float shz_vec4_dot(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_dot8f(vec1.x, vec1.y, vec1.z, vec1.w,
                     vec2.x, vec2.y, vec2.z, vec2.w);
}

SHZ_FORCE_INLINE float shz_vec3_dot(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_dot((shz_vec4_t){ .vec3 = vec1 }, (shz_vec4_t){ .vec3 = vec2 });
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_sqr(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_dot(vec, vec);
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_sqr(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(vec.x, vec.y, vec.z, vec.w);
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_sqr(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(vec.x, vec.y, vec.z, 0.0f);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_vec4_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_inv(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_inverse_sqrtf(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_inv(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_inverse_sqrtf(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_inv(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_inverse_sqrtf(shz_vec4_magnitude_sqr(vec));
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

    return (mag != 0.0f)?
        shz_vec2_scale(vec, shz_inverse_sqrtf(mag)) :
        (shz_vec2_t) { 0.0f, 1.0f };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize_safe(shz_vec3_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec3_magnitude_sqr(vec);

    return (mag != 0.0f)?
        shz_vec3_scale(vec, shz_inverse_sqrtf(mag)) :
        (shz_vec3_t) { 0.0f, 0.0f, 1.0f };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize_safe(shz_vec4_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec4_magnitude_sqr(vec);

    return (mag != 0.0f)?
        shz_vec4_scale(vec, shz_inverse_sqrtf(mag)) :
        (shz_vec4_t) { 0.0f, 0.0f, 0.0f, 1.0f };
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
    return (shz_vec2_t) {
        shz_lerpf(a.x, b.x, t),
        shz_lerpf(a.y, b.y, t)
    };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_lerp(shz_vec3_t a, shz_vec3_t b, float t) SHZ_NOEXCEPT {
    return (shz_vec3_t) {
        shz_lerpf(a.x, b.x, t),
        shz_lerpf(a.y, b.y, t),
        shz_lerpf(a.z, b.z, t)
    };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_lerp(shz_vec4_t a, shz_vec4_t b, float t) SHZ_NOEXCEPT {
    return (shz_vec4_t) {
        shz_lerpf(a.x, b.x, t),
        shz_lerpf(a.y, b.y, t),
        shz_lerpf(a.z, b.z, t),
        shz_lerpf(a.w, b.w, t)
    };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_reflect(shz_vec2_t incidence, shz_vec2_t normal) SHZ_NOEXCEPT  {
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
    return (shz_vec3_t) {
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec1.x
    };
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

SHZ_FORCE_INLINE float shz_vec2_angle(shz_vec2_t vec) SHZ_NOEXCEPT {
    return atan2f(vec.y, vec.x);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_angles(shz_vec3_t vec) SHZ_NOEXCEPT {
    return (shz_vec3_t) {
        atan2f(shz_vec2_magnitude((shz_vec2_t) { vec.y, vec.z }), vec.x),
        atan2f(shz_vec2_magnitude((shz_vec2_t) { vec.z, vec.x }), vec.y),
        atan2f(shz_vec2_magnitude((shz_vec2_t) { vec.x, vec.y }), vec.z)
    };
}

SHZ_FORCE_INLINE float shz_vec2_angle_between(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return atan2f(shz_vec2_dot(vec1, vec2), shz_vec2_cross(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_angle_between(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return atan2f(shz_vec3_dot(vec1, vec2), shz_vec3_magnitude(shz_vec3_cross(vec1, vec2)));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_sincos(shz_sincos_t sincos) SHZ_NOEXCEPT {
    return (shz_vec2_t) { sincos.sin, sincos.cos };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_sincos(shz_sincos_t azimuth, shz_sincos_t elevation) SHZ_NOEXCEPT {
    return (shz_vec3_t) { azimuth.cos * elevation.cos, azimuth.sin * elevation.cos, elevation.sin };
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

SHZ_DECLS_END

#ifndef __cplusplus
#   define shz_vec_add(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_add, \
                 shz_vec3_t: shz_vec3_add, \
                 shz_vec4_t: shz_vec4_add)(vec1, vec2)

#   define shz_vec_sub(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_sub, \
                 shz_vec3_t: shz_vec3_sub, \
                 shz_vec4_t: shz_vec4_sub)(vec1, vec2)

#   define shz_vec_mul(vec1, vec2) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_mul, \
                 shz_vec3_t: shz_vec3_mul, \
                 shz_vec4_t: shz_vec4_mul)(vec, vec2)

#   define shz_vec_div(vec1, vec2) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_div, \
                 shz_vec3_t: shz_vec3_div, \
                 shz_vec4_t: shz_vec4_div)(vec1, vec2)

#   define shz_vec_scale(vec, factor) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_scale, \
                 shz_vec3_t: shz_vec3_scale, \
                 shz_vec4_t: shz_vec4_div)(vec, factor)

#   define shz_vec_dot(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_dot, \
                 shz_vec3_t: shz_vec3_dot, \
                 shz_vec4_t: shz_vec4_dot)(vec1, vec2)

#   define shz_vec_magnitude_sqr(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_magnitude_sqr, \
                 shz_vec3_t: shz_vec3_magnitude_sqr, \
                 shz_vec4_t: shz_vec4_magnitude_sqr)(vec)

#   define shz_vec_magnitude(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_magnitude, \
                 shz_vec3_t: shz_vec3_magnitude, \
                 shz_vec4_t: shz_vec4_magnitude)(vec)

#   define shz_vec_magnitude_inv(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_magnitude_inv, \
                 shz_vec3_t: shz_vec3_magnitude_inv, \
                 shz_vec4_t: shz_vec4_magnitude_inv)(vec)

#   define shz_vec_normalize(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_normalize, \
                 shz_vec3_t: shz_vec3_normalize, \
                 shz_vec4_t: shz_vec4_normalize)(vec)

#   define shz_vec_normalize_safe(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_normalize_safe, \
                 shz_vec3_t: shz_vec3_normalize_safe, \
                 shz_vec4_t: shz_vec4_normalize_safe)(vec)

#   define shz_vec_distance(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_distance, \
                 shz_vec3_t: shz_vec3_distance, \
                 shz_vec4_t: shz_vec4_distance)(vec1, vec2)

#   define shz_vec_distance_sqr(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_distance_sqr, \
                 shz_vec3_t: shz_vec3_distance_sqr, \
                 shz_vec4_t: shz_vec4_distance_sqr)(vec1, vec2)

#   define shz_vec_lerp(vec1, vec2, t) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_lerp, \
                 shz_vec3_t: shz_vec3_lerp, \
                 shz_vec4_t: shz_vec4_lerp)(vec1, vec2, t)

#   define shz_vec_reflect(incidence, normal) \
        _Generic((incidence), \
                 shz_vec2_t: shz_vec2_reflect, \
                 shz_vec3_t: shz_vec3_reflect, \
                 shz_vec4_t: shz_vec4_reflect)(incidence, normal)

#   define shz_vec_cross(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_cross, \
                 shz_vec3_t: shz_vec3_cross)(vec1, vec2)

#   define shz_vec_project(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_project, \
                 shz_vec3_t: shz_vec3_project, \
                 shz_vec4_t: shz_vec4_project)(vec1, vec2)

#   define shz_vec_project_safe(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_project_safe, \
                 shz_vec3_t: shz_vec3_project_safe, \
                 shz_vec4_t: shz_vec4_project_safe)(vec1, vec2)

#   define shz_vec_angles(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_angle, \
                 shz_vec3_t: shz_vec3_angles)(vec)

#   define shz_vec_angle_between(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_angle_between, \
                 shz_vec3_t: shz_vec3_angle_between)(vec1, vec2)

#else
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_add(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_add(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_add(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_add(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_sub(T vec1, T vec2) SHZ_NOEXCEPT  {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_sub(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_sub(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_sub(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_mul(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_mul(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_mul(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_mul(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_div(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_div(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_div(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_div(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_scale(T vec, float factor) SHZ_NOEXCEPT  {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_scale(vec, factor);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_scale(vec, factor);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_scale(vec, factor);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_dot(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_dot(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_scale(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_scale(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    SHZ_FORCE_INLINE float shz_vec_magnitude_sqr(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_magnitude_sqr(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_magnitude_sqr(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_magnitude_sqr(vec);
        else static_assert(false, "Incompatible type!");
    }

    SHZ_FORCE_INLINE float shz_vec_magnitude(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_magnitude(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_magnitude(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_magnitude(vec);
        else static_assert(false, "Incompatible type!");
    }

    SHZ_FORCE_INLINE float shz_vec_magnitude_inv(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_magnitude_inv(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_magnitude_inv(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_magnitude_inv(vec);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_normalize(T vec) noexcept {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_normalize(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_normalize(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_normalize(vec);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_normalize_safe(T vec) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_normalize_safe(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_normalize_safe(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_normalize_safe(vec);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_distance(T vec1, T vec2) SHZ_NOEXCEPT  {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_distance(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_distance(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_distance(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_distance_sqr(T vec1, T vec2) SHZ_NOEXCEPT  {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_distance_sqr(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_distance_sqr(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_distance_sqr(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_lerp(T vec1, T vec2, float t) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_lerp(vec1, vec2, t);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_lerp(vec1, vec2, t);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_lerp(vec1, vec2, t);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_reflect(T incidence, T normal) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_reflect(incidence, normal);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_reflect(incidence, normal);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_reflect(incidence, normal);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_cross(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_cross(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_cross(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_project(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_project(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_project(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_project(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_project_safe(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_project_safe(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_project_safe(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_project_safe(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    SHZ_FORCE_INLINE auto shz_vec_angles(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_angle(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_angles(vec);
        else static_assert(false, "Invalid type!");
    }

    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_angle_between(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_angle_between(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_angle_between(vec1, vec2);
        else static_assert(false, "Invalid type!");
    }
#endif

#endif // SHZ_VECTOR_H
