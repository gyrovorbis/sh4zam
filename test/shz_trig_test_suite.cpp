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
    auto test = [&](volatile float radians) GBL_FP_PRECISE {
        volatile float degrees = shz::rad_to_deg(radians);
        auto sincos = shz::sincos::from_degrees(degrees);
        auto sincos1 = shz::sincos::from_degrees(degrees + 180.0f);
        auto sincos2 = shz::sincos::from_degrees(degrees + 360.0f);

        GBL_CTX_BEGIN(pCtx);
        GBL_TEST_CALL([&] {
            GBL_CTX_BEGIN(pCtx);
            GBL_TEST_ERROR(sincos.sinf(),          sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(shz::sinf_deg(degrees), sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos.cosf(),          cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos1.sinf(), sinf(radians + shz::deg_to_rad(180.0f)), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos2.sinf(), sinf(radians + shz::deg_to_rad(360.0f)), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
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
    GBL_TEST_VERIFY(benchmark_cmp(float, shz::atanf, atanf, gblRandUniform(-shz::pi_f, shz::pi_f)));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(asinf)
    GBL_TEST_ERROR(shz_asinf(0.0f), asinf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_asinf(11.0f), asinf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_asinf(-2.0f), asinf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_VERIFY(benchmark_cmp(float, shz::asinf, asinf, gblRandUniform(-shz::pi_f, shz::pi_f)));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(acosf)
    GBL_TEST_ERROR(shz_acosf(0.0f), acosf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_acosf(11.0f), acosf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_acosf(-2.0f), acosf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_VERIFY(benchmark_cmp(float, shz::acosf, acosf, gblRandUniform(-shz::pi_f, shz::pi_f)));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(sinhf)
    auto test = [&](volatile float value) {
        float shz_result, c_result;

        benchmark(&shz_result, shz::sinhf, value);
        benchmark(&c_result, sinhf, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::coshf, value);
        benchmark(&c_result, coshf, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::tanhf, value);
        benchmark(&c_result, tanhf, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::cschf, value);
        benchmark(&c_result, [](float val) { return 1.0f / sinhf(val); }, value);;
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
        if(isinf(shz_result) && isinf(c_result)) return true;
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

        benchmark(&shz_result, shz::sechf, value);
        benchmark(&c_result, [](float val) { return 1.0f / coshf(val); }, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::cothf, value);
        benchmark(&c_result, [](float val) { return 1.0f / tanhf(val); }, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
        if(isinf(shz_result) && isinf(c_result)) return true;
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

        benchmark(&shz_result, shz::asinhf, value);
        benchmark(&c_result, asinhf, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::acoshf, value);
        benchmark(&c_result, acoshf, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::atanhf, value);
        benchmark(&c_result, atanhf, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::acschf, value);
        benchmark(&c_result, [](float val) { return asinhf(1.0f / val); }, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::asechf, value);
        benchmark(&c_result, [](float val) { return acoshf(1.0f / val); }, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
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

        benchmark(&shz_result, shz::acothf, value);
        benchmark(&c_result, [](float val) { return atanhf(1.0f / val); }, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(2.0f));
    GBL_TEST_VERIFY(test(-2.0f));
    GBL_TEST_VERIFY(test(1.0001f));
GBL_TEST_CASE_END


GBL_TEST_CASE(benches)
    volatile float result;

    for(volatile unsigned i = 0; i < 1; ++i) {
        float random = gblRandUniform(-SHZ_F_PI, SHZ_F_PI);
        float random2 = gblRandUniform(-SHZ_F_PI, SHZ_F_PI);
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