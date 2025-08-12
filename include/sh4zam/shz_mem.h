/*! \file
 *  \brief   Memory API 
 *  \ingroup memory
 *
 *  API built around copying, assigning, and working with memory.
 * 
 *  \author Falco Girgis
 * 
 *  \todo
 *      - memset2()
 *      - memset4()
 *      - memset32()
 *      - shz_macw()
 */
#ifndef SHZ_MEM_H
#define SHZ_MEM_H

#include "shz_cdefs.h"
#include "shz_fpscr.h"

/*! \defgroup memory
 *  \brief    Routines for managing memory
 */

#define SHZ_MEMORY_BARRIER_SOFT()   asm volatile("" : : : "memory")
#define SHZ_MEMORY_BARRIER_HARD()   __sync_synchronize()

SHZ_DECLS_BEGIN

SHZ_INLINE uint16_t shz_ipv4_checksum(const void* data, size_t bytes, uint16_t partial) {
    uint32_t sum = partial;
    size_t i = bytes;
    uintptr_t end = (uintptr_t)data + bytes;

    /* Make sure we don't do any unaligned memory accesses */
    if(((uint32_t)data) & 0x01) {
        const uint8_t *ptr = data;

        while((size_t)(&ptr[2]) <= end) {
            sum += ptr[0] | ((ptr[1]) << 8);
            ptr += 2;
        }
    }
    else {
        const shz_alias_uint16_t *ptr = (const shz_alias_uint16_t *)data;
        while((size_t)(&ptr[1]) <= end) {
            sum += *ptr++;
        }
    }

    i &= 0x1;

    /* Handle the last byte, if we have an odd byte count */
    if(i)
        sum += ((uint8_t *)data)[bytes - 1];

    /* Take care of any carry bits */
    while(sum >> 16)
        sum = (sum >> 16) + (sum & 0xFFFF);

    return (uint16_t)~sum;
}

SHZ_FORCE_INLINE bool shz_cmp_str(uint32_t a, uint32_t b) {
    bool t;

    asm volatile(R"(
        cmp/str %[a], %[b]
        movt    %[t]
    )"
    : [t] "=r" (t)
    : [a] "r" (a), [b] "r" (b)
    : "t");

    return t;
}

//! Thanks to Paul Cercueil for the C pattern!
SHZ_FORCE_INLINE uint32_t shz_xtrct(uint32_t a, uint32_t b) {
    return (b << 16) | (a >> 16);
}

SHZ_FORCE_INLINE void shz_dcache_alloc_line(void *src) {
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

SHZ_FORCE_INLINE void* shz_memcpy1(void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                   size_t bytes) {
    void *ret = dst;
    uint32_t scratch;

    SHZ_PREFETCH(src);

    if(bytes) {
        asm volatile(R"(
        1:
            mov.b   @%[src]+, %[tmp]
            dt      %[cnt]
            mov.b   %[tmp], @%[dst]
            bf/s    1b;
            add     #1, %[dst]
        )"
        : [src] "+&r" (src), [dst] "+&r" (dst),
          [cnt] "+&r" (bytes), [tmp] "=r" (scratch)
        : "m" (*((uint8_t (*)[])src))
        : "t", "memory");
    }

    return ret;
}

SHZ_INLINE void* shz_memcpy2(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    const shz_alias_uint16_t *s = (const shz_alias_uint16_t *)src;
          shz_alias_uint16_t *d = (      shz_alias_uint16_t *)dst;

    assert(!(bytes % 2) && !(dst & 1) && !(src & 1));

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
        bytes &= 0xf;
    }

    while(SHZ_LIKELY(bytes--))
        d[bytes] = s[bytes];

    return dst;
}

