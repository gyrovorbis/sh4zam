//! \cond INTERNAL
/*! \file
 *  \brief   Software Matrix API Implementation.
 *  \ingroup matrix
 *
 *  This file contains the generic SW implementation of the Matrix API.
 *
 *  \author 2026 Falco Girgis
 *
 *  \copyright MIT License
 */

#ifndef SHZ_MATRIX_SW_INL_H
#define SHZ_MATRIX_SW_INL_H

SHZ_INLINE shz_vec3_t shz_mat4x4_transform_vec3_sw(const shz_mat4x4_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
    return shz_vec3_add(shz_vec3_scale(mat->col[0].xyz, v.x),
           shz_vec3_add(shz_vec3_scale(mat->col[1].xyz, v.y),
                        shz_vec3_scale(mat->col[2].xyz, v.z)));
}

SHZ_INLINE shz_vec4_t shz_mat4x4_transform_vec4_sw(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT {
    shz_vec4_t r = shz_vec4_scale(mat->col[0], in.x);

    r = shz_vec4_add(r, shz_vec4_scale(mat->col[1], in.y));
    r = shz_vec4_add(r, shz_vec4_scale(mat->col[2], in.z));
    r = shz_vec4_add(r, shz_vec4_scale(mat->col[3], in.w));

    return r;
}

SHZ_INLINE shz_vec4_t shz_mat4x4_transform_vec4_transpose_sw(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT {
   return shz_vec4_init(
        shz_vec4_dot(in, mat->col[0]),
        shz_vec4_dot(in, mat->col[1]),
        shz_vec4_dot(in, mat->col[2]),
        shz_vec4_dot(in, mat->col[3])
    );
}

SHZ_INLINE shz_vec3_t shz_mat4x4_transform_vec3_transpose_sw(const shz_mat4x4_t* m, shz_vec3_t v) SHZ_NOEXCEPT {
    return shz_vec3_init(
        shz_vec3_dot(v, m->col[0].xyz),
        shz_vec3_dot(v, m->col[1].xyz),
        shz_vec3_dot(v, m->col[2].xyz)
    );
}

SHZ_INLINE void shz_mat4x4_copy_sw(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
    *dst = *src;
}

SHZ_INLINE shz_vec3_t shz_mat3x3_transform_vec3_sw(const shz_mat3x3_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
    return shz_vec3_add(shz_vec3_scale(mat->col[0], v.x),
           shz_vec3_add(shz_vec3_scale(mat->col[1], v.y),
                        shz_vec3_scale(mat->col[2], v.z)));
}

SHZ_INLINE shz_vec3_t shz_mat3x3_transform_vec3_transpose_sw(const shz_mat3x3_t* mat, shz_vec3_t v) SHZ_NOEXCEPT {
    return shz_vec3_add(shz_vec3_scale(shz_vec3_init(mat->elem2D[0][0], mat->elem2D[1][0], mat->elem2D[2][0]), v.x),
           shz_vec3_add(shz_vec3_scale(shz_vec3_init(mat->elem2D[0][1], mat->elem2D[1][1], mat->elem2D[2][1]), v.y),
                        shz_vec3_scale(shz_vec3_init(mat->elem2D[0][2], mat->elem2D[1][2], mat->elem2D[2][2]), v.z)));
}

#endif