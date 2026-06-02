/*! \file
 *  \brief   Out-of-line SH4 implementation of complex routines.
 *  \ingroup complex
 *
 *  This file contains the hand-optimized shz_fft() algorithm and utility
 *  routines it uses, which have been hand-optimized for the SH4 architecture,
 *  and accelerated to use its vector instructions.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

#include "sh4zam/shz_complex.h"
#include "sh4zam/shz_xmtrx.h"

// Fully initializes XMTRX to be a 2pt butterfly with the given twiddle factor.
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

/*  Updates the angle of the 2pt butterfly held within XMTRX to use the given twiddle factor.

    WARNING: This routine calls FSCHG only once, returning in FMOV.D mode, for gainz.
*/
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
        fneg    fr11
        frchg
    )"
    :
    : "f" (twiddle_angle)
    : "fpul");
}

/* ====================== N-POINT BUTTERFLY SPECIALIZATIONS =========================
   The following routines are specializations of the inner-most FFT loops, which perform
   the actual 2-pt butterfly operation, unrolled for N times into N-pt butterflies.
   ================================================================================== */

// Apply all twiddle factors for the given stage with a 2-PT butterfly DIF.
static void shz_fft_2pt(shz_complex_t* s, size_t size, size_t stage, float factor) {
    float angle = 0.0f;

    for(size_t twiddle = 0; twiddle < stage; ++twiddle) {
        shz_xmtrx_update_fft_butterfly(angle);

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

// Apply all twiddle factors for the given stage with an unrolled 4-PT butterfly DIF.
static void shz_fft_4pt(shz_complex_t* s, size_t size, size_t stage, float factor) {
    const size_t inc = (stage << 1);
    float angle = 0.0f;

    for(size_t twiddle = 0; twiddle < stage; ++twiddle) {
        shz_xmtrx_update_fft_butterfly(angle);

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

// Apply all twiddle factors for the given stage with an unrolled 8-PT butterfly DIF.
static void shz_fft_8pt(shz_complex_t* s, size_t size, size_t stage, float factor) {
    const size_t inc1   = (stage << 1);
    const size_t inc2   = (inc1  << 1);

    float angle = 0.0f;

    for(size_t twiddle = 0; twiddle < stage; ++twiddle) {
        shz_xmtrx_update_fft_butterfly(angle);

        for(size_t i = twiddle; i < size; i += (inc2 << 1)) {
            asm volatile(R"(
                fmov.d  @%[x], dr0
                fmov.d  @%[y], dr2
                fmov.d  @%[z], dr4
                ftrv    xmtrx, fv0

                fmov.d  @%[w], dr6
                fmov.d  @%[a], dr8
                fmov.d  @%[b], dr10
                ftrv    xmtrx, fv4

                fmov.d  dr0, @%[x]
                fmov.d  dr2, @%[y]
                fmov.d  @%[c], dr0
                fmov.d  @%[d], dr2
                ftrv    xmtrx, fv8

                fmov.d  dr4, @%[z]
                fmov.d  dr6, @%[w]
                ftrv    xmtrx, fv0

                fmov.d  dr8, @%[a]
                fmov.d  dr10, @%[b]
                fmov.d  dr0, @%[c]
                fmov.d  dr2, @%[d]
            )"
            : "+m" (s[i]), "+m" (s[i + stage]),
              "+m" (s[i + inc1]), "+m" (s[i + inc1 + stage]),
              "+m" (s[i + inc2]), "+m" (s[i + inc2 + stage]),
              "+m" (s[i + inc2 + inc1]), "+m" (s[i + inc2 + inc1 + stage])
            : [x] "r" (&s[i]), [y] "r" (&s[i + stage]),
              [z] "r" (&s[i + inc1]), [w] "r" (&s[i + inc1 + stage]),
              [a] "r" (&s[i + inc2]), [b] "r" (&s[i + inc2 + stage]),
              [c] "r" (&s[i + inc2 + inc1]), [d] "r" (&s[i + inc2 + inc1 + stage])
            : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5",
              "fr6", "fr7", "fr8", "fr9", "fr10", "fr11");
        }

        SHZ_FSCHG();
        angle += factor;
    }
}

// Apply all twiddle factors for the given stage with an unrolled 16-PT butterfly DIF.
static void shz_fft_16pt(shz_complex_t* s, size_t size, size_t stage, float factor) {
    const size_t inc1   = (stage << 1);
    const size_t inc2   = (inc1  << 1);
    const size_t inc3   = (inc2  << 1);
    const size_t stride = (inc3  << 1);

    float angle = 0.0f;

    for(size_t twiddle = 0; twiddle < stage; ++twiddle) {
        shz_xmtrx_update_fft_butterfly(angle);

        for(size_t i = twiddle; i < size; i += stride) {
            asm volatile(R"(
                fmov.d  @%[x], dr0
                fmov.d  @%[y], dr2
                fmov.d  @%[z], dr4
                ftrv    xmtrx, fv0

                fmov.d  @%[w], dr6
                fmov.d  @%[a], dr8
                fmov.d  @%[b], dr10
                ftrv    xmtrx, fv4

                fmov.d  dr0, @%[x]
                fmov.d  dr2, @%[y]
                fmov.d  @%[c], dr0
                fmov.d  @%[d], dr2
                ftrv    xmtrx, fv8

                fmov.d  dr4, @%[z]
                fmov.d  dr6, @%[w]
                fmov.d  @(r0, %[x]), dr4
                fmov.d  @(r0, %[y]), dr6
                ftrv    xmtrx, fv0

                fmov.d  dr8, @%[a]
                fmov.d  dr10, @%[b]
                fmov.d  @(r0, %[z]), dr8
                fmov.d  @(r0, %[w]), dr10
                ftrv    xmtrx, fv4

                fmov.d  dr0, @%[c]
                fmov.d  dr2, @%[d]
                fmov.d  @(r0, %[a]), dr0
                fmov.d  @(r0, %[b]), dr2
                ftrv    xmtrx, fv8

                fmov.d  dr4, @(r0, %[x])
                fmov.d  dr6, @(r0, %[y])
                fmov.d  @(r0, %[c]), dr4
                fmov.d  @(r0, %[d]), dr6
                ftrv    xmtrx, fv0

                fmov.d  dr8, @(r0, %[z])
                fmov.d  dr10, @(r0, %[w])
                ftrv    xmtrx, fv4

                fmov.d  dr0, @(r0, %[a])
                fmov.d  dr2, @(r0, %[b])
                fmov.d  dr4, @(r0, %[c])
                fmov.d  dr6, @(r0, %[d])
            )"
            :
            : [x] "r" (&s[i]), [y] "r" (&s[i + stage]),
              [z] "r" (&s[i + inc1]), [w] "r" (&s[i + inc1 + stage]),
              [a] "r" (&s[i + inc2]), [b] "r" (&s[i + inc2 + stage]),
              [c] "r" (&s[i + inc2 + inc1]), [d] "r" (&s[i + inc2 + inc1 + stage]),
              "z" (inc3 << 3)
            : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5",
              "fr6", "fr7", "fr8", "fr9", "fr10", "fr11",
              "memory");
        }

        SHZ_FSCHG();
        angle += factor;
    }
}

// Iterates over the given complex sample buffer, reversing the order of its bits.
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

/* Main entry-point for performing an FFT in-place over the given buffer of the given size.

   NOTE: Buffer must be 8-byte aligned for pairwise loads/stores into the FPU, and size must
         be a power-of-two.

   This particular algorithm is a radix-2 decimation in frequency domain (DIF) FFT, where the
   innermost loops of 2-point butterfly operations have been unrolled to N-points per iteration.
*/
void shz_fft_dc(shz_complex_t* s, size_t size) {
    // Buffer must be 8-byte aligned and a nonzero power-of-two size.
    assert(size && !(size & (size - 1)) && !((uintptr_t)s & 0x7));

    // Calculate smallest angle in radians for each twiddle increment.
    const float twiddle_inc = shz_divf_fsrra(-2.0f * SHZ_F_PI, size) * SHZ_FSCA_RAD_FACTOR;

    // Loop for each stage of the FFT.
    for(size_t twiddle = size >> 1, pairs = 1; twiddle > 0; twiddle >>= 1, pairs <<= 1) {
        // Scale twiddle increment by the current stage.
        const float twiddle_scale = twiddle_inc * shz_divf_fsrra(size, twiddle << 1);

        // Preinitialize XMTRX with an FFT butterfly matrix.
        shz_xmtrx_init_fft_butterfly(0.0f);

        /* Select the unrolled, optimized inner loop implementation,
           based on how many pairs are being processed at the current stage.
        */
        if(!(pairs & 7))
            shz_fft_16pt(s, size, twiddle, twiddle_scale);  // 16-PT DIF butterfly.
        else  if (!(pairs & 3))
            shz_fft_8pt(s, size, twiddle, twiddle_scale);   //  8-PT DIF butterfly.
        else  if (!(pairs & 1))
            shz_fft_4pt(s, size, twiddle, twiddle_scale);   //  4-PT DIF butterfly.
        else
            shz_fft_2pt(s, size, twiddle, twiddle_scale);   //  2-PT DIF butterfly.
    }

    // Reverse the order of the bits in our buffer for the final stage of a DIF FFT.
    shz_fft_bit_reverse(s, size);
}
