#ifndef SHZ_VECTOR_H
#define SHZ_VECTOR_H

#include <math.h>

#include "shz_intrin.h"

SHZ_BEGIN_DECLS

typedef struct {
    float x, y;
} shz_vec2_t;

typedef struct {
    float x, y, z;
} shz_vec3_t;

typedef struct {
    float x, y, z, w;
} shz_vec4_t;

/* Type conversion functions */

SHZ_FORCE_INLINE shz_vec2_t shz_vec3_to_vec2(shz_vec3_t vec) {
    return (shz_vec2_t){ vec.x, vec.y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec4_to_vec3(shz_vec4_t vec) {
    return (shz_vec3_t){ vec.x, vec.y, vec.z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec3_to_vec4(shz_vec3_t vec) {
    return (shz_vec4_t){ vec.x, vec.y, vec.z, 0.0f };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_to_vec3(shz_vec2_t vec) {
    return (shz_vec4_t){ vec.x, vec.y, 0.0f };
}

/* Simple arithmetic */

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) {
    return (shz_vec2_t) { vec1.x + vec2.x, vec1.y + vec2.y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2) {
    return (shz_vec2_t) { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2) {
    return (shz_vec2_t) { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w };
}

#define shz_vec_add(vec1, vec2) \
	_Generic((vec1), \
		 shz_vec2_t: shz_vec2_add, \
		 shz_vec3_t: shz_vec3_add, \
		 shz_vec4_t: shz_vec4_add)(vec1, vec2)

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) {
    return (shz_vec2_t) { vec1.x - vec2.x, vec1.y - vec2.y };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2) {
    return (shz_vec2_t) { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2) {
    return (shz_vec2_t) { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w };
}

#define shz_vec_sub(vec1, vec2) \
	_Generic((vec1), \
		 shz_vec2_t: shz_vec2_sub, \
		 shz_vec3_t: shz_vec3_sub, \
		 shz_vec4_t: shz_vec4_sub)(vec1, vec2)

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec, float factor) {
    return (shz_vec2_t) { vec.x * factor, vec.y * factor };
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_mul(shz_vec3_t vec, float factor) {
    return (shz_vec2_t) { vec.x * factor, vec.y * factor, vec.z * factor };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul(shz_vec4_t vec, float factor) {
    return (shz_vec2_t) { vec.x * factor, vec.y * factor, vec.z * factor, vec.w * factor };
}

#define shz_vec_mul(vec, factor) \
	_Generic((vec), \
		 shz_vec2_t: shz_vec2_mul, \
		 shz_vec3_t: shz_vec3_mul, \
		 shz_vec4_t: shz_vec4_mul)(vec, factor)

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec, float factor) {
    return shz_vec2_mul(vec, shz_fast_invf(factor));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_div(shz_vec3_t vec, float factor) {
    return shz_vec3_mul(vec, shz_fast_invf(factor));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div(shz_vec4_t vec, float factor) {
    return shz_vec4_mul(vec, shz_fast_invf(factor));
}

#define shz_vec_div(vec, factor) \
	_Generic((vec), \
		 shz_vec2_t: shz_vec2_div, \
		 shz_vec3_t: shz_vec3_div, \
		 shz_vec4_t: shz_vec4_div)(vec, factor)

/* 3D math */

SHZ_FORCE_INLINE float shz_vec2_dot(shz_vec2_t vec1, shz_vec2_t vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_sqr(shz_vec2_t vec) {
    return shz_vec2_dot(vec, vec);
}

SHZ_FORCE_INLINE float shz_vec2_magnitude(shz_vec2_t vec) {
    return shz_sqrtf(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec2_inv_magnitude(shz_vec2_t vec) {
    return shz_inverse_sqrtf(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize(shz_vec2_t vec) {
    return shz_vec2_mul(vec, shz_vec2_inv_magnitude(vec));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize_safe(shz_vec2_t vec) {
    float mag = shz_vec2_magnitude_sqr(vec);

    if(mag != 0.0f) {
        return shz_vec2_mul(vec, shz_inverse_sqrtf(mag));
    } else
        return (shz_vec2_t) { 1.0f, 0.0f };
}

SHZ_FORCE_INLINE float shz_vec2_distance(shz_vec2_t vec1, shz_vec2_t vec2) {
    return shz_vec2_magnitude(shz_vec2_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec2_distance_sqr(shz_vec2_t vec1, shz_vec2_t vec2) {
    return shz_vec2_magnitude_sqr(shz_vec2_sub(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec2_cross(shz_vec2_t vec1, shz_vec2_t vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_lerp(shz_vec2_t a, shz_vec2_t b, float t) {
    return (shz_vec2_t) { shz_lerpf(a.x, b.x, t), shz_lerpf(a.y, b.y, t) };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_reflect(shz_vec2_t incidence, shz_vec2_t normal) {

}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project(shz_vec2_t vec, sh_vec2_t onto) {

}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_sincos(shz_sincos_t sincos) {
    return (shz_vec2_t) { __real__ sincos.f, __image__ sincos.f };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle(float radians) {
    return shz_vec2_from_sincos(shz_sincosf(radians));
}

SHZ_FORCE_INLINE float shz_vec2_angle(shz_vec2_t vec) {
    return atan2f(vec.y, vec.x);
}

SHZ_FORCE_INLINE float sh2_vec2_angle_between(sh2_vec2_t vec1, sh2_vec2_t vec2);

void shz_vec3_angles(shz_vec3_t vec, float *azimuth, float *elevation, float *roll);
shz_vec3_t shz_vec3_from_angles(float azimuth, float elevation, float roll);
float shz_vec3_dot(shz_vec3_t vec1, shz_vec3_t vec2);
float shz_vec3_magnitude(shz_vec3_t vec);
float shz_vec3_magnitude_sqr(shz_vec3_t vec);
shz_vec3_t shz_vec3_normalize(shz_vec3_t vec);
shz_vec3_t shz_vec3_normalize_safe(shz_vec3_t vec);
shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2);
shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2);
shz_vec3_t shz_vec3_mul(shz_vec3_t vec, float factor);
shz_vec3_t shz_vec3_div(shz_vec3_t vec, float factor);
float shz_vec3_distance(shz_vec3_t vec1, shz_vec3_t vec2);
float shz_vec3_distance_sqr(shz_vec3_t vec1, shz_vec3_t vec2);
shz_vec3_t shz_vec3_cross(shz_vec3_t vec1, shz_vec3_t vec2);

float shz_vec4_dot(shz_vec4_t vec1, shz_vec4_t vec2);
float shz_vec4_magnitude(shz_vec4_t vec);
float shz_vec4_magnitude_sqr(shz_vec4_t vec);
shz_vec4_t shz_vec4_normalize(shz_vec4_t vec);
shz_vec4_t shz_vec4_normalize_safe(shz_vec4_t vec);
shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2);
shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2);
shz_vec4_t shz_vec4_mul(shz_vec4_t vec, float factor);
shz_vec4_t shz_vec4_div(shz_vec4_t vec, float factor);
float shz_vec4_distance(shz_vec4_t vec1, shz_vec4_t vec2);
shz_vec4_t shz_vec4_cross(shz_vec4_t vec1, shz_vec4_t vec2);

SHZ_END_DECLS

#endif // SHZ_VECTOR_H
