#include "shz_test_scenario.h"
#include "shz_test.hpp"
#include "sh4zam/shz_quat.hpp"

#include <print>
#include <array>
#include <cglm/quat.h>
#include <cglm/euler.h>

#define GBL_SELF_TYPE   shz_quat_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

namespace {
    shz_vec3_t raylibVector3RotateByQuaternion(shz_quat_t q, shz_vec3_t v) {
        shz_vec3_t result = { 0 };

        result.x = v.x*(q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z) + v.y*(2*q.x*q.y - 2*q.w*q.z) + v.z*(2*q.x*q.z + 2*q.w*q.y);
        result.y = v.x*(2*q.w*q.z + 2*q.x*q.y) + v.y*(q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z) + v.z*(-2*q.w*q.x + 2*q.y*q.z);
        result.z = v.x*(-2*q.w*q.y + 2*q.x*q.z) + v.y*(2*q.w*q.x + 2*q.y*q.z)+ v.z*(q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);

        return result;
    }

    // Reference SLERP: same algorithm but using scalar division instead of FSRRA.
    // On SH4, the optimized path uses the FSRRA hardware reciprocal; this scalar
    // baseline makes the benchmark meaningful.
    shz_quat_t raylibQuaternionSlerp(shz_quat_t q1, shz_quat_t q2, float amount) {
        shz_quat_t result = { 0 };

        float cosHalfTheta =  q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

        if (fabs(cosHalfTheta) >= 1.0f) result = q1;
        else if (cosHalfTheta > 0.95f) result = shz_quat_nlerp(q1, q2, amount);
        else
        {
            float halfTheta = static_cast<float>(acos(cosHalfTheta));
            float sinHalfTheta = static_cast<float>(sqrt(1.0f - cosHalfTheta*cosHalfTheta));

            if (fabs(sinHalfTheta) < 0.001f)
            {
                result.x = (q1.x*0.5f + q2.x*0.5f);
                result.y = (q1.y*0.5f + q2.y*0.5f);
                result.z = (q1.z*0.5f + q2.z*0.5f);
                result.w = (q1.w*0.5f + q2.w*0.5f);
            }
            else
            {
                float ratioA = sinf((1 - amount)*halfTheta)/sinHalfTheta;
                float ratioB = sinf(amount*halfTheta)/sinHalfTheta;

                result.x = (q1.x*ratioA + q2.x*ratioB);
                result.y = (q1.y*ratioA + q2.y*ratioB);
                result.z = (q1.z*ratioA + q2.z*ratioB);
                result.w = (q1.w*ratioA + q2.w*ratioB);
            }
        }

        return result;
    }

    // q and -q represent the same rotation.
    bool same_rotation(const shz::quat& q, const shz::quat& p) {
        return q == p || q == -p;
    }

    float max_element_error(const shz_mat4x4_t& a, const shz_mat4x4_t& b) {
        float err = 0.0f;

        for(unsigned i = 0; i < 16; ++i)
            err = shz_fmaxf(err, shz_fabsf(a.elem[i] - b.elem[i]));

        return err;
    }

    // Converts a 16-bit binary angle (65536 per turn) to radians, in [-PI, PI).
    float u16_to_radians(uint16_t angle) {
        return static_cast<float>(static_cast<int16_t>(angle)) *
               (2.0f * shz::pi_f / 65536.0f);
    }

    /* Each Euler order, with the matching matrix initializer and the axes of the
       three angles in argument order, which is also the order of the product of
       axis quaternions that the rotation equals. */
    struct euler_order {
        const char* name;
        void      (*mat_init)(shz_mat4x4_t*, float, float, float);
        shz::quat (*from_rotation)(float, float, float);
        shz::quat (*from_rotation_u16)(uint16_t, uint16_t, uint16_t);
        shz::vec3 (*to_angles)(const shz::quat&);
        shz::vec3   axes[3];
    };

