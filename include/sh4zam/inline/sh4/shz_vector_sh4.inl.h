//! \cond INTERNAL
/*! \file
    \brief SH4 implementation of the Vector API
    \ingroup trig

    This file contains the Dreamcast implementation routines for
    vector math, which have been hand-optimized specifically for
    the SH4 architecture.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_VECTOR_SH4_INL_H
#define SHZ_VECTOR_SH4_INL_H

#include "../../shz_mem.h"

SHZ_INLINE float shz_vec3_triple_sh4(shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT {
    register float ax asm("fr8")  = a.x;
    register float bx asm("fr9")  = b.x;
    register float cx asm("fr10") = c.x;

    float dotPQ = shz_dot6f(ax, bx, cx, b.y * c.z, -c.y * a.z, -a.y * b.z);
    float dotRS = shz_dot6f(ax, bx, cx, c.y * b.z,  a.y * c.z,  b.y * a.z);

    return -(dotPQ + dotRS);
}

SHZ_INLINE shz_vec3_t shz_vec3_barycenter_sh4(shz_vec3_t p,
                                             shz_vec3_t a,
                                             shz_vec3_t b,
                                             shz_vec3_t c) SHZ_NOEXCEPT {
    shz_vec3_t result;

    // Assign v0 immediately to FV0.
    register float fr0  asm("fr0") = b.x - a.x; // v0.x
    register float fr1  asm("fr1") = b.y - a.y; // v0.y
    register float fr2  asm("fr2") = b.z - a.z; // v0.z
    register float fr3  asm("fr3") = 0.0f;
    register float fr4  asm("fr4");             // v1.x
    register float fr5  asm("fr5");             // v1.y
    register float fr6  asm("fr6");             // v1.z
    register float fr7  asm("fr7");
    register float fr8  asm("fr8");             // v2.x
    register float fr9  asm("fr9");             // v2.y
    register float fr10 asm("fr10");            // v2.z
    register float fr11 asm("fr11");

    // Load v1 into FV4
    SHZ_MEMORY_BARRIER_SOFT();
    fr4 = c.x - a.x;
    fr5 = c.y - a.y;
    fr6 = c.z - a.z;
    fr8 = p.x - a.x;
    SHZ_MEMORY_BARRIER_SOFT();

    // Begin calculating d01
    asm("fipr fv0, fv4"
        : "+&f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    // Load v2 into FV4
    SHZ_MEMORY_BARRIER_SOFT();
    fr9  = p.y - a.y;
    fr10 = p.z - a.z;
    SHZ_MEMORY_BARRIER_SOFT();

    // Begin calculating d00.
    asm("fipr fv0, fv0"
        : "+&f" (fr3)
        : "f" (fr0), "f" (fr1), "f" (fr2));

    SHZ_MEMORY_BARRIER_SOFT();
    float d01 = fr7;
    fr7 = 0.0f;
    SHZ_MEMORY_BARRIER_SOFT();

    // Begin calculating d12
    asm("fipr fv4, fv8"
        : "+&f" (fr11)
        : "f" (fr4), "f" (fr5), "f" (fr6), "f" (fr7),
          "f" (fr8), "f" (fr9), "f" (fr10));

    SHZ_MEMORY_BARRIER_SOFT();
    float d00 = fr3;
    fr3 = 0.0f;
    SHZ_MEMORY_BARRIER_SOFT();

    // Begin calculating d11
    asm("fipr fv4, fv4"
        : "+&f" (fr7)
        : "f" (fr4), "f" (fr5), "f" (fr6));

    SHZ_MEMORY_BARRIER_SOFT();
    float d12 = fr11;
    SHZ_MEMORY_BARRIER_SOFT();

    // Begin calculating d02
    asm("fipr fv0, fv8"
        : "+&f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));

    SHZ_MEMORY_BARRIER_SOFT();
    float d02 = fr11;
    SHZ_MEMORY_BARRIER_SOFT();
    float d11 = fr7;

    float inv_denom = shz_invf_fsrra((d00 * d11) - (d01 * d01));

    result.y = ((d11 * d02) - (d01 * d12)) * inv_denom;
    result.z = ((d00 * d12) - (d01 * d02)) * inv_denom;
    result.x = 1.0f - (result.z + result.y);

    return result;
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cubic_hermite_sh4(shz_vec3_t vec1, shz_vec3_t tangent1, shz_vec3_t vec2, shz_vec3_t tangent2, float t) SHZ_NOEXCEPT {
    shz_vec3_t result;

    float t2 = t * t;
    float t3 = t * t * t;

    float b0 =  2.0f * t3 - 3.0f * t2 + 1.0f;
    float b1 = t3 - 2.0f * t2 + t;
    float b2 = -2.0f * t3 + 3.0f * t2;
    float b3 = t3 - t2;

    // Basis vector
    register float h00 asm("fr4");
    register float h10 asm("fr5");
    register float h01 asm("fr6");
    register float h11 asm("fr7");

    // Control X component
    register float v0_0 asm("fr8");
    register float v0_1 asm("fr9");
    register float v0_2 asm("fr10");
    register float v0_3 asm("fr11");

    register float v1_0 asm("fr0");
    register float v1_1 asm("fr1");
    register float v1_2 asm("fr2");
    register float v1_3 asm("fr3");

    SHZ_MEMORY_BARRIER_SOFT();
    h00 = b0;
    h10 = b1;
    h01 = b2;
    h11 = b3;
    SHZ_MEMORY_BARRIER_SOFT();
    v0_0 = vec1.x;
    v0_1 = tangent1.x;
    v0_2 = vec2.x;
    v0_3 = tangent2.x;
    SHZ_MEMORY_BARRIER_SOFT();
    v1_0 = vec1.y;
    v1_1 = tangent1.y;
    SHZ_MEMORY_BARRIER_SOFT();

    asm("fipr   fv4, fv8"
        : "+f" (v0_3)
        : "f" (h00 ),  "f" (h10),  "f" (h01), "f" (h11),
          "f" (v0_0), "f" (v0_1), "f" (v0_2));

    SHZ_MEMORY_BARRIER_SOFT();
    v1_2 = vec2.y;
    v1_3 = tangent2.y;
    v0_0 = vec1.z;
    v0_1 = tangent1.z;
    SHZ_MEMORY_BARRIER_SOFT();
    v0_2 = vec2.z;
    SHZ_MEMORY_BARRIER_SOFT();
    result.x = v0_3;
    SHZ_MEMORY_BARRIER_SOFT();
    v0_3 = tangent2.z;
    SHZ_MEMORY_BARRIER_SOFT();

    asm("fipr   fv4, fv0"
        : "+f" (v1_3)
        : "f" (h00 ),  "f" (h10),  "f" (h01), "f" (h11),
          "f" (v1_0), "f" (v1_1), "f" (v1_2));

    SHZ_MEMORY_BARRIER_SOFT();

    asm("fipr   fv4, fv8"
        : "+f" (v0_3)
        : "f" (h00 ),  "f" (h10),  "f" (h01), "f" (h11),
          "f" (v0_0), "f" (v0_1), "f" (v0_2));

    SHZ_MEMORY_BARRIER_SOFT();
    result.y = v1_3;
    SHZ_MEMORY_BARRIER_SOFT();
    result.z = v0_3;

    return result;
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec2_dot2_sh4(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2) SHZ_NOEXCEPT {
#if 1 // NOW A WIN!
    register float lx asm("fr4") = l.x;
    register float ly asm("fr5") = l.y;
    register float lz asm("fr6") = 0.0f;
    register float lw asm("fr7") = 0.0f;

    register float r1x asm("fr8") = r1.x;
    register float r1y asm("fr9") = r1.y;
    register float r1w asm("fr11");

    register float r2x asm("fr12");
    register float r2y asm("fr13");
    register float r2w asm("fr15");

    //SHZ_MEMORY_BARRIER_SOFT();
    r2x = r2.x;
    r2y = r2.y;
    //SHZ_MEMORY_BARRIER_SOFT();

    asm("fipr   fv4, fv8\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y));

    asm("fipr   fv4, fv12\n"
        : "=f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y));

    return shz_vec2_init(r1w, r2w);
#else
    return shz_vec2_init(l.x * r1.x + l.y * r1.y,
                         l.x * r2.x + l.y * r2.y);
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec2_dot3_sh4(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2, shz_vec2_t r3) SHZ_NOEXCEPT {
#if 0
    shz_vec3_t res;

    register float r1x asm("fr12") = r1.x;
    register float r1y asm("fr13") = r1.y;
    register float r1w asm("fr15");

    register float lx asm("fr8")  = l.x;
    register float ly asm("fr9")  = l.y;
    register float lz asm("fr10") = 0.0f;
    register float lw asm("fr11") = 0.0f;

    register float r2x asm("fr0");
    register float r2y asm("fr1");
    register float r2w asm("fr3");

    register float r3x asm("fr4");
    register float r3y asm("fr5");
    register float r3w asm("fr7");

    SHZ_MEMORY_BARRIER_SOFT();
    r2x = r2.x;
    r2y = r2.y;
    SHZ_MEMORY_BARRIER_SOFT();

    asm("fipr   fv8, fv12\n"
        : "=f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y));

    SHZ_MEMORY_BARRIER_SOFT();
    r3x = r3.x;
    r3y = r3.y;
    SHZ_MEMORY_BARRIER_SOFT();

    asm("fipr   fv8, fv0\n"
        : "=f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y));

    asm("fipr   fv8, fv4\n"
        : "=f" (r3w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r3x), "f" (r3y));

    SHZ_MEMORY_BARRIER_SOFT();
    res.x = r1w;
    SHZ_MEMORY_BARRIER_SOFT();
    res.y = r2w;
    SHZ_MEMORY_BARRIER_SOFT();
    res.z = r3w;

    return res;
#else
    return shz_vec3_init(l.x * r1.x + l.y * r1.y,
                         l.x * r2.x + l.y * r2.y,
                         l.x * r3.x + l.y * r3.y);
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec3_dot2_sh4(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2) SHZ_NOEXCEPT {
#if 1 // FINALLY FUCKING GODDAMN GAINZ!!!
    register float lx asm("fr5") = l.x;
    register float ly asm("fr4") = l.y;
    register float lz asm("fr7") = l.z;
    register float lw asm("fr6") = 0.0f;

    register float r1x asm("fr9")  = r1.x;
    register float r1y asm("fr8")  = r1.y;
    register float r1z asm("fr11") = r1.z;
    register float r1w asm("fr10") = 0.0f;

    register float r2x asm("fr1") = r2.x;
    register float r2y asm("fr0") = r2.y;
    register float r2z asm("fr3") = r2.z;
    register float r2w asm("fr2") = 0.0f;

    asm("fipr   fv4, fv8\n"
        : "+f" (r1z)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1w));

    asm("fipr   fv4, fv0\n"
        : "+f" (r2z)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2w));

    return shz_vec2_init(r1z, r2z);
#else
    return shz_vec2_init(shz_vec3_dot(l, r1),
                         shz_vec3_dot(l, r2));
#endif
}

SHZ_FORCE_INLINE shz_vec3_t shz_vec3_dot3_sh4(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT {
#if 1
    register float lx asm("fr5") = l.x;
    register float ly asm("fr4") = l.y;
    register float lz asm("fr6") = l.z;
    register float lw asm("fr7") = 0.0f;

    register float r1x asm("fr9")  = r1.x;
    register float r1y asm("fr8")  = r1.y;
    register float r1z asm("fr10") = r1.z;
    register float r1w asm("fr11") = 0.0f;

    register float r2x asm("fr1") = r2.x;
    register float r2y asm("fr0") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3") = 0.0f;

    asm("fipr   fv4, fv8\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    r1x = r3.x;
    r1y = r3.y;
    r1z = r3.z;

    asm volatile("fipr   fv4, fv0\n"
        : "=f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    r2x = r1w;

    asm("fipr   fv4, fv8\n"
        : "=f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    return shz_vec3_init(r2x, r2w, r1w);
#else
/*
    return shz_vec3_init(l.x * r1.x + l.y * r1.y + l.z * r1.z,
                         l.x * r2.x + l.y * r2.y + l.z * r2.z,
                         l.x * r3.x + l.y * r3.y + l.z * r3.z);
                         */
