#include "sh4zam/shz_sh4zam.hpp"

template struct shz::vecN<shz::vec2, shz_vec2_t, 2>;
template struct shz::vecN<shz::vec3, shz_vec3_t, 3>;

namespace {
    struct raw_vec3 { float x, y, z; };
}

float shz_cpp_standard_check(const float* values) noexcept {
    raw_vec3 raw { values[0], values[1], values[2] };

    shz::vec2 a2(values[0], values[1]);
    shz::vec3 a3(values[0], values[1], values[2]);
    shz::vec4 a4(values[0], values[1], values[2], values[3]);
    shz::vec3 b3 = shz::vec3::from(raw);
    shz_vec3_t c3 = shz_vec3_from(raw);
    raw_vec3 back = a3.to<raw_vec3>();

    shz::vec3 n3 = a3.cross(b3);
    n3.normalize();
    n3.normalize_safe();
    n3.normalize_or(b3);

    shz::mat4x4 m, inv;
    m.init_identity();
    m.rotate_x(values[0]);
    m.add_translation(a3);
    m.scale_3x3(b3);
    m.rotate(shz::quat::identity());
    m.permute_wzyx();
    m.inverse(&inv);

    shz::xmtrx::load(m);
    shz::xmtrx::rotate_quat(shz::quat::from_look_axis(a3, b3));
    shz::xmtrx::invert();
    const float det = shz::xmtrx::determinant();

    shz::complex c(values[0], values[1]);
    c += c;

    return a2.dot(a2) + a4.dot(a4) + n3.x + c3.x + back.z + inv.elem[0] + det + c.real;
}
