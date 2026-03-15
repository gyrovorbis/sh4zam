#include "../shz_scalar.h"
#include "../shz_trig.h"

#include <math.h>

//! CONSTEXPR EVALUATION

SHZ_FORCE_INLINE float shz_crealf(shz_complex_t c) SHZ_NOEXCEPT {
    return c.real;
}

SHZ_FORCE_INLINE float shz_cimagf(shz_complex_t c) SHZ_NOEXCEPT {
    return c.imag;
}

SHZ_FORCE_INLINE shz_complex_t shz_cinitf(float real, float imag) SHZ_NOEXCEPT {
    return (shz_complex_t){ real, imag };
}

SHZ_FORCE_INLINE shz_complex_t shz_cpolarf(float r, float theta) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(theta);
    return shz_cinitf(r * sc.cos, r * sc.sin);
}

SHZ_FORCE_INLINE bool shz_cequalf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT {
    return shz_equalf(lhs.real, rhs.real) && shz_equalf(lhs.imag, rhs.imag);
}

SHZ_FORCE_INLINE shz_complex_t shz_caddf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT {
    return shz_cinitf(lhs.real + rhs.real, lhs.imag + rhs.imag);
}

SHZ_FORCE_INLINE shz_complex_t shz_csubf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT {
    return shz_cinitf(lhs.real - rhs.real, lhs.imag - rhs.imag);
}

SHZ_FORCE_INLINE shz_complex_t shz_cmulf(shz_complex_t lhs, shz_complex_t rhs) SHZ_NOEXCEPT {
    return shz_cinitf((lhs.real * rhs.real) - (rhs.imag * lhs.imag),
                      (lhs.real * rhs.imag) + (lhs.imag * rhs.real));
}

SHZ_FORCE_INLINE shz_complex_t shz_cdivf(shz_complex_t num, shz_complex_t denom) SHZ_NOEXCEPT {
    float inv_mag = shz_inv_cabsf(denom);

    return shz_cinitf((num.real * denom.real + num.imag * denom.imag) * inv_mag,
                      (num.imag * denom.real - num.real * denom.imag) * inv_mag);
}

SHZ_INLINE shz_complex_t shz_cscalef(shz_complex_t c, float v) SHZ_NOEXCEPT {
    return shz_cinitf(c.real * v, c.imag * v);
}

SHZ_INLINE shz_complex_t shz_crecipf(shz_complex_t c) SHZ_NOEXCEPT {
    float denom = shz_cnormf(c);

    if(denom == 0.0f)
        return shz_cinitf(0.0f, 0.0f);
    else
        denom = shz_invf_fsrra(denom);

    return shz_cinitf(c.real * denom, -c.imag * denom);
}

SHZ_FORCE_INLINE float shz_cabsf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_sqrtf(shz_cnormf(c));
}

SHZ_FORCE_INLINE float shz_inv_cabsf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_inv_sqrtf(shz_cnormf(c));
}

SHZ_FORCE_INLINE float shz_cnormf(shz_complex_t c) SHZ_NOEXCEPT {
    return c.real * c.real + c.imag * c.imag;
}

SHZ_FORCE_INLINE float shz_cargf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_atan2f(c.imag, c.real);
}

SHZ_FORCE_INLINE shz_complex_t shz_conjf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_cinitf(c.real, -c.imag);
}

SHZ_INLINE shz_complex_t shz_cprojf(shz_complex_t c) SHZ_NOEXCEPT {
    return (isinf(c.real) || isinf(c.imag))?
        shz_cinitf(INFINITY, shz_copysignf(0.0f, c.imag)) : c;
}

SHZ_INLINE shz_complex_t shz_csqrtf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_complex_t result;
    float mag = shz_cabsf(c);

    result.real = shz_sqrtf((mag + c.real) * 0.5f);
    result.imag = shz_sqrtf(shz_fabsf((mag - c.real) * 0.5f));
    result.imag = shz_copysignf(result.imag, c.imag);

    return result;
}

SHZ_FORCE_INLINE shz_complex_t shz_cpowf(shz_complex_t base, shz_complex_t exp) SHZ_NOEXCEPT {
    return shz_cexpf(shz_cmulf(exp, shz_clogf(base)));
}

SHZ_INLINE shz_complex_t shz_clogf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_cinitf(shz_logf(shz_cabsf(c)), shz_cargf(c));
}

SHZ_INLINE shz_complex_t shz_cexpf(shz_complex_t c) SHZ_NOEXCEPT {
    float           exp = shz_expf(c.real);
    shz_sincos_t sincos = shz_sincosf(c.imag);

    return shz_cinitf(exp * sincos.cos, exp * sincos.sin);
}

SHZ_INLINE shz_complex_t shz_clog10f(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_cinitf(shz_log10f(shz_cabsf(c)), shz_cargf(c) / shz_logf(10.0f));
}

SHZ_INLINE shz_complex_t shz_csinf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(c.real);

    return shz_cinitf(sc.sin * shz_coshf(c.imag),
                      sc.cos * shz_sinhf(c.imag));
}

SHZ_INLINE shz_complex_t shz_ccosf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(c.real);

    return shz_cinitf( sc.cos * shz_coshf(c.imag),
                      -sc.sin * shz_sinhf(c.imag));
}

SHZ_INLINE shz_complex_t shz_ctanf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(2.0f * c.real);

    /* Mathematically guaranteed to be >= 0.0f, and 0.0f is already
       returning complex NaN per the stdlib spec, so legit. */
    float denom = shz_invf_fsrra(sc.cos + shz_coshf(2.0f * c.imag));

    return shz_cinitf(sc.sin * denom, shz_sinhf(2.0f * c.imag) * denom);
}

SHZ_INLINE shz_complex_t shz_ccscf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_crecipf(shz_csinf(c));
}

