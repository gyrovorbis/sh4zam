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

SHZ_INLINE float shz_vec3_triple_sw(shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT {
    return a.x * (b.y * c.z - b.z * c.y) -
           a.y * (b.x * c.z - b.z * c.x) +
           a.z * (b.x * c.y - b.y * c.x);
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_barycenter_sw(shz_vec3_t p,
                                                   shz_vec3_t a,
                                                   shz_vec3_t b,
                                                   shz_vec3_t c) SHZ_NOEXCEPT {
    shz_vec3_t r;
    
    shz_vec3_t v0 = shz_vec3_sub(b, a);
    shz_vec3_t v1 = shz_vec3_sub(c, a);
    shz_vec3_t v2 = shz_vec3_sub(p, a);

    float d00 = shz_vec3_magnitude_sqr(v0);
    float d01 = shz_vec3_dot(v0, v1);
    float d11 = shz_vec3_magnitude_sqr(v1);
    float d20 = shz_vec3_dot(v2, v0);
    float d21 = shz_vec3_dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;

    r.y = (d11 * d20 - d01 * d21) / denom;
    r.z = (d00 * d21 - d01 * d20) / denom;
    r.x = 1.0f - (r.z + r.y);

    return r;
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
