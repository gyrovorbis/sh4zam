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
 *  \author 2025 Falco Girgis
 *  \author 2025 Paul Cercueil
 *
 *  \copyright MIT License
 */

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
    return shz_vec3_init(v, v, v);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_fill(float v) SHZ_NOEXCEPT {
    return shz_vec4_init(v, v, v, v);
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
    return (shz_vec4_t){ .x = vec.x, .y = vec.y, .z = z, .w = w };
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec3_vec4(shz_vec3_t vec, float w) SHZ_NOEXCEPT {
    return (shz_vec4_t){ .xyz = vec, .w = w };
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x + vec2.x, vec1.y + vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_init(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_init(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x - vec2.x, vec1.y - vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_init(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_init(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(vec1.x * vec2.x, vec1.y * vec2.y);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_mul(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_init(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_init(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return shz_vec2_init(shz_divf(vec1.x, vec2.x), shz_divf(vec1.y, vec2.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_div(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_init(shz_divf(vec1.x, shz_invf(vec2.x)),
                         shz_divf(vec1.y, shz_invf(vec2.y)),
                         shz_divf(vec1.z, shz_invf(vec2.z)));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT {
    return shz_vec4_init(shz_divf(vec1.x, vec2.x),
                         shz_divf(vec1.y, vec2.y),
                         shz_divf(vec1.z, vec2.z),
                         shz_divf(vec1.w, vec2.w));
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_scale(shz_vec2_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec2_init(vec.x * factor, vec.y * factor);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_scale(shz_vec3_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec3_init(vec.x * factor, vec.y * factor, vec.z * factor);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_scale(shz_vec4_t vec, float factor) SHZ_NOEXCEPT {
    return shz_vec4_init(vec.x * factor, vec.y * factor, vec.z * factor, vec.w * factor);
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
    return shz_sqrtf(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_vec4_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec2_magnitude_inv(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(shz_vec2_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec3_magnitude_inv(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(shz_vec3_magnitude_sqr(vec));
}

SHZ_FORCE_INLINE float shz_vec4_magnitude_inv(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(shz_vec4_magnitude_sqr(vec));
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

    return (mag > 0.0f)?
        shz_vec2_scale(vec, shz_inv_sqrtf(mag)) :
        shz_vec2_init(0.0f, 0.0f);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize_safe(shz_vec3_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec3_magnitude_sqr(vec);

    return (mag > 0.0f)?
        shz_vec3_scale(vec, shz_inv_sqrtf(mag)) :
        shz_vec3_init(0.0f, 0.0f, 0.0f);
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize_safe(shz_vec4_t vec) SHZ_NOEXCEPT {
    float mag = shz_vec4_magnitude_sqr(vec);

    return (mag > 0.0f)?
        shz_vec4_scale(vec, shz_inv_sqrtf(mag)) :
        shz_vec4_init(0.0f, 0.0f, 0.0f, 0.0f);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_abs(shz_vec2_t vec) SHZ_NOEXCEPT {
    return shz_vec2_init(fabsf(vec.x), fabsf(vec.y));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_abs(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_abs(vec.xy), fabsf(vec.z));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_abs(shz_vec4_t vec) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_abs(vec.xyz), fabsf(vec.w));
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
    return SHZ_MAX(vec.x, vec.y);
}

SHZ_FORCE_INLINE float shz_vec3_max(shz_vec3_t vec) SHZ_NOEXCEPT {
    return SHZ_MAX(shz_vec2_max(vec.xy), vec.z);
}

SHZ_FORCE_INLINE float shz_vec4_max(shz_vec4_t vec) SHZ_NOEXCEPT {
    return SHZ_MAX(shz_vec3_max(vec.xyz), vec.w);
}

SHZ_FORCE_INLINE float shz_vec2_min(shz_vec2_t vec) SHZ_NOEXCEPT {
    return SHZ_MIN(vec.x, vec.y);
}

SHZ_FORCE_INLINE float shz_vec3_min(shz_vec3_t vec) SHZ_NOEXCEPT {
    return SHZ_MIN(shz_vec2_min(vec.xy), vec.z);
}

SHZ_FORCE_INLINE float shz_vec4_min(shz_vec4_t vec) SHZ_NOEXCEPT {
    return SHZ_MIN(shz_vec3_min(vec.xyz), vec.w);
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_clamp(shz_vec2_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec2_init(SHZ_CLAMP(vec.x, min, max),
                         SHZ_CLAMP(vec.y, min, max));
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_clamp(shz_vec3_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec2_vec3(shz_vec2_clamp(vec.xy, min, max),
                         SHZ_CLAMP(vec.z, min, max));
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_clamp(shz_vec4_t vec, float min, float max) SHZ_NOEXCEPT {
    return shz_vec3_vec4(shz_vec3_clamp(vec.xyz, min, max),
                         SHZ_CLAMP(vec.w, min, max));
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
    return shz_vec2_init(
        shz_lerpf(a.x, b.x, t),
        shz_lerpf(a.y, b.y, t)
    );
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_lerp(shz_vec3_t a, shz_vec3_t b, float t) SHZ_NOEXCEPT {
    return shz_vec3_init(
        shz_lerpf(a.x, b.x, t),
        shz_lerpf(a.y, b.y, t),
        shz_lerpf(a.z, b.z, t)
    );
}

SHZ_FORCE_INLINE shz_vec4_t shz_vec4_lerp(shz_vec4_t a, shz_vec4_t b, float t) SHZ_NOEXCEPT {
    return shz_vec4_init(
        shz_lerpf(a.x, b.x, t),
        shz_lerpf(a.y, b.y, t),
        shz_lerpf(a.z, b.z, t),
        shz_lerpf(a.w, b.w, t)
    );
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

SHZ_INLINE shz_vec2_t shz_vec2_refract(shz_vec2_t incidence, shz_vec2_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec2_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k < 0.0f)
        return shz_vec2_init(0.0f, 0.0f);
    else {
        incidence = shz_vec2_scale(incidence, eta);

        if(k == 0.0f)
            return incidence;
        else
            return shz_vec2_sub(incidence, shz_vec2_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
    }
}

SHZ_INLINE shz_vec3_t shz_vec3_refract(shz_vec3_t incidence, shz_vec3_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec3_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k < 0.0f)
        return shz_vec3_init(0.0f, 0.0f, 0.0f);
    else {
        incidence = shz_vec3_scale(incidence, eta);

        if(k == 0.0f)
            return incidence;
        else
            return shz_vec3_sub(incidence, shz_vec3_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
    }
}

SHZ_INLINE shz_vec4_t shz_vec4_refract(shz_vec4_t incidence, shz_vec4_t normal, float eta) SHZ_NOEXCEPT {
    const float dot = shz_vec4_dot(incidence, normal);
    const float k   = 1.0f - eta * eta * (1.0f - dot * dot);

    if(k < 0.0f)
        return shz_vec4_init(0.0f, 0.0f, 0.0f, 0.0f);
    else {
        incidence = shz_vec4_scale(incidence, eta);

        if(k == 0.0f)
            return incidence;
        else
            return shz_vec4_sub(incidence, shz_vec4_scale(normal, eta * dot * shz_sqrtf_fsrra(k)));
    }
}

SHZ_FORCE_INLINE float shz_vec2_cross(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT {
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cross(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return shz_vec3_init(
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x
    );
}

SHZ_INLINE float shz_vec3_triple(shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT {
    register float ax asm("fr8")  = a.x;
    register float bx asm("fr9")  = b.x;
    register float cx asm("fr10") = c.x;

    float dotPQ = shz_dot6f(ax, bx, cx, b.y * c.z, -c.y * a.z, -a.y * b.z);
    float dotRS = shz_dot6f(ax, bx, cx, c.y * b.z,  a.y * c.z,  b.y * a.z);

    return -(dotPQ + dotRS);
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
    return atan2f(shz_vec2_dot(vec1, vec2), shz_vec2_cross(vec1, vec2));
}

SHZ_FORCE_INLINE float shz_vec3_angle_between(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT {
    return atan2f(shz_vec3_dot(vec1, vec2), shz_vec3_magnitude(shz_vec3_cross(vec1, vec2)));
}

SHZ_FORCE_INLINE float shz_vec2_angle(shz_vec2_t vec) SHZ_NOEXCEPT {
    return atan2f(vec.y, vec.x);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_angles(shz_vec3_t vec) SHZ_NOEXCEPT {
    return shz_vec3_init(
        atan2f(shz_vec2_magnitude(shz_vec2_init(vec.y, vec.z)), vec.x),
        atan2f(shz_vec2_magnitude(shz_vec2_init(vec.z, vec.x)), vec.y),
        atan2f(shz_vec2_magnitude(shz_vec2_init(vec.x, vec.y)), vec.z)
    );
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

 //! \endcond
 