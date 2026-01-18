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
	shz_mat3x3_t mat;

	mat.forward = shz_vec3_normalize(forward);
	mat.left 	= shz_vec3_normalize(shz_vec3_cross(up, mat.forward));
	mat.up  	= shz_vec3_normalize(shz_vec3_cross(mat.forward, mat.left));

	float s = 1.0f + mat.elem2D[0][0] + mat.elem2D[1][1] + mat.elem2D[2][2];
	float i = shz_inv_sqrtf_fsrra(s) * 2.0f;
	
	return shz_quat_init(
		s * i * 0.25f,
		(mat.elem2D[2][1] - mat.elem2D[1][2]) * i,
		(mat.elem2D[0][2] - mat.elem2D[2][0]) * i,
		(mat.elem2D[1][0] - mat.elem2D[0][1]) * i
	);
}
