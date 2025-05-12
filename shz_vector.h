#ifndef SHZ_VECTOR_H
#define SHZ_VECTOR_H

#include <math.h>

#include "shz_intrin.h"

SHZ_BEGIN_DECLS

typedef struct shz_vec2 {
    union {
        struct {
            float x;
            float y;
        };
        float elem SHZ_SIMD(8);
    };
} shz_vec2_t;

SHZ_FORCE_INLINE float shz_vec2_dot(shz_vec2_t vec1, shz_vec2_t vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_sqr(shz_vec2_t vec) {
    return vec.x * vec.x + vec.y * vec.y;
}

SHZ_FORCE_INLINE float shz_vec2_magnitude(shz_vec2_t vec) {
    return shz_sqrtf(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize(shz_vec2_t vec) {
    float inv_mag = shz_inverse_sqrtf(shz_vec2_magnitude_sqr(vec));
    return (shz_vec2_t) { vec.x * inv_mag, vec.y * inv_mag };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize_safe(shz_vec2_t vec) {
    float mag = shz_vec2_magnitude_sqr(vec);

    if(mag != 0.0f) {
        float inv_mag = shz_inverse_sqrtf(shz_vec2_magnitude_sqr(vec));
        return (shz_vec2_t) { vec.x * inv_mag, vec.y * inv_mag };
    } else
        return (shz_vec2_t) { 1.0f, 0.0f };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) {
    return (shz_vec2_t) { vec1.x + vec2.x, vec1.y + vec2.y };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) {
    return (shz_vec2_t) { vec1.x - vec2.x, vec1.y - vec2.y };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec, float factor) {
    return (shz_vec2_t) { vec.x * factor, vec.y * factor };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec, float factor) {
    float inv_factor = shz_inverse_posf(factor);

    if(factor < 0.0f)
        inv_factor *= -1.0f;

    return (shz_vec2_t) { vec.x * inv_factor, vec.y * inv_factor };
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

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle(float radians) {
    shz_sincos_t sin_cos = shz_sincosf(radians);
    return (shz_vec2_t) { __real__ sin_cos.f, __image__ sincos.f };}
}

SHZ_FORCE_INLINE float shz_vec2_angle(shz_vec2_t vec) {
    return atan2f(vec.y, vec.x);
}

SHZ_FORCE_INLINE float sh2_vec2_angle_between(sh2_vec2_t vec1, sh2_vec2_t vec2);

typedef union shz_vec3 {
    struct {
        union {
            struct {
                float x;
                float y;
            };
            shz_vec2_t vec2;
        };
        float z;
    };
    float elem[3];
} shz_vec3_t;

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

typedef union shz_vec4 {
    struct {
        union {
            struct {
                union {
                    struct {
                        float x;
                        float y;
                    };
                    shz_vec2_t vec2;
                };
                float z;
            };
            shz_vec3_t vec3;
        };
        float w;
    };
    float elem SHZ_SIMD(16);
} shz_vec4_t;

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
