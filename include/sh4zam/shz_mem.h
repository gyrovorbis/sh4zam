/*! \file
 *  \brief   Memory API 
 *  \ingroup memory
 *
 *  API built around copying and assigning memory.
 * 
 *  \author Falco Girgis
 * 
 *  \todo
 *      - memset2()
 *      - memset4()
 *      - memset32()
 */
#ifndef SHZ_MEM_H
#define SHZ_MEM_H

#include "shz_cdefs.h"

#define SHZ_MEMORY_BARRIER_SOFT()   asm volatile("" : : : "memory")
#define SHZ_MEMORY_BARRIER_HARD()   __sync_synchronize()

/*! \defgroup memory
 *  \brief    Routines for managing memory
 */

SHZ_DECLS_BEGIN

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

SHZ_INLINE void *shz_memcpy2(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    const shz_alias_uint16_t *s = (const shz_alias_uint16_t *)src;
          shz_alias_uint16_t *d = (      shz_alias_uint16_t *)dst;

    count >>= 1;

    assert(count);

    size_t blocks = count >> 3; // Block size of 16 bytes

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
        count &= 0xf;
    }

    while(SHZ_LIKELY(count--))
        d[count] = s[count];

    return dst;
}

SHZ_INLINE void *shz_memcpy4(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    const shz_alias_uint32_t *s = (const shz_alias_uint32_t *)src;
          shz_alias_uint32_t *d = (      shz_alias_uint32_t *)dest;
    
    count >>= 2;

    assert(count);

    size_t blocks = count >> 3; // Block size of 32 bytes
    
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
        count &= 0x1f;
    }

    while(SHZ_LIKELY(count--))
        d[count] = s[count];

    return dst;
}

SHZ_CONST void *shz_memcpy8(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes);
SHZ_CONST void *shz_memset8(void *dst, uint64_t value, size_t bytes);

SHZ_FORCE_INLINE void shz_memcpy32_load_(const uint32_t *SHZ_RESTRICT *src) {
    asm volatile(R"(
        fmov.d    @%[src]+, dr4
        fmov.d    @%[src]+, dr6
        fmov.d    @%[src]+, dr8
        fmov.d    @%[src]+, dr10
    )"
    : [src] "+&r" (*src)
    : "m" (*src[0]), "m" (*src[1]), "m" (*src[2]), "m" (*src[3]),
      "m" (*src[4]), "m" (*src[5]), "m" (*src[6]), "m" (*src[7])
    );
}

SHZ_FORCE_INLINE inline static void shz_memcpy32_store_(uint32_t *SHZ_RESTRICT *dst) {
    asm volatile(R"(
        movca.l   r0,   @%[dst]
        add       #32,  %[dst]
        fmov.d    dr10, @-%[dst]
        fmov.d    dr8,  @-%[dst]
        fmov.d    dr6,  @-%[dst]    
        fmov.d    dr4,  @-%[dst]
    )"
    : [dst] "+r" (*dst)
      "=m" (*dst[0]), "=m" (*dst[1]), "=m" (*dst[2]), "=m" (*dst[3]),
      "=m" (*dst[4]), "=m" (*dst[5]), "=m" (*dst[6]), "=m" (*dst[7]));
}

SHZ_INLINE void *shz_memcpy32(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t *)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t *)dst;

   size_t cnt = (bytes >> 5);

    if(cnt) {
        SHZ_PREFETCH(s);
        asm volatile("fschg");

        while(SHZ_LIKELY(cnt-- > 1)) {
            shz_memcpy32_load_(&s);
            shz_memcpy32_store_(&d);
            SHZ_PREFETCH(s);
            d += 8;
        }

        shz_memcpy32_load_(&s);
        shz_memcpy32_store_(&d);

        asm volatile("fschg");
    }

    return dst;
}

SHZ_INLINE void shz_memswap32_1(void *SHZ_RESTRICT p1, void *SHZ_RESTRICT p2) {
    assert(0); //wrong clobbers still, fixme
    asm volatile(R"(
        pref    @%[b]
        fschg

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
        fschg
    )"
    : [a] "+r" (a), [b] "+r" (b)
    :
    : "memory");
}


SHZ_INLINE void *shz_sq_memcpy32(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    void *ret = dst;

    count >>= 5;
    assert(count && !(dst & 7) && !(src & 7));

    asm volatile(R"(
        fschg
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

        fschg
    )"
    : [dst] "+r" (dst), [src] "+&r" (s), [blks] "+r" (count)
    : "memory"
    : "memory");

    return ret;
}

SHZ_INLINE void *shz_sq_memcpy32_1(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src) {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t *)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t *)dst;

    assert(!(s & 7) && !(d & 7));

    asm volatile(R"(
        fschg

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

        fschg
    )"
    : [dst] "+r" (d), [src] "+&r" (s),
      "=m" (d[0]), "=m" (d[1]), "=m" (d[2]), "=m" (d[3]),
      "=m" (d[4]), "=m" (d[5]), "=m" (d[6]), "=m" (d[7])
    : "m" (s[0]), "m" (s[1]), "m" (s[2]), "m" (s[3]),
      "m" (s[4]), "m" (s[5]), "m" (s[6]), "m" (s[7]));
}

SHZ_INLINE void shz_memcpy4_16(void *dst, const void *src) {
    const shz_alias_uint32_t (*d)[16] = (const shz_alias_uint32_t (*)[16])dst;
          shz_alias_uint32_t (*s)[16] = (      shz_alias_uint32_t (*)[16])src;

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

SHZ_INLINE void *shz_memcpy2(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    const shz_alias_uint16_t *s = (const shz_alias_uint16_t *)src;
          shz_alias_uint16_t *d = (      shz_alias_uint16_t *)dst;

    count >>= 1;

    assert(count);

    size_t blocks = count >> 3; // Block size of 16 bytes

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
        count &= 0xf;
    }

    while(SHZ_LIKELY(count--))
        d[count] = s[count];

    return dst;
}

SHZ_FORCE_INLINE void shz_memcpy2_16(void* restrict dst, const void* restrict src) {
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

SHZ_FORCE_INLINE void shz_memset2_16(void* dst, uint16_t value) {
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
          shz_alias_uint32_t *d = (      shz_alias_uint32_t *)dest;

    count >>= 2;

    assert(count);

    size_t blocks = count >> 3; // Block size of 32 bytes

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
    }
}

SHZ_DECLS_END

#endif
