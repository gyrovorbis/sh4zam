/*! \file
 *  \brief   Out-of-line complex routines.
 *  \ingroup complex
 *
 *  This file contains the implementations for any routines within
 *  the complex API which have been declared out-of-line, including
 *  the implementations of the SH4ZAM FFT and its utilities.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

#include "sh4zam/shz_complex.h"
#include "sh4zam/shz_xmtrx.h"

SHZ_INLINE void shz_xmtrx_init_fft_butterfly(float twiddle_angle) {
    asm volatile(R"(
        ftrc    %0, fpul
        frchg
        fsca    fpul, dr6

        fldi1   fr0
        fldi0   fr1
        fmov    fr7, fr2
        fldi0   fr4
        fmov    fr6, fr3
        fldi1   fr5

        fschg
        fmov    dr2, dr10
        fneg    fr10
        fneg    fr11
        fmov    dr6, dr14
        fneg    fr6
        fmov    dr0, dr8
        fneg    fr15
        fmov    dr4, dr12
        fschg

        frchg
    )"
    :
    : "f" (twiddle_angle)
    : "fpul");
}

SHZ_INLINE void shz_xmtrx_update_fft_butterfly(float twiddle_angle) {
    asm volatile(R"(
        ftrc    %0, fpul
        frchg
        fsca    fpul, dr6

        fmov    fr7, fr2
        fmov    fr6, fr3

        fschg
        fmov    dr6, dr14
        fneg    fr6
        fmov    dr2, dr10
        fneg    fr15
        fneg    fr10
        fschg
        fneg    fr11
        frchg
    )"
    :
    : "f" (twiddle_angle)
    : "fpul");
}

static void shz_fft_2pt(shz_complex_t* s, size_t size, size_t stage, float factor) {
    float angle = 0.0f;

    for(size_t twiddle = 0; twiddle < stage; ++twiddle) {
        shz_xmtrx_update_fft_butterfly(angle);

        SHZ_FSCHG();
        for(size_t i = twiddle; i < size; i += (stage << 1)) {
           asm volatile(R"(
                fmov.d  @%[x], dr4
                fmov.d  @%[y], dr6
                ftrv    xmtrx, fv4
                fmov.d  dr4, @%[x]
                fmov.d  dr6, @%[y]
            )"
            : "+m" (s[i]), "+m" (s[i + stage])
            : [x] "r" (&s[i]), [y] "r" (&s[i + stage])
            : "fr4", "fr5" ,"fr6", "fr7");
        }
        SHZ_FSCHG();

        angle += factor;
    }
}

static void shz_fft_4pt(shz_complex_t* s, size_t size, size_t stage, float factor) {
    const size_t inc = (stage << 1);
    float angle = 0.0f;

    for(size_t twiddle = 0; twiddle < stage; ++twiddle) {
        shz_xmtrx_update_fft_butterfly(angle);

        SHZ_FSCHG();
        for(size_t i = twiddle; i < size; i += (inc << 1)) {
            asm volatile(R"(
                fmov.d  @%[x], dr4
                fmov.d  @%[y], dr6
                fmov.d  @%[z], dr8
                fmov.d  @%[w], dr10
                ftrv    xmtrx, fv4
                ftrv    xmtrx, fv8
                fmov.d  dr4, @%[x]
                fmov.d  dr6, @%[y]
                fmov.d  dr8, @%[z]
                fmov.d  dr10, @%[w]
            )"
            : "+m" (s[i]), "+m" (s[i + stage]), "+m" (s[i + inc]), "+m" (s[i + inc + stage])
            : [x] "r" (&s[i]), [y] "r" (&s[i + stage]),
              [z] "r" (&s[i + inc]), [w] "r" (&s[i + inc + stage])
            : "fr4", "fr5" ,"fr6", "fr7", "fr8", "fr9", "fr10", "fr11");
        }
        SHZ_FSCHG();

        angle += factor;
    }
}

SHZ_INLINE void shz_fft_8pt(shz_complex_t* s, size_t size, size_t stage, float factor) {
   const size_t inc1 = (stage << 1);
   const size_t inc2 = (inc1  << 1);
   float angle = 0.0f;

    for(size_t twiddle = 0; twiddle < stage; ++twiddle) {
        shz_xmtrx_update_fft_butterfly(angle);

        SHZ_FSCHG();
        for(size_t i = twiddle; i < size; i += (inc2 << 1)) {
            asm volatile(R"(
                fmov.d  @%[x], dr0
                fmov.d  @%[y], dr2
                fmov.d  @%[z], dr4
                fmov.d  @%[w], dr6
                ftrv    xmtrx, fv0

                fmov.d  @%[a], dr8
                fmov.d  @%[b], dr10
                ftrv    xmtrx, fv4

                fmov.d  @%[c], dr12
                fmov.d  @%[d], dr14
                ftrv    xmtrx, fv8

                fmov.d  dr0, @%[x]
                fmov.d  dr2, @%[y]
                ftrv    xmtrx, fv12

                fmov.d  dr4, @%[z]
                fmov.d  dr6, @%[w]
                fmov.d  dr8, @%[a]
                fmov.d  dr10, @%[b]
                fmov.d  dr12, @%[c]
                fmov.d  dr14, @%[d]
            )"
            : "+m" (s[i]), "+m" (s[i + stage]),
              "+m" (s[i + inc1]), "+m" (s[i + inc1 + stage]),
              "+m" (s[i + inc2]), "+m" (s[i + inc2 + stage]),
              "+m" (s[i + inc2 + inc1]), "+m" (s[i + inc2 + inc1 + stage])
            : [x] "r" (&s[i]), [y] "r" (&s[i + stage]),
              [z] "r" (&s[i + inc1]), [w] "r" (&s[i + inc1 + stage]),
              [a] "r" (&s[i + inc2]), [b] "r" (&s[i + inc2 + stage]),
              [c] "r" (&s[i + inc2 + inc1]), [d] "r" (&s[i + inc2 + inc1 + stage])
            : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5" ,"fr6", "fr7",
              "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
        }
        SHZ_FSCHG();

        angle += factor;
    }
}

static void shz_fft_bit_reverse(shz_complex_t* s, size_t size) {
    size_t j = 0, k;

    SHZ_FSCHG();
    for(size_t i = 1; i < (size - 1); i++) {
        k = size >> 1;
        
        while(k <= j) {
            j -= k;
            k >>= 1;
        }
        
        j += k;
        
        if(i < j) {
            asm(R"(
                fmov.d @%[i], xd0
                fmov.d @%[j], xd2
                fmov.d xd0, @%[j]
                fmov.d xd2, @%[i]
            )"
            : "+m" (s[i]), "+m" (s[j])
            : [i] "r" (&s[i]), [j] "r" (&s[j]));
        }
    }
    SHZ_FSCHG();
}

void shz_fft(shz_complex_t* s, size_t size) {
    // Buffer must be 8-byte aligned and a nonzero power-of-two size.
    assert(size && !(size & (size - 1)) && !((uintptr_t)s & 0x7));

    // Calculate smallest angle in radians for each twiddle increment.
    const float twiddle_inc = shz_divf_fsrra(-2.0f * SHZ_F_PI, size) * SHZ_FSCA_RAD_FACTOR;

    // Loop for each stage of the FFT.
    for(size_t twiddle = size >> 1, pairs = 1;
        twiddle > 0;
        twiddle >>= 1, pairs <<= 1)
    {
        // Scale twiddle increment by the current stage.
        const float twiddle_scale = twiddle_inc * shz_divf_fsrra(size, (twiddle << 1));

        // Preinitialize XMTRX with an FFT butterfly matrix.
        shz_xmtrx_init_fft_butterfly(0.0f);

        /* Select the unrolled, optimized inner loop implementation,
           based on how many pairs are being processed at the current stage. */
        if (!(pairs & 3))
            shz_fft_8pt(s, size, twiddle, twiddle_scale);
        else if (!(pairs & 1))
            shz_fft_4pt(s, size, twiddle, twiddle_scale);
        else
            shz_fft_2pt(s, size, twiddle, twiddle_scale);
    }

    // DIF FFTs do bit reversal of the buffer at the end.
    shz_fft_bit_reverse(s, size);
}
