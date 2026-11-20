#include "shz_test_scenario.h"
#include "shz_test.hpp"
#include <sh4zam/shz_trig.hpp>
#include <gimbal/algorithms/gimbal_random.h>
#include <cmath>
#include <print>

namespace {
    SHZ_NO_INLINE bool within_error(float actual, double precise, double epsilon) {
        if((std::bit_cast<uint32_t>(actual) & 0x7f800000u) == 0x7f800000u)
            return false;

        return std::fabs(static_cast<double>(actual) - precise) <= epsilon;
    }

    // Distance between two angles in radians, where +pi and -pi are the same direction.
    SHZ_NO_INLINE double angle_error(float actual, double precise) {
        const double pi    = 3.14159265358979323846;
        const double error = std::fabs(static_cast<double>(actual) - precise);

        return (error > pi)? std::fabs(error - 2.0 * pi) : error;
    }

    SHZ_NO_FAST_MATH SHZ_NO_INLINE float  ref_sinf(float x)  { return ::sinf(x); }
    SHZ_NO_FAST_MATH SHZ_NO_INLINE float  ref_cosf(float x)  { return ::cosf(x); }
    SHZ_NO_FAST_MATH SHZ_NO_INLINE float  ref_tanf(float x)  { return ::tanf(x); }
    SHZ_NO_FAST_MATH SHZ_NO_INLINE double ref_sin(double x)  { return ::sin(x);  }
    SHZ_NO_FAST_MATH SHZ_NO_INLINE double ref_cos(double x)  { return ::cos(x);  }
}

#define GBL_SELF_TYPE   shz_trig_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

#define SHZ_FSCA_ERROR_APPROX 0.0052

