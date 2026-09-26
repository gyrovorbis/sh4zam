//! \cond INTERNAL
/*! \file
    \brief SH4 implementation of the Memory API.
    \ingroup memory

    This file contains the Dreamcast implementation routines for
    SH4ZAM's memory API, which have been hand-optimized specifically
    for the SH4 architecture.

    \author    2025, 2026 Falco Girgis
    \author    2020 MoopTheHedgehog

    \copyright MIT License
*/
#ifndef SHZ_MEM_SH4_INL_H
#define SHZ_MEM_SH4_INL_H

#include <string.h>
#include <assert.h>

#define SHZ_FSCHG()         asm volatile("fschg")
#define SHZ_PREFETCH_(ptr)  asm volatile("pref @%0" : : "r" (ptr))

extern void* shz_memcpy128_sh4_  (void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) SHZ_NOEXCEPT;
extern void* shz_sq_memcpy32_sh4_(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) SHZ_NOEXCEPT;
extern void* shz_memcpy_large_sh4_(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) SHZ_NOEXCEPT;
extern void* shz_memcpy_mis_sh4_  (void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) SHZ_NOEXCEPT;

SHZ_FORCE_INLINE void shz_dcache_alloc_line_sh4(void* src) SHZ_NOEXCEPT {
    shz_alias_uint32_t *src32 = (shz_alias_uint32_t *)src;

    asm volatile(
       "movca.l r0, @%8"
     : "=m" (src32[0]),
       "=m" (src32[1]),
       "=m" (src32[2]),
       "=m" (src32[3]),
       "=m" (src32[4]),
       "=m" (src32[5]),
       "=m" (src32[6]),
       "=m" (src32[7])
     : "r" (src32));
}

SHZ_FORCE_INLINE void shz_memcpy32_store_sh4_(uint64_t* SHZ_RESTRICT* dst) SHZ_NOEXCEPT {
    asm(R"(
        add       #32, %[dst]
        fmov.d    dr10, @-%[dst]
        fmov.d    dr8,  @-%[dst]
        fmov.d    dr6,  @-%[dst]
        fmov.d    dr4,  @-%[dst]
    )"
    : "=m" ((*dst)[0]), "=m" ((*dst)[1]), "=m" ((*dst)[2]), "=m" ((*dst)[3])
    : [dst] "r" (*dst));
}

SHZ_FORCE_INLINE void shz_memcpy32_load_sh4_(const uint64_t* SHZ_RESTRICT* src) SHZ_NOEXCEPT {
    asm(R"(
        fmov.d    @%[src]+, dr4
        fmov.d    @%[src]+, dr6
        fmov.d    @%[src]+, dr8
        fmov.d    @%[src]+, dr10
    )"
    : [src] "+r" (*src), "+m" (*src)
    : "m" ((*src)[0]), "m" ((*src)[1]), "m" ((*src)[2]), "m" ((*src)[3])
    : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11");
}

SHZ_FORCE_INLINE void shz_memcpy64_load_sh4_(const uint64_t* SHZ_RESTRICT* src) SHZ_NOEXCEPT {
    asm(R"(
        fmov.d    @%[src]+, dr0
        fmov.d    @%[src]+, dr2
        fmov.d    @%[src]+, dr4
        fmov.d    @%[src]+, dr6
        fmov.d    @%[src]+, dr8
        fmov.d    @%[src]+, dr10
        fmov.d    @%[src]+, dr12
        fmov.d    @%[src]+, dr14
    )"
    : [src] "+&r" (*src), "+m" (*src)
    : "m" ((*src)[0]), "m" ((*src)[1]), "m" ((*src)[2]), "m" ((*src)[3]),
      "m" ((*src)[4]), "m" ((*src)[5]), "m" ((*src)[6]), "m" ((*src)[7])
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");
}

SHZ_FORCE_INLINE void shz_memcpy64_store_sh4_(uint64_t* SHZ_RESTRICT* dst) SHZ_NOEXCEPT {
    asm(R"(
        add       #32, %[dst]
        movca.l   r0, @%[dst]
        add       #32, %[dst]

        fmov.d    dr14, @-%[dst]
        fmov.d    dr12, @-%[dst]
        fmov.d    dr10, @-%[dst]
        fmov.d    dr8,  @-%[dst]

        add       #-32, %[dst]
        movca.l   r0, @%[dst]
        add       #32, %[dst]

        fmov.d    dr6,  @-%[dst]
        fmov.d    dr4,  @-%[dst]
        fmov.d    dr2,  @-%[dst]
        fmov.d    dr0,  @-%[dst]
    )"
    : [dst] "+r" (*dst),
      "=m" ((*dst)[0]), "=m" ((*dst)[1]), "=m" ((*dst)[2]), "=m" ((*dst)[3]),
      "=m" ((*dst)[4]), "=m" ((*dst)[5]), "=m" ((*dst)[6]), "=m" ((*dst)[7]));
}

