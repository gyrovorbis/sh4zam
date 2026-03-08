#include "../shz_scalar.h"
#include "../shz_trig.h"

#include <math.h>

SHZ_FORCE_INLINE float shz_crealf(shz_complex_t c) SHZ_NOEXCEPT {
    return c.real;
}

SHZ_FORCE_INLINE float shz_cimagf(shz_complex_t c) SHZ_NOEXCEPT {
    return c.imag;
}

SHZ_FORCE_INLINE shz_complex_t shz_cinitf(float real, float imag) SHZ_NOEXCEPT {
    return (shz_complex_t){ real, imag };
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
    float inv_mag = shz_inv_sqrtf(denom.real * denom.real + denom.imag * denom.imag);

    return shz_cinitf((num.real * denom.real + num.imag * denom.imag) * inv_mag,
                      (num.imag * denom.real + num.real * denom.imag) * inv_mag);

}

SHZ_FORCE_INLINE float shz_cabsf(shz_complex_t c) SHZ_NOEXCEPT {
    return shz_sqrtf(c.real * c.real + c.imag * c.imag);
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

    result.real = shz_sqrtf((mag + c.real) / 2.0f);
    result.imag = shz_sqrtf(shz_fabsf((mag - c.real) / 2.0f));
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

SHZ_FORCE_INLINE shz_complex_t shz_csinf(shz_complex_t c) SHZ_NOEXCEPT {
    // sin(x+iy)=sin(x)cosh(y)+icos(x)sinh(y)
    shz_sincos_t sc = shz_sincosf(c.real);
    return shz_cinitf(sc.sin * shz_coshf(c.imag),
                      sc.cos * shz_sinhf(c.imag));
}