SHZ_INLINE void shz_memcpy2_16(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src) {
    asm volatile(R"(
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
    : [d] "+r"(dst), [s] "+r"(src), "=m" (*((shz_alias_uint16_t (*)[16])dst))
    : "m" (*((shz_alias_uint16_t (*)[16])src))
    : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7");
}

SHZ_INLINE void shz_memset2_16(void* dst, uint16_t value) {
    asm volatile(R"(
        add     #32 %0
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

SHZ_INLINE void *shz_memcpy4(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    const shz_alias_uint32_t *s = (const shz_alias_uint32_t *)src;
          shz_alias_uint32_t *d = (      shz_alias_uint32_t *)dst;

    assert(!(bytes % 4) && !(dst & 3) && !(src & 3));

    bytes >>= 2;
    size_t blocks = bytes >> 5; // Block size of 32 bytes

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
        bytes &= 0x1f;
    }

    while(SHZ_LIKELY(bytes--))
        d[bytes] = s[bytes];

    return dst;
}

SHZ_INLINE void shz_memcpy4_16(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src) {
    const shz_alias_uint32_t (*s)[16] = (const shz_alias_uint32_t (*)[16])src;
          shz_alias_uint32_t (*d)[16] = (      shz_alias_uint32_t (*)[16])dst;

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

extern void* shz_memcpy8(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes);
extern void* shz_memset8(void *dst, uint64_t value, size_t bytes);
extern void* shz_memcpy128_(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes);

SHZ_FORCE_INLINE void shz_memcpy32_store_(uint64_t* SHZ_RESTRICT* dst) {
    asm volatile(R"(
        add       #32, %[dst]
        fmov.d    dr10, @-%[dst]
        fmov.d    dr8,  @-%[dst]
        fmov.d    dr6,  @-%[dst]
        fmov.d    dr4,  @-%[dst]
    )"
    : "=m" ((*dst)[0]), "=m" ((*dst)[1]), "=m" ((*dst)[2]), "=m" ((*dst)[3])
    : [dst] "r" (*dst));
}

SHZ_FORCE_INLINE void shz_memcpy32_load_(const uint64_t* SHZ_RESTRICT* src) {
    asm volatile(R"(
        fmov.d    @%[src]+, dr4
        fmov.d    @%[src]+, dr6
        fmov.d    @%[src]+, dr8
        fmov.d    @%[src]+, dr10
    )"
    : [src] "+r" (*src)
    : "m" (src[0]), "m" (src[1]), "m" (src[2]), "m" (src[3]));
}

SHZ_INLINE void* shz_memcpy32(void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                              size_t bytes) {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t *)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t *)src;

    assert(!(bytes % 32) && !(dst & 31) && !(src & 31));

    size_t cnt = (bytes >> 5);

    SHZ_FSCHG(true);

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

    SHZ_FSCHG(false);

    return dst;
}

SHZ_INLINE void shz_memswap32_1(void *SHZ_RESTRICT p1, void *SHZ_RESTRICT p2) {
    const shz_alias_uint32_t (*a)[8] = (const shz_alias_uint32_t (*)[8])p1;
          shz_alias_uint32_t (*b)[8] = (      shz_alias_uint32_t (*)[8])p2;

    SHZ_PREFETCH(b);
    SHZ_FSCHG(true);

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
    : [a] "+r" (a), [b] "+r" (b), "=&m" (*a), "=&m" (*b));

    SHZ_FSCHG(false);
}

SHZ_INLINE void *shz_sq_memcpy32(void *SHZ_RESTRICT dst,
                                const void *SHZ_RESTRICT src,
                                size_t bytes) {
    void *ret = dst;

    assert(!(bytes % 32) && !(dst & 7) && !(src & 7));

    bytes >>= 5;

    SHZ_FSCHG(true);

    asm volatile(R"(
    1:
        fmov.d @%[src]+, xd0
        fmov.d @%[src]+, xd2
        fmov.d @%[src]+, xd4
        fmov.d @%[src]+, xd6
        pref   @%[src]          ! Prefetch 32 bytes for next loop
        dt     %[blks]          ! while(n--)
        add    #32, %[dst]
        fmov.d xd6, @-%[dst]
        fmov.d xd4, @-%[dst]
        fmov.d xd2, @-%[dst]
        fmov.d xd0, @-%[dst]
        add    #32, %[dst]
        bf.s   1b
        pref   @%[dst]          ! Fire off store queue
    )"
    : [dst] "+r" (dst), [src] "+&r" (src), [blks] "+r" (bytes)
    : "m" ((const char (*)[])src)
    : "memory");

    SHZ_FSCHG(false);

    return ret;
}

