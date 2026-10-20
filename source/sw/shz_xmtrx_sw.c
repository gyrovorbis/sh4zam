/*! \file
 *  \brief   Out-of-line SW implementation of XMTRX routines.
 *  \ingroup complex
 *
 *  This file contains the out-of-line generic SW routines
 *  which back the XMTRX API, including the thread-local
 *  emulated XMTRX registers, which are only fetched via
 *  the shz_xmtrx_state_() accessor for access within the
 *  inline routines for the SW back-end.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

 // Force this translation unit to use the SW back-end.
#undef  SHZ_BACKEND
#define SHZ_BACKEND     SHZ_SW

#include "sh4zam/shz_xmtrx.h"
#include "sh4zam/shz_matrix.h"

alignas(64) SHZ_TLS_DECL(shz_xmtrx__t, xmtrx_state_, { 0 })

shz_xmtrx__t* shz_xmtrx_state_(void) {
    return SHZ_TLS_REF(xmtrx_state_);
}

void shz_xmtrx_load_apply_store_4x4_sw(shz_mat4x4_t* out,
                                       const shz_mat4x4_t* matrix1,
                                       const shz_mat4x4_t* matrix2) {
    shz_xmtrx_load_4x4(matrix1);
    shz_xmtrx_apply_4x4(matrix2);
    shz_xmtrx_store_4x4(out);
}

void shz_xmtrx_load_apply_store_3x4_sw(shz_mat3x4_t* out,
                                       const shz_mat3x4_t* matrix1,
                                       const shz_mat3x4_t* matrix2) {
    shz_xmtrx_load_3x4(matrix1);
    shz_xmtrx_apply_3x4(matrix2);
    shz_xmtrx_store_3x4(out);
}

void shz_xmtrx_load_apply_store_3x3_sw(shz_mat3x3_t* out,
                                       const shz_mat3x3_t* matrix1,
                                       const shz_mat3x3_t* matrix2) {
    shz_xmtrx_load_3x3(matrix1);
    shz_xmtrx_apply_3x3(matrix2);
    shz_xmtrx_store_3x3(out);
}

void shz_xmtrx_blend_4x4_sw(const shz_mat4x4_t* matrix, float weight) {
    shz_xmtrx__t* xmtrx_state_ = shz_xmtrx_state_();

    for(int i = 0; i < 16; ++i)
        xmtrx_state_->elem[i] += matrix->elem[i] * weight;
}

void shz_xmtrx_blend_3x4_sw(const shz_mat3x4_t* matrix, float weight) {
    shz_xmtrx__t* xmtrx_state_ = shz_xmtrx_state_();

    for(int c = 0; c < 4; ++c)
        for(int r = 0; r < 3; ++r)
            xmtrx_state_->elem[c * 4 + r] += matrix->elem2D[c][r] * weight;
}

/* Reference implementation of the skinned batch projection: the same operations, in the same order, as the SH4
   loop in source/sh4/shz_xmtrx_skin_sh4.s (FTRV of (x, y, z, 1), 1 / sqrt(w * w), the outcode compares on the
   projected x / y and on the transformed z, u / v = UV * scale + offset). */
static unsigned int shz_xmtrx_project_skinned_sw_(const shz_project_skinned_t* b, int unsigned_uv) {
    const uint8_t* rec  = (const uint8_t*)b->records;
    float*         out  = (float*)b->vertices;
    unsigned int   cur  = ~0u;
    unsigned int   i;

    for(i = 0; i < b->count; ++i, rec += b->record_stride, out += 8) {
        const int16_t* r  = (const int16_t*)rec;
        const int16_t* p  = b->positions + (int)r[0] * 4;
        unsigned int   mi = (unsigned int)(int)p[3];

        if(mi >= b->matrix_count) mi = 0;
        if(mi != cur) {
            if(!b->matrix_ready[mi]) break;
            shz_xmtrx_load_4x4(&b->matrices[mi]);
            cur = mi;
        }

        const uint8_t* t = (const uint8_t*)b->uvs + (int)r[2] * 4;
        float tu, tv;
        if(unsigned_uv) {
            tu = (float)((const uint16_t*)t)[0];
            tv = (float)((const uint16_t*)t)[1];
        } else {
            tu = (float)((const int16_t*)t)[0];
            tv = (float)((const int16_t*)t)[1];
        }

        const shz_vec4_t v   = shz_xmtrx_transform_vec4(shz_vec4_init((float)p[0], (float)p[1], (float)p[2], 1.0f));
        const float      inv = shz_inv_sqrtf(v.w * v.w);
        const float      x   = v.x * inv;
        const float      y   = v.y * inv;
        unsigned int     oc  = 0;

        if(b->near_z > v.z) oc |= SHZ_PROJECT_OUTCODE_NEAR;
        if(v.z > b->far_z)  oc |= SHZ_PROJECT_OUTCODE_FAR;
        if(0.0f > x)        oc |= SHZ_PROJECT_OUTCODE_LEFT;
        if(x > b->width)    oc |= SHZ_PROJECT_OUTCODE_RIGHT;
        if(0.0f > y)        oc |= SHZ_PROJECT_OUTCODE_TOP;
        if(y > b->height)   oc |= SHZ_PROJECT_OUTCODE_BOTTOM;

        out[1] = x;
        out[2] = y;
        out[3] = inv;
        out[4] = tu * b->u_scale + b->u_offset;
        out[5] = tv * b->v_scale + b->v_offset;
        b->outcodes[i] = (uint8_t)oc;
    }

    return b->count - i;
}

unsigned int shz_xmtrx_project_skinned_s16_sw(const shz_project_skinned_t* batch) {
    return shz_xmtrx_project_skinned_sw_(batch, 0);
}

unsigned int shz_xmtrx_project_skinned_u16_sw(const shz_project_skinned_t* batch) {
    return shz_xmtrx_project_skinned_sw_(batch, 1);
}
