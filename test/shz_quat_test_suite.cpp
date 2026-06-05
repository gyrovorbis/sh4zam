#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_quat.hpp"

#include <print>

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
            float halfTheta = (float) acos(cosHalfTheta);
            float sinHalfTheta = (float) sqrt(1.0f - cosHalfTheta*cosHalfTheta);

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
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI) == shz::quat(0.0f, 1.0f, 0.0f, 0.0f));
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI) == shz::quat(0.0f, 0.0f, 1.0f, 0.0f));
    GBL_TEST_VERIFY(shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI) == shz::quat(0.0f, 0.0f, 0.0f, 1.0f));

    // 90° around each basis axis: verify axis()/angle() round-trip
    {
        const auto q = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
        GBL_TEST_VERIFY(q.axis() == shz::vec3(1.0f, 0.0f, 0.0f));
        GBL_TEST_VERIFY(shz_equalf(q.angle(), SHZ_F_PI_2));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }
    {
        const auto q = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2);
        GBL_TEST_VERIFY(q.axis() == shz::vec3(0.0f, 1.0f, 0.0f));
        GBL_TEST_VERIFY(shz_equalf(q.angle(), SHZ_F_PI_2));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }
    {
        const auto q = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_2);
        GBL_TEST_VERIFY(q.axis() == shz::vec3(0.0f, 0.0f, 1.0f));
        GBL_TEST_VERIFY(shz_equalf(q.angle(), SHZ_F_PI_2));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }

    // arbitrary axis: axis()/angle() round-trip
    {
        const shz::vec3 axis  = shz::vec3(1.0f, 1.0f, 0.0f).direction();
        const float     angle = SHZ_F_PI / 3.0f;
        const auto q = shz::quat::from_axis_angle(axis, angle);
        GBL_TEST_VERIFY(q.axis() == axis);
        GBL_TEST_VERIFY(shz_equalf(q.angle(), angle));
        GBL_TEST_VERIFY(shz_equalf(q.magnitude(), 1.0f));
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(from_angles_xyz)
    // all-zero angles yield identity
    GBL_TEST_VERIFY(shz::quat::from_angles_xyz(0.0f, 0.0f, 0.0f) == shz::quat::identity());

    // single-axis 180° must agree with from_axis_angle
    GBL_TEST_VERIFY(shz::quat::from_angles_xyz(SHZ_F_PI,   0.0f,       0.0f      ) ==
                    shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI));
    GBL_TEST_VERIFY(shz::quat::from_angles_xyz(0.0f,       SHZ_F_PI,   0.0f      ) ==
                    shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI));
    GBL_TEST_VERIFY(shz::quat::from_angles_xyz(0.0f,       0.0f,       SHZ_F_PI  ) ==
                    shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI));

    // single-axis 90° must agree with from_axis_angle
    GBL_TEST_VERIFY(shz::quat::from_angles_xyz(SHZ_F_PI_2, 0.0f,       0.0f      ) ==
                    shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2));
    GBL_TEST_VERIFY(shz::quat::from_angles_xyz(0.0f,       SHZ_F_PI_2, 0.0f      ) ==
                    shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2));
    GBL_TEST_VERIFY(shz::quat::from_angles_xyz(0.0f,       0.0f,       SHZ_F_PI_2) ==
                    shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_2));

    // produced quaternions must be unit quaternions
    GBL_TEST_VERIFY(shz_equalf(shz::quat::from_angles_xyz(0.5f,       1.0f,       1.5f      ).magnitude(), 1.0f));
    GBL_TEST_VERIFY(shz_equalf(shz::quat::from_angles_xyz(SHZ_F_PI_4, SHZ_F_PI_2, SHZ_F_PI  ).magnitude(), 1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(transform_vec3)
    auto check = [](shz::quat q, volatile shz::vec3 v, shz::vec3 expected) {
        return q.transform(v) == expected;
    };

    auto cross_check = [](shz::quat q, volatile shz::vec3 v) {
        return q.transform(v) == shz::vec3(raylibVector3RotateByQuaternion(q, const_cast<shz::vec3&>(v)));
    };

    // identity leaves any vector unchanged
    GBL_TEST_VERIFY(check(shz::quat::identity(), { 1.0f,  2.0f,  3.0f}, { 1.0f,  2.0f,  3.0f}));
    GBL_TEST_VERIFY(check(shz::quat::identity(), {-4.0f,  0.0f,  7.5f}, {-4.0f,  0.0f,  7.5f}));
    GBL_TEST_VERIFY(check(shz::quat::identity(), { 0.0f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}));

    {
        const auto rot90x  = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
        const auto rot90y  = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2);
        const auto rot90z  = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_2);
        const auto rot180x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI);
        const auto rot180y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI);
        const auto rot180z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI);

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
    GBL_TEST_VERIFY(cross_check(shz::quat::from_axis_angle(shz::vec3{1.0f, 1.0f, 0.0f}.direction(), SHZ_F_PI / 3.0f), {1.0f, 2.0f, 3.0f}));
    GBL_TEST_VERIFY(cross_check(shz::quat::from_axis_angle(shz::vec3{1.0f, 1.0f, 1.0f}.direction(), SHZ_F_PI / 4.0f), {-1.0f, 0.5f, 2.0f}));
    GBL_TEST_VERIFY(cross_check(shz::quat::from_angles_xyz(0.5f, 1.0f, 1.5f), {3.0f, -1.0f, 0.0f}));
    GBL_TEST_VERIFY(cross_check(shz::quat::from_angles_xyz(0.3f, 0.7f, 0.2f), {0.0f, 1.0f, -1.0f}));

    GBL_TEST_VERIFY(
        (benchmark_cmp<shz::vec3>)("shz::quat::transform(shz::vec3)",
                                   [](const shz::quat& q, const shz::vec3& v) {
                                        return q.transform(v);
                                   },
                                   "Vector3RotateByQuaternion",
                                   raylibVector3RotateByQuaternion,
                                   shz::quat::from_angles_xyz(0.5f, 1.0f, 1.5f), shz::vec3{ 43.0f, -232.0f, 344.0f })
    );
