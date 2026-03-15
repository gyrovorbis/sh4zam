/*! \file
 *  \brief   Out-of-line complex routines.
 *  \ingroup matrix
 *
 *  This file contains the implementations for any routines within
 *  the complex API which have been declared out-of-line.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

#include "sh4zam/shz_complex.h"
#include "sh4zam/shz_xmtrx.h"

#include <stdlib.h>

void shz_fft(shz_complex_t* s, size_t size) {
    // size must be a power of 2.
    assert((size > 0) && ((size & (size - 1)) == 0));

    const float a = -2.0f * SHZ_F_PI / (float)size; // Twiddle factor angle increments

    // This loop determines the stage of the FFT
    for(int len = size >> 1; len > 0 ; len >>= 1) { 

        // This loop determines which Twiddle factor to use
        for(int j = 0; j < len; ++j) {
            shz_xmtrx_init_fft_weights(a * j * ((float)size / (float)(len << 1)));

            // Butterfly operation for a particular Twiddle factor in a particular stage
            for(int i = j; i < size; i += (len << 1)) {
                shz_complex_t* x = &s[i];
                shz_complex_t* y = &s[i + len];

                // Calculate input vector from two input samples
                shz_vec4_t in = shz_vec4_init(x->real,
                                              x->imag,
                                              y->real,
                                              y->imag);

                shz_vec4_t out = shz_xmtrx_transform_vec4(in);

                // Store output vector as two samples
                *x = *(shz_complex_t*)&out.e[0];
                *y = *(shz_complex_t*)&out.e[2];
            }
        }
    }

    int j = 0, k;
    for(int i = 1; i < (size - 1); i++) {
        k = size / 2;
        
        while(k <= j) {
            j -= k;
            k /= 2;
        }
        
        j += k;
        
        if(i < j) {
            shz_complex_t temp = s[j];
            s[j] = s[i];
            s[i] = temp;
        }
    }
}
