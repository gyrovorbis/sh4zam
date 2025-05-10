#ifndef SH4_MATRIX_H
#define SH4_MATRIX_H

#include "sh4_vector.h"

SH4_BEGIN_DECLS

typedef union sh4_matrix_2x2 {
    float       elem[4];
    float       elem2D[2][2];
    sh4_vec2_t  col[2];
} sh4_matrix_2x2_t;

typedef union sh4_matrix_3x3 {
    float      elem[9];
    float      elem2D[3][3];
    sh4_vec3_t col[3];
    struct {
        sh4_vec3_t left;
        sh4_vec3_t up;
        sh4_vec3_t forward;
    };
} sh4_matrix_3x3_t;

typedef union sh4_matrix_3x4 {
    float      elem[12];
    float      elem2D[3][4];
    sh4_vec3_t col[4];
    struct {
        sh4_vec3_t left;
        sh4_vec3_t up;
        sh4_vec3_t forward;
        sh4_vec3_t pos;
    };
} sh4_matrix_3x4_t;

typedef SH4_ALIGNAS(8) union sh4_matrix_4x4 {
    float      elem[16];
    float      elem2D[4][4];
    sh4_vec4_t col[4];
    struct {
        sh4_vec4_t left;
        sh4_vec4_t up;
        sh4_vec4_t forward;
        sh4_vec4_t pos;
    };
} sh4_matrix_4x4_t;


SH4_END_DECLS

#endif // SH4_MATRIX_H