SHZ_INLINE shz_complex_t shz_csecf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_crecipf(shz_ccosf(c));
}

SHZ_INLINE shz_complex_t shz_ccotf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_crecipf(shz_ctanf(c));
}

SHZ_INLINE shz_complex_t shz_casinf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_complex_t ic = shz_cmulf(SHZ_I, c);
    shz_complex_t cc = shz_cmulf(c, c);
    shz_complex_t sq = shz_csqrtf(shz_csubf(shz_cinitf(1.0f, 0.0f), cc));

    return shz_cmulf(shz_cscalef(SHZ_I, -1.0f), shz_clogf(shz_caddf(ic, sq)));
}

SHZ_INLINE shz_complex_t shz_cacosf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_complex_t one_sub_sq = shz_csubf(shz_cinitf(1.0f, 0.0f), shz_cmulf(c, c));
    shz_complex_t sqrt_term  = shz_csqrtf(one_sub_sq);
    shz_complex_t inside_log = shz_caddf(c, shz_cmulf(SHZ_I, sqrt_term));

    return shz_cmulf(shz_cscalef(SHZ_I, -1.0f), shz_clogf(inside_log));
}

SHZ_INLINE shz_complex_t shz_catanf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_complex_t one = shz_cinitf(1.0f, 0.0f);
    shz_complex_t num = shz_csubf(one, shz_cmulf(SHZ_I, c));
    shz_complex_t den = shz_caddf(one, shz_cmulf(SHZ_I, c));
    shz_complex_t log = shz_clogf(shz_cdivf(num, den));

    return shz_cmulf(shz_cscalef(SHZ_I, 0.5f), log);
}

SHZ_INLINE shz_complex_t shz_cacscf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_casinf(shz_crecipf(c));
}

SHZ_INLINE shz_complex_t shz_casecf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_cacosf(shz_crecipf(c));
}

SHZ_INLINE shz_complex_t shz_cacotf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_catanf(shz_crecipf(c));
}

SHZ_INLINE shz_complex_t shz_csinhf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(c.imag);

    return shz_cinitf(shz_sinhf(c.real) * sc.cos,
                      shz_coshf(c.real) * sc.sin);
}

SHZ_INLINE shz_complex_t shz_ccoshf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(c.imag);

    return shz_cinitf(shz_coshf(c.real) * sc.cos,
                      shz_sinhf(c.real) * sc.sin);
}

SHZ_INLINE shz_complex_t shz_ctanhf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc    = shz_sincosf(2.0f * c.imag);
    float        denom = shz_coshf(2.0f * c.real) + sc.cos;

    if(denom == 0.0f) // Protect against FSRRA-by-zero
        return shz_cinitf((c.real > 0.0f)? 1.0f : -1.0f, 0.0f);
    else
        denom = shz_invf_fsrra(denom);

    return shz_cinitf(shz_sinhf(2.0f * c.real) * denom,
                      sc.sin * denom);
}

SHZ_INLINE shz_complex_t shz_ccschf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(c.imag);

    // Technically can FSRRA-by-zero, but result should be INF anyway.
    float denom = shz_invf_fsrra(shz_coshf(2.0f * c.real) -
                                 shz_cosf(2.0f * c.imag));

    return shz_cinitf(2.0f * shz_sinhf(c.real) * sc.cos * denom,
                     -2.0f * shz_coshf(c.real) * sc.sin * denom);
}

SHZ_INLINE shz_complex_t shz_csechf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(c.imag);

    // Technically can FSRRA-by-zero, but result should be INF anyway.
    float denom = shz_invf_fsrra(shz_coshf(2.0f * c.real) +
                                 shz_cosf(2.0f * c.imag));

    return shz_cinitf(2.0f * shz_coshf(c.real) * sc.cos * denom,
                     -2.0f * shz_sinhf(c.real) * sc.sin * denom);
}

SHZ_INLINE shz_complex_t shz_ccothf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_sincos_t sc = shz_sincosf(2.0f * c.imag);

    // Technically can FSRRA-by-zero, but result should be INF anyway.
    float denom = shz_invf_fsrra(shz_coshf(2.0f * c.real) - sc.cos);

    return shz_cinitf(shz_sinhf(2.0f * c.real) * denom,
                      -sc.sin * denom);
}

SHZ_INLINE shz_complex_t shz_casinhf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_complex_t csqr_plus_1 =
        shz_cinitf((c.real * c.real) - (c.imag * c.imag) + 1.0f,
                   2.0f * c.real * c.imag);

    return shz_clogf(shz_caddf(c, shz_csqrtf(csqr_plus_1)));
}

SHZ_INLINE shz_complex_t shz_cacoshf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_complex_t s1 = shz_csqrtf(shz_csubf(c, shz_cinitf(1.0f, 0.0f)));
    shz_complex_t s2 = shz_csqrtf(shz_caddf(c, shz_cinitf(1.0f, 0.0f)));

    return shz_clogf(shz_caddf(c, shz_cmulf(s1, s2)));
}

SHZ_INLINE shz_complex_t shz_catanhf(shz_complex_t c) SHZ_NOEXCEPT {
    shz_complex_t num = shz_caddf(shz_cinitf(1.0f, 0.0f), c);
    shz_complex_t den = shz_csubf(shz_cinitf(1.0f, 0.0f), c);

    return shz_cscalef(shz_clogf(shz_cdivf(num, den)), 0.5f);
}

SHZ_INLINE shz_complex_t shz_cacschf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_casinhf(shz_crecipf(c));
}

SHZ_INLINE shz_complex_t shz_casechf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_cacoshf(shz_crecipf(c));
}

SHZ_INLINE shz_complex_t shz_cacothf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_catanhf(shz_crecipf(c));
}