SHZ_FORCE_INLINE void shz_memcpy64_store_no_movca_sh4_(uint64_t* SHZ_RESTRICT dst) SHZ_NOEXCEPT {
    asm(R"(
        pref      @%[dst]
        add       #64, %[dst]
        fmov.d    dr14, @-%[dst]
        fmov.d    dr12, @-%[dst]
        fmov.d    dr10, @-%[dst]
        fmov.d    dr8,  @-%[dst]
        fmov.d    dr6,  @-%[dst]
        fmov.d    dr4,  @-%[dst]
        fmov.d    dr2,  @-%[dst]
        fmov.d    dr0,  @-%[dst]
    )"
    : "=m" ((dst)[0]), "=m" ((dst)[1]), "=m" ((dst)[2]), "=m" ((dst)[3]),
      "=m" ((dst)[4]), "=m" ((dst)[5]), "=m" ((dst)[6]), "=m" ((dst)[7])
    : [dst] "r" (dst));
}

// Based on routine from GLdc from MoopTheHedgehog.
SHZ_FORCE_INLINE void shz_memcpy1_sh4_(      void* SHZ_RESTRICT dst,
                                       const void* SHZ_RESTRICT src,
                                            size_t              bytes) SHZ_NOEXCEPT {
       uint32_t scratch;
       uint32_t diff = (uintptr_t)dst - (((uintptr_t)src) + 1);

       asm(R"(
            shlr    %[cnt]
            bf      0f

            cmp/pl  %[cnt]
            mov.b   @%[in]+, %[scr]
            bf.s    1f
            mov.b   %[scr], @(%[offset], %[in])
        0:
            mov.b   @%[in]+, %[scr]
            dt      %[cnt]
            mov.b   %[scr], @(%[offset], %[in])
            mov.b   @%[in]+, %[scr]
            bf.s    0b
            mov.b   %[scr], @(%[offset], %[in])
        1:
        )"
        : [in] "+&r" (src), [scr] "=&r" (scratch), [cnt] "+&r" (bytes), "=m" (*((uint8_t (*)[])dst))
        : [offset] "z" (diff), "m" (*((const uint8_t (*)[])src))
        : "t");
}

SHZ_FORCE_INLINE void* shz_memcpy1_sh4(      void* SHZ_RESTRICT dst,
                                 const void* SHZ_RESTRICT src,
                                      size_t              bytes) SHZ_NOEXCEPT {
    if(bytes)
        shz_memcpy1_sh4_(dst, src, bytes);

    return dst;
}

SHZ_INLINE void* shz_memcpy2_sh4(void*       SHZ_RESTRICT dst,
                                 const void* SHZ_RESTRICT src,
                                      size_t              bytes) SHZ_NOEXCEPT {
    const shz_alias_uint16_t* s = (const shz_alias_uint16_t*)src;
          shz_alias_uint16_t* d = (      shz_alias_uint16_t*)dst;

    assert(!(bytes % 2) && !((uintptr_t)dst & 1) && !((uintptr_t)src & 1));

    bytes >>= 1;

    size_t blocks = bytes >> 3; // Block size of 16 bytes

    if(blocks) {
        s += 8;

        do {
            const uint16_t r0 = *(--s);
            const uint16_t r1 = *(--s);
            const uint16_t r2 = *(--s);
            const uint16_t r3 = *(--s);
            const uint16_t r4 = *(--s);
            const uint16_t r5 = *(--s);
            const uint16_t r6 = *(--s);
            const uint16_t r7 = *(--s);
            s += 16;
            *d++ = r7;
            *d++ = r6;
            *d++ = r5;
            *d++ = r4;
            *d++ = r3;
            *d++ = r2;
            *d++ = r1;
            *d++ = r0;
        } while(SHZ_LIKELY(--blocks));
        s -= 8;
        bytes &= 0x7;
    }

    if(SHZ_LIKELY(bytes)) {
       uint32_t diff = (uintptr_t)d - (((uintptr_t)s) + 2);
       uintptr_t scratch;

        asm(R"(
            .align 2
            0:
                dt      %[cnt]
                mov.w   @%[in]+, %[scr]
                bf.s    0b
                mov.w   %[scr], @(%[offset], %[in])
        )"
        : [scr] "=&r" (scratch), [in] "+&r" (s),
          [cnt] "+&r" (bytes), "=m" (*((uint16_t (*)[])d))
        : [offset] "z" (diff), "m" (*((const uint16_t (*)[])s))
        : "t");
    }

    return dst;
}

SHZ_INLINE void* shz_memcpy4_sh4(void*       SHZ_RESTRICT dst,
                                 const void* SHZ_RESTRICT src,
                                 size_t                   bytes) SHZ_NOEXCEPT {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t*)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t*)dst;

    assert(!(bytes % 4) && !((uintptr_t)dst & 3) && !((uintptr_t)src & 3));

    bytes >>= 2;

    size_t blocks = bytes >> 3;

   if(blocks) {
        s += 8;

        do {
            const uint32_t r0 = *(--s);
            const uint32_t r1 = *(--s);
            const uint32_t r2 = *(--s);
            const uint32_t r3 = *(--s);
            const uint32_t r4 = *(--s);
            const uint32_t r5 = *(--s);
            const uint32_t r6 = *(--s);
            const uint32_t r7 = *(--s);
            s += 16;
            *d++ = r7;
            *d++ = r6;
            *d++ = r5;
            *d++ = r4;
            *d++ = r3;
            *d++ = r2;
            *d++ = r1;
            *d++ = r0;
        } while(SHZ_LIKELY(--blocks));
        s -= 8;
        bytes &= 0x7;
    }

    if(SHZ_LIKELY(bytes)) {
       uint32_t diff = (uintptr_t)d - (((uintptr_t)s) + 4);
       uintptr_t scratch;

        asm(R"(
            .align 2
            0:
                dt      %[cnt]
                mov.l   @%[in]+, %[scr]
                bf.s    0b
                mov.l   %[scr], @(%[offset], %[in])
        )"
        : [scr] "=&r" (scratch), [in] "+&r" (s),
          [cnt] "+&r" (bytes), "=m" (*((uint32_t (*)[])d))
        : [offset] "z" (diff), "m" (*((const uint32_t (*)[])s))
        : "t");
    }

    return dst;
}