GBL_TEST_CASE_END

GBL_TEST_CASE(slerp)
    { // At zero.
        const auto id   = shz::quat::identity();
        const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
        const auto r90y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2);
        const auto r45z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_4);
        const auto arb  = shz::quat::from_angles_xyz(0.4f, 0.8f, 1.2f);

        // All these pairs have dot >= 0 so q1 == q and t=0 yields the start quaternion.
        GBL_TEST_VERIFY(shz::quat::slerp(id,   r90x, 0.0f) == id);
        GBL_TEST_VERIFY(shz::quat::slerp(r90x, r90y, 0.0f) == r90x);
        GBL_TEST_VERIFY(shz::quat::slerp(r45z, arb,  0.0f) == r45z);
        GBL_TEST_VERIFY(shz::quat::slerp(arb,  id,   0.0f) == arb);
    }{ // At one.
        const auto id   = shz::quat::identity();
        const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
        const auto r90y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2);
        const auto r45z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_4);
        const auto arb  = shz::quat::from_angles_xyz(0.4f, 0.8f, 1.2f);

        // t=1 always yields the end quaternion regardless of dot sign.
        GBL_TEST_VERIFY(shz::quat::slerp(id,   r90x, 1.0f) == r90x);
        GBL_TEST_VERIFY(shz::quat::slerp(r90x, r90y, 1.0f) == r90y);
        GBL_TEST_VERIFY(shz::quat::slerp(r45z, arb,  1.0f) == arb);
        GBL_TEST_VERIFY(shz::quat::slerp(arb,  id,   1.0f) == id);
    }{ // Identical.
        const auto id   = shz::quat::identity();
        const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
        const auto arb  = shz::quat::from_angles_xyz(0.4f, 0.8f, 1.2f);

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
        const auto q  = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI / 3.0f);
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
        const auto r270 = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 3.0f * SHZ_F_PI_2);

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
            const auto p = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);

            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.25f) == shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_4 * 0.5f));
            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.50f) == shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_4));
            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.75f) == shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_4 * 1.5f));
        }{
            // Non-zero start angle: 45° → 135° around Y, midpoint should be 90°.
            const auto q2 = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_4);
            const auto p2 = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_4 * 3.0f);
            GBL_TEST_VERIFY(shz::quat::slerp(q2, p2, 0.5f) == shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2));
        }
    }{ // Unit magnitude
        const auto id   = shz::quat::identity();
        {
            // The result of slerp on two unit quaternions must itself be a unit quaternion.
            const auto r90x = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
            const auto r90y = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2);
            const auto arb  = shz::quat::from_angles_xyz(0.5f, 1.0f, 1.5f);

            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id,   r90x, 0.0f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id,   r90x, 0.5f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id,   r90x, 1.0f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(r90x, r90y, 0.3f).magnitude(), 1.0f));
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(r90y, arb,  0.7f).magnitude(), 1.0f));
        }{
            // Negative-dot path must also preserve unit magnitude.
            const auto r270 = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, 3.0f * SHZ_F_PI_2);
            GBL_TEST_VERIFY(shz_equalf(shz::quat::slerp(id, r270, 0.5f).magnitude(), 1.0f));
        }
    }{ // Constant angular velocity
        /* slerp travels at constant angular velocity: equal t-steps span equal arcs.
           identity → 90° around Z in four quarter-steps; each step covers PI/8.
        */
        const auto q = shz::quat::identity();
        const auto p = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_2);

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
        GBL_TEST_ERROR(d01, SHZ_F_PI / 8.0f, 0.03f, GBL_TEST_ERROR_FUZZY);
    }{ // Commutativity
        /* slerp(q, p, t) == slerp(p, q, 1-t) when dot(q,p) >= 0.
           The interpolated quaternion is the same point on the arc regardless of direction.
        */
        const auto q = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_4);
        const auto p = shz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, SHZ_F_PI_2);

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
            const auto p_far = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
            GBL_TEST_VERIFY(shz::quat::slerp(q, p_far, 1.0f) == p_far);
        }
    }{ // Geometric correctness
        const shz::vec3 v = {1.0f, 0.0f, 0.0f};
        {
            /* Verify slerp produces the geometrically correct intermediate rotation by
               checking vector transforms at known fractions.
            */
            const auto id   = shz::quat::identity();
            const auto r90z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_2);
            const auto r45z = shz::quat::from_axis_angle({0.0f, 0.0f, 1.0f}, SHZ_F_PI_4);

            // identity → 90° around Z at t=0.5 must rotate {1,0,0} by exactly 45°.
            GBL_TEST_VERIFY(shz::quat::slerp(id, r90z, 0.5f).transform(v) == r45z.transform(v));
            GBL_TEST_VERIFY(shz::quat::slerp(id, r90z, 0.0f).transform(v) == id.transform(v));
            GBL_TEST_VERIFY(shz::quat::slerp(id, r90z, 1.0f).transform(v) == r90z.transform(v));
        }{
            // Same-axis: slerp midpoint of 45°→135° around X must rotate like 90° around X.
            const auto q   = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_4);
            const auto p   = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_4 * 3.0f);
            const auto mid = shz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, SHZ_F_PI_2);
            GBL_TEST_VERIFY(shz::quat::slerp(q, p, 0.5f).transform(v) == mid.transform(v));

            GBL_TEST_VERIFY(
                (benchmark_cmp<shz::quat>)(
                    "shz::quat::slerp",
                    [](shz::quat a, shz::quat b, float t) { return shz::quat::slerp(a, b, t); },
                    "raylibQuaternionSlerp",
                    raylibQuaternionSlerp,
                    shz::quat::from_axis_angle(shz::vec3{1.0f, 1.0f, 1.0f}.direction(), SHZ_F_PI / 3.0f),
                    shz::quat::from_angles_xyz(0.5f, 1.0f, 1.5f),
                    0.35f)
            );
        }
    }
GBL_TEST_CASE_END

GBL_TEST_REGISTER(identity,
                  from_axis_angle,
                  from_angles_xyz,
                  transform_vec3,
                  slerp)