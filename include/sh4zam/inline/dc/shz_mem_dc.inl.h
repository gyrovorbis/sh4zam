//! \cond INTERNAL
/*! \file
    \brief SH4 implementation of the Memory API.
    \ingroup quat

    This file contains the Dreamcast implementation routines for
    SH4ZAM's memory API, which have been hand-optimized specifically
    for the SH4 architecture.

    \author    2025, 2026 Falco Girgis
    \author    2020 MoopTheHedgehog

    \copyright MIT License
*/
#ifndef SHZ_MEM_DC_INL_H
#define SHZ_MEM_DC_INL_H

#include <assert.h>

#define SHZ_FSCHG() asm volatile("fschg")

extern void shz_memcpy128_(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) SHZ_NOEXCEPT;

SHZ_FORCE_INLINE void shz_dcache_alloc_line(void* src) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_memcpy32_store_(uint64_t* SHZ_RESTRICT* dst) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_memcpy32_load_(const uint64_t* SHZ_RESTRICT* src) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_memcpy64_load_(const uint64_t* SHZ_RESTRICT* src) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_memcpy64_store_(uint64_t* SHZ_RESTRICT* dst) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_memcpy64_store_no_movca_(uint64_t* SHZ_RESTRICT dst) SHZ_NOEXCEPT {
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
SHZ_FORCE_INLINE void* shz_memcpy1(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
    SHZ_PREFETCH(src);

    if(SHZ_LIKELY(bytes)) {
       uint32_t scratch;
       uint32_t diff = (uintptr_t)dst - (((uintptr_t)src) + 1);

       asm(R"(
        .align 2
        0:
            dt      %[cnt]
            mov.b   @%[in]+, %[scr]
            bf.s    0b
            mov.b   %[scr], @(%[offset], %[in])
        )"
        : [in] "+&r" (src), [scr] "=&r" (scratch), [cnt] "+&r" (bytes), "=m" (*((uint8_t (*)[])dst))
        : [offset] "z" (diff), "m" (*((const uint8_t (*)[])src))
        : "t");
    }

    return dst;
}

SHZ_INLINE void* shz_memcpy2(void*       SHZ_RESTRICT dst,
                             const void* SHZ_RESTRICT src,
                                  size_t              bytes) SHZ_NOEXCEPT {
    const shz_alias_uint16_t* s = (const shz_alias_uint16_t*)src;
          shz_alias_uint16_t* d = (      shz_alias_uint16_t*)dst;

    assert(!(bytes % 2) && !((uintptr_t)dst & 1) && !((uintptr_t)src & 1));

    bytes >>= 1;

    size_t blocks = bytes >> 3; // Block size of 16 bytes

    if(blocks) {
        do {
            s += 8;
            SHZ_PREFETCH(s); // Prefetch 16 bytes for next iteration
            d[7] = *(--s);
            d[6] = *(--s);
            d[5] = *(--s);
            d[4] = *(--s);
            d[3] = *(--s);
            d[2] = *(--s);
            d[1] = *(--s);
            d[0] = *(--s);
            d += 8;
            s += 8;
        } while(SHZ_LIKELY(--blocks));
        bytes &= 0x7;
    }

    while(SHZ_LIKELY(bytes--))
        d[bytes] = s[bytes];

    return dst;
}

SHZ_INLINE void* shz_memcpy4(void*       SHZ_RESTRICT dst,
                             const void* SHZ_RESTRICT src,
                             size_t                   bytes) SHZ_NOEXCEPT {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t*)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t*)dst;

    assert(!(bytes % 4) && !((uintptr_t)dst & 3) && !((uintptr_t)src & 3));

    bytes >>= 2;

    size_t blocks = bytes >> 3;

    if(blocks) {
        do {
            s += 8;
            SHZ_PREFETCH(s); // Prefetch 32 bytes for next iteration
            d[7] = *(--s);
            d[6] = *(--s);
            d[5] = *(--s);
            d[4] = *(--s);
            d[3] = *(--s);
            d[2] = *(--s);
            d[1] = *(--s);
            d[0] = *(--s);
            d += 8;
            s += 8;
        } while(SHZ_LIKELY(--blocks));
        bytes &= 0x7;
    }

    while(SHZ_LIKELY(bytes--))
        d[bytes] = s[bytes];

    return dst;
}