SHZ_INLINE void* shz_memcpy8_sh4(      void* SHZ_RESTRICT dst,
                                 const void* SHZ_RESTRICT src,
                                      size_t              bytes) SHZ_NOEXCEPT {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;

    assert(!(bytes % 8) && !((uintptr_t)dst & 7) && !((uintptr_t)src & 7));

    SHZ_FSCHG();

    bytes >>= 3;
    size_t blocks = bytes >> 3;

    if(blocks) {
        do {
            SHZ_PREFETCH(s + 4);
            shz_memcpy64_load_sh4_(&s);
            shz_memcpy64_store_no_movca_sh4_(d);
            SHZ_PREFETCH(s);
            d += 8;
        } while(SHZ_LIKELY(--blocks));
        bytes &= 0x7;
    }

    if(SHZ_LIKELY(bytes)) {
       uint32_t diff = (uintptr_t)d - (((uintptr_t)s) + 8);

        asm(R"(
            .align 2
            0:
                dt       %[cnt]
                fmov.d   @%[in]+, dr4
                bf.s     0b
                fmov.d   dr4, @(%[offset], %[in])
        )"
        : [in] "+&r" (s), [cnt] "+&r" (bytes), "=m" (*((uint64_t (*)[])d))
        : [offset] "z" (diff), "m" (*((const uint64_t (*)[])s))
        : "t", "fr4", "fr5");
    }

    SHZ_FSCHG();

    return dst;
}

void* shz_memset8_sh4_(void* dst, uint64_t value, size_t bytes) SHZ_NOEXCEPT;

SHZ_FORCE_INLINE void* shz_memset8_sh4(void* dst, uint64_t value, size_t bytes) SHZ_NOEXCEPT {
    return shz_memset8_sh4_(dst, value, bytes);
}

SHZ_COLD
SHZ_FORCE_INLINE void* shz_memcpy32_sh4(      void* SHZ_RESTRICT dst,
                                        const void* SHZ_RESTRICT src,
                                        size_t                   bytes) SHZ_NOEXCEPT {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;

    SHZ_PREFETCH(src);

    assert(!(bytes % 32) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    size_t cnt = (bytes >> 5);
    size_t rem = (cnt & 0x3);

    if(SHZ_LIKELY(rem)) {
        const shz_alias_uint64_t* p = s;

        SHZ_FSCHG();
        do {
            SHZ_PREFETCH(p += 4);
            shz_memcpy32_load_sh4_(&s);
            shz_dcache_alloc_line(d);
            shz_memcpy32_store_sh4_(&d);
            d += 4;
        } while(--rem);
        SHZ_FSCHG();
    }

    if((cnt >>= 2))
        shz_memcpy128_sh4_(d, s, (cnt << 7));

    return dst;
}

SHZ_FORCE_INLINE void* shz_sq_memcpy32_sh4(      void* SHZ_RESTRICT dst,
                                           const void* SHZ_RESTRICT src,
                                           size_t                   bytes) SHZ_NOEXCEPT {
    assert(!(bytes & 31) && !((uintptr_t)dst & 3) && !((uintptr_t)src & 7));

    return shz_sq_memcpy32_sh4_(dst, src, bytes);
}

SHZ_INLINE void* shz_sq_memcpy32_xmtrx_sh4(      void* SHZ_RESTRICT dst,
                                           const void* SHZ_RESTRICT src,
                                           size_t                   bytes) SHZ_NOEXCEPT {
    void* ret = dst;

    assert(!(bytes % 32) && !((uintptr_t)dst & 7) && !((uintptr_t)src & 7));

    bytes >>= 5;

    if(bytes) {
        SHZ_FSCHG();

        asm volatile(R"(
        1:
            fmov.d @%[src]+, xd0
            fmov.d @%[src]+, xd2
            fmov.d @%[src]+, xd4
            fmov.d @%[src]+, xd6
            pref   @%[src]          ! Prefetch 32 bytes for next loop
            add    #32, %[dst]
            dt     %[blks]          ! while(n--)
            fmov.d xd6, @-%[dst]
            fmov.d xd4, @-%[dst]
            fmov.d xd2, @-%[dst]
            fmov.d xd0, @-%[dst]
            pref   @%[dst]          ! Fire off store queue
            bf.s   1b
            add    #32, %[dst]
        )"
        : [dst] "+r" (dst), [src] "+&r" (src), [blks] "+r" (bytes), "=m" ((char (*)[])dst)
        : "m" (*(const char (*)[])src)
        : "t");

        SHZ_FSCHG();
    }

    return ret;
}