    const euler_order euler_orders[] = {
        { "xyz", shz_mat4x4_init_rotation_xyz, shz::quat::from_rotation_xyz, shz::quat::from_rotation_xyz,
          [](const shz::quat& q) { return q.to_angles_xyz(); },
          { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } } },
        { "zyx", shz_mat4x4_init_rotation_zyx, shz::quat::from_rotation_zyx, shz::quat::from_rotation_zyx,
          [](const shz::quat& q) { return q.to_angles_zyx(); },
          { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } } },
        { "zxy", shz_mat4x4_init_rotation_zxy, shz::quat::from_rotation_zxy, shz::quat::from_rotation_zxy,
          [](const shz::quat& q) { return q.to_angles_zxy(); },
          { { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } } },
        { "yxz", shz_mat4x4_init_rotation_yxz, shz::quat::from_rotation_yxz, shz::quat::from_rotation_yxz,
          [](const shz::quat& q) { return q.to_angles_yxz(); },
          { { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } } },
    };

    GBL_RESULT verify_from_rotation(GblTestSuite* pSelf, const euler_order& order) {
        const float angles[][3] = {
            {  0.0f,          0.0f,          0.0f         },
            {  shz::pi_f_2,   0.0f,          0.0f         },
            {  0.0f,          shz::pi_f_2,   0.0f         },
            {  0.0f,          0.0f,          shz::pi_f_2  },
            { -shz::pi_f_2,   shz::pi_f_2,  -shz::pi_f_2  },
            {  shz::pi_f,     0.0f,          0.0f         },
            {  0.0f,          shz::pi_f,     0.0f         },
            {  0.0f,          0.0f,          shz::pi_f    },
            {  shz::pi_f,     shz::pi_f_2,  -shz::pi_f_2  },
            {  0.3f,         -1.2f,          2.5f         },
            { -2.9f,          0.7f,         -0.4f         },
            {  5.0f,         -4.0f,          3.5f         },
        };

        GBL_CTX_BEGIN(pSelf);

        GBL_TEST_VERIFY(order.from_rotation(0.0f, 0.0f, 0.0f) == shz::quat::identity());

        for(const auto& a : angles) {
            const shz::quat q = order.from_rotation(a[0], a[1], a[2]);

            shz::mat4x4 expected, actual;
            order.mat_init(&expected, a[0], a[1], a[2]);
            actual.init_rotation(q);
            GBL_TEST_VERIFY(actual == expected);
            GBL_TEST_VERIFY(shz::fabsf(max_element_error(actual, expected)) <= 2e-3f);

            const shz::quat product = shz::quat::from_axis_angle(order.axes[0], a[0]) *
                                      shz::quat::from_axis_angle(order.axes[1], a[1]) *
                                      shz::quat::from_axis_angle(order.axes[2], a[2]);
            GBL_TEST_VERIFY(q == product);

            GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
        }

        for(unsigned i = 0; i < 3; ++i) {
            float a[3] = { 0.0f, 0.0f, 0.0f };
            a[i] = shz::pi_f / 3.0f;
            GBL_TEST_VERIFY(order.from_rotation(a[0], a[1], a[2]) ==
                            shz::quat::from_axis_angle(order.axes[i], shz::pi_f / 3.0f));
        }

        GBL_CTX_END();
    }

    GBL_RESULT verify_from_rotation_u16(GblTestSuite* pSelf, const euler_order& order) {
        const uint16_t angles[][3] = {
            { 0x0000, 0x0000, 0x0000 },
            { 0x4000, 0x0000, 0x0000 },
            { 0x0000, 0x4000, 0x0000 },
            { 0x0000, 0x0000, 0x4000 },
            { 0xc000, 0x4000, 0xc000 },
            { 0x8000, 0x0000, 0x0000 },
            { 0x0000, 0x8000, 0x0000 },
            { 0x0000, 0x0000, 0x8000 },
            { 0x8000, 0x4000, 0xc000 },
            { 0x04d2, 0xfedc, 0x7fff },
            { 0x0001, 0xffff, 0x8001 },
            { 0x2aaa, 0x9c40, 0x5555 },
        };

        GBL_CTX_BEGIN(pSelf);

        GBL_TEST_VERIFY(order.from_rotation_u16(0, 0, 0) == shz::quat::identity());

        for(const auto& a : angles) {
            const float x = u16_to_radians(a[0]);
            const float y = u16_to_radians(a[1]);
            const float z = u16_to_radians(a[2]);

            const shz::quat q = order.from_rotation_u16(a[0], a[1], a[2]);

            GBL_TEST_VERIFY(same_rotation(q, order.from_rotation(x, y, z)));

            shz::mat4x4 expected, actual;
            order.mat_init(&expected, x, y, z);
            actual.init_rotation(q);
            GBL_TEST_VERIFY(actual == expected);
            GBL_TEST_VERIFY(shz::fabsf(max_element_error(actual, expected)) <= 2e-3f);

            GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
        }

        GBL_CTX_END();
    }

    GBL_RESULT verify_to_angles(GblTestSuite* pSelf, const euler_order& order) {
        auto near = [](float a, float b) {
            return shz::fabsf(a - b) <= 3e-2f;
        };

        const float outers[]  = { -3.0f, -1.7f, -0.4f, 0.0f, 0.9f, 2.2f, 3.0f };
        const float middles[] = { -1.4f, -0.8f, -0.2f, 0.0f, 0.5f, 1.1f, 1.4f };
        const float locks[]   = { shz::pi_f_2, -shz::pi_f_2 };

        GBL_CTX_BEGIN(pSelf);

        GBL_TEST_VERIFY(order.to_angles(shz::quat::identity()) == shz::vec3(0.0f, 0.0f, 0.0f));

        for(unsigned i = 0; i < 3; ++i) {
            for(float angle : { 0.6f, -1.2f }) {
                const auto r = order.to_angles(shz::quat::from_axis_angle(order.axes[i], angle));

                for(unsigned j = 0; j < 3; ++j)
                    GBL_TEST_VERIFY(near(r[j], (i == j)? angle : 0.0f));
            }
        }

        for(float a : outers) {
            for(float b : middles) {
                for(float c : outers) {
                    const auto q = order.from_rotation(a, b, c);
                    const auto r = order.to_angles(q);

                    GBL_TEST_VERIFY(near(r.x, a));
                    GBL_TEST_VERIFY(near(r.y, b));
                    GBL_TEST_VERIFY(near(r.z, c));
                    GBL_TEST_VERIFY(same_rotation(order.from_rotation(r.x, r.y, r.z), q));
                }
            }
        }

        for(float b : locks) {
            for(float a : outers) {
                for(float c : outers) {
                    const auto q = order.from_rotation(a, b, c);
                    const auto r = order.to_angles(q);

                    GBL_TEST_VERIFY(near(r.y, b));
                    GBL_TEST_VERIFY(same_rotation(order.from_rotation(r.x, r.y, r.z), q));
                }
            }
        }

        GBL_CTX_END();
    }

    shz::quat glm_euler_quat(auto euler, float x, float y, float z) {
        vec3   angles = { x, y, z };
        versor v;
        euler(angles, v);
        return shz::quat(v[3], v[0], v[1], v[2]);
    }

    shz::vec3 glm_to_angles_xyz(const shz::quat& q) {
        versor       v = { q.x, q.y, q.z, q.w };
        mat4         m;
        shz_glm_vec3 r;
        glm_quat_mat4(v, m);
        glm_euler_angles(m, r.glm);
        return r.shz;
    }
}

