#include "shz_test.h"
#include "sh4zam/shz_trig.hpp"
#include <gimbal/algorithms/gimbal_random.h>
#include <cmath>

#define GBL_SELF_TYPE   shz_trig_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

#define SHZ_FSCA_ERROR_APPROX 0.0052

GBL_TEST_CASE(sincos_from_radians)
    auto test = [&](float radians) {
        auto sincos = shz::sincos::from_radians(radians);

        GBL_CTX_BEGIN(pCtx);
        GBL_TEST_CALL([&] GBL_NO_INLINE {
            GBL_CTX_BEGIN(pCtx);
            GBL_TEST_ERROR(sincos.sinf(),      sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(shz::sinf(radians), sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos.cosf(),      cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_CTX_END();
        }());
        GBL_TEST_ERROR(shz::cosf(radians), cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_TEST_ERROR(sincos.tanf(),      tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_TEST_ERROR(shz::tanf(radians), tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_CTX_END();
    };

    GBL_TEST_CALL(test(0.0f));
    GBL_TEST_CALL(test(SHZ_F_PI));
    GBL_TEST_CALL(test(SHZ_F_PI * -2.0f));
    GBL_TEST_CALL(test(SHZ_F_PI * 3.01f / 2.0f));
    GBL_TEST_CALL(test(-SHZ_F_PI));
    GBL_TEST_CALL(test(-SHZ_F_PI * -2.777f));
    GBL_TEST_CALL(test(-SHZ_F_PI * 3.41f / 45.656f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(sincos_from_degrees)
    auto test = [&](float radians) GBL_FP_PRECISE {
        float degrees = shz::rad_to_deg(radians);
        auto  sincos  = shz::sincos::from_degrees(degrees);

        GBL_CTX_BEGIN(pCtx);
        GBL_TEST_CALL([&] GBL_NO_INLINE {
            GBL_CTX_BEGIN(pCtx);
            GBL_TEST_ERROR(sincos.sinf(),          sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(shz::sinf_deg(degrees), sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos.cosf(),          cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_CTX_END();
        }());
        GBL_TEST_ERROR(shz::cosf_deg(degrees), cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_TEST_ERROR(sincos.tanf(),          tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_TEST_ERROR(shz::tanf_deg(degrees), tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_CTX_END();
    };
    GBL_TEST_CALL(test(0.0f));
    GBL_TEST_CALL(test(SHZ_F_PI));
    GBL_TEST_CALL(test(SHZ_F_PI * -2.0f));
    GBL_TEST_CALL(test(SHZ_F_PI * 3.01f / 2.0f));
    GBL_TEST_CALL(test(-SHZ_F_PI));
    GBL_TEST_CALL(test(-SHZ_F_PI * -2.777f));
    GBL_TEST_CALL(test(-SHZ_F_PI * 3.41f / 45.656f));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(sincos_from_radians,
                  sincos_from_degrees)