SHZ_FORCE_INLINE void* shz_memcpy64_sh4(      void* SHZ_RESTRICT dst,
                                        const void* SHZ_RESTRICT src,
                                             size_t              bytes) SHZ_NOEXCEPT {
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;

    assert(!(bytes % 64) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    size_t cnt = (bytes >> 6);

    if(SHZ_LIKELY(cnt >= 4)) {
        size_t copied = bytes / 128 * 128;
        shz_memcpy128_sh4_(d, s, copied);
        cnt -= copied / 64;
        d += copied / sizeof(uint64_t);
        s += copied / sizeof(uint64_t);
    }

    while(SHZ_LIKELY(cnt--)) {
        SHZ_FSCHG();
        SHZ_PREFETCH(s + 4);
        shz_memcpy64_load_sh4_(&s);
        shz_memcpy64_store_sh4_(&d);
        SHZ_PREFETCH(s);
        d += 8;
        SHZ_FSCHG();
    }

    return dst;
}

SHZ_FORCE_INLINE void* shz_memcpy128_sh4(      void* SHZ_RESTRICT dst,
                                         const void* SHZ_RESTRICT src,
                                             size_t               bytes) SHZ_NOEXCEPT {
    assert(!(bytes % 128) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    return shz_memcpy128_sh4_(dst, src, bytes);
}

/* Copies blocks * 32 bytes (blocks >= 1) to a 4-byte aligned dst from a src
   that is NOT 4-byte aligned, reading aligned words and stitching them back
   together: XTRCT when src is off by 2, SHLD + OR when off by 1 or 3.
   Software-pipelined through a ring of four registers, with the final block
   peeled so that no word past the end of src is ever read. The odd-offset
   schedule keeps every SHLD 0 or 3+ cycles after a load: one issued exactly
   2 cycles after any load stalls for a cycle, even when independent of it.
   18 cycles per 32 bytes when off by 2, 27 when off by 1 or 3. The loops are
   32-byte aligned: the odd-offset one is 51 instructions, and an unlucky
   placement costs an extra i-cache line per iteration (~4 cycles). */
SHZ_FORCE_INLINE void shz_memcpy_shift_sh4_(      void* SHZ_RESTRICT dst,
                                            const void* SHZ_RESTRICT src,
                                                 size_t              blocks) SHZ_NOEXCEPT {
    uintptr_t k = (uintptr_t)src & 3;
    const void* s = (const uint8_t*)src - k;
    int n = (int)blocks - 1;
    uint32_t a0, a1, a2, a3, t0, t1;

    if(k == 2) {
        asm(R"(
            mov.l   @%[s]+, %[a3]
            mov.l   @%[s]+, %[a0]
            mov.l   @%[s]+, %[a1]
            cmp/pl  %[n]
            bf      2f
            .balign 32
        1:
            xtrct   %[a0], %[a3]
            mov.l   @%[s]+, %[a2]
            xtrct   %[a1], %[a0]
            mov.l   %[a3], @(0, %[d])
            mov.l   @%[s]+, %[a3]
            xtrct   %[a2], %[a1]
            mov.l   %[a0], @(4, %[d])
            mov.l   @%[s]+, %[a0]
            xtrct   %[a3], %[a2]
            mov.l   %[a1], @(8, %[d])
            mov.l   @%[s]+, %[a1]
            xtrct   %[a0], %[a3]
            mov.l   %[a2], @(12, %[d])
            mov.l   @%[s]+, %[a2]
            xtrct   %[a1], %[a0]
            mov.l   %[a3], @(16, %[d])
            mov.l   @%[s]+, %[a3]
            xtrct   %[a2], %[a1]
            mov.l   %[a0], @(20, %[d])
            mov.l   @%[s]+, %[a0]
            xtrct   %[a3], %[a2]
            mov.l   %[a1], @(24, %[d])
            mov.l   @%[s]+, %[a1]
            mov.l   %[a2], @(28, %[d])
            dt      %[n]
            bf.s    1b
            add     #32, %[d]
        2:
            xtrct   %[a0], %[a3]
            mov.l   @%[s]+, %[a2]
            xtrct   %[a1], %[a0]
            mov.l   %[a3], @(0, %[d])
            mov.l   @%[s]+, %[a3]
            xtrct   %[a2], %[a1]
            mov.l   %[a0], @(4, %[d])
            mov.l   @%[s]+, %[a0]
            xtrct   %[a3], %[a2]
            mov.l   %[a1], @(8, %[d])
            mov.l   @%[s]+, %[a1]
            xtrct   %[a0], %[a3]
            mov.l   %[a2], @(12, %[d])
            mov.l   @%[s]+, %[a2]
            xtrct   %[a1], %[a0]
            mov.l   %[a3], @(16, %[d])
            mov.l   @%[s]+, %[a3]
            xtrct   %[a2], %[a1]
            mov.l   %[a0], @(20, %[d])
            xtrct   %[a3], %[a2]
            mov.l   %[a1], @(24, %[d])
            mov.l   %[a2], @(28, %[d])
        )"
        : [d] "+&r" (dst), [s] "+&r" (s), [n] "+&r" (n),
          [a0] "=&r" (a0), [a1] "=&r" (a1), [a2] "=&r" (a2), [a3] "=&r" (a3),
          "=m" (*(uint8_t (*)[])dst)
        : "m" (*(const uint8_t (*)[])src)
        : "t");
    } else {
        int rsh = -(int)(k << 3);
        int lsh = 32 + rsh;
        asm(R"(
            mov.l   @%[s]+, %[a3]
            mov.l   @%[s]+, %[a0]
            mov     %[a0], %[t0]
            cmp/pl  %[n]
            bf      2f
            .balign 32
        1:
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a1]
            shld    %[rsh], %[a3]
            or      %[t0], %[a3]
            mov     %[a1], %[t1]
            shld    %[lsh], %[t1]
            mov.l   @%[s]+, %[a2]
            shld    %[rsh], %[a0]
            mov.l   %[a3], @(0, %[d])
            or      %[t1], %[a0]
            mov     %[a2], %[t0]
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a3]
            shld    %[rsh], %[a1]
            mov.l   %[a0], @(4, %[d])
            or      %[t0], %[a1]
            mov     %[a3], %[t1]
            shld    %[lsh], %[t1]
            mov.l   @%[s]+, %[a0]
            shld    %[rsh], %[a2]
            mov.l   %[a1], @(8, %[d])
            or      %[t1], %[a2]
            mov     %[a0], %[t0]
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a1]
            shld    %[rsh], %[a3]
            mov.l   %[a2], @(12, %[d])
            or      %[t0], %[a3]
            mov     %[a1], %[t1]
            shld    %[lsh], %[t1]
            mov.l   @%[s]+, %[a2]
            shld    %[rsh], %[a0]
            mov.l   %[a3], @(16, %[d])
            or      %[t1], %[a0]
            mov     %[a2], %[t0]
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a3]
            shld    %[rsh], %[a1]
            mov.l   %[a0], @(20, %[d])
            or      %[t0], %[a1]
            mov     %[a3], %[t1]
            shld    %[lsh], %[t1]
            mov.l   @%[s]+, %[a0]
            shld    %[rsh], %[a2]
            mov.l   %[a1], @(24, %[d])
            or      %[t1], %[a2]
            mov     %[a0], %[t0]
            mov.l   %[a2], @(28, %[d])
            dt      %[n]
            bf.s    1b
            add     #32, %[d]
        2:
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a1]
            shld    %[rsh], %[a3]
            or      %[t0], %[a3]
            mov     %[a1], %[t1]
            shld    %[lsh], %[t1]
            mov.l   @%[s]+, %[a2]
            shld    %[rsh], %[a0]
            mov.l   %[a3], @(0, %[d])
            or      %[t1], %[a0]
            mov     %[a2], %[t0]
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a3]
            shld    %[rsh], %[a1]
            mov.l   %[a0], @(4, %[d])
            or      %[t0], %[a1]
            mov     %[a3], %[t1]
            shld    %[lsh], %[t1]
            mov.l   @%[s]+, %[a0]
            shld    %[rsh], %[a2]
            mov.l   %[a1], @(8, %[d])
            or      %[t1], %[a2]
            mov     %[a0], %[t0]
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a1]
            shld    %[rsh], %[a3]
            mov.l   %[a2], @(12, %[d])
            or      %[t0], %[a3]
            mov     %[a1], %[t1]
            shld    %[lsh], %[t1]
            mov.l   @%[s]+, %[a2]
            shld    %[rsh], %[a0]
            mov.l   %[a3], @(16, %[d])
            or      %[t1], %[a0]
            mov     %[a2], %[t0]
            shld    %[lsh], %[t0]
            mov.l   @%[s]+, %[a3]
            shld    %[rsh], %[a1]
            mov.l   %[a0], @(20, %[d])
            or      %[t0], %[a1]
            mov     %[a3], %[t1]
            shld    %[lsh], %[t1]
            shld    %[rsh], %[a2]
            mov.l   %[a1], @(24, %[d])
            or      %[t1], %[a2]
            mov.l   %[a2], @(28, %[d])
        )"
        : [d] "+&r" (dst), [s] "+&r" (s), [n] "+&r" (n),
          [a0] "=&r" (a0), [a1] "=&r" (a1), [a2] "=&r" (a2), [a3] "=&r" (a3),
          [t0] "=&r" (t0), [t1] "=&r" (t1), "=m" (*(uint8_t (*)[])dst)
        : [lsh] "r" (lsh), [rsh] "r" (rsh), "m" (*(const uint8_t (*)[])src)
        : "t");
    }
}