GBL_TEST_CASE(identity)
    const auto q = shz::quat::identity();
    GBL_TEST_VERIFY(q.w == 1.0f);
    GBL_TEST_VERIFY(q.x == 0.0f);
    GBL_TEST_VERIFY(q.y == 0.0f);
    GBL_TEST_VERIFY(q.z == 0.0f);
    GBL_TEST_VERIFY(q == shz::quat(1.0f, 0.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(from_axis_angle)
    // zero angle around any axis yields identity
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 0.0f) == shz::quat::identity());
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, 0.0f) == shz::quat::identity());
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, 0.0f) == shz::quat::identity());

    // 180° around each basis axis: w≈0, corresponding component≈1, others=0
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f) == shz::quat(0.0f, 1.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f) == shz::quat(0.0f, 0.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f) == shz::quat(0.0f, 0.0f, 0.0f, 1.0f));

    // 90° around each basis axis: verify axis()/angle() round-trip
    {
        const auto q = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
        GBL_TEST_VERIFY(q.axis() == shz::vec3(1.0f, 0.0f, 0.0f));
        GBL_TEST_VERIFY(shz_equalf(q.angle(), shz::pi_f_2));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }
    {
        const auto q = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2);
        GBL_TEST_VERIFY(q.axis() == shz::vec3(0.0f, 1.0f, 0.0f));
        GBL_TEST_VERIFY(shz_equalf(q.angle(), shz::pi_f_2));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }
    {
        const auto q = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f_2);
        GBL_TEST_VERIFY(q.axis() == shz::vec3(0.0f, 0.0f, 1.0f));
        GBL_TEST_VERIFY(shz_equalf(q.angle(), shz::pi_f_2));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }

    // arbitrary axis: axis()/angle() round-trip
    {
        const shz::vec3 axis  = shz::vec3(1.0f, 1.0f, 0.0f).direction();
        const float     angle = shz::pi_f / 3.0f;
        const auto q = shz::quat::from_axis_angle(axis, angle);
        GBL_TEST_VERIFY(q.axis() == axis);
        GBL_TEST_VERIFY(shz_equalf(q.angle(), angle));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_xyz)
    GBL_TEST_CALL(verify_from_rotation(GBL_SELF_TYPE_NAME, euler_orders[0]));

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_xyz",
                                   [](float x, float y, float z) { return shz::quat::from_rotation_xyz(x, y, z); },
                                   "glm_euler_xyz_quat",
                                   [](float x, float y, float z) { return glm_euler_quat(glm_euler_xyz_quat, x, y, z); },
                                   0.4f, -0.8f, 1.2f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_zyx)
    GBL_TEST_CALL(verify_from_rotation(GBL_SELF_TYPE_NAME, euler_orders[1]));

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_zyx",
                                   [](float x, float y, float z) { return shz::quat::from_rotation_zyx(z, y, x); },
                                   "glm_euler_zyx_quat",
                                   [](float x, float y, float z) { return glm_euler_quat(glm_euler_zyx_quat, x, y, z); },
                                   0.4f, -0.8f, 1.2f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_zxy)
    GBL_TEST_CALL(verify_from_rotation(GBL_SELF_TYPE_NAME, euler_orders[2]));

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_zxy",
                                   [](float x, float y, float z) { return shz::quat::from_rotation_zxy(z, x, y); },
                                   "glm_euler_zxy_quat",
                                   [](float x, float y, float z) { return glm_euler_quat(glm_euler_zxy_quat, x, y, z); },
                                   0.4f, -0.8f, 1.2f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_yxz)
    GBL_TEST_CALL(verify_from_rotation(GBL_SELF_TYPE_NAME, euler_orders[3]));

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_yxz",
                                   [](float x, float y, float z) { return shz::quat::from_rotation_yxz(y, x, z); },
                                   "glm_euler_yxz_quat",
                                   [](float x, float y, float z) { return glm_euler_quat(glm_euler_yxz_quat, x, y, z); },
                                   0.4f, -0.8f, 1.2f)
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_xyz_u16)
    GBL_TEST_CALL(verify_from_rotation_u16(GBL_SELF_TYPE_NAME, euler_orders[0]));

    (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_xyz(uint16_t)",
                               [](uint16_t x, uint16_t y, uint16_t z) { return shz::quat::from_rotation_xyz(x, y, z); },
                               "glm_euler_xyz_quat",
                               [](uint16_t x, uint16_t y, uint16_t z) {
                                   return glm_euler_quat(glm_euler_xyz_quat, u16_to_radians(x), u16_to_radians(y), u16_to_radians(z));
                               },
                               uint16_t{0x1234}, uint16_t{0xdcba}, uint16_t{0x5678});
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_zyx_u16)
    GBL_TEST_CALL(verify_from_rotation_u16(GBL_SELF_TYPE_NAME, euler_orders[1]));

    (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_zyx(uint16_t)",
                               [](uint16_t x, uint16_t y, uint16_t z) { return shz::quat::from_rotation_zyx(z, y, x); },
                               "glm_euler_zyx_quat",
                               [](uint16_t x, uint16_t y, uint16_t z) {
                                   return glm_euler_quat(glm_euler_zyx_quat, u16_to_radians(x), u16_to_radians(y), u16_to_radians(z));
                               },
                               uint16_t{0x1234}, uint16_t{0xdcba}, uint16_t{0x5678});
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_zxy_u16)
    GBL_TEST_CALL(verify_from_rotation_u16(GBL_SELF_TYPE_NAME, euler_orders[2]));

    (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_zxy(uint16_t)",
                               [](uint16_t x, uint16_t y, uint16_t z) { return shz::quat::from_rotation_zxy(z, x, y); },
                               "glm_euler_zxy_quat",
                               [](uint16_t x, uint16_t y, uint16_t z) {
                                   return glm_euler_quat(glm_euler_zxy_quat, u16_to_radians(x), u16_to_radians(y), u16_to_radians(z));
                               },
                               uint16_t{0x1234}, uint16_t{0xdcba}, uint16_t{0x5678});
GBL_TEST_CASE_END

GBL_TEST_CASE(from_rotation_yxz_u16)
    GBL_TEST_CALL(verify_from_rotation_u16(GBL_SELF_TYPE_NAME, euler_orders[3]));

    GBL_TEST_VERIFY(shz::quat::from_rotation_yxz(uint16_t{0}, static_cast<uint16_t>(int16_t{-0x4000}), uint16_t{0}) ==
                    shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, -shz::pi_f_2));

    (benchmark_cmp<shz::quat>)("shz::quat::from_rotation_yxz(uint16_t)",
                               [](uint16_t x, uint16_t y, uint16_t z) { return shz::quat::from_rotation_yxz(y, x, z); },
                               "glm_euler_yxz_quat",
                               [](uint16_t x, uint16_t y, uint16_t z) {
                                   return glm_euler_quat(glm_euler_yxz_quat, u16_to_radians(x), u16_to_radians(y), u16_to_radians(z));
                               },
                               uint16_t{0x1234}, uint16_t{0xdcba}, uint16_t{0x5678});