#endif
}

SHZ_FORCE_INLINE shz_vec2_t shz_vec4_dot2_sh4(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2) SHZ_NOEXCEPT {
    register float lx asm("fr4") = l.x;
    register float ly asm("fr5") = l.y;
    register float lz asm("fr6") = l.z;
    register float lw asm("fr7") = l.w;

    register float r1x asm("fr8")  = r1.x;
    register float r1y asm("fr9")  = r1.y;
    register float r1z asm("fr10") = r1.z;
    register float r1w asm("fr11") = r1.w;

    register float r2x asm("fr0") = r2.x;
    register float r2y asm("fr1") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3") = r2.w;

    asm("fipr   fv4, fv8\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    asm("fipr   fv4, fv0\n"
        : "+f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    return shz_vec2_init(r1w, r2w);
}

SHZ_COLD
SHZ_FORCE_INLINE shz_vec3_t shz_vec4_dot3_sh4(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2, shz_vec4_t r3) SHZ_NOEXCEPT {
#if 1
    register float lx asm("fr4") = l.x;
    register float ly asm("fr5") = l.y;
    register float lz asm("fr6") = l.z;
    register float lw asm("fr7") = l.w;

    register float r1x asm("fr8")  = r1.x;
    register float r1y asm("fr9")  = r1.y;
    register float r1z asm("fr10") = r1.z;
    register float r1w asm("fr11") = r1.w;

    register float r2x asm("fr0") = r2.x;
    register float r2y asm("fr1") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3") = r2.w;

    asm("fipr   fv4, fv8\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    r1x = r3.x;
    r1y = r3.y;
    r1z = r3.z;

    asm volatile("fipr   fv4, fv0\n"
        : "+f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    r2x = r1w;
    r1w = r3.w;

    asm("fipr   fv4, fv8\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    return shz_vec3_init(r2x, r2w, r1w);
#else
    return shz_vec3_init(l.x * r1.x + l.y * r1.y + l.z * r1.z + l.w * r1.w,
                         l.x * r2.x + l.y * r2.y + l.z * r2.z + l.w * r2.w,
                         l.x * r3.x + l.y * r3.y + l.z * r3.z + l.w * r3.w);
#endif
}

//! \endcond

#endif
