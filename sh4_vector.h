#ifndef SH4_VECTOR_H
#define SH4_VECTOR_H

#include <math.h>

#include "sh4_intrin.h"

SH4_BEGIN_DECLS

typedef struct sh4_vec2 {
    union {
        struct {
            float x;
            float y;
        };
        float elem SH4_SIMD(8);
    };
} sh4_vec2_t;

sh4_vec2_t sh4_vec2_from_angle(float angle);

float sh4_vec2_angle(sh4_vec2_t vec);

SH4_FORCE_INLINE float sh4_vec2_dot(sh4_vec2_t vec1, sh4_vec2_t vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

SH4_FORCE_INLINE float sh4_vec2_magnitude_sqr(sh4_vec2_t vec) {
    return vec.x * vec.x + vec.y * vec.y;
}

SH4_FORCE_INLINE float sh4_vec2_magnitude(sh4_vec2_t vec) {
    return sh4_sqrtf(sh4_vec2_magnitude_sqr(vec));
}

SH4_FORCE_INLINE sh4_vec2_t sh4_vec2_normalize(sh4_vec2_t vec) {
    float inv_mag = sh4_inverse_sqrtf(sh4_vec2_magnitude_sqr(vec));
    return (sh4_vec2_t) { vec.x * inv_mag, vec.y * inv_mag };
}

SH4_FORCE_INLINE sh4_vec2_t sh4_vec2_normalize_safe(sh4_vec2_t vec) {
    float mag = sh4_vec2_magnitude_sqr(vec);

    if(mag != 0.0f) {
        float inv_mag = sh4_inverse_sqrtf(sh4_vec2_magnitude_sqr(vec));
        return (sh4_vec2_t) { vec.x * inv_mag, vec.y * inv_mag };
    } else
        return (sh4_vec2_t) { 1.0f, 0.0f };
}

SH4_FORCE_INLINE sh4_vec2_t sh4_vec2_add(sh4_vec2_t vec1, sh4_vec2_t vec2) {
    return (sh4_vec2_t) { vec1.x + vec2.x, vec1.y + vec2.y };
}

SH4_FORCE_INLINE sh4_vec2_t sh4_vec2_sub(sh4_vec2_t vec1, sh4_vec2_t vec2) {
    return (sh4_vec2_t) { vec1.x - vec2.x, vec1.y - vec2.y };
}

SH4_FORCE_INLINE sh4_vec2_t sh4_vec2_mul(sh4_vec2_t vec, float factor) {
    return (sh4_vec2_t) { vec.x * factor, vec.y * factor };
}

SH4_FORCE_INLINE sh4_vec2_t sh4_vec2_div(sh4_vec2_t vec, float factor) {
    float inv_factor = sh4_inverse_posf(factor);

    if(factor < 0.0f)
        inv_factor *= -1.0f;

    return (sh4_vec2_t) { vec.x * inv_factor, vec.y * inv_factor };
}

SH4_FORCE_INLINE float sh4_vec2_distance(sh4_vec2_t vec1, sh4_vec2_t vec2) {
    return sh4_vec2_magnitude(sh4_vec2_sub(vec1, vec2));
}

SH4_FORCE_INLINE float sh4_vec2_cross(sh4_vec2_t vec1, sh4_vec2_t vec2) {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

SH4_FORCE_INLINE sh4_vec2_t sh4_vec2_reflect(sh4_vec2_t incidence, sh4_vec2_t normal) {

}

typedef union sh4_vec3 {
    struct {
        union {
            struct {
                float x;
                float y;
            };
            sh4_vec2_t vec2;
        };
        float z;
    };
    float elem[3];
} sh4_vec3_t;

void sh4_vec3_angles(sh4_vec3_t vec, float *azimuth, float *elevation, float *roll);
sh4_vec3_t sh4_vec3_from_angles(float azimuth, float elevation, float roll);
float sh4_vec3_dot(sh4_vec3_t vec1, sh4_vec3_t vec2);
float sh4_vec3_magnitude(sh4_vec3_t vec);
float sh4_vec3_magnitude_sqr(sh4_vec3_t vec);
sh4_vec3_t sh4_vec3_normalize(sh4_vec3_t vec);
sh4_vec3_t sh4_vec3_normalize_safe(sh4_vec3_t vec);
sh4_vec3_t sh4_vec3_add(sh4_vec3_t vec1, sh4_vec3_t vec2);
sh4_vec3_t sh4_vec3_sub(sh4_vec3_t vec1, sh4_vec3_t vec2);
sh4_vec3_t sh4_vec3_mul(sh4_vec3_t vec, float factor);
sh4_vec3_t sh4_vec3_div(sh4_vec3_t vec, float factor);
float sh4_vec3_distance(sh4_vec3_t vec1, sh4_vec3_t vec2);
sh4_vec3_t sh4_vec3_cross(sh4_vec3_t vec1, sh4_vec3_t vec2);

typedef union sh4_vec4 {
    struct {
        union {
            struct {
                union {
                    struct {
                        float x;
                        float y;
                    };
                    sh4_vec2_t vec2;
                };
                float z;
            };
            sh4_vec3_t vec3;
        };
        float w;
    };
    float elem SH4_SIMD(16);
} sh4_vec4_t;

float sh4_vec4_dot(sh4_vec4_t vec1, sh4_vec4_t vec2);
float sh4_vec4_magnitude(sh4_vec4_t vec);
float sh4_vec4_magnitude_sqr(sh4_vec4_t vec);
sh4_vec4_t sh4_vec4_normalize(sh4_vec4_t vec);
sh4_vec4_t sh4_vec4_normalize_safe(sh4_vec4_t vec);
sh4_vec4_t sh4_vec4_add(sh4_vec4_t vec1, sh4_vec4_t vec2);
sh4_vec4_t sh4_vec4_sub(sh4_vec4_t vec1, sh4_vec4_t vec2);
sh4_vec4_t sh4_vec4_mul(sh4_vec4_t vec, float factor);
sh4_vec4_t sh4_vec4_div(sh4_vec4_t vec, float factor);
float sh4_vec4_distance(sh4_vec4_t vec1, sh4_vec4_t vec2);
sh4_vec4_t sh4_vec4_cross(sh4_vec4_t vec1, sh4_vec4_t vec2);

SH4_END_DECLS

#endif // SH4_VECTOR_H
