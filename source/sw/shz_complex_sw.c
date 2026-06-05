/*! \file
 *  \brief   Out-of-line SW implementation of complex routines.
 *  \ingroup complex
 *
 *  This file contains the generic software routines which back
 *  the complex number API and the FFT algorithm.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

#include "sh4zam/shz_complex.h"
#include <assert.h>

void shz_fft_sw(shz_complex_t* spectrum, size_t size) {
    // Buffer must be nonzero power-of-two size.
    assert(size && !(size & (size - 1)));

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
        shz_sincos_t sincos = shz_sincosf(angle_rad);
        shz_complex_t twiddle_unit = { sincos.cos, sincos.sin };

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