SHZ_INLINE void* shz_memcpy8(      void* SHZ_RESTRICT dst,
                             const void* SHZ_RESTRICT src,
                                  size_t              bytes) SHZ_NOEXCEPT {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;

    assert(!(bytes % 8) && !((uintptr_t)dst & 7) && !((uintptr_t)src & 7));

    SHZ_PREFETCH(s);
    SHZ_FSCHG();

    bytes >>= 3;
    size_t blocks = bytes >> 3;

    if(blocks) {
        do {
            SHZ_PREFETCH(s + 4);
            shz_memcpy64_load_(&s);
            shz_memcpy64_store_no_movca_(d);
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

void* shz_memset8_(void* dst, uint64_t value, size_t bytes) SHZ_NOEXCEPT;

SHZ_FORCE_INLINE void* shz_memset8(void* dst, uint64_t value, size_t bytes) SHZ_NOEXCEPT {
    return shz_memset8_(dst, value, bytes);
}

SHZ_INLINE void* shz_memcpy32(      void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                              size_t                   bytes) SHZ_NOEXCEPT {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;

    assert(!(bytes % 32) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    size_t cnt = (bytes >> 5);

    SHZ_PREFETCH(s);
    SHZ_FSCHG();

    if(SHZ_LIKELY(cnt >= 8)) {
        shz_memcpy128_(d, s, bytes);
        size_t copied = bytes / 128 * 128;
        cnt -= copied / 32;
        d += copied / sizeof(uint64_t);
        s += copied / sizeof(uint64_t);
    }

    while(SHZ_LIKELY(cnt--)) {
        shz_memcpy32_load_(&s);
        shz_dcache_alloc_line(d);
        shz_memcpy32_store_(&d);
        SHZ_PREFETCH(s);
        d += 4;
    }

    SHZ_FSCHG();

    return dst;
}

SHZ_INLINE void* shz_sq_memcpy32(     void* SHZ_RESTRICT dst,
                                const void* SHZ_RESTRICT src,
                                size_t                   bytes) SHZ_NOEXCEPT {
    void* ret = dst;

    assert(!(bytes % 32) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    bytes >>= 5;

    if(bytes) {
        SHZ_FSCHG();

        asm volatile(R"(
        1:
            fmov.d @%[src]+, dr0
            fmov.d @%[src]+, dr2
            fmov.d @%[src]+, dr4
            fmov.d @%[src]+, dr6
            pref   @%[src]          ! Prefetch 32 bytes for next loop
            dt     %[blks]          ! while(n--)
            add    #32, %[dst]
            fmov.d dr6, @-%[dst]
            fmov.d dr4, @-%[dst]
            fmov.d dr2, @-%[dst]
            fmov.d dr0, @-%[dst]
            pref   @%[dst]          ! Fire off store queue
            bf.s   1b
            add    #32, %[dst]
        )"
        : [dst] "+r" (dst), [src] "+&r" (src), [blks] "+r" (bytes), "=m" ((char (*)[])dst)
        : "m" (*(const char (*)[])src)
        : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7", "t");

        SHZ_FSCHG();
    }

    return ret;
}

SHZ_INLINE void* shz_sq_memcpy32_xmtrx(      void* SHZ_RESTRICT dst,
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

SHZ_INLINE void* shz_memcpy64(      void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                                   size_t              bytes) SHZ_NOEXCEPT {
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;

    assert(!(bytes % 64) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    SHZ_FSCHG();

    size_t cnt = (bytes >> 6);

    if(SHZ_LIKELY(cnt >= 4)) {
        shz_memcpy128_(d, s, bytes);
        size_t copied = bytes / 128 * 128;
        cnt -= copied / 64;
        d += copied / sizeof(uint64_t);
        s += copied / sizeof(uint64_t);
    }

    while(SHZ_LIKELY(cnt--)) {
        SHZ_PREFETCH(s + 4);
        shz_memcpy64_load_(&s);
        shz_memcpy64_store_(&d);
        SHZ_PREFETCH(s);
        d += 8;
    }

    SHZ_FSCHG();

    return dst;
}

SHZ_INLINE void* shz_memcpy128(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src,
                                   size_t               bytes) SHZ_NOEXCEPT {
    assert(!(bytes % 128) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    if(bytes & ~0x7f) {
        SHZ_FSCHG();
        shz_memcpy128_(dst, src, bytes);
        SHZ_FSCHG();
    }

    return dst;
}

SHZ_INLINE void* shz_memcpy(      void* SHZ_RESTRICT dst,
                            const void* SHZ_RESTRICT src,
                                size_t               bytes) SHZ_NOEXCEPT {
    const uint8_t *s = (const uint8_t *)src;
          uint8_t *d = (      uint8_t *)dst;
    size_t copied;

    if(SHZ_LIKELY(bytes < 32))
        shz_memcpy1(d, s, bytes);
    else {
        if((uintptr_t)d & 31) {
            copied = (((uintptr_t)d + 31) & ~31) - (uintptr_t)d;
            shz_memcpy1(d, s, copied);
            bytes -= copied;
            d     += copied;
            s     += copied;
        }

        copied = 0;
        if(!(((uintptr_t)s) & 0x7)) {
            if(SHZ_LIKELY(bytes >= 32)) {
                copied = bytes & ~31;
                shz_memcpy32(d, s, copied);
            } else if(bytes >= 8) {
                copied = bytes & ~7;
                shz_memcpy8(d, s, copied);
            }
        } else if(bytes >= 4 && !(((uintptr_t)s) & 3)) {
            copied = bytes & ~3;
            shz_memcpy4(d, s, copied);
        } else if(bytes >= 2 && !(((uintptr_t)s) & 1)) {
            copied = bytes & ~1;
            shz_memcpy2(d, s, copied);
        }

        bytes -= copied;
        d     += copied;
        s     += copied;

        shz_memcpy1(d, s, bytes);
    }

    return dst;
}

SHZ_INLINE void shz_memcpy2_16(      void* SHZ_RESTRICT dst,
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
    : [d] "+r" (dst), [s] "+r" (src), "=m" (*((shz_alias_uint16_t (*)[16])dst))
    : "m" (*((shz_alias_uint16_t (*)[16])src))
    : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7");
}

SHZ_INLINE void shz_memset2_16(void* dst, uint16_t value) SHZ_NOEXCEPT {
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
    : "=m" (*((shz_alias_uint16_t (*)[16])dst))
    : "r" (dst), "r" (value));
}

SHZ_INLINE void shz_memcpy4_16(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    const shz_alias_uint32_t (*s)[16] = (const shz_alias_uint32_t (*)[16])src;
          shz_alias_uint32_t (*d)[16] = (      shz_alias_uint32_t (*)[16])dst;

    assert(!((uintptr_t)s & 0x3) && !((uintptr_t)d & 0x3));

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
    : "=m" (*d)
    : [s] "r" (s), [d] "r" (d), "m" (*s)
    : "r0", "r1", "r2", "r3");
}

SHZ_INLINE void shz_memcpy32_1(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;

    assert(!((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    SHZ_PREFETCH(s);

    SHZ_FSCHG();

    shz_memcpy32_load_(&s);
    shz_dcache_alloc_line(d);
    shz_memcpy32_store_(&d);

    SHZ_FSCHG();
}

SHZ_INLINE void shz_memswap32_1(void* SHZ_RESTRICT p1,
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
    : "+m" (*a), "+m" (*b)
    : [a] "r" (a), [b] "r" (b)
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");

    SHZ_FSCHG();
}

SHZ_INLINE void shz_memswap32_1_xmtrx(void* SHZ_RESTRICT p1,
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
    : "+m" (*a), "+m" (*b)
    : [a] "r" (a), [b] "r" (b));

    SHZ_FSCHG();
}

SHZ_INLINE void* shz_sq_memcpy32_1(      void* SHZ_RESTRICT dst,
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

SHZ_INLINE void* shz_sq_memcpy32_1_xmtrx(      void* SHZ_RESTRICT dst,
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
