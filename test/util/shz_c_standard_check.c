#include "sh4zam/shz_sh4zam.h"

float shz_c_standard_check(const float* values) {
    shz_vec3_t   a  = shz_vec3_init(values[0], values[1], values[2]);
    shz_vec3_t   b  = shz_vec3_normalize(a);
    shz_vec4_t   c  = shz_vec3_vec4(a, 1.0f);
    shz_quat_t   q  = shz_quat_from_look_axis(a, b);
    shz_complex_t z = shz_cmulf(shz_cinitf(values[0], values[1]), shz_cinitf(values[1], values[0]));
    shz_sincos_t sc = shz_sincosf(values[2]);
    shz_vec3_t   t, s;
    shz_quat_t   r;
    shz_mat4x4_t m, inv;

    shz_mat4x4_init_identity(&m);
    shz_mat4x4_rotate_x(&m, values[0]);
    shz_mat4x4_add_translation(&m, a.x, a.y, a.z);
    shz_mat4x4_3x3_scale(&m, b.x, b.y, b.z);
    shz_mat4x4_permute_wzyx(&m);
    shz_mat4x4_inverse(&m, &inv);
    shz_mat4x4_decompose(&inv, &t, &r, &s);

    shz_xmtrx_load_4x4(&m);
    shz_xmtrx_rotate_quat(q);
    shz_xmtrx_invert();

    return shz_vec3_dot(a, b) + c.w + inv.elem[0] + q.w + r.w + t.x + s.x + z.real + sc.sin
         + shz_xmtrx_determinant() + shz_sqrtf(values[3]);
}
