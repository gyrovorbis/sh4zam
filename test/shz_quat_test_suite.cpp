#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_quat.hpp"

#include <print>

#define GBL_SELF_TYPE   shz_quat_test_suite

GBL_TEST_FIXTURE {
    char dummy;
};

GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

shz_vec3_t raylibVector3RotateByQuaternion(shz_quat_t q, shz_vec3_t v) {
    shz_vec3_t result = { 0 };

    result.x = v.x*(q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z) + v.y*(2*q.x*q.y - 2*q.w*q.z) + v.z*(2*q.x*q.z + 2*q.w*q.y);
    result.y = v.x*(2*q.w*q.z + 2*q.x*q.y) + v.y*(q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z) + v.z*(-2*q.w*q.x + 2*q.y*q.z);
    result.z = v.x*(-2*q.w*q.y + 2*q.x*q.z) + v.y*(2*q.w*q.x + 2*q.y*q.z)+ v.z*(q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);

    return result;
}

GBL_TEST_CASE(transform_vec3)
   auto test = [&](shz::quat q, shz::vec3 v) SHZ_NO_INLINE {
        shz::vec3 fipr_res, cross_res;

        benchmark(&fipr_res, [&]{ return q.transform(v); });

        benchmark(&cross_res, raylibVector3RotateByQuaternion, q, v);
#if 0
        std::println("<{}, {}, {}> vs <{}, {}, {}>",
                     fipr_res.x, fipr_res.y, fipr_res.z,
                     cross_res.x, cross_res.y, cross_res.z);
#endif
        return gblFloatEquals(fipr_res.x, cross_res.x, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.y, cross_res.y, shz::fipr_max_error) &&
               gblFloatEquals(fipr_res.z, cross_res.z, shz::fipr_max_error);
    };

    for(unsigned i = 0; i < 10; ++i)
        GBL_TEST_VERIFY(test(shz::quat(rand(), rand(), rand(), rand()).normalized_safe(),
                        shz::vec3(rand(), rand(), rand())));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(transform_vec3)