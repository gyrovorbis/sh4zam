//! \cond INTERNAL
/*! \file
    \brief Internal implementation of Quaternion API
    \ingroup quat

    This file contains the implementation of the inline functions declared
    within the Quaternion API.

    \author 2025 Falco Girgis
    \author 2025 Oleg Endo

    \copyright MIT License
*/

SHZ_FORCE_INLINE shz_quat_t shz_quat_init(float w, float x, float y, float z) SHZ_NOEXCEPT {
    return (shz_quat_t) { .w = w, .x = x, .y = y, .z = z };
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_identity(void) SHZ_NOEXCEPT {
    return shz_quat_init(1.0f, 0.0f, 0.0f, 0.0f);
}

SHZ_INLINE shz_quat_t shz_quat_from_angles_xyz(float xangle, float yangle, float zangle) SHZ_NOEXCEPT {
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

SHZ_INLINE shz_quat_t shz_quat_from_axis_angle(shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    shz_sincos_t half_alpha = shz_sincosf(angle * 0.5f);

    return shz_quat_init(half_alpha.cos,
                         half_alpha.sin * axis.x,
                         half_alpha.sin * axis.y,
                         half_alpha.sin * axis.z);
}

shz_quat_t shz_quat_from_look_axis(shz_vec3_t forward, shz_vec3_t up) SHZ_NOEXCEPT;

SHZ_INLINE float shz_quat_angle(shz_quat_t q) SHZ_NOEXCEPT {
    return shz_acosf(q.w);
}

SHZ_INLINE shz_vec3_t shz_quat_axis(shz_quat_t q) SHZ_NOEXCEPT {
    float angle = shz_quat_angle(q);
    float invS = shz_invf_fsrra(shz_sinf(angle));
    return shz_vec3_init(q.x * invS, q.y * invS, q.z * invS);
}

SHZ_INLINE void shz_quat_axis_angle(shz_quat_t q, shz_vec3_t* vec, float* angle) SHZ_NOEXCEPT {
    *angle = shz_quat_angle(q);
    float invS = shz_invf_fsrra(shz_sinf(*angle));
    *vec = shz_vec3_init(q.x * invS, q.y * invS, q.z * invS);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_add(shz_quat_t q, shz_quat_t p) SHZ_NOEXCEPT {
    return shz_quat_init(q.w + p.w, q.x + p.x, q.y + p.y, q.z + p.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_scale(shz_quat_t q, float f) SHZ_NOEXCEPT {
    return shz_quat_init(q.w * f, q.x * f, q.y * f, q.z * f);
}

SHZ_FORCE_INLINE float shz_quat_magnitude_sqr(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_mag_sqr4f(quat.w, quat.x, quat.y, quat.z);
}

SHZ_FORCE_INLINE float shz_quat_magnitude(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE float shz_quat_magnitude_inv(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(shz_quat_magnitude_sqr(quat));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_scale(quat, shz_quat_magnitude_inv(quat));
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_normalize_safe(shz_quat_t quat) SHZ_NOEXCEPT {
    float mag = shz_quat_magnitude_sqr(quat);

    return (mag != 0.0f)?
        shz_quat_scale(quat, shz_inv_sqrtf(mag)) :
        shz_quat_identity();
}

SHZ_FORCE_INLINE float shz_quat_dot(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    return shz_dot8f(q1.x, q1.y, q1.z, q1.w,
                     q2.x, q2.y, q2.z, q2.w);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_conjugate(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_init(quat.w, -quat.x, -quat.y, -quat.z);
}

SHZ_FORCE_INLINE shz_quat_t shz_quat_inverse(shz_quat_t quat) SHZ_NOEXCEPT {
    return shz_quat_scale(shz_quat_conjugate(quat), shz_quat_magnitude_inv(quat));
}

SHZ_INLINE shz_quat_t shz_quat_from_rotated_axis(shz_vec3_t v1, shz_vec3_t v2) SHZ_NOEXCEPT {
	shz_vec3_t a = shz_vec3_cross(v1, v2);
	shz_quat_t q = shz_quat_init(
		shz_sqrtf_fsrra(shz_vec3_magnitude_sqr(v1) * shz_vec3_magnitude_sqr(v2)),
		a.x,
		a.y,
		a.z
	);

	return shz_quat_normalize(q);
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

SHZ_INLINE shz_quat_t shz_quat_slerp(shz_quat_t q, shz_quat_t p, float t) SHZ_NOEXCEPT {
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
        float s = shz_invf_fsrra(shz_sinf(phi));
        /* Add the two vectors, which have been scaled by their respective ratios. */
        return shz_quat_add(shz_quat_scale(q1, shz_sinf((1.0f - t) * phi) * s),
                            shz_quat_scale(p,  shz_sinf(t * phi) * s));
    }

	return q1;
}

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
        t1w = q2x;
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

SHZ_INLINE shz_vec3_t shz_quat_transform_vec3(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec3_t a   = shz_vec3_init(q.x, q.y, q.z);
	shz_vec3_t uv  = shz_vec3_cross(a, v);
	shz_vec3_t uuv = shz_vec3_cross(a, uv);
	shz_vec3_t tr  = shz_vec3_scale(shz_vec3_add(shz_vec3_scale(uv, q.w), uuv), 2.0f);

	return shz_vec3_add(v, tr);
}

//! \endcond