/* Byte loop for tiny copies (any alignment); bytes may be 0. Kept compact,
   since small copies are often made with cold code. */
SHZ_FORCE_INLINE void shz_memcpy_tiny_sh4_(      void* SHZ_RESTRICT dst,
                                           const void* SHZ_RESTRICT src,
                                                size_t              bytes) SHZ_NOEXCEPT {
    uint32_t a;

    asm(R"(
        tst     %[n], %[n]
        bt      1f
    0:
        mov.b   @%[s]+, %[a]
        dt      %[n]
        mov.b   %[a], @%[d]
        bf.s    0b
        add     #1, %[d]
    1:
    )"
    : [d] "+&r" (dst), [s] "+&r" (src), [n] "+&r" (bytes), [a] "=&r" (a),
      "=m" (*(uint8_t (*)[])dst)
    : "m" (*(const uint8_t (*)[])src)
    : "t");
}

/* Copies bytes >= 4 between co-aligned buffers ((dst ^ src) & 3 == 0): up to a
   byte + a halfword to reach word alignment, pairs of words, then a halfword
   and/or byte tail. Uses few enough registers that callers need no stack frame. */
SHZ_FORCE_INLINE void shz_memcpy_small_sh4_(      void* SHZ_RESTRICT dst,
                                            const void* SHZ_RESTRICT src,
                                                 size_t              bytes) SHZ_NOEXCEPT {
    uint32_t a, b, w;
    register uintptr_t r0 asm("r0");

    asm(R"(
        mov     %[d], r0
        tst     #1, r0
        bt      .La2%=
        mov.b   @%[s]+, %[a]
        add     #-1, %[n]
        mov.b   %[a], @%[d]
        add     #1, %[d]
        mov     %[d], r0
    .La2%=:
        tst     #2, r0
        bt      .Lw%=
        mov.w   @%[s]+, %[a]
        add     #-2, %[n]
        mov.w   %[a], @%[d]
        add     #2, %[d]
    .Lw%=:
        mov     %[n], %[w]
        shlr2   %[w]
        shlr    %[w]
        bf      .Lwp%=
        mov.l   @%[s]+, %[a]
        mov.l   %[a], @%[d]
        add     #4, %[d]
    .Lwp%=:
        tst     %[w], %[w]
        bt      .Lwt%=
    .Lwl%=:
        mov.l   @%[s]+, %[a]
        mov.l   @%[s]+, %[b]
        dt      %[w]
        mov.l   %[a], @%[d]
        mov.l   %[b], @(4, %[d])
        bf.s    .Lwl%=
        add     #8, %[d]
    .Lwt%=:
        mov     %[n], r0
        tst     #2, r0
        bt      .Lw1%=
        mov.w   @%[s]+, %[a]
        mov.w   %[a], @%[d]
        add     #2, %[d]
    .Lw1%=:
        tst     #1, r0
        bt      .Lend%=
        mov.b   @%[s]+, %[a]
        mov.b   %[a], @%[d]
    .Lend%=:
    )"
    : [d] "+&r" (dst), [s] "+&r" (src), [n] "+&r" (bytes),
      [a] "=&r" (a), [b] "=&r" (b), [w] "=&r" (w), "=&z" (r0),
      "=m" (*(uint8_t (*)[])dst)
    : "m" (*(const uint8_t (*)[])src)
    : "t");
}

