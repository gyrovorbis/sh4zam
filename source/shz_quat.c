/*! \file
    \brief Non-inlined Quaternion API implementations.
    \ingroup quat

    This file contains the non-inlined functions implementing the quaternion
    C API.

    \author 2025 Falco Girgis

    \copyright MIT License
*/

#include "sh4zam/shz_quat.h"
#include "sh4zam/shz_matrix.h"

shz_quat_t shz_quat_from_look_axis(shz_vec3_t forward, shz_vec3_t up) SHZ_NOEXCEPT {
    shz_mat4x4_t mat;

    mat.forward = shz_vec3_vec4(shz_vec3_normalize(forward), 0.0f);
    mat.left    = shz_vec3_vec4(shz_vec3_normalize(shz_vec3_cross(up, mat.forward.xyz)), 0.0f);
    mat.up      = shz_vec3_vec4(shz_vec3_normalize(shz_vec3_cross(mat.forward.xyz, mat.left.xyz)), 0.0f);

    return shz_mat4x4_to_quat(&mat);
}
