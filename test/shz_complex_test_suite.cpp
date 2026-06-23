#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_complex.hpp"
#include <complex>
#include <cmath>

#define GBL_SELF_TYPE   shz_complex_test_suite

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

static constexpr float SHZ_COMPLEX_ERROR_EXACT  = 1e-4f;
static constexpr float SHZ_COMPLEX_ERROR_APPROX = 0.09f;

// =====================================================================
// Constructors
// =====================================================================

GBL_TEST_CASE(ctor_default)
    shz::complex c;
    (void)c;
GBL_TEST_CASE_END

GBL_TEST_CASE(ctor_value)
    shz::complex c1(3.0f, 4.0f);
    shz::complex c2(5.0f);

    GBL_TEST_VERIFY(c1.real == 3.0f && c1.imag == 4.0f);
    GBL_TEST_VERIFY(c2.real == 5.0f && c2.imag == 0.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(ctor_c_type)
    shz_complex_t raw = { 2.0f, -1.0f };
    shz::complex c(raw);
    GBL_TEST_VERIFY(c.real == 2.0f && c.imag == -1.0f);
GBL_TEST_CASE_END

// =====================================================================
// Member Operators
// =====================================================================

GBL_TEST_CASE(op_assign)
    shz::complex c;
    shz_complex_t raw = { 7.0f, -3.0f };
    c = raw;
    GBL_TEST_VERIFY(c.real == 7.0f && c.imag == -3.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_add_assign)
    shz::complex c(1.0f, 2.0f);
    c += shz::complex(3.0f, 4.0f);
    GBL_TEST_VERIFY(c.real == 4.0f && c.imag == 6.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_sub_assign)
    shz::complex c(5.0f, 7.0f);
    c -= shz::complex(2.0f, 3.0f);
    GBL_TEST_VERIFY(c.real == 3.0f && c.imag == 4.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_mul_assign_complex)
    shz::complex c(1.0f, 2.0f);
    c *= shz::complex(3.0f, 4.0f);
    // (1+2i)(3+4i) = 3+4i+6i+8i^2 = -5+10i
    GBL_TEST_VERIFY(fabsf(c.real - (-5.0f)) <= SHZ_COMPLEX_ERROR_EXACT &&
                    fabsf(c.imag -  10.0f)  <= SHZ_COMPLEX_ERROR_EXACT);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_mul_assign_scalar)
    shz::complex c(2.0f, 3.0f);
    c *= 4.0f;
    GBL_TEST_VERIFY(fabsf(c.real -  8.0f) <= SHZ_COMPLEX_ERROR_EXACT &&
                    fabsf(c.imag - 12.0f) <= SHZ_COMPLEX_ERROR_EXACT);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_div_assign)
    shz::complex c(4.0f, 2.0f), d(2.0f, 1.0f);
    shz::complex expected = shz::cdivf(c, d);
    c /= d;
    GBL_TEST_VERIFY(c == expected);
GBL_TEST_CASE_END

// =====================================================================
// Global Operators
// =====================================================================

GBL_TEST_CASE(op_add)
    shz::complex r = shz::complex(1.0f, 2.0f) + shz::complex(3.0f, 4.0f);
    GBL_TEST_VERIFY(r.real == 4.0f && r.imag == 6.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_sub)
    shz::complex r = shz::complex(5.0f, 7.0f) - shz::complex(2.0f, 3.0f);
    GBL_TEST_VERIFY(r.real == 3.0f && r.imag == 4.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_mul_complex)
    shz::complex r = shz::complex(1.0f, 2.0f) * shz::complex(3.0f, 4.0f);
    // (1+2i)(3+4i) = -5+10i
    GBL_TEST_VERIFY(fabsf(r.real - (-5.0f)) <= SHZ_COMPLEX_ERROR_EXACT &&
                    fabsf(r.imag -  10.0f)  <= SHZ_COMPLEX_ERROR_EXACT);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_mul_scalar)
    shz::complex r = shz::complex(2.0f, 3.0f) * 4.0f;
    GBL_TEST_VERIFY(fabsf(r.real -  8.0f) <= SHZ_COMPLEX_ERROR_EXACT &&
                    fabsf(r.imag - 12.0f) <= SHZ_COMPLEX_ERROR_EXACT);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_div_complex)
    shz::complex a(4.0f, 2.0f), b(2.0f, 1.0f);
    shz::complex r1 = shz::cdivf(a, b);
    shz::complex r2 = a / b;
    GBL_TEST_VERIFY(r1 == r2);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_div_scalar)
    shz::complex r = shz::complex(6.0f, 4.0f) / 2.0f;
    GBL_TEST_VERIFY(fabsf(r.real - 3.0f) <= SHZ_COMPLEX_ERROR_APPROX &&
                    fabsf(r.imag - 2.0f) <= SHZ_COMPLEX_ERROR_APPROX);