GBL_TEST_CASE(sincos_from_radians)
    auto test = [&](float radians_) {
        volatile float radians = radians_;
        auto sincos = shz::sincos::from_radians(radians);

        GBL_CTX_BEGIN(pCtx);
        GBL_TEST_CALL([&] GBL_NO_INLINE {
            GBL_CTX_BEGIN(pCtx);
            GBL_TEST_ERROR(sincos.sinf(),      ref_sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(shz::sinf(radians), ref_sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos.cosf(),      ref_cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_CTX_END();
        }());
        GBL_TEST_ERROR(shz::cosf(radians), ref_cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_TEST_ERROR(sincos.tanf(),      ref_tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_TEST_ERROR(shz::tanf(radians), ref_tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_CTX_END();
    };

    GBL_TEST_CALL(test(0.0f));
    GBL_TEST_CALL(test(shz::pi_f));
    GBL_TEST_CALL(test(shz::pi_f * -2.0f));
    GBL_TEST_CALL(test(shz::pi_f * 3.01f / 2.0f));
    GBL_TEST_CALL(test(-shz::pi_f));
    GBL_TEST_CALL(test(-shz::pi_f * -2.777f));
    GBL_TEST_CALL(test(-shz::pi_f * 3.41f / 45.656f));
GBL_TEST_CASE_END

GBL_TEST_CASE(sincos_from_u16)
    auto test = [&](uint16_t angle) {
        const auto   sincos = shz::sincos::from_radians(angle);
        const auto   pair   = shz::sincosu16(angle);
        const double rad    = static_cast<double>(angle) *
                              (static_cast<double>(shz::tau_f) / (UINT16_MAX + 1));

        GBL_CTX_BEGIN(pCtx);
        GBL_TEST_ERROR(sincos.sinf(), ref_sin(rad), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_TEST_ERROR(sincos.cosf(), ref_cos(rad), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_TEST_ERROR(pair.sin,      ref_sin(rad), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_TEST_ERROR(pair.cos,      ref_cos(rad), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_CTX_END();
    };

    auto quadrant = [&](uint16_t angle, int sinSign, int cosSign) {
        const auto p = shz::sincos::from_radians(angle);

        GBL_CTX_BEGIN(pCtx);
        GBL_TEST_VERIFY(sinSign > 0? (p.sinf() > 0.0f) : (p.sinf() < 0.0f));
        GBL_TEST_VERIFY(cosSign > 0? (p.cosf() > 0.0f) : (p.cosf() < 0.0f));
        GBL_CTX_END();
    };

    auto sweep = [&]() {
        GBL_CTX_BEGIN(pCtx);
        for(unsigned a = 0; a <= UINT16_MAX; a += 64)
            GBL_TEST_CALL(test(static_cast<uint16_t>(a)));
        GBL_CTX_END();
    };

    GBL_TEST_CALL(test(0));
    GBL_TEST_CALL(test(1));
    GBL_TEST_CALL(test(8192));
    GBL_TEST_CALL(test(16384));
    GBL_TEST_CALL(test(32768));
    GBL_TEST_CALL(test(49152));
    GBL_TEST_CALL(test(65535));

    GBL_TEST_CALL(sweep());

    GBL_TEST_CALL(quadrant( 8192,  1,  1));
    GBL_TEST_CALL(quadrant(24576,  1, -1));
    GBL_TEST_CALL(quadrant(40960, -1, -1));
    GBL_TEST_CALL(quadrant(57344, -1,  1));

    GBL_TEST_VERIFY(shz_equalf(shz::sincos::from_radians(static_cast<uint16_t>(0)).cosf(), 1.0f));
    GBL_TEST_VERIFY(shz_equalf(shz::sincos::from_radians(static_cast<uint16_t>(0)).sinf(), 0.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(sincos_from_degrees)
    auto test = [&](float radians_) {
        volatile float radians = radians_;
        volatile float degrees = shz::rad_to_deg(radians);
        auto sincos = shz::sincos::from_degrees(degrees);
        auto sincos1 = shz::sincos::from_degrees(degrees + 180.0f);
        auto sincos2 = shz::sincos::from_degrees(degrees + 360.0f);

        GBL_CTX_BEGIN(pCtx);
        GBL_TEST_CALL([&] {
            GBL_CTX_BEGIN(pCtx);
            GBL_TEST_ERROR(sincos.sinf(),          ref_sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(shz::sinf_deg(degrees), ref_sinf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos.cosf(),          ref_cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos1.sinf(), ref_sinf(radians + shz::deg_to_rad(180.0f)), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_TEST_ERROR(sincos2.sinf(), ref_sinf(radians + shz::deg_to_rad(360.0f)), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
            GBL_CTX_END();
        }());
        GBL_TEST_ERROR(shz::cosf_deg(degrees), ref_cosf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_ABSOLUTE);
        GBL_TEST_ERROR(sincos.tanf(),          ref_tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_TEST_ERROR(shz::tanf_deg(degrees), ref_tanf(radians), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
        GBL_CTX_END();
    };
    GBL_TEST_CALL(test(0.0f));
    GBL_TEST_CALL(test(shz::pi_f));
    GBL_TEST_CALL(test(shz::pi_f * -2.0f));
    GBL_TEST_CALL(test(shz::pi_f * 3.01f / 2.0f));
    GBL_TEST_CALL(test(-shz::pi_f));
    GBL_TEST_CALL(test(-shz::pi_f * -2.777f));
    GBL_TEST_CALL(test(-shz::pi_f * 3.41f / 45.656f));
GBL_TEST_CASE_END

GBL_TEST_CASE(sinf)
    GBL_TEST_ERROR(shz::sinf(0.0f), ref_sinf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz::sinf(11.0f), ref_sinf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz::sinf(-2.0f), ref_sinf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::sinf", bench_fn(shz::sinf), "sinf", bench_fn(ref_sinf), gblRandUniform(-shz::pi_f, shz::pi_f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(cosf)
    GBL_TEST_ERROR(shz::cosf(0.0f), ref_cosf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz::cosf(11.0f), ref_cosf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz::cosf(-2.0f), ref_cosf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::cosf", bench_fn(shz::cosf), "cosf", bench_fn(ref_cosf), gblRandUniform(-shz::pi_f, shz::pi_f)));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(atanf)
    GBL_TEST_ERROR(shz_atanf(0.0f), atanf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_atanf(11.0f), atanf(11.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_atanf(-2.0f), atanf(-2.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::atanf", bench_fn(shz::atanf), "atanf", bench_fn(atanf), gblRandUniform(-shz::pi_f, shz::pi_f)));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(asinf)
    GBL_TEST_ERROR(shz_asinf(0.0f), asinf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_asinf(0.5f), asinf(0.5f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_asinf(-0.75f), asinf(-0.75f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);

    // Arguments beyond the domain clamp to the end values rather than returning NaN.
    GBL_TEST_VERIFY(shz_asinf(11.0f) ==  SHZ_F_PI_2);
    GBL_TEST_VERIFY(shz_asinf(-2.0f) == -SHZ_F_PI_2);

    GBL_TEST_CALL([&] GBL_NO_INLINE {
        GBL_CTX_BEGIN(pCtx);
        for(float v : unit_domain) {
            const float actual = shz::asinf(volatile_cast(v));
            GBL_TEST_VERIFY(within_error(actual, std::asin(static_cast<double>(v)), SHZ_ASINF_ERROR_MAX));
        }

        for(int i = -1000; i <= 1000; ++i) {
            const float v      = static_cast<float>(i) / 1000.0f;
            const float actual = shz::asinf(volatile_cast(v));
            GBL_TEST_VERIFY(within_error(actual, std::asin(static_cast<double>(v)), SHZ_ASINF_ERROR_MAX));
        }

        GBL_TEST_VERIFY(shz::asinf(volatile_cast( 0.0f))       ==  0.0f);
        GBL_TEST_VERIFY(shz::asinf(volatile_cast( 1.0000001f)) ==  SHZ_F_PI_2);
        GBL_TEST_VERIFY(shz::asinf(volatile_cast(-1.0000001f)) == -SHZ_F_PI_2);
        GBL_TEST_VERIFY(shz::asinf(volatile_cast( 11.0f))      ==  SHZ_F_PI_2);
        GBL_TEST_VERIFY(shz::asinf(volatile_cast(-2.0f))       == -SHZ_F_PI_2);
        GBL_TEST_VERIFY(shz::asinf(volatile_cast(-0.3f))       == -shz::asinf(volatile_cast(0.3f)));
        GBL_CTX_END();
    }());

    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::asinf", bench_fn(shz::asinf), "asinf", bench_fn(asinf), gblRandUniform(-shz::pi_f, shz::pi_f)));
    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::asinf (|x| < 1)", bench_fn(shz::asinf), "asinf", bench_fn(asinf), gblRandUniform(-0.99f, 0.99f)));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(acosf)
    GBL_TEST_ERROR(shz_acosf(0.0f), acosf(0.0f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_acosf(0.5f), acosf(0.5f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);
    GBL_TEST_ERROR(shz_acosf(-0.75f), acosf(-0.75f), SHZ_FSCA_ERROR_APPROX, GBL_TEST_ERROR_FUZZY);

    // Arguments beyond the domain clamp to the end values rather than returning NaN.
    GBL_TEST_VERIFY(shz_acosf(11.0f) == 0.0f);
    GBL_TEST_VERIFY(within_error(shz_acosf(-2.0f), 3.14159265358979323846, 1.0e-6));

    GBL_TEST_CALL([&] GBL_NO_INLINE {
        GBL_CTX_BEGIN(pCtx);
        for(float v : unit_domain) {
            const float actual = shz::acosf(volatile_cast(v));
            GBL_TEST_VERIFY(within_error(actual, std::acos(static_cast<double>(v)), SHZ_ASINF_ERROR_MAX));
        }

        for(int i = -1000; i <= 1000; ++i) {
            const float v      = static_cast<float>(i) / 1000.0f;
            const float actual = shz::acosf(volatile_cast(v));
            GBL_TEST_VERIFY(within_error(actual, std::acos(static_cast<double>(v)), SHZ_ASINF_ERROR_MAX));
        }

        GBL_TEST_VERIFY(shz::acosf(volatile_cast( 1.0000001f)) == 0.0f);
        GBL_TEST_VERIFY(shz::acosf(volatile_cast(-1.0000001f)) == SHZ_F_PI);
        GBL_TEST_VERIFY(shz::acosf(volatile_cast( 11.0f))      == 0.0f);
        GBL_TEST_VERIFY(shz::acosf(volatile_cast(-2.0f))       == SHZ_F_PI);
        GBL_CTX_END();
    }());

    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::acosf", bench_fn(shz::acosf), "acosf", bench_fn(acosf), gblRandUniform(-shz::pi_f, shz::pi_f)));
    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::acosf (|x| < 1)", bench_fn(shz::acosf), "acosf", bench_fn(acosf), gblRandUniform(-0.99f, 0.99f)));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(atan2f)
    GBL_TEST_VERIFY(within_error(shz_atan2f(1.0f, 1.0f), 3.14159265358979323846 / 4.0, 1.0e-6));

    GBL_TEST_CALL([&] GBL_NO_INLINE {
        GBL_CTX_BEGIN(pCtx);
        // Directions all the way around the circle, at several magnitudes.
        for(float magnitude : { 1.0e-3f, 1.0f, 37.5f, 1.0e6f }) {
            for(int i = 0; i < 720; ++i) {
                const double theta  = (i + 0.25) * (2.0 * 3.14159265358979323846 / 720.0);
                const float  x      = static_cast<float>(magnitude * std::cos(theta));
                const float  y      = static_cast<float>(magnitude * std::sin(theta));
                const float  actual = shz::atan2f(volatile_cast(y), volatile_cast(x));
                const double precise = std::atan2(static_cast<double>(y), static_cast<double>(x));

                GBL_TEST_VERIFY(angle_error(actual, precise) <= SHZ_ATAN2F_ERROR_MAX);
                GBL_TEST_VERIFY((actual < 0.0f) == (y < 0.0f));
            }
        }
        GBL_CTX_END();
    }());

    // The polynomial has no error of its own on the axes, whatever SHZ_ATAN2F_TERMS is.
    GBL_TEST_VERIFY(within_error(shz::atan2f(volatile_cast( 0.0f), volatile_cast( 2.0f)),  0.0,                          6.0e-7));
    GBL_TEST_VERIFY(within_error(shz::atan2f(volatile_cast( 3.0f), volatile_cast( 0.0f)),  3.14159265358979323846 / 2.0, 6.0e-7));
    GBL_TEST_VERIFY(within_error(shz::atan2f(volatile_cast(-3.0f), volatile_cast( 0.0f)), -3.14159265358979323846 / 2.0, 6.0e-7));
    GBL_TEST_VERIFY(within_error(shz::atan2f(volatile_cast( 0.0f), volatile_cast(-2.0f)),  3.14159265358979323846,       6.0e-7));

    // The zero vector, and a zero y of either sign counting as positive.
    GBL_TEST_VERIFY(shz::atan2f(volatile_cast( 0.0f), volatile_cast( 0.0f)) == 0.0f);
    GBL_TEST_VERIFY(shz::atan2f(volatile_cast(-0.0f), volatile_cast(-0.0f)) == 0.0f);
    GBL_TEST_VERIFY(shz::atan2f(volatile_cast(-0.0f), volatile_cast(-2.0f)) >  0.0f);
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(sinhf)
    for(auto v : std::array<float, 3>{ 0.0f, 1.0f, -1.0f }) {
        GBL_TEST_ERROR(shz::sinhf(v), sinhf(v), 0.05f, GBL_TEST_ERROR_FUZZY);
        auto vv = static_cast<volatile float>(v);
        GBL_TEST_ERROR(shz::sinhf(vv), sinhf(vv), 0.05f, GBL_TEST_ERROR_FUZZY);
    }

    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::sinhf", bench_fn(shz::sinhf), "sinhf", bench_fn(sinhf), static_cast<float volatile>(-1.0f)));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(coshf)
    for(auto v : std::array<float, 3>{ 0.0f, 1.0f, -1.0f }) {
        GBL_TEST_ERROR(shz::coshf(v), coshf(v), 0.05f, GBL_TEST_ERROR_FUZZY);
        auto vv = static_cast<volatile float>(v);
        GBL_TEST_ERROR(shz::coshf(vv), coshf(vv), 0.05f, GBL_TEST_ERROR_FUZZY);
    }

    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::coshf", bench_fn(shz::coshf), "coshf", bench_fn(coshf), static_cast<float volatile>(-1.0f)));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(tanhf)
    for(auto v : std::array<float, 3>{ 0.0f, 1.0f, -1.0f }) {
        GBL_TEST_VERIFY(shz_equalf(shz_tanhf(v), tanhf(v)));
        //GBL_TEST_ERROR(shz::tanhf(v), tanhf(v), 0.05f, GBL_TEST_ERROR_FUZZY);
        auto vv = static_cast<volatile float>(v);
        GBL_TEST_VERIFY(shz_equalf(shz_tanhf(vv), tanhf(vv)));
        //GBL_TEST_ERROR(shz::tanhf(vv), tanhf(vv), 0.05f, GBL_TEST_ERROR_FUZZY);
    }

    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::tanhf", bench_fn(shz::tanhf), "tanhf", bench_fn(tanhf), static_cast<float volatile>(-1.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(cschf)
    GBL_TEST_SKIP("FUCKED");
    for(auto v : std::array<float, 3>{ 0.5f, 1.0f, -1.0f }) {
        GBL_TEST_ERROR(shz::cschf(v), 1.0f / sinhf(v), 0.2f, GBL_TEST_ERROR_FUZZY);
        auto vv = static_cast<volatile float>(v);
        GBL_TEST_ERROR(shz::cschf(vv), 1.0f / sinhf(vv), 0.2f, GBL_TEST_ERROR_FUZZY);
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::cschf", bench_fn(shz::cschf),
                               "1.0f / sinhf", [](float value) { return 1.0f / sinhf(value); },
                               static_cast<volatile float>(-1.0f))
    );
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(sechf)
    GBL_TEST_SKIP("FUCKED");
    for(auto v : std::array<float, 3>{ 0.1f, 1.0f, -1.0f }) {
        GBL_TEST_ERROR(shz::sechf(v), 1.0f / coshf(v), 0.07f, GBL_TEST_ERROR_FUZZY);
        auto vv = static_cast<volatile float>(v);
        GBL_TEST_ERROR(shz::sechf(vv), 1.0f / coshf(vv), 0.07f, GBL_TEST_ERROR_FUZZY);
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<float>)("shz::sechf", bench_fn(shz::sechf),
                               "1.0f / coshf", [](float value) { return 1.0f / coshf(value); },
                               static_cast<volatile float>(-1.0f))
    );
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(cothf)
    auto test = [&](float value_) {
        volatile float value = value_;
        float shz_result, c_result;

        (benchmark)(&shz_result, "shz::cothf", bench_fn(shz::cothf), value);
        benchmark(&c_result, [](float val) { return 1.0f / tanhf(val); }, value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(asinhf)
    auto test = [&](float value_) {
        volatile float value = value_;
        float shz_result, c_result;

        (benchmark)(&shz_result, "shz::asinhf", bench_fn(shz::asinhf), value);
        (benchmark)(&c_result, "asinhf", bench_fn(asinhf), value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.0f));
    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(-1.0f));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(acoshf)
    auto test = [&](float value_) {
        volatile float value = value_;
        float shz_result, c_result;

        (benchmark)(&shz_result, "shz::acoshf", bench_fn(shz::acoshf), value);
        (benchmark)(&c_result, "acoshf", bench_fn(acoshf), value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(1.0f));
    GBL_TEST_VERIFY(test(1.5430806f));
    GBL_TEST_VERIFY(test(2.0f));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(atanhf)
    auto test = [&](float value_) {
        volatile float value = value_;
        float shz_result, c_result;

        (benchmark)(&shz_result, "shz::atanhf", bench_fn(shz::atanhf), value);
        (benchmark)(&c_result, "atanhf", bench_fn(atanhf), value);
#if 0
        std::println("{} vs {}", shz_result, c_result);
#endif
        return fabsf(shz_result - c_result) <= 0.05f;
    };

    GBL_TEST_VERIFY(test(0.01f));
    GBL_TEST_VERIFY(test(0.99f));
    GBL_TEST_VERIFY(test(-0.99f));
GBL_TEST_CASE_END

SHZ_NO_FAST_MATH
GBL_TEST_CASE(acschf)
    auto test = [&](float value_) {
        volatile float value = value_;
        float shz_result, c_result;

        (benchmark)(&shz_result, "shz::acschf", bench_fn(shz::acschf), value);
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

SHZ_NO_FAST_MATH
GBL_TEST_CASE(asechf)
    auto test = [&](float value_) {
        volatile float value = value_;
        float shz_result, c_result;

        (benchmark)(&shz_result, "shz::asechf", bench_fn(shz::asechf), value);
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

SHZ_NO_FAST_MATH
GBL_TEST_CASE(acothf)
    auto test = [&](float value_) {
        volatile float value = value_;
        float shz_result, c_result;

        (benchmark)(&shz_result, "shz::acothf", bench_fn(shz::acothf), value);
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


GBL_TEST_CASE(lgammaf)
    for(float x : { 0.5f, 1.0f, 2.0f, 3.5f, 6.0f, 10.0f, 20.0f, -0.5f, -1.5f, -2.5f })
        GBL_TEST_ERROR(shz::lgammaf(volatile_cast(x)), std::lgamma(static_cast<double>(x)), 0.2f, GBL_TEST_ERROR_FUZZY);

    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::lgammaf", bench_fn(shz::lgammaf), "lgammaf", bench_fn(lgammaf), 3.5f));
    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::lgammaf (x < 0)", bench_fn(shz::lgammaf), "lgammaf", bench_fn(lgammaf), -2.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(tgammaf)
    for(float x : { 0.5f, 1.0f, 2.0f, 3.5f, 5.0f, -0.5f, -1.5f, -2.5f })
        GBL_TEST_ERROR(shz::tgammaf(volatile_cast(x)), std::tgamma(static_cast<double>(x)), 0.2f, GBL_TEST_ERROR_FUZZY);

    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::tgammaf", bench_fn(shz::tgammaf), "tgammaf", bench_fn(tgammaf), 3.5f));
    GBL_TEST_VERIFY((benchmark_cmp<float>)("shz::tgammaf (x < 0)", bench_fn(shz::tgammaf), "tgammaf", bench_fn(tgammaf), -2.5f));
GBL_TEST_CASE_END

GBL_TEST_CASE(benches)
    volatile float result;

    for(volatile unsigned i = 0; i < 1; i = i + 1) {
        float random = gblRandUniform(-shz::pi_f, shz::pi_f);
        float random2 = gblRandUniform(-shz::pi_f, shz::pi_f);
        (benchmark)(&result, "shz::atan2f", bench_fn(shz::atan2f), random, random2);
        (benchmark)(&result, "atan2f", bench_fn(atan2f), random, random2);
        printf("\n");
    }
    (benchmark)(&result, "shz_pow2f", bench_fn(shz_pow2f), 13.0f);
    (benchmark)(&result, "shz_log2f", bench_fn(shz_log2f), 13.0f);
    (benchmark)(&result, "shz_powf", bench_fn(shz_powf), -3.124f, 13.0f);
    (benchmark)(&result, "powf", bench_fn(powf), -3.124f, 13.0f);
GBL_TEST_CASE_END

GBL_TEST_REGISTER(sincos_from_radians,
                  sincos_from_degrees,
                  sincos_from_u16,
                  sinf,
                  cosf,
                  atanf,
                  asinf,
                  acosf,
                  atan2f,
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
                  lgammaf,
                  tgammaf,
                  benches)