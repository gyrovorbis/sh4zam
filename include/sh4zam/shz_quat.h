/*! \file
    \brief Routines for operating upon quaternions.
    \ingroup quat
    \todo
    - to and from axis/angle
    - to and from matrix

    \author Falco Girgis
    \author Oleg Endo
*/

#ifndef SHZ_QUAT_H
#define SHZ_QUAT_H

#include "shz_vector.h"

/*! \defgroup quat Quaternions
    \brief         Routines for quaternion math.
@{
*/

#define SHZ_QUAT_SLERP_PHI_EPSILON 0.00001f

SHZ_DECLS_BEGIN

// consider force aligning
typedef struct shz_quat {
    float w;
    float x;
    float y;
    float z;
} shz_quat_t;

SHZ_FORCE_INLINE float shz_quat_dot(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    return shz_dot8f(q1.x, q1.y, q1.z, q1.w,
                     q2.x, q2.y, q2.z, q2.w);
}

//SHZ_FORCE_INLINE float shz_quat_angle_between     (shz_quat_t q1, shz_quat_t q2)             SHZ_NOEXCEPT;

SHZ_FORCE_INLINE shz_quat_t shz_quat_init(float w, float x, float y, float z) SHZ_NOEXCEPT {
    return (shz_quat_t) { .w = w, .x = x, .y = y, .z = z };
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_add(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    return shz_quat_init(q.w + p.w, q.x + p.x, q.y + p.y, q.z + p.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_scale(shz_quat_t q, float f) SHZ_NOEXCEPT {
    return shz_quat_init(q.w * f, q.x * f, q.y * f, q.z * f);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_identity(void) SHZ_NOEXCEPT {
    return shz_quat_init(0.0f, 0.0f, 0.0f, 1.0f);
}

//SHZ_FORCE_INLINE shz_quat_t shz_quat_from_angle_axis(float radians, shz_vec3_t axis)           SHZ_NOEXCEPT;

SHZ_FORCE_INLINE shz_quat_t shz_quat_from_euler_angles (float xangle, float yangle, float zangle) SHZ_NOEXCEPT {
    shz_sincos_t scx = shz_sincosf(xangle * 0.5f);
    shz_sincos_t scy = shz_sincosf(yangle * 0.5f);
    shz_sincos_t scz = shz_sincosf(zangle * 0.5f);

    return shz_quat_init(
        ((scy.cos * scx.cos) * scz.sin) - ((scy.sin * scx.sin) * scz.cos),
        ((scx.sin * scy.cos) * scz.cos) + ((scy.sin * scx.cos) * scz.sin),
        ((scy.sin * scx.cos) * scz.cos) - ((scx.sin * scy.cos) * scz.sin),
        ((scy.cos * scx.cos) * scz.cos) + ((scy.sin * scx.sin) * scz.sin)
    );
}

//SHZ_FORCE_INLINE shz_quat_t shz_quat_from_rotated_axis (shz_vec3_t from_dir, shz_vec3_t to_dir)         SHZ_NOEXCEPT;
//SHZ_FORCE_INLINE shz_quat_t shz_quat_from_look_axes    (shz_vec3_t forward_dir, shz_vec3_t upwards_dir) SHZ_NOEXCEPT;
// shz_quat_rotate_towards()

SHZ_FORCE_INLINE bool shz_quat_equals(shz_quat_t q, shz_quat_t v) SHZ_NOEXCEPT {
    float dot = shz_quat_dot(q, v);
    return (dot >= 0.999f && dot <= 1.001f);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_inverse(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_init(-quat.x, -quat.y, -quat.z, quat.w);
}

SHZ_INLINE shz_quat_t shz_quat_lerp(shz_quat_t a, shz_quat_t b, float t) SHZ_NOEXCEPT {
	if(shz_quat_dot(a, b) < 0.0f) {
        return shz_quat_init(t * (b.w + a.w) - a.w,
                             t * (b.x + a.x) - a.x,
                             t * (b.y + a.y) - a.y,
                             t * (b.z + a.z) - a.z);
	} else {
        return shz_quat_init(t * (b.w - a.w) + a.w,
                             t * (b.x - a.x) + a.x,
                             t * (b.y - a.y) + a.y,
                             t * (b.z - a.z) + a.z);
    }
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_slerp(shz_quat_t q, shz_quat_t p, float t) SHZ_NOEXCEPT {
	shz_quat_t q1 = q;

    float c = shz_quat_dot(q1, p);
    if(c < 0.0f) {
        c = -c;
        q1 = shz_quat_inverse(q1);
    }

    float phi = shz_acosf(c);
    // Check for a minimum epsilon, below which we do no interpolation.
    if(phi > SHZ_QUAT_SLERP_PHI_EPSILON) {
        /* The output of acosf() is in the range of [0 : PI],
           giving us a sine that is guaranteed to be a positive value. */
        float s = shz_inverse_posf(shz_sinf(phi));
        /* Add the two vectors, which have been scaled by their respective ratios. */
        return shz_quat_add(shz_quat_scale(q1, shz_sinf((1.0f - t) * phi) * s),
                            shz_quat_scale(p,  shz_sinf(t * phi) * s));
    }

	return q1;
}

//SHZ_FORCE_INLINE void shz_quat_angle_axis(shz_quat_t q, float *angle, shz_vec3_t *axis) SHZ_NOEXCEPT;
//SHZ_FORCE_INLINE void shz_quat_euler_angles(shz_quat_t q, float *x, float *y, float *z) SHZ_NOEXCEPT;
//SHZ_FORCE_INLINE shz_quat_t shz_quat_normalized(shz_quat_t q);

SHZ_INLINE shz_quat_t shz_quat_mult(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    shz_quat_t r;
    /*
        // reorder the coefficients so that q1 stays in constant order {x,y,z,w}
        // q2 then needs to be rotated after each inner product
        x =  (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y) + (q1.w * q2.x);
        y = -(q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x) + (q1.w * q2.y);
        z =  (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w) + (q1.w * q2.z);
        w = -(q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z) + (q1.w * q2.w);
    */
        // keep q1 in fv4
        register float q1x asm("fr4") = (q1.x);
        register float q1y asm("fr5") = (q1.y);
        register float q1z asm("fr6") = (q1.z);
        register float q1w asm("fr7") = (q1.w);

        // load q2 into fv8, use it to get the shuffled reorder into fv0
        register float q2x asm("fr8")  = (q2.x);
        register float q2y asm("fr9")  = (q2.y);
        register float q2z asm("fr10") = (q2.z);
        register float q2w asm("fr11") = (q2.w);

        // temporary operand / result in fv0
        register float t1x asm("fr0");
        register float t1y asm("fr1");
        register float t1z asm("fr2");
        register float t1w asm("fr3");

        // x =  (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y) + (q1.w * q2.x);
        t1x = q2w;
        t1y = q2z;
        t1z = -q2y;
        t1w = q2w;
        asm("fipr	fv4,fv0\n"
            : "+f" (t1w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (t1x), "f" (t1y), "f" (t1z)
        );
        // x = t1w;  try to avoid the stall by not reading the fipr result immediately

        // y = -(q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x) + (q1.w * q2.y);
        t1x = -q2z;
        t1y = q2w;
        t1z = q2x;
        SHZ_MEMORY_BARRIER_HARD();
        r.x = t1w;   // get previous result
        t1w = q2y;
        asm("fipr	fv4,fv0\n"
            : "+f" (t1w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (t1x), "f" (t1y), "f" (t1z)
        );
        //y = t1w;

        // z =  (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w) + (q1.w * q2.z);
        t1x = q2y;
        t1y = -q2x;
        t1z = q2w;
        SHZ_MEMORY_BARRIER_HARD();
        r.y = t1w;   // get previous result
        t1w = q2z;
        asm("fipr	fv4,fv0\n"
            : "+f" (t1w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (t1x), "f" (t1y), "f" (t1z)
        );
        //z = t1w;
        SHZ_MEMORY_BARRIER_HARD();

        // w = -(q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z) + (q1.w * q2.w);
        q2x = -q2x;
        q2y = -q2y;
        q2z = -q2z;
        asm("fipr	fv4,fv8\n"
            : "+f" (q2w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (q2x), "f" (q2y), "f" (q2z)
        );

        SHZ_MEMORY_BARRIER_HARD();
        r.z = t1w;
        SHZ_MEMORY_BARRIER_HARD();
        r.w = q2w;

        return r;
}

//! @}

SHZ_DECLS_END

#endif // SHZ_QUAT_H
