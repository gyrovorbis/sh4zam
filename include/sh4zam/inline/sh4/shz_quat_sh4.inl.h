//! \cond INTERNAL
/*! \file
    \brief SH4 implementation of Quaternion API
    \ingroup quat

    This file contains the Dreamcast implementation routines for
    quaternion math, which have been hand-optimized specifically
    for the SH4 architecture.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_QUAT_SH4_INL_H
#define SHZ_QUAT_SH4_INL_H

SHZ_INLINE shz_quat_t shz_quat_mult_sh4(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
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
        asm("fipr	fv4,fv0"
            : "+f" (t1w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (t1x), "f" (t1y), "f" (t1z)
        );
        // x = t1w;  try to avoid the stall by not reading the fipr result immediately

        // y = -(q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x) + (q1.w * q2.y);
        t1x = -q2z;
        t1y = q2w;
        t1z = q2x;
        SHZ_MEMORY_BARRIER_SOFT();
        r.x = t1w;   // get previous result
        t1w = q2y;
        asm("fipr	fv4,fv0"
            : "+f" (t1w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (t1x), "f" (t1y), "f" (t1z)
        );
        //y = t1w;

        // z =  (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w) + (q1.w * q2.z);
        t1x = q2y;
        t1y = -q2x;
        t1z = q2w;
        SHZ_MEMORY_BARRIER_SOFT();
        r.y = t1w;   // get previous result
        t1w = q2z;
        asm("fipr	fv4,fv0"
            : "+f" (t1w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (t1x), "f" (t1y), "f" (t1z)
        );
        //z = t1w;
        SHZ_MEMORY_BARRIER_SOFT();

        // w = -(q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z) + (q1.w * q2.w);
        q2x = -q2x;
        q2y = -q2y;
        q2z = -q2z;
        asm("fipr	fv4,fv8"
            : "+f" (q2w)
            : "f" (q1x), "f" (q1y), "f" (q1z), "f" (q1w),
              "f" (q2x), "f" (q2y), "f" (q2z)
        );

        SHZ_MEMORY_BARRIER_SOFT();
        r.z = t1w;
        SHZ_MEMORY_BARRIER_SOFT();
        r.w = q2w;

        return r;
}

SHZ_COLD
SHZ_FORCE_INLINE shz_vec3_t shz_quat_vec3_dot3_sh4_(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT {
    register float lx asm("fr4") = l.x;
    register float ly asm("fr5") = l.y;
    register float lz asm("fr6") = l.z;
    register float lw asm("fr7") = 0.0f;

    register float r1x asm("fr8")  = r1.x;
    register float r1y asm("fr9")  = r1.y;
    register float r1z asm("fr10") = r1.z;
    register float r1w asm("fr11");

    register float r2x asm("fr0") = r2.x;
    register float r2y asm("fr1") = r2.y;
    register float r2z asm("fr2") = r2.z;
    register float r2w asm("fr3");

    asm volatile("fipr   fv4, fv8\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    r1x = r3.x;
    r1y = r3.y;
    r1z = r3.z;

    asm("fipr   fv4, fv0\n"
        : "+f" (r2w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r2x), "f" (r2y), "f" (r2z));

    r2x = r1w;

    asm volatile("fipr   fv4, fv8\n"
        : "+f" (r1w)
        : "f" (lx), "f" (ly), "f" (lz), "f" (lw),
          "f" (r1x), "f" (r1y), "f" (r1z));

    return shz_vec3_init(r2x, r2w, r1w);
}

SHZ_FORCE_INLINE shz_vec3_t shz_quat_transform_vec3_sh4(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT {
    register float qx asm("fr12") = q.x;
    register float qy asm("fr13") = q.y;
    register float qz asm("fr14") = q.z;
    register float qw asm("fr15") = 0.0f;

    register float vx asm("fr8")  = v.x;
    register float vy asm("fr9")  = v.y;
    register float vz asm("fr10") = v.z;
    register float vw asm("fr11") = 0.0f;

    register float tx asm("fr4")  = 2.0f;
    register float ty asm("fr5");
    register float tz asm("fr6")  = q.w;

    asm volatile("fipr   fv12, fv12"
        : "+f" (qw)
        : "f" (qx), "f" (qy), "f" (qz));

    ty = tz * tz;

    asm("fipr   fv12, fv8"
        : "+f" (vw)
        : "f" (qx), "f" (qy), "f" (qz), "f" (qw),
          "f" (vx), "f" (vy), "f" (vz));

    tz *= tx;
    ty -= qw;
    tx *= vw;

    return shz_quat_vec3_dot3_sh4_(
                shz_vec3_init(tx, ty, tz),
                shz_vec3_init(qx, vx, qy * vz - qz * vy),
                shz_vec3_init(qy, vy, qz * vx - qx * vz),
                shz_vec3_init(qz, vz, qx * vy - qy * vx)
           );
}
//! \endcond

#endif