GBL_TEST_CASE_END

GBL_TEST_CASE(op_equal)
    GBL_TEST_VERIFY(  shz::complex(1.0f, 2.0f) == shz::complex(1.0f, 2.0f));
    GBL_TEST_VERIFY(!(shz::complex(1.0f, 2.0f) == shz::complex(1.0f, 3.0f)));
    GBL_TEST_VERIFY(!(shz::complex(1.0f, 2.0f) == shz::complex(2.0f, 2.0f)));
GBL_TEST_CASE_END

GBL_TEST_CASE(op_not_equal)
    GBL_TEST_VERIFY(  shz::complex(1.0f, 2.0f) != shz::complex(3.0f, 4.0f));
    GBL_TEST_VERIFY(!(shz::complex(1.0f, 2.0f) != shz::complex(1.0f, 2.0f)));
GBL_TEST_CASE_END

// =====================================================================
// Basic
// =====================================================================

GBL_TEST_CASE(cinitf)
    shz::complex c = shz::cinitf(3.0f, -2.0f);
    GBL_TEST_VERIFY(c.real == 3.0f && c.imag == -2.0f);
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(cpolarf)
    auto test = [](float r, float theta, float exp_re, float exp_im) {
        shz::complex c = shz::cpolarf(r, theta);
        return fabsf(c.real - exp_re) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(c.imag - exp_im) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    // At theta=0, r=1: (1, 0)
    GBL_TEST_VERIFY(test(1.0f, 0.0f,          1.0f, 0.0f));
    // At theta=pi/2, r=1: (0, 1)
    GBL_TEST_VERIFY(test(1.0f, SHZ_F_PI/2.0f, 0.0f, 1.0f));
    // At theta=pi, r=1: (-1, 0)
    GBL_TEST_VERIFY(test(1.0f, SHZ_F_PI,     -1.0f, 0.0f));
    // At theta=pi/4, r=sqrt(2): (1, 1)
    GBL_TEST_VERIFY(test(sqrtf(2.0f), SHZ_F_PI/4.0f, 1.0f, 1.0f));
GBL_TEST_CASE_END

GBL_TEST_CASE(crealf_cimagf)
    shz::complex c(5.0f, -3.0f);
    GBL_TEST_VERIFY(shz::crealf(c) ==  5.0f);
    GBL_TEST_VERIFY(shz::cimagf(c) == -3.0f);
GBL_TEST_CASE_END

GBL_TEST_CASE(cequalf)
    GBL_TEST_VERIFY( shz::cequalf(shz::complex(1.0f, 2.0f), shz::complex(1.0f, 2.0f)));
    GBL_TEST_VERIFY(!shz::cequalf(shz::complex(1.0f, 2.0f), shz::complex(1.0f, 3.0f)));
    GBL_TEST_VERIFY(!shz::cequalf(shz::complex(1.0f, 2.0f), shz::complex(2.0f, 2.0f)));
GBL_TEST_CASE_END

// =====================================================================
// Arithmetic
// =====================================================================

GBL_TEST_CASE(caddf)
    auto test = [](shz::complex a, shz::complex b) {
        std::complex<float> ra(a.real, a.imag), rb(b.real, b.imag);
        shz::complex        result   = shz::caddf(a, b);
        std::complex<float> expected = ra + rb;
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_EXACT &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_EXACT;
    };
    GBL_TEST_VERIFY(test({1.0f, 2.0f}, {3.0f,  4.0f}));
    GBL_TEST_VERIFY(test({-1.0f, 0.0f}, {1.0f,  0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 0.0f}, {0.0f,  0.0f}));
    GBL_TEST_VERIFY(test({1.5f, -2.5f}, {-0.5f, 3.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::caddf",
        [](shz::complex a) { return shz::caddf(a, shz::complex{3.0f, 4.0f}); },
        "operator+(complex)",
        [](shz::complex a) {
            auto r = std::complex<float>(a.real, a.imag) + std::complex<float>(3.0f, 4.0f);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 2.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(csubf)
    auto test = [](shz::complex a, shz::complex b) {
        std::complex<float> ra(a.real, a.imag), rb(b.real, b.imag);
        shz::complex        result   = shz::csubf(a, b);
        std::complex<float> expected = ra - rb;
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_EXACT &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_EXACT;
    };
    GBL_TEST_VERIFY(test({5.0f,  7.0f}, {2.0f,  3.0f}));
    GBL_TEST_VERIFY(test({1.0f,  1.0f}, {1.0f,  1.0f}));
    GBL_TEST_VERIFY(test({0.0f,  0.0f}, {3.0f, -2.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::csubf",
        [](shz::complex a) { return shz::csubf(a, shz::complex{3.0f, 4.0f}); },
        "operator-(complex)",
        [](shz::complex a) {
            auto r = std::complex<float>(a.real, a.imag) - std::complex<float>(3.0f, 4.0f);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 2.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cmulf)
    auto test = [](shz::complex a, shz::complex b) {
        std::complex<float> ra(a.real, a.imag), rb(b.real, b.imag);
        shz::complex        result   = shz::cmulf(a, b);
        std::complex<float> expected = ra * rb;
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_EXACT &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_EXACT;
    };
    // (1+2i)(3+4i) = -5+10i
    GBL_TEST_VERIFY(test({1.0f,  2.0f}, {3.0f,  4.0f}));
    // (1+i)(1-i) = 2
    GBL_TEST_VERIFY(test({1.0f,  1.0f}, {1.0f, -1.0f}));
    // i*i = -1
    GBL_TEST_VERIFY(test({0.0f,  1.0f}, {0.0f,  1.0f}));
    GBL_TEST_VERIFY(test({-2.0f, 3.0f}, {1.0f, -1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cmulf",
        [](shz::complex a) { return shz::cmulf(a, shz::complex{3.0f, 4.0f}); },
        "operator*(complex)",
        [](shz::complex a) {
            auto r = std::complex<float>(a.real, a.imag) * std::complex<float>(3.0f, 4.0f);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 2.0f}));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(cdivf)
    auto test = [](shz::complex a, shz::complex b) {
        std::complex<float> ra(a.real, a.imag), rb(b.real, b.imag);
        shz::complex        result   = shz::cdivf(a, b);
        std::complex<float> expected = ra / rb;
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    // Pure-real division: 4/2 = 2
    GBL_TEST_VERIFY(test({4.0f, 0.0f}, {2.0f, 0.0f}));
    // Unit denominator: result equals numerator
    GBL_TEST_VERIFY(test({3.0f, 4.0f}, {1.0f, 0.0f}));
    // (1+2i)/(1+1i)
    GBL_TEST_VERIFY(test({1.0f, 2.0f}, {1.0f, 1.0f}));
    // (2+2i)/(1+1i) = 2
    GBL_TEST_VERIFY(test({2.0f, 2.0f}, {1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cdivf",
        [](shz::complex a) { return shz::cdivf(a, shz::complex{3.0f, 4.0f}); },
        "operator/(complex)",
        [](shz::complex a) {
            auto r = std::complex<float>(a.real, a.imag) / std::complex<float>(3.0f, 4.0f);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 2.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cscalef)
    auto test = [](shz::complex c, float s) {
        shz::complex result = shz::cscalef(c, s);
        return fabsf(result.real - c.real * s) <= SHZ_COMPLEX_ERROR_EXACT &&
               fabsf(result.imag - c.imag * s) <= SHZ_COMPLEX_ERROR_EXACT;
    };
    GBL_TEST_VERIFY(test({2.0f,  3.0f},  4.0f));
    GBL_TEST_VERIFY(test({1.0f, -1.0f}, -2.0f));
    GBL_TEST_VERIFY(test({0.0f,  0.0f},  5.0f));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cscalef",
        [](shz::complex a) { return shz::cscalef(a, 2.0f); },
        "operator*(float)",
        [](shz::complex a) {
            auto r = std::complex<float>(a.real, a.imag) * 2.0f;
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 2.0f}));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(crecipf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::crecipf(c);
        std::complex<float> expected = std::complex<float>(1.0f, 0.0f) / rc;
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY(test({3.0f, 4.0f}));
    {
        // Zero denominator: expect (0, 0) per implementation
        shz::complex r0 = shz::crecipf({0.0f, 0.0f});
        GBL_TEST_VERIFY(r0.real == 0.0f && r0.imag == 0.0f);
    }
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::crecipf",
        shz::crecipf,
        "1/(std::complex)",
        [](shz::complex c) {
            auto r = std::complex<float>(1.0f, 0.0f) / std::complex<float>(c.real, c.imag);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{3.0f, 4.0f}));
GBL_TEST_CASE_END

// =====================================================================
// Manipulation
// =====================================================================

GBL_FP_PRECISE
GBL_TEST_CASE(cabsf)
    auto test = [](shz::complex c) {
        float result   = shz::cabsf(c);
        float expected = abs(std::complex<float>(c.real, c.imag));
        return fabsf(result - expected) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({3.0f,  4.0f}));   // |3+4i| = 5
    GBL_TEST_VERIFY(test({1.0f,  0.0f}));   // |1|    = 1
    GBL_TEST_VERIFY(test({0.0f,  1.0f}));   // |i|    = 1
    GBL_TEST_VERIFY(test({0.0f,  0.0f}));   // |0|    = 0
    GBL_TEST_VERIFY(test({-3.0f, -4.0f}));  // |-3-4i| = 5
    GBL_TEST_VERIFY((benchmark_cmp<float>)(
        "shz::cabsf",
        shz::cabsf,
        "std::abs",
        [](shz::complex c) { return abs(std::complex<float>(c.real, c.imag)); },
        shz::complex{3.0f, 4.0f}));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(inv_cabsf)
    auto test = [](shz::complex c) {
        float result   = shz::inv_cabsf(c);
        float expected = 1.0f / abs(std::complex<float>(c.real, c.imag));
        return fabsf(result - expected) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({3.0f, 4.0f}));   // 1/5
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));   // 1/1 = 1
    GBL_TEST_VERIFY(test({0.0f, 2.0f}));   // 1/2
    GBL_TEST_VERIFY((benchmark_cmp<float>)(
        "shz::inv_cabsf",
        shz::inv_cabsf,
        "1.0f/std::abs",
        [](shz::complex c) { return 1.0f / abs(std::complex<float>(c.real, c.imag)); },
        shz::complex{3.0f, 4.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cnormf)
    auto test = [](shz::complex c) {
        float result   = shz::cnormf(c);
        float expected = std::norm(std::complex<float>(c.real, c.imag));
        return fabsf(result - expected) <= SHZ_COMPLEX_ERROR_EXACT;
    };
    GBL_TEST_VERIFY(test({3.0f,  4.0f}));   // 9+16 = 25
    GBL_TEST_VERIFY(test({1.0f,  0.0f}));   // 1
    GBL_TEST_VERIFY(test({0.0f,  1.0f}));   // 1
    GBL_TEST_VERIFY(test({0.0f,  0.0f}));   // 0
    GBL_TEST_VERIFY(test({-2.0f, -2.0f}));  // 8
    GBL_TEST_VERIFY((benchmark_cmp<float>)(
        "std::norm",
        [](volatile shz::complex c) { return std::norm(std::complex<float>(static_cast<float>(c.real), static_cast<float>(c.imag))); },
        "shz::cnormf",
        shz::cnormf,
        shz::complex{3.0f, 4.0f}));
GBL_TEST_CASE_END

GBL_FP_PRECISE
GBL_TEST_CASE(cargf)
    auto test = [](shz::complex c) {
        float result   = shz::cargf(c);
        float expected = std::arg(std::complex<float>(c.real, c.imag));
        return fabsf(result - expected) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f,  0.0f}));   // arg = 0
    GBL_TEST_VERIFY(test({0.0f,  1.0f}));   // arg = pi/2
    GBL_TEST_VERIFY(test({-1.0f, 0.0f}));   // arg = pi
    GBL_TEST_VERIFY(test({1.0f,  1.0f}));   // arg = pi/4
    GBL_TEST_VERIFY(test({1.0f, -1.0f}));   // arg = -pi/4
    GBL_TEST_VERIFY((benchmark_cmp<float>)(
        "shz::cargf",
        shz::cargf,
        "std::arg",
        [](shz::complex c) { return std::arg(std::complex<float>(c.real, c.imag)); },
        shz::complex{1.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(conjf)
    auto test = [](shz::complex c) {
        shz::complex        result   = shz::conjf(c);
        std::complex<float> expected = std::conj(std::complex<float>(c.real, c.imag));
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_EXACT &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_EXACT;
    };
    GBL_TEST_VERIFY(test({1.0f,  2.0f}));
    GBL_TEST_VERIFY(test({-3.0f, 4.0f}));
    GBL_TEST_VERIFY(test({0.0f,  0.0f}));
    GBL_TEST_VERIFY(test({5.0f,  0.0f}));
    GBL_TEST_VERIFY(test({0.0f, -7.0f}));
    {
        [[maybe_unused]]
        bool gainz = (benchmark_cmp<shz::complex>)(
            "shz::conjf",
            shz::conjf,
            "std::conj",
            [](volatile shz::complex c) {
                auto r = std::conj(std::complex<float>(static_cast<float>(c.real), static_cast<float>(c.imag)));
                return shz::complex{r.real(), r.imag()};
            },
            shz::complex{1.0f, 2.0f});
        //GBL_TEST_VERIFY(gainz);
    }
GBL_TEST_CASE_END

GBL_TEST_CASE(cprojf)
#if !defined(__FAST_MATH__) && (!defined(__FINITE_MATH_ONLY__) || (__FINITE_MATH_ONLY__ == 0))
    {
        // Normal: returns input unchanged
        shz::complex r1 = shz::cprojf({1.0f, 2.0f});
        GBL_TEST_VERIFY(r1.real == 1.0f && r1.imag == 2.0f);
    }

    {
        // Inf real: project to (inf, copysign(0, imag))
        shz::complex r2 = shz::cprojf({INFINITY, 2.0f});
        GBL_TEST_VERIFY(isinf(r2.real) && r2.imag == 0.0f);
    }

    {
        // Inf imag: project to (inf, copysign(0, imag))
        shz::complex r3 = shz::cprojf({1.0f, -INFINITY});
        GBL_TEST_VERIFY(isinf(r3.real) && r3.imag == -0.0f);
    }
#else
    GBL_TEST_SKIP("Disabled with -ffast-math!");
#endif
GBL_TEST_CASE_END

// =====================================================================
// Transcendental
// =====================================================================

GBL_TEST_CASE(csqrtf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::csqrtf(c);
        std::complex<float> expected = std::sqrt(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({4.0f,  0.0f}));   // sqrt(4) = 2
    GBL_TEST_VERIFY(test({0.0f,  0.0f}));   // sqrt(0) = 0
    GBL_TEST_VERIFY(test({-1.0f, 0.0f}));   // sqrt(-1) = i
    GBL_TEST_VERIFY(test({0.0f,  2.0f}));   // sqrt(2i)
    GBL_TEST_VERIFY(test({3.0f,  4.0f}));   // sqrt(3+4i)
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::csqrtf",
        shz::csqrtf,
        "std::sqrt",
        [](shz::complex c) {
            auto r = std::sqrt(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{3.0f, 4.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cpowf)
    auto test = [](shz::complex base, shz::complex exp) {
        std::complex<float> rb(base.real, base.imag), re(exp.real, exp.imag);
        shz::complex        result   = shz::cpowf(base, exp);
        std::complex<float> expected = std::pow(rb, re);

        return fabsf(result.real - expected.real()) <= 1.0f &&
               fabsf(result.imag - expected.imag()) <= 1.0f;
    };
    GBL_TEST_VERIFY(test({2.0f, 0.0f}, {3.0f, 0.0f}));  // 2^3 = 8
    GBL_TEST_VERIFY(test({0.0f, 1.0f}, {2.0f, 0.0f}));  // i^2 = -1
    GBL_TEST_VERIFY(test({1.0f, 1.0f}, {2.0f, 0.0f}));  // (1+i)^2 = 2i
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cpowf",
        [](shz::complex a) { return shz::cpowf(a, shz::complex{2.0f, 0.0f}); },
        "std::pow",
        [](shz::complex a) {
            auto r = std::pow(std::complex<float>(a.real, a.imag), std::complex<float>(2.0f, 0.0f));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 1.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cexpf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::cexpf(c);
        std::complex<float> expected = std::exp(rc);
        return fabsf(result.real - expected.real()) <= 1.0f &&
               fabsf(result.imag - expected.imag()) <= 1.0f;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));               // e^0 = 1
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));               // e^1 = e
    GBL_TEST_VERIFY(test({0.0f, SHZ_F_PI}));           // e^(i*pi) = -1  (Euler)
    GBL_TEST_VERIFY(test({0.0f, SHZ_F_PI / 2.0f}));   // e^(i*pi/2) = i
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cexpf",
        shz::cexpf,
        "std::exp",
        [](shz::complex c) {
            auto r = std::exp(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(clogf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::clogf(c);
        std::complex<float> expected = std::log(rc);

        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f,  0.0f}));  // ln(1) = 0
    GBL_TEST_VERIFY(test({0.0f,  1.0f}));  // ln(i) = i*pi/2
    GBL_TEST_VERIFY(test({-1.0f, 0.0f}));  // ln(-1) = i*pi
    GBL_TEST_VERIFY(test({1.0f,  1.0f}));
    GBL_TEST_VERIFY(test({3.0f,  4.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::clogf",
        shz::clogf,
        "std::log",
        [](shz::complex c) {
            auto r = std::log(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{3.0f, 4.0f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(clog10f)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::clog10f(c);
        std::complex<float> expected = std::log10(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({10.0f, 0.0f}));  // log10(10) = 1
    GBL_TEST_VERIFY(test({1.0f,  0.0f}));  // log10(1)  = 0
    GBL_TEST_VERIFY(test({1.0f,  1.0f}));
    GBL_TEST_VERIFY(test({0.0f,  1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::clog10f",
        shz::clog10f,
        "std::log10",
        [](shz::complex c) {
            auto r = std::log10(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{3.0f, 4.0f}));
GBL_TEST_CASE_END

// =====================================================================
// Spherical Trigonometry
// =====================================================================

GBL_TEST_CASE(csinf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::csinf(c);
        std::complex<float> expected = std::sin(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY(test({SHZ_F_PI / 2.0f, 0.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::csinf",
        shz::csinf,
        "std::sin",
        [](shz::complex c) {
            auto r = std::sin(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ccosf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ccosf(c);
        std::complex<float> expected = std::cos(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY(test({SHZ_F_PI, 0.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ccosf",
        shz::ccosf,
        "std::cos",
        [](shz::complex c) {
            auto r = std::cos(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ctanf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ctanf(c);
        std::complex<float> expected = std::tan(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ctanf",
        shz::ctanf,
        "std::tan",
        [](shz::complex c) {
            auto r = std::tan(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ccscf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ccscf(c);
        std::complex<float> expected = std::complex<float>(1.0f, 0.0f) / std::sin(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ccscf",
        shz::ccscf,
        "1/std::sin",
        [](shz::complex c) {
            auto r = std::complex<float>(1.0f, 0.0f) / std::sin(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(csecf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::csecf(c);
        std::complex<float> expected = std::complex<float>(1.0f, 0.0f) / std::cos(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::csecf",
        shz::csecf,
        "1/std::cos",
        [](shz::complex c) {
            auto r = std::complex<float>(1.0f, 0.0f) / std::cos(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ccotf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ccotf(c);
        std::complex<float> expected = std::cos(rc) / std::sin(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ccotf",
        shz::ccotf,
        "std::cos/std::sin",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::cos(sc) / std::sin(sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(casinf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::casinf(c);
        std::complex<float> expected = std::asin(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::casinf",
        shz::casinf,
        "std::asin",
        [](shz::complex c) {
            auto r = std::asin(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{0.5f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cacosf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::cacosf(c);
        std::complex<float> expected = std::acos(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cacosf",
        shz::cacosf,
        "std::acos",
        [](shz::complex c) {
            auto r = std::acos(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{0.5f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(catanf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::catanf(c);
        std::complex<float> expected = std::atan(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 0.5f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::catanf",
        shz::catanf,
        "std::atan",
        [](shz::complex c) {
            auto r = std::atan(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{0.5f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cacscf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::cacscf(c);
        // acsc(z) = asin(1/z)
        std::complex<float> expected = std::asin(std::complex<float>(1.0f, 0.0f) / rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({2.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cacscf",
        shz::cacscf,
        "std::asin(1/z)",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::asin(std::complex<float>(1.0f, 0.0f) / sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(casecf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::casecf(c);
        // asec(z) = acos(1/z)
        std::complex<float> expected = std::acos(std::complex<float>(1.0f, 0.0f) / rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({2.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::casecf",
        shz::casecf,
        "std::acos(1/z)",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::acos(std::complex<float>(1.0f, 0.0f) / sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cacotf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::cacotf(c);
        // acot(z) = atan(1/z)
        std::complex<float> expected = std::atan(std::complex<float>(1.0f, 0.0f) / rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({2.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cacotf",
        shz::cacotf,
        "std::atan(1/z)",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::atan(std::complex<float>(1.0f, 0.0f) / sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

// =====================================================================
// Hyperbolic Trigonometry
// =====================================================================

GBL_TEST_CASE(csinhf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::csinhf(c);
        std::complex<float> expected = std::sinh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::csinhf",
        shz::csinhf,
        "std::sinh",
        [](shz::complex c) {
            auto r = std::sinh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ccoshf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ccoshf(c);
        std::complex<float> expected = std::cosh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ccoshf",
        shz::ccoshf,
        "std::cosh",
        [](shz::complex c) {
            auto r = std::cosh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ctanhf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ctanhf(c);
        std::complex<float> expected = std::tanh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ctanhf",
        shz::ctanhf,
        "std::tanh",
        [](shz::complex c) {
            auto r = std::tanh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ccschf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ccschf(c);
        std::complex<float> expected = std::complex<float>(1.0f, 0.0f) / std::sinh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ccschf",
        shz::ccschf,
        "1/std::sinh",
        [](shz::complex c) {
            auto r = std::complex<float>(1.0f, 0.0f) / std::sinh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(csechf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::csechf(c);
        std::complex<float> expected = std::complex<float>(1.0f, 0.0f) / std::cosh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::csechf",
        shz::csechf,
        "1/std::cosh",
        [](shz::complex c) {
            auto r = std::complex<float>(1.0f, 0.0f) / std::cosh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(ccothf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::ccothf(c);
        std::complex<float> expected = std::cosh(rc) / std::sinh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::ccothf",
        shz::ccothf,
        "std::cosh/std::sinh",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::cosh(sc) / std::sinh(sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(casinhf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::casinhf(c);
        std::complex<float> expected = std::asinh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 0.5f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::casinhf",
        shz::casinhf,
        "std::asinh",
        [](shz::complex c) {
            auto r = std::asinh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cacoshf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::cacoshf(c);
        std::complex<float> expected = std::acosh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({2.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cacoshf",
        shz::cacoshf,
        "std::acosh",
        [](shz::complex c) {
            auto r = std::acosh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(catanhf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::catanhf(c);
        std::complex<float> expected = std::atanh(rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({0.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.0f}));
    GBL_TEST_VERIFY(test({0.0f, 0.5f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::catanhf",
        shz::catanhf,
        "std::atanh",
        [](shz::complex c) {
            auto r = std::atanh(std::complex<float>(c.real, c.imag));
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{0.5f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cacschf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::cacschf(c);
        // acsch(z) = asinh(1/z)
        std::complex<float> expected = std::asinh(std::complex<float>(1.0f, 0.0f) / rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({2.0f, 0.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cacschf",
        shz::cacschf,
        "std::asinh(1/z)",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::asinh(std::complex<float>(1.0f, 0.0f) / sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{1.0f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(casechf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::casechf(c);
        // asech(z) = acosh(1/z)
        std::complex<float> expected = std::acosh(std::complex<float>(1.0f, 0.0f) / rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({1.0f, 0.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.0f}));
    GBL_TEST_VERIFY(test({0.5f, 0.5f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::casechf",
        shz::casechf,
        "std::acosh(1/z)",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::acosh(std::complex<float>(1.0f, 0.0f) / sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{0.5f, 0.5f}));
GBL_TEST_CASE_END

GBL_TEST_CASE(cacothf)
    auto test = [](shz::complex c) {
        std::complex<float> rc(c.real, c.imag);
        shz::complex        result   = shz::cacothf(c);
        // acoth(z) = atanh(1/z)
        std::complex<float> expected = std::atanh(std::complex<float>(1.0f, 0.0f) / rc);
        return fabsf(result.real - expected.real()) <= SHZ_COMPLEX_ERROR_APPROX &&
               fabsf(result.imag - expected.imag()) <= SHZ_COMPLEX_ERROR_APPROX;
    };
    GBL_TEST_VERIFY(test({2.0f, 0.0f}));
    GBL_TEST_VERIFY(test({2.0f, 1.0f}));
    GBL_TEST_VERIFY(test({1.0f, 1.0f}));
    GBL_TEST_VERIFY((benchmark_cmp<shz::complex>)(
        "shz::cacothf",
        shz::cacothf,
        "std::atanh(1/z)",
        [](shz::complex c) {
            auto sc = std::complex<float>(c.real, c.imag);
            auto r  = std::atanh(std::complex<float>(1.0f, 0.0f) / sc);
            return shz::complex{r.real(), r.imag()};
        },
        shz::complex{2.0f, 0.5f}));
GBL_TEST_CASE_END

namespace {
void cooley_tukey_fft(shz_complex_t* spectrum, size_t size) {
    size_t j = 0;

    for (size_t i = 1; i < size - 1; i++) {
        size_t bit = size >> 1;

        while (j >= bit) {
            j -= bit;
            bit >>= 1;
        }

        j += bit;
        if (i < j) {
            shz_complex_t tmp = spectrum[i];
            spectrum[i] = spectrum[j];
            spectrum[j] = tmp;
        }
    }

    for (size_t len = 2; len <= size; len <<= 1) {
        float angle_rad = -2.0f * SHZ_F_PI / len;
        shz_complex_t twiddle_unit = { cosf(angle_rad), sinf(angle_rad) };

        for (size_t i = 0; i < size; i += len) {
            shz_complex_t twiddle_cur = {1.0f, 0.0f};

            for (size_t k = 0; k < len / 2; k++) {
                shz_complex_t even = spectrum[i + k];
                shz_complex_t odd = spectrum[i + k + len / 2];
                shz_complex_t twiddled_odd = {
                    odd.real * twiddle_cur.real - odd.imag * twiddle_cur.imag,
                    odd.real * twiddle_cur.imag + odd.imag * twiddle_cur.real
                };
                float twiddle_real_next = twiddle_cur.real * twiddle_unit.real - twiddle_cur.imag * twiddle_unit.imag;

                spectrum[i + k].real = even.real + twiddled_odd.real;
                spectrum[i + k].imag = even.imag + twiddled_odd.imag;
                spectrum[i + k + len / 2].real = even.real - twiddled_odd.real;
                spectrum[i + k + len / 2].imag = even.imag - twiddled_odd.imag;
                twiddle_cur.imag = twiddle_cur.real * twiddle_unit.imag + twiddle_cur.imag * twiddle_unit.real;
                twiddle_cur.real = twiddle_real_next;
            }
        }
    }
}
}

GBL_TEST_CASE(fft)
    constexpr float FFT_ERROR_MAX = 1.0f;
    alignas(8) shz::complex samples[2][1024];

    for(unsigned s = 0; s < 1024; ++s) {
        samples[0][s].real = samples[1][s].real = gblRandUniform(0.0f, 1.0f);
        samples[0][s].imag = samples[1][s].imag = 0.0f;
    }

    shz::fft(samples[0], 1024);
    cooley_tukey_fft(samples[1], 1024);

    for(unsigned s = 0; s < 1024; ++s) {
        GBL_TEST_ERROR(samples[1][s].real, samples[0][s].real, FFT_ERROR_MAX, GBL_TEST_ERROR_FUZZY);
        GBL_TEST_ERROR(samples[1][s].imag, samples[0][s].imag, FFT_ERROR_MAX, GBL_TEST_ERROR_FUZZY);
    }

    GBL_TEST_VERIFY(
        (benchmark_cmp<std::nullptr_t>(
            "shz::fft",         shz::fft,
            "cooley_tukey_fft", cooley_tukey_fft,
            samples[0], 1024
        ))
    );
GBL_TEST_CASE_END

GBL_TEST_REGISTER(ctor_default,
                  ctor_value,
                  ctor_c_type,
                  op_assign,
                  op_add_assign,
                  op_sub_assign,
                  op_mul_assign_complex,
                  op_mul_assign_scalar,
                  op_div_assign,
                  op_add,
                  op_sub,
                  op_mul_complex,
                  op_mul_scalar,
                  op_div_complex,
                  op_div_scalar,
                  op_equal,
                  op_not_equal,
                  cinitf,
                  cpolarf,
                  crealf_cimagf,
                  cequalf,
                  caddf,
                  csubf,
                  cmulf,
                  cdivf,
                  cscalef,
                  crecipf,
                  cabsf,
                  inv_cabsf,
                  cnormf,
                  cargf,
                  conjf,
                  cprojf,
                  csqrtf,
                  cpowf,
                  cexpf,
                  clogf,
                  clog10f,
                  csinf,
                  ccosf,
                  ctanf,
                  ccscf,
                  csecf,
                  ccotf,
                  casinf,
                  cacosf,
                  catanf,
                  cacscf,
                  casecf,
                  cacotf,
                  csinhf,
                  ccoshf,
                  ctanhf,
                  ccschf,
                  csechf,
                  ccothf,
                  casinhf,
                  cacoshf,
                  catanhf,
                  cacschf,
                  casechf,
                  cacothf,
                  fft)
