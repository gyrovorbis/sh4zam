#include "shz_test.h"
#include "shz_test.hpp"
#include <sh4zam/shz_trig.hpp>
#include <gimbal/algorithms/gimbal_random.h>
#include <cmath>
#include <print>

#define GBL_SELF_TYPE   shz_trig_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

#define SHZ_FSCA_ERROR_APPROX 0.0052

GBL_TEST_CASE(sincos_from_radians)
    auto test = [&](float radians) GBL_FP_PRECISE {
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

GBL_FP_PRECISE
GBL_TEST_CASE(atanf)
    GBL_TEST_ERROR(shz_atanf(0.0f), atanf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_atanf(11.0f), atanf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_atanf(-2.0f), atanf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(asinf)
    GBL_TEST_ERROR(shz_asinf(0.0f), asinf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_asinf(11.0f), asinf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_asinf(-2.0f), asinf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(acosf)
    GBL_TEST_ERROR(shz_acosf(0.0f), acosf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_acosf(11.0f), acosf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_acosf(-2.0f), acosf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(sinhf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_sinhf, value);
        benchmark(&c_result, sinhf, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(coshf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_coshf, value);
        benchmark(&c_result, coshf, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(tanhf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_tanhf, value);
        benchmark(&c_result, tanhf, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(cschf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_cschf, value);
        benchmark(&c_result, [](float val) { return 1.0f / sinhf(val); }, value);;

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(sechf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_sechf, value);
        benchmark(&c_result, [](float val) { return 1.0f / coshf(val); }, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(cothf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_cothf, value);
        benchmark(&c_result, [](float val) { return 1.0f / tanhf(val); }, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(asinhf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_asinhf, value);
        benchmark(&c_result, asinhf, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(acoshf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_acoshf, value);
        benchmark(&c_result, acoshf, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(1.5430806f));
    GBL_TEST_VERIFY(test(2.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(atanhf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_atanhf, value);
        benchmark(&c_result, atanhf, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.01f));
    GBL_TEST_VERIFY(test(0.99f));
    GBL_TEST_VERIFY(test(-0.99f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(acschf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_acschf, value);
        benchmark(&c_result, [](float val) { return asinhf(1.0f / val); }, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(5.5f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(asechf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_asechf, value);
        benchmark(&c_result, [](float val) { return acoshf(1.0f / val); }, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(0.5f));
    GBL_TEST_VERIFY(test(0.001f));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(acothf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz_acothf, value);
        benchmark(&c_result, [](float val) { return atanhf(1.0f / val); }, value);

        std::println("{} vs {}", shz_result, c_result);
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(2.0f));
    GBL_TEST_VERIFY(test(-2.0f));
    GBL_TEST_VERIFY(test(1.0001f));
GBL_TEST_CASE_END


GBL_TEST_CASE(benches)
    volatile float result;

    for(volatile unsigned i = 0; i < 1; ++i) {
        float random = gblRandUniform(-F_PI, F_PI);
        float random2 = gblRandUniform(-F_PI, F_PI);
        printf("RANDOMNUM: %f\n", random);
        benchmark(&result, shz_atanf, random);
        benchmark(&result, atanf, random);
        benchmark(&result, shz_asinf, random);
        benchmark(&result, asinf, random);
        benchmark(&result, shz_acosf, random);
        benchmark(&result, acosf, random);
        benchmark(&result, shz::atan2f, random, random2);
        benchmark(&result, atan2f, random, random2);
        printf("\n");
    }
    benchmark(&result, shz_pow2f, 13.0f);
    benchmark(&result, shz_log2f, 13.0f);
    benchmark(&result, shz_powf, -3.124f, 13.0f);
    benchmark(&result, powf, -3.124f, 13.0f);
GBL_TEST_CASE_END

GBL_TEST_REGISTER(sincos_from_radians,
                  sincos_from_degrees,
                  atanf,
                  asinf,
                  acosf,
                  sinhf,
                  coshf,
                  tanhf,
                  cschf,
                  sechf,
                  cothf,
                  asinhf,
                  acoshf,
                  atanhf,
                  acschf,
                  asechf,
                  acothf,
                  benches)