GBL_TEST_CASE_END

GBL_TEST_CASE(to_angles_xyz)
    {
        auto near = [](float a, float b) {
            return shz::fabsf(a - b) <= 3e-2f;
        };

        GBL_TEST_VERIFY(shz::quat::identity().to_angles_xyz() == shz::vec3(0.0f, 0.0f, 0.0f));

        { // Single-axis rotations recover their angle, with its sign, on their own axis only.
            const shz::vec3 axes[] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };
            const float     angles[] = { shz::pi_f / 3.0f, -shz::pi_f / 3.0f, 0.6f, -1.2f };

            for(unsigned i = 0; i < 3; ++i) {
                for(float angle : angles) {
                    const auto q = shz::quat::from_axis_angle(axes[i], angle);
                    const auto r = q.to_angles_xyz();

                    for(unsigned j = 0; j < 3; ++j)
                        GBL_TEST_VERIFY(near(r[j], (i == j)? angle : 0.0f));

                    GBL_TEST_VERIFY(near(q.angle_x(), (i == 0)? angle : 0.0f));
                    GBL_TEST_VERIFY(near(q.angle_y(), (i == 1)? angle : 0.0f));
                    GBL_TEST_VERIFY(near(q.angle_z(), (i == 2)? angle : 0.0f));
                }
            }
        }{ // Inverse of from_rotation_xyz() away from gimbal lock: X and Z in (-PI, PI), Y in (-PI/2, PI/2).
            const float xzs[] = { -3.0f, -1.7f, -0.4f, 0.0f, 0.9f, 2.2f, 3.0f };
            const float ys[]  = { -1.4f, -0.8f, -0.2f, 0.0f, 0.5f, 1.1f, 1.4f };

            for(float x : xzs) {
                for(float y : ys) {
                    for(float z : xzs) {
                        const auto q = shz::quat::from_rotation_xyz(x, y, z);
                        const auto r = q.to_angles_xyz();

                        GBL_TEST_VERIFY(near(r.x, x));
                        GBL_TEST_VERIFY(near(r.y, y));
                        GBL_TEST_VERIFY(near(r.z, z));

                        GBL_TEST_VERIFY(near(q.angle_x(), r.x));
                        GBL_TEST_VERIFY(near(q.angle_y(), r.y));
                        GBL_TEST_VERIFY(near(q.angle_z(), r.z));

                        GBL_TEST_VERIFY(same_rotation(shz::quat::from_rotation_xyz(r.x, r.y, r.z), q));
                    }
                }
            }
        }{ // Gimbal lock: the angles aren't unique, but they must still describe the same rotation.
            const float locks[] = { shz::pi_f_2, -shz::pi_f_2 };
            const float xzs[]   = { -2.0f, -0.5f, 0.0f, 0.7f, 2.5f };

            for(float y : locks) {
                for(float x : xzs) {
                    for(float z : xzs) {
                        const auto q = shz::quat::from_rotation_xyz(x, y, z);
                        const auto r = q.to_angles_xyz();

                        GBL_TEST_VERIFY(near(r.y, y));
                        GBL_TEST_VERIFY(same_rotation(shz::quat::from_rotation_xyz(r.x, r.y, r.z), q));
                    }
                }
            }
        }
    }

    GBL_TEST_CALL(verify_to_angles(GBL_SELF_TYPE_NAME, euler_orders[0]));

    {
        const auto q = shz::quat::from_rotation_xyz(0.4f, -0.8f, 1.2f);
        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)("shz::quat::to_angles_xyz", [](const shz::quat& q) { return q.to_angles_xyz(); },
                                       "glm_quat_mat4 + glm_euler_angles", bench_fn(glm_to_angles_xyz), q)
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(to_angles_zyx)
    GBL_TEST_CALL(verify_to_angles(GBL_SELF_TYPE_NAME, euler_orders[1]));

    {
        const auto q = shz::quat::from_rotation_xyz(0.4f, -0.8f, 1.2f);
        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)("shz::quat::to_angles_zyx", [](const shz::quat& q) { return q.to_angles_zyx(); },
                                       "glm_quat_mat4 + glm_euler_angles", bench_fn(glm_to_angles_xyz), q)
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(to_angles_zxy)
    GBL_TEST_CALL(verify_to_angles(GBL_SELF_TYPE_NAME, euler_orders[2]));

    {
        const auto q = shz::quat::from_rotation_xyz(0.4f, -0.8f, 1.2f);
        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)("shz::quat::to_angles_zxy", [](const shz::quat& q) { return q.to_angles_zxy(); },
                                       "glm_quat_mat4 + glm_euler_angles", bench_fn(glm_to_angles_xyz), q)
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(to_angles_yxz)
    GBL_TEST_CALL(verify_to_angles(GBL_SELF_TYPE_NAME, euler_orders[3]));

    {
        const auto q = shz::quat::from_rotation_xyz(0.4f, -0.8f, 1.2f);
        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)("shz::quat::to_angles_yxz", [](const shz::quat& q) { return q.to_angles_yxz(); },
                                       "glm_quat_mat4 + glm_euler_angles", bench_fn(glm_to_angles_xyz), q)
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec3)
    auto check = [](shz::quat q, shz::vec3 v, shz::vec3 expected) {
        return (q * v) == expected;
    };

    auto cross_check = [](shz::quat q, shz::vec3 v) {
        return (q * v) == shz::vec3(raylibVector3RotateByQuaternion(q, const_cast<shz::vec3&>(v)));
    };

    // identity leaves any vector unchanged
    GBL_TEST_VERIFY(check(shz::quat::identity(), { 1.0f,  2.0f,  3.0f}, { 1.0f,  2.0f,  3.0f}));
    GBL_TEST_VERIFY(check(shz::quat::identity(), {-4.0f,  0.0f,  7.5f}, {-4.0f,  0.0f,  7.5f}));
    GBL_TEST_VERIFY(check(shz::quat::identity(), { 0.0f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}));

    {
        const auto rot90x  = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
        const auto rot90y  = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2);
        const auto rot90z  = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f_2);
        const auto rot180x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f);
        const auto rot180y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f);
        const auto rot180z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f);

        // 90° around X: X unchanged, Y→Z, Z→-Y
        GBL_TEST_VERIFY(check(rot90x, {1.0f, 0.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot90x, {0.0f, 1.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}));
        GBL_TEST_VERIFY(check(rot90x, {0.0f, 0.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}));

        // 90° around Y: Y unchanged, Z→X, X→-Z
        GBL_TEST_VERIFY(check(rot90y, {0.0f, 1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot90y, {0.0f, 0.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot90y, {1.0f, 0.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}));

        // 90° around Z: Z unchanged, X→Y, Y→-X
        GBL_TEST_VERIFY(check(rot90z, {0.0f, 0.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}));
        GBL_TEST_VERIFY(check(rot90z, {1.0f, 0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot90z, {0.0f, 1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}));

        // 180° around X: X unchanged, Y→-Y, Z→-Z
        GBL_TEST_VERIFY(check(rot180x, {1.0f, 0.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot180x, {0.0f, 1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot180x, {0.0f, 0.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}));

        // 180° around Y: Y unchanged, X→-X, Z→-Z
        GBL_TEST_VERIFY(check(rot180y, {0.0f, 1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot180y, {1.0f, 0.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot180y, {0.0f, 0.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}));

        // 180° around Z: Z unchanged, X→-X, Y→-Y
        GBL_TEST_VERIFY(check(rot180z, {0.0f, 0.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}));
        GBL_TEST_VERIFY(check(rot180z, {1.0f, 0.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}));
        GBL_TEST_VERIFY(check(rot180z, {0.0f, 1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}));

        // zero vector is invariant under any rotation
        GBL_TEST_VERIFY(check(rot90x,  {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}));
        GBL_TEST_VERIFY(check(rot180y, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}));
    }

    // cross-validate arbitrary rotations against reference implementation
    GBL_TEST_VERIFY(cross_check(shz::quat::from_axis_angle(shz::vec3{1.0f, 1.0f, 0.0f}.direction(), shz::pi_f / 3.0f), {1.0f, 2.0f, 3.0f}));
    GBL_TEST_VERIFY(cross_check(shz::quat::from_axis_angle(shz::vec3{1.0f, 1.0f, 1.0f}.direction(), shz::pi_f / 4.0f), {-1.0f, 0.5f, 2.0f}));
    GBL_TEST_VERIFY(cross_check(shz::quat::from_rotation_xyz(0.5f, 1.0f, 1.5f), {3.0f, -1.0f, 0.0f}));
    GBL_TEST_VERIFY(cross_check(shz::quat::from_rotation_xyz(0.3f, 0.7f, 0.2f), {0.0f, 1.0f, -1.0f}));
    {
        const shz::quat q     = shz::quat::from_rotation_xyz(0.0f, 1.0f, 2.0f);
        volatile float  in[7] = { q.x, q.y, q.z, q.w,
                                  gblRandUniform(-100.0f, 100.0f),
                                  gblRandUniform(-100.0f, 100.0f),
                                  gblRandUniform(-100.0f, 100.0f) };

        GBL_TEST_VERIFY(
            (benchmark_cmp<shz::vec3>)("shz::quat::transform(shz::vec3)",
                                       [](const volatile float* a) {
                                            return shz::quat(a[3], a[0], a[1], a[2]) * shz::vec3(a[4], a[5], a[6]);
                                       },
                                       "Vector3RotateByQuaternion",
                                       [](const volatile float* a) {
                                            shz::quat q(a[3], a[0], a[1], a[2]);
                                            shz::vec3 v(a[4], a[5], a[6]);
                                            return shz::vec3(raylibVector3RotateByQuaternion(q, v));
                                       },
                                       &in[0])
        );
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(slerp)
    { // At zero.
        const auto id   = shz::quat::identity();
        const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
        const auto r90y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2);
        const auto r45z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f_4);
        const auto arb  = shz::quat::from_rotation_xyz(0.4f, 0.8f, 1.2f);

        // All these pairs have dot >= 0 so q1 == q and t=0 yields the start quaternion.
        GBL_TEST_VERIFY(shz::quat::slerp(id,   r90x, 0.0f) == id);
        GBL_TEST_VERIFY(shz::quat::slerp(r90x, r90y, 0.0f) == r90x);
        GBL_TEST_VERIFY(shz::quat::slerp(r45z, arb,  0.0f) == r45z);
        GBL_TEST_VERIFY(shz::quat::slerp(arb,  id,   0.0f) == arb);
    }{ // At one.
        const auto id   = shz::quat::identity();
        const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
        const auto r90y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2);
        const auto r45z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f_4);
        const auto arb  = shz::quat::from_rotation_xyz(0.4f, 0.8f, 1.2f);

        // t=1 always yields the end quaternion regardless of dot sign.
        GBL_TEST_VERIFY(shz::quat::slerp(id,   r90x, 1.0f) == r90x);
        GBL_TEST_VERIFY(shz::quat::slerp(r90x, r90y, 1.0f) == r90y);
        GBL_TEST_VERIFY(shz::quat::slerp(r45z, arb,  1.0f) == arb);
        GBL_TEST_VERIFY(shz::quat::slerp(arb,  id,   1.0f) == id);
    }{ // Identical.
        const auto id   = shz::quat::identity();
        const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
        const auto arb  = shz::quat::from_rotation_xyz(0.4f, 0.8f, 1.2f);

        GBL_TEST_VERIFY(shz::quat::slerp(id,   id,   0.0f) == id);
        GBL_TEST_VERIFY(shz::quat::slerp(id,   id,   0.5f) == id);
        GBL_TEST_VERIFY(shz::quat::slerp(id,   id,   1.0f) == id);
        GBL_TEST_VERIFY(shz::quat::slerp(r90x, r90x, 0.5f) == r90x);
        GBL_TEST_VERIFY(shz::quat::slerp(arb,  arb,  0.5f) == arb);
    }{ // Antopodal
        /* -q and q represent the same 3D rotation.  slerp(q, -q, t) triggers the
           negative-dot path: negate q → phi == 0 → epsilon short-circuit → returns -q
           for all t, which is still the same physical rotation as q.
        */
        const auto q  = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f / 3.0f);
        const auto nq = q.negated();
        const shz::vec3 v = {1.0f, 2.0f, -1.0f};

        GBL_TEST_VERIFY(q.transform(v) == nq.transform(v));

        GBL_TEST_VERIFY(shz::quat::slerp(q, nq, 0.0f).transform(v) == q.transform(v));
        GBL_TEST_VERIFY(shz::quat::slerp(q, nq, 0.5f).transform(v) == q.transform(v));
        GBL_TEST_VERIFY(shz::quat::slerp(q, nq, 1.0f).transform(v) == q.transform(v));
    }{ // Negative dot, shorter arc
        /* identity → 270° around X has dot < 0: the long arc is 270°, the short arc is
           -90°.  Internally slerp negates q to take the short path, so slerp(q, p, t)
           must equal slerp(-q, p, t) (both end up interpolating from -identity to r270).
        */
        const auto id   = shz::quat::identity();
        const auto r270 = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 3.0f * shz::pi_f_2);

        GBL_TEST_VERIFY(id.dot(r270) < 0.0f);

        GBL_TEST_VERIFY(shz::quat::slerp(id, r270, 0.25f) == shz::quat::slerp(id.negated(), r270, 0.25f));
        GBL_TEST_VERIFY(shz::quat::slerp(id, r270, 0.50f) == shz::quat::slerp(id.negated(), r270, 0.50f));
        GBL_TEST_VERIFY(shz::quat::slerp(id, r270, 0.75f) == shz::quat::slerp(id.negated(), r270, 0.75f));
    }{ // Same axis, midpoints
        {
            /* For same-axis rotations, slerp reduces to linear angle interpolation.
               identity → 90° around X: each fraction gives the proportional angle.
            */
            const auto q = shz::quat::identity();
            const auto p = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);

            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.25f) == shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_4 * 0.5f));
            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.50f) == shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_4));
            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.75f) == shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_4 * 1.5f));
        }{
            // Non-zero start angle: 45° → 135° around Y, midpoint should be 90°.
            const auto q2 = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_4);
            const auto p2 = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_4 * 3.0f);
            GBL_TEST_VERIFY(shz::quat::slerp(q2, p2, 0.5f) == shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2));
        }
    }{ // Unit magnitude
        const auto id   = shz::quat::identity();
        {
            // The result of slerp on two unit quaternions must itself be a unit quaternion.
            const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
            const auto r90y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2);
            const auto arb  = shz::quat::from_rotation_xyz(0.5f, 1.0f, 1.5f);

            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id,   r90x, 0.0f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id,   r90x, 0.5f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id,   r90x, 1.0f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(r90x, r90y, 0.3f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(r90y, arb,  0.7f).magnitude(), 1.0f));
        }{
            // Negative-dot path must also preserve unit magnitude.
            const auto r270 = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 3.0f * shz::pi_f_2);
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id, r270, 0.5f).magnitude(), 1.0f));
        }
    }{ // Constant angular velocity
        /* slerp travels at constant angular velocity: equal t-steps span equal arcs.
           identity → 90° around Z in four quarter-steps; each step covers PI/8.
        */
        const auto q = shz::quat::identity();
        const auto p = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f_2);

        const auto q0 = shz::quat::slerp(q, p, 0.00f);
        const auto q1 = shz::quat::slerp(q, p, 0.25f);
        const auto q2 = shz::quat::slerp(q, p, 0.50f);
        const auto q3 = shz::quat::slerp(q, p, 0.75f);
        const auto q4 = shz::quat::slerp(q, p, 1.00f);

        const float d01 = q0.angle_between(q1);
        const float d12 = q1.angle_between(q2);
        const float d23 = q2.angle_between(q3);
        const float d34 = q3.angle_between(q4);

        GBL_TEST_ERROR(d01, d12, 0.03f, GBL_TEST_ERROR_FUZZY);
        GBL_TEST_ERROR(d12, d23, 0.03f, GBL_TEST_ERROR_FUZZY);
        GBL_TEST_ERROR(d23, d34, 0.03f, GBL_TEST_ERROR_FUZZY);;
        GBL_TEST_ERROR(d01, shz::pi_f / 8.0f, 0.03f, GBL_TEST_ERROR_FUZZY);
    }{ // Commutativity
        /* slerp(q, p, t) == slerp(p, q, 1-t) when dot(q,p) >= 0.
           The interpolated quaternion is the same point on the arc regardless of direction.
        */
        const auto q = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_4);
        const auto p = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2);

        GBL_TEST_VERIFY(q.dot(p) >= 0.0f);

        GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.25f) == shz::quat::slerp(p, q, 0.75f));
        GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.50f) == shz::quat::slerp(p, q, 0.50f));
        GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.75f) == shz::quat::slerp(p, q, 0.25f));
    }{ // Phi Epsilon
        const auto q = shz::quat::identity();
        {
            /* When phi = acos(dot(q,p)) <= SHZ_QUAT_SLERP_PHI_EPSILON, slerp short-circuits
               and returns the start quaternion for ALL t values — including t=1.
               angle=0.01 → half-angle phi = 0.005 < SHZ_QUAT_SLERP_PHI_EPSILON = 0.01
            */
            const auto p_near = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 0.01f);

            GBL_TEST_VERIFY(shz::quat::slerp(q, p_near, 0.0f) == q);
            GBL_TEST_VERIFY(shz::quat::slerp(q, p_near, 0.5f) == q);
            GBL_TEST_VERIFY(shz::quat::slerp(q, p_near, 1.0f) == q);  // ← short-circuit: not p_near
        }{
            // Well above epsilon: t=1 returns the end quaternion normally.
            const auto p_far = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
            GBL_TEST_VERIFY(shz::quat::slerp(q, p_far, 1.0f) == p_far);
        }
    }{ // Geometric correctness
        const shz::vec3 v = {1.0f, 0.0f, 0.0f};
        {
            /* Verify slerp produces the geometrically correct intermediate rotation by
               checking vector transforms at known fractions.
            */
            const auto id   = shz::quat::identity();
            const auto r90z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f_2);
            const auto r45z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, shz::pi_f_4);

            // identity → 90° around Z at t=0.5 must rotate {1,0,0} by exactly 45°.
            GBL_TEST_VERIFY(shz::quat::slerp(id, r90z, 0.5f).transform(v) == r45z.transform(v));
            GBL_TEST_VERIFY(shz::quat::slerp(id, r90z, 0.0f).transform(v) == id.transform(v));
            GBL_TEST_VERIFY(shz::quat::slerp(id, r90z, 1.0f).transform(v) == r90z.transform(v));
        }{
            // Same-axis: slerp midpoint of 45°→135° around X must rotate like 90° around X.
            const auto q   = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_4);
            const auto p   = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_4 * 3.0f);
            const auto mid = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.5f).transform(v) == mid.transform(v));

            GBL_TEST_VERIFY(
                (benchmark_cmp<shz::quat>)(
                    "shz::quat::slerp",
                    [](shz::quat a, shz::quat b, float t) { return shz::quat::slerp(a, b, t); },
                    "raylibQuaternionSlerp",
                    bench_fn(raylibQuaternionSlerp),
                    shz::quat::from_axis_angle(shz::vec3{1.0f, 1.0f, 1.0f}.direction(), shz::pi_f / 3.0f),
                    shz::quat::from_rotation_xyz(0.5f, 1.0f, 1.5f),
                    0.35f)
            );
        }
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(nlerp)
    const auto id   = shz::quat::identity();
    const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, shz::pi_f_2);
    const auto r90y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, shz::pi_f_2);
    const auto arb1 = shz::quat::from_rotation_yxz(0.4f, -0.8f, 1.2f);
    const auto arb2 = shz::quat::from_rotation_zyx(-2.0f, 0.5f, 2.5f);
    const float ts[] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };

    { // Sign invariance: b and -b are the same orientation, so they must give the same result.
        const std::pair<shz::quat, shz::quat> pairs[] = {
            { id, r90x }, { r90x, r90y }, { arb1, arb2 }, { arb2, id }
        };

        for(const auto& [a, b] : pairs) {
            for(float t : ts) {
                const auto r = shz::quat::nlerp(a, b, t);
                GBL_TEST_VERIFY(r == shz::quat::nlerp(a, -b, t));
                GBL_TEST_VERIFY(shz::fabsf(r.magnitude() - 1.0f) <= 1e-4f);
            }

            // The end points are a and b, as rotations.
            GBL_TEST_VERIFY(shz::quat::nlerp(a, -b, 0.0f) == a);
            GBL_TEST_VERIFY(same_rotation(shz::quat::nlerp(a, -b, 1.0f), b));
        }
    }{ // Negative dot: takes the short arc.
        /* identity -> 270 degrees around X has a negative dot product. The short arc is
           -90 degrees, so the midpoint is -45 degrees, not the +135 degrees of the long
           arc. At t = 0.5 this is exact, since nlerp and slerp agree there.
        */
        const auto r270 = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 3.0f * shz::pi_f_2);
        const shz::vec3 v = {0.0f, 1.0f, 0.0f};

        GBL_TEST_VERIFY(id.dot(r270) < 0.0f);

        const auto mid = shz::quat::nlerp(id, r270, 0.5f);

        GBL_TEST_VERIFY(same_rotation(mid, shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, -shz::pi_f_4)));
        GBL_TEST_VERIFY(!same_rotation(mid, shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 3.0f * shz::pi_f_4)));
        GBL_TEST_VERIFY(mid.transform(v) == shz::quat::slerp(id, r270, 0.5f).transform(v));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(magnitude)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).magnitude(), 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, comp).magnitude(), 5.0f));
    GBL_TEST_VERIFY(shz::quat(0.0f, 0.0f, 0.0f, 0.0f).magnitude() == 0.0f);

    (benchmark_cmp<float>)("shz::quat::magnitude",
                           [](const shz::quat& q) { return q.magnitude(); },
                           "glm_quat_norm",
                           [](const shz::quat& q) { versor v = { q.x, q.y, q.z, q.w }; return glm_quat_norm(v); },
                           shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(magnitude_fsrra)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).magnitude_fsrra(), 5.0f));
    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, comp).magnitude_fsrra(), 5.0f));

    (benchmark_cmp<float>)("shz::quat::magnitude_fsrra",
                           [](const shz::quat& q) { return q.magnitude_fsrra(); },
                           "glm_quat_norm",
                           [](const shz::quat& q) { versor v = { q.x, q.y, q.z, q.w }; return glm_quat_norm(v); },
                           shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(magnitude_inv)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).magnitude_inv(), 0.2f));
    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, comp).magnitude_inv(), 0.2f));
    GBL_TEST_VERIFY(shz::quat(0.0f, 0.0f, 0.0f, 0.0f).magnitude_inv() == 0.0f);

    (benchmark_cmp<float>)("shz::quat::magnitude_inv",
                           [](const shz::quat& q) { return q.magnitude_inv(); },
                           "1.0f / glm_quat_norm",
                           [](const shz::quat& q) { versor v = { q.x, q.y, q.z, q.w }; return 1.0f / glm_quat_norm(v); },
                           shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(magnitude_inv_fsrra)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).magnitude_inv_fsrra(), 0.2f));
    GBL_TEST_VERIFY(shz::equalf(shz::quat(1.0f, 2.0f, 2.0f, comp).magnitude_inv_fsrra(), 0.2f));

    (benchmark_cmp<float>)("shz::quat::magnitude_inv_fsrra",
                           [](const shz::quat& q) { return q.magnitude_inv_fsrra(); },
                           "1.0f / glm_quat_norm",
                           [](const shz::quat& q) { versor v = { q.x, q.y, q.z, q.w }; return 1.0f / glm_quat_norm(v); },
                           shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(normalized)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).normalized() == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, comp).normalized() == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));

    (benchmark_cmp<shz::quat>)("shz::quat::normalized",
                               [](const shz::quat& q) { return q.normalized(); },
                               "glm_quat_normalize_to",
                               [](const shz::quat& q) {
                                   versor v = { q.x, q.y, q.z, q.w };
                                   versor d;
                                   glm_quat_normalize_to(v, d);
                                   return shz::quat(d[3], d[0], d[1], d[2]);
                               },
                               shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(normalized_or)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).normalized_or(shz::quat(0.0f, 1.0f, 0.0f, 0.0f)) == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, comp).normalized_or(shz::quat(0.0f, 1.0f, 0.0f, 0.0f)) == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::quat(0.0f, 0.0f, 0.0f, 0.0f).normalized_or(shz::quat(0.0f, 1.0f, 0.0f, 0.0f)) == shz::quat(0.0f, 1.0f, 0.0f, 0.0f));

    (benchmark_cmp<shz::quat>)("shz::quat::normalized_or",
                               [](const shz::quat& q) { return q.normalized_or(shz::quat::identity()); },
                               "glm_quat_normalize_to",
                               [](const shz::quat& q) {
                                   versor v = { q.x, q.y, q.z, q.w };
                                   versor d;
                                   glm_quat_normalize_to(v, d);
                                   return shz::quat(d[3], d[0], d[1], d[2]);
                               },
                               shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(normalized_safe)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).normalized_safe() == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, comp).normalized_safe() == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(shz::quat(0.0f, 0.0f, 0.0f, 0.0f).normalized_safe() == shz::quat::identity());

    (benchmark_cmp<shz::quat>)("shz::quat::normalized_safe",
                               [](const shz::quat& q) { return q.normalized_safe(); },
                               "glm_quat_normalize_to",
                               [](const shz::quat& q) {
                                   versor v = { q.x, q.y, q.z, q.w };
                                   versor d;
                                   glm_quat_normalize_to(v, d);
                                   return shz::quat(d[3], d[0], d[1], d[2]);
                               },
                               shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(normalize_or)
    volatile float comp = 4.0f;

    shz::quat q1 = shz::quat(1.0f, 2.0f, 2.0f, 4.0f);
    shz::quat q2 = shz::quat(1.0f, 2.0f, 2.0f, comp);
    shz::quat q3(0.0f, 0.0f, 0.0f, 0.0f);

    q1.normalize_or(shz::quat(0.0f, 1.0f, 0.0f, 0.0f));
    q2.normalize_or(shz::quat(0.0f, 1.0f, 0.0f, 0.0f));
    q3.normalize_or(shz::quat(0.0f, 1.0f, 0.0f, 0.0f));

    GBL_TEST_VERIFY(q1 == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(q2 == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(q3 == shz::quat(0.0f, 1.0f, 0.0f, 0.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(normalize_safe)
    volatile float comp = 4.0f;

    shz::quat q1 = shz::quat(1.0f, 2.0f, 2.0f, 4.0f);
    shz::quat q2 = shz::quat(1.0f, 2.0f, 2.0f, comp);
    shz::quat q3(0.0f, 0.0f, 0.0f, 0.0f);

    q1.normalize_safe();
    q2.normalize_safe();
    q3.normalize_safe();

    GBL_TEST_VERIFY(q1 == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(q2 == shz::quat(0.2f, 0.4f, 0.4f, 0.8f));
    GBL_TEST_VERIFY(q3 == shz::quat::identity());
GBL_TEST_CASE_END

GBL_TEST_CASE(inverse)
    volatile float comp = 4.0f;

    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, 4.0f).inverse() == shz::quat(0.04f, -0.08f, -0.08f, -0.16f));
    GBL_TEST_VERIFY(shz::quat(1.0f, 2.0f, 2.0f, comp).inverse() == shz::quat(0.04f, -0.08f, -0.08f, -0.16f));

    (benchmark_cmp<shz::quat>)("shz::quat::inverse",
                               [](const shz::quat& q) { return q.inverse(); },
                               "glm_quat_inv",
                               [](const shz::quat& q) {
                                   versor v = { q.x, q.y, q.z, q.w };
                                   versor d;
                                   glm_quat_inv(v, d);
                                   return shz::quat(d[3], d[0], d[1], d[2]);
                               },
                               shz::quat(gblRandf(), gblRandf(), gblRandf(), gblRandf()));
GBL_TEST_CASE_END

GBL_TEST_CASE(invert)
    volatile float comp = 4.0f;

    shz::quat q1 = shz::quat(1.0f, 2.0f, 2.0f, 4.0f);
    shz::quat q2 = shz::quat(1.0f, 2.0f, 2.0f, comp);

    q1.invert();
    q2.invert();

    GBL_TEST_VERIFY(q1 == shz::quat(0.04f, -0.08f, -0.08f, -0.16f));
    GBL_TEST_VERIFY(q2 == shz::quat(0.04f, -0.08f, -0.08f, -0.16f));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(identity,
                  from_axis_angle,
                  from_rotation_xyz,
                  from_rotation_zyx,
                  from_rotation_zxy,
                  from_rotation_yxz,
                  from_rotation_xyz_u16,
                  from_rotation_zyx_u16,
                  from_rotation_zxy_u16,
                  from_rotation_yxz_u16,
                  transform_vec3,
                  slerp,
                  nlerp,
                  magnitude,
                  magnitude_fsrra,
                  magnitude_inv,
                  magnitude_inv_fsrra,
                  normalized,
                  normalized_or,
                  normalized_safe,
                  normalize_or,
                  normalize_safe,
                  inverse,
                  invert,
                  to_angles_xyz,
                  to_angles_zyx,
                  to_angles_zxy,
                  to_angles_yxz)