//! \cond INTERNAL
/*! \file
    \brief Software implementation of Quaternion API
    \ingroup quat

    This file contains the generic implementation routines for
    quaternion math, which are platform-independent, shared,
    and may be run anywhere. They're offered as part of the
    SW back-end.

    \author 2026 Falco Girgis

    \copyright MIT License
*/
#ifndef SHZ_QUAT_SW_INL_H
#define SHZ_QUAT_SW_INL_H

SHZ_INLINE shz_quat_t shz_quat_mult_sw(shz_quat_t q1, shz_quat_t q2) SHZ_NOEXCEPT {
    shz_quat_t result;

    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;

    return result;
}

SHZ_INLINE shz_vec3_t shz_quat_transform_vec3_sw(shz_quat_t q, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec2_t dot_qqv = shz_vec3_dot2(q.axis,
                                       q.axis, v);

    shz_vec3_t cross_qv = shz_vec3_cross(q.axis, v);

    return shz_vec3_dot3(shz_vec3_init(2.0f * dot_qqv.y, 
                                      (q.w * q.w) - dot_qqv.x,
                                      2.0f * q.w),
                         shz_vec3_init(q.x, v.x, cross_qv.x),
                         shz_vec3_init(q.y, v.y, cross_qv.y),
                         shz_vec3_init(q.z, v.z, cross_qv.z));
}

//! \endcond

#endif