/* Only the small cases are inline, and they use no callee-saved registers; the
   block paths live out of line in shz_memcpy_large_sh4_(), whose stack frame
   would otherwise be paid on every call, including 1-byte copies.
   Copies below 8 bytes take a compact byte loop: fewest i-cache lines touched
   when the code is cold. Co-aligned copies below 128 bytes take the inline
   word path. All mutually misaligned copies go straight to
   shz_memcpy_mis_sh4_(), which makes its own size decisions: every extra test
   here cost ~19 cycles of taken branches and literal loads on that path. */
SHZ_FORCE_INLINE void* shz_memcpy_sh4(      void* SHZ_RESTRICT dst,
                                      const void* SHZ_RESTRICT src,
                                          size_t               bytes) SHZ_NOEXCEPT {
    if(bytes < 8) {
        shz_memcpy_tiny_sh4_(dst, src, bytes);
        return dst;
    }

    if(((uintptr_t)dst ^ (uintptr_t)src) & 3)
        return shz_memcpy_mis_sh4_(dst, src, bytes);

    if(bytes < 128) {
        shz_memcpy_small_sh4_(dst, src, bytes);
        return dst;
    }

    return shz_memcpy_large_sh4_(dst, src, bytes);
}

/* shz_memcpy() is only safe for disjoint ranges: its 32-byte paths allocate
   dst cache lines with MOVCA.L, wiping any overlapping src not yet read. */
SHZ_FORCE_INLINE void* shz_memmove_sh4(void* dst, const void* src, size_t bytes) SHZ_NOEXCEPT {
    if((uintptr_t)dst + bytes <= (uintptr_t)src || (uintptr_t)src + bytes <= (uintptr_t)dst)
        return shz_memcpy(dst, src, bytes);
    else
        return memmove(dst, src, bytes);
}

SHZ_INLINE void shz_memcpy2_8_sh4(      void* SHZ_RESTRICT dst,
                                  const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    assert(!((uintptr_t)dst & 0x1) && !((uintptr_t)src & 0x1));

    asm(R"(
        mov.w   @%[s]+, r0
        mov.w   @%[s]+, r1
        mov.w   @%[s]+, r2
        mov.w   @%[s]+, r3
        mov.w   @%[s]+, r4
        mov.w   @%[s]+, r5
        mov.w   @%[s]+, r6
        mov.w   @%[s]+, r7
        add     #16, %[d]
        mov.w   r7, @-%[d]
        mov.w   r6, @-%[d]
        mov.w   r5, @-%[d]
        mov.w   r4, @-%[d]
        mov.w   r3, @-%[d]
        mov.w   r2, @-%[d]
        mov.w   r1, @-%[d]
        mov.w   r0, @-%[d]
    )"
    : [s] "+&r" (src), "=m" (*(uint8_t (*)[16])dst)
    : [d] "r" (dst), "m" (*(const uint8_t (*)[16])src)
    : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7");
}