SHZ_INLINE void *shz_sq_memcpy32_1(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src) {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t *)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t *)dst;

    assert(!(s & 7) && !(d & 7));

    SHZ_FSCHG(true);

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
    : [dst] "+r" (d), [src] "+&r" (s),
      "=m" (d[0]), "=m" (d[1]), "=m" (d[2]), "=m" (d[3]),
      "=m" (d[4]), "=m" (d[5]), "=m" (d[6]), "=m" (d[7])
    : "m" (s[0]), "m" (s[1]), "m" (s[2]), "m" (s[3]),
      "m" (s[4]), "m" (s[5]), "m" (s[6]), "m" (s[7]));

    SHZ_FSCHG(false);

    return dst;
}
SHZ_FORCE_INLINE void shz_memcpy64_load_(const uint64_t *SHZ_RESTRICT *src) {
    asm volatile(R"(
        fmov.d    @%[src]+, dr0
        fmov.d    @%[src]+, dr2
        fmov.d    @%[src]+, dr4
        fmov.d    @%[src]+, dr6
        fmov.d    @%[src]+, dr8
        fmov.d    @%[src]+, dr10
        fmov.d    @%[src]+, dr12
        fmov.d    @%[src]+, dr14
    )"
    : [src] "+r" (*src)
    : "m" ((*src)[0]), "m" ((*src)[1]), "m" ((*src)[2]), "m" ((*src)[3]),
      "m" ((*src)[4]), "m" ((*src)[5]), "m" ((*src)[6]), "m" ((*src)[7]));
}

SHZ_FORCE_INLINE void shz_memcpy64_store_(uint64_t *SHZ_RESTRICT *dst) {
    asm volatile(R"(
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

SHZ_INLINE void* shz_memcpy64(void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                              size_t bytes) {
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t *)src;
          shz_alias_uint64_t* d = (      shz_alias_uint64_t *)dst;

    assert(!(bytes % 64) && !(dst & 31) && !(src & 7));

    SHZ_FSCHG(true);

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

    SHZ_FSCHG(false);

    return dst;
}

SHZ_INLINE void* shz_memcpy128(void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src,
                               size_t bytes) {
    assert(!(bytes % 128) && !(dst & 31) && !(src & 7));

    if(bytes & ~0x7f) {
        SHZ_FSCHG(true);
        shz_memcpy128_(dst, src, bytes);
        SHZ_FSCHG(false);
    }

    return dst;
}

SHZ_INLINE void* shz_memcpy(void* SHZ_RESTRICT dst,
                            const void* SHZ_RESTRICT src,
                            size_t bytes) {
    const uint8_t *s = (const uint8_t *)src;
          uint8_t *d = (      uint8_t *)dst;
    size_t copied;

    if(SHZ_UNLIKELY(!bytes))
        return dst;
    else if(SHZ_LIKELY(bytes < 64)) {
        shz_memcpy1(d, s, bytes);
    } else {
        if((uintptr_t)d & 0x1f) {
            copied = (((uintptr_t)d + 31) & ~0x1f) - (uintptr_t)d;
            shz_memcpy1(d, s, copied);

            bytes -= copied;
            d     += copied;
            s     += copied;
        }

        if(SHZ_LIKELY(bytes >= 32)) {
            copied = 0;

            if(!(((uintptr_t)s) & 0x7)) {
                copied = bytes - (bytes & ~7);
                shz_memcpy8(d, s, copied);
            } else if(!(((uintptr_t)s) & 0x3)) {
                copied = bytes - (bytes & ~3);
                shz_memcpy4(d, s, copied);
            } else if(!(((uintptr_t)s) & 0x1)) {
                copied = bytes - (bytes & ~1);
                shz_memcpy2(d, s, copied);
            }

            bytes -= copied;
            d     += copied;
            s     += copied;
        }

        shz_memcpy1(d, s, bytes);
    }

    return dst;
}

SHZ_DECLS_END

#endif
