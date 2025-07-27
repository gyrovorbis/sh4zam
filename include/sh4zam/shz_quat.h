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

#include "shz_intrin.h"

/*! \defgroup quat Quaternions
    \brief         Routines for quaternion math.
@{
*/

SHZ_DECLS_BEGIN

typedef struct shz_quat {
    float x;
    float y;
    float z;
    float w;
} shz_quat_t;

SHZ_INLINE shz_quat_t shz_quat_mult(shz_quat_t q1, shz_quat_t q2) {
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
        register float q1x __asm__ ("fr4") = (q1.x);
        register float q1y __asm__ ("fr5") = (q1.y);
        register float q1z __asm__ ("fr6") = (q1.z);
        register float q1w __asm__ ("fr7") = (q1.w);

        // load q2 into fv8, use it to get the shuffled reorder into fv0
        register float q2x __asm__ ("fr8")  = (q2.x);
        register float q2y __asm__ ("fr9")  = (q2.y);
        register float q2z __asm__ ("fr10") = (q2.z);
        register float q2w __asm__ ("fr11") = (q2.w);

        // temporary operand / result in fv0
        register float t1x __asm__ ("fr0");
        register float t1y __asm__ ("fr1");
        register float t1z __asm__ ("fr2");
        register float t1w __asm__ ("fr3");

        // x =  (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y) + (q1.w * q2.x);
        t1x = q2w;
        t1y = q2z;
        t1z = -q2y;
        t1w = q2w;
        __asm__ ("\n"
            " fipr	fv4,fv0\n"
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
        __asm__ ("\n"
            "	fipr	fv4,fv0\n"
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
        __asm__ ("\n"
            "	fipr	fv4,fv0\n"
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
        __asm__ ("\n"
            "	fipr	fv4,fv8\n"
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