SHZ_INLINE void shz_memcpy2_16_sh4(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    assert(!((uintptr_t)dst & 0x1) && !((uintptr_t)src & 0x1));

    asm(R"(
        mov.w   @%[s]+, r0
        mov.w   @%[s]+, r1
        mov.w   @%[s]+, r2
        mov.w   @%[s]+, r3
        mov.w   @%[s]+, r4
        mov.w   @%[s]+, r5
        mov.w   @%[s]+, r6
        mov.w   @%[s]+, r7
        add     #16, %[d]
        mov.w   r7, @-%[d]
        mov.w   r6, @-%[d]
        mov.w   r5, @-%[d]
        mov.w   r4, @-%[d]
        mov.w   r3, @-%[d]
        mov.w   r2, @-%[d]
        mov.w   r1, @-%[d]
        mov.w   r0, @-%[d]
        mov.w   @%[s]+, r0
        mov.w   @%[s]+, r1
        mov.w   @%[s]+, r2
        mov.w   @%[s]+, r3
        mov.w   @%[s]+, r4
        mov.w   @%[s]+, r5
        mov.w   @%[s]+, r6
        mov.w   @%[s]+, r7
        add     #32, %[d]
        mov.w   r7, @-%[d]
        mov.w   r6, @-%[d]
        mov.w   r5, @-%[d]
        mov.w   r4, @-%[d]
        mov.w   r3, @-%[d]
        mov.w   r2, @-%[d]
        mov.w   r1, @-%[d]
        mov.w   r0, @-%[d]
    )"
    : [d] "+r" (dst), [s] "+r" (src), "=m" (*(uint8_t (*)[32])dst)
    : "m" (*(uint8_t (*)[32])src)
    : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7");
}

SHZ_INLINE void shz_memset2_16_sh4(void* dst, uint16_t value) SHZ_NOEXCEPT {
    assert(!((uintptr_t)dst & 0x1));

    asm(R"(
        add     #32, %1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
        mov.w   %2, @-%1
    )"
    : "=m" (*(uint8_t (*)[32])dst)
    : "r" (dst), "r" (value));
}

SHZ_INLINE void shz_memcpy4_16_sh4(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    assert(!((uintptr_t)src & 0x3) && !((uintptr_t)dst & 0x3));

    asm(R"(
        mov.l   @%[s]+, r0
        mov.l   @%[s]+, r1
        mov.l   @%[s]+, r2
        mov.l   @%[s]+, r3
        mov.l   r0, @%[d]
        mov.l   r1, @( 4, %[d])
        mov.l   r2, @( 8, %[d])
        mov.l   r3, @(12, %[d])
        mov.l   @%[s]+, r0
        mov.l   @%[s]+, r1
        mov.l   @%[s]+, r2
        mov.l   @%[s]+, r3
        mov.l   r0, @(16, %[d])
        mov.l   r1, @(20, %[d])
        mov.l   r2, @(24, %[d])
        mov.l   r3, @(28, %[d])
        mov.l   @%[s]+, r0
        mov.l   @%[s]+, r1
        mov.l   @%[s]+, r2
        mov.l   @%[s]+, r3
        mov.l   r0, @(32, %[d])
        mov.l   r1, @(36, %[d])
        mov.l   r2, @(40, %[d])
        mov.l   r3, @(44, %[d])
        mov.l   @%[s]+, r0
        mov.l   @%[s]+, r1
        mov.l   @%[s]+, r2
        mov.l   @%[s]+, r3
        mov.l   r0, @(48, %[d])
        mov.l   r1, @(52, %[d])
        mov.l   r2, @(56, %[d])
        mov.l   r3, @(60, %[d])
        add     #-64, %[s]
    )"
    : "=m" (*(uint8_t (*)[64])dst)
    : [s] "r" (src), [d] "r" (dst), "m" (*(const uint8_t (*)[64])src)
    : "r0", "r1", "r2", "r3");
}

SHZ_INLINE void shz_memcpy32_1_sh4(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;

    assert(!((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    SHZ_PREFETCH(s);

    SHZ_FSCHG();

    shz_memcpy32_load_sh4_(&s);
    shz_dcache_alloc_line(d);
    shz_memcpy32_store_sh4_(&d);

    SHZ_FSCHG();
}

SHZ_INLINE void shz_memswap32_1_sh4(void* SHZ_RESTRICT p1,
                                    void* SHZ_RESTRICT p2) SHZ_NOEXCEPT {
    shz_alias_uint32_t (*a)[8] = (shz_alias_uint32_t (*)[8])p1;
    shz_alias_uint32_t (*b)[8] = (shz_alias_uint32_t (*)[8])p2;

    assert(!((uintptr_t)p1 & 7) && !((uintptr_t)p2 & 7));

    SHZ_PREFETCH(b);
    SHZ_FSCHG();

    asm(R"(
        fmov.d  @%[a]+, dr0
        fmov.d  @%[a]+, dr2
        fmov.d  @%[a]+, dr4
        fmov.d  @%[a]+, dr6

        fmov.d  @%[b]+, dr8
        fmov.d  @%[b]+, dr10
        fmov.d  @%[b]+, dr12
        fmov.d  @%[b]+, dr14

        fmov.d  dr14, @-%[a]
        fmov.d  dr12, @-%[a]
        fmov.d  dr10, @-%[a]
        fmov.d  dr8,  @-%[a]

        fmov.d  dr6, @-%[b]
        fmov.d  dr4, @-%[b]
        fmov.d  dr2, @-%[b]
        fmov.d  dr0, @-%[b]
    )"
    : "+m" (*(uint8_t (*)[32])a), "+m" (*(uint8_t (*)[32])b)
    : [a] "r" (a), [b] "r" (b)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");

    SHZ_FSCHG();
}

SHZ_INLINE void shz_memswap32_1_xmtrx_sh4(void* SHZ_RESTRICT p1,
                                          void* SHZ_RESTRICT p2) SHZ_NOEXCEPT {
    shz_alias_uint32_t (*a)[8] = (shz_alias_uint32_t (*)[8])p1;
    shz_alias_uint32_t (*b)[8] = (shz_alias_uint32_t (*)[8])p2;

    assert(!((uintptr_t)p1 & 7) && !((uintptr_t)p2 & 7));

    SHZ_PREFETCH(b);
    SHZ_FSCHG();

    asm volatile(R"(
        fmov.d  @%[a]+, xd0
        fmov.d  @%[a]+, xd2
        fmov.d  @%[a]+, xd4
        fmov.d  @%[a]+, xd6

        fmov.d  @%[b]+, xd8
        fmov.d  @%[b]+, xd10
        fmov.d  @%[b]+, xd12
        fmov.d  @%[b]+, xd14

        fmov.d  xd14, @-%[a]
        fmov.d  xd12, @-%[a]
        fmov.d  xd10, @-%[a]
        fmov.d  xd8,  @-%[a]

        fmov.d  xd6, @-%[b]
        fmov.d  xd4, @-%[b]
        fmov.d  xd2, @-%[b]
        fmov.d  xd0, @-%[b]
    )"
    : "+m" (*(uint8_t (*)[32])a), "+m" (*(uint8_t (*)[32])b)
    : [a] "r" (a), [b] "r" (b));

    SHZ_FSCHG();
}

SHZ_INLINE void* shz_sq_memcpy32_1_sh4(      void* SHZ_RESTRICT dst,
                                       const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t*)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t*)dst;

    assert(!((uintptr_t)s & 7) && !((uintptr_t)d & 7));

    SHZ_FSCHG();

    asm volatile(R"(
        fmov.d @%[src]+, dr0
        fmov.d @%[src]+, dr2
        fmov.d @%[src]+, dr4
        fmov.d @%[src]+, dr6
        add    #32, %[dst]
        fmov.d dr6, @-%[dst]
        fmov.d dr4, @-%[dst]
        fmov.d dr2, @-%[dst]
        fmov.d dr0, @-%[dst]
        pref   @%[dst]          ! Fire off store queue
    )"
    : [src] "+r" (s), [dst] "+r" (d),
      "=m" (d[0]), "=m" (d[1]), "=m" (d[2]), "=m" (d[3]),
      "=m" (d[4]), "=m" (d[5]), "=m" (d[6]), "=m" (d[7])
    : "m" (s[0]), "m" (s[1]), "m" (s[2]), "m" (s[3]),
      "m" (s[4]), "m" (s[5]), "m" (s[6]), "m" (s[7])
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7");

    SHZ_FSCHG();

    return dst;
}

SHZ_INLINE void* shz_sq_memcpy32_1_xmtrx_sh4(      void* SHZ_RESTRICT dst,
                                             const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t*)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t*)dst;

    assert(!((uintptr_t)s & 7) && !((uintptr_t)d & 7));

    SHZ_FSCHG();

    asm volatile(R"(
        fmov.d @%[src]+, xd0
        fmov.d @%[src]+, xd2
        fmov.d @%[src]+, xd4
        fmov.d @%[src]+, xd6
        add    #32, %[dst]
        fmov.d xd6, @-%[dst]
        fmov.d xd4, @-%[dst]
        fmov.d xd2, @-%[dst]
        fmov.d xd0, @-%[dst]
        pref   @%[dst]          ! Fire off store queue
    )"
    : [src] "+r" (s), [dst] "+r" (d),
      "=m" (d[0]), "=m" (d[1]), "=m" (d[2]), "=m" (d[3]),
      "=m" (d[4]), "=m" (d[5]), "=m" (d[6]), "=m" (d[7])
    : "m" (s[0]), "m" (s[1]), "m" (s[2]), "m" (s[3]),
      "m" (s[4]), "m" (s[5]), "m" (s[6]), "m" (s[7]));

    SHZ_FSCHG();

    return dst;
}

#endif
