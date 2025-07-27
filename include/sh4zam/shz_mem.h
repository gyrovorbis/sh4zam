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
    SHZ_ALIASING uint32_t *src32 = (SHZ_ALIASING uint32_t *)src;

    asm volatile("movca.l r0, @%8\n"
     : "=m"(src32[0]),
       "=m"(src32[1]),
       "=m"(src32[2]),
       "=m"(src32[3]),
       "=m"(src32[4]),
       "=m"(src32[5]),
       "=m"(src32[6]),
       "=m"(src32[7])
     : "r" (src32));
}

SHZ_INLINE void *shz_memcpy2(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    SHZ_ALIASING uint16_t *d = (SHZ_ALIASING uint16_t *)dst;
    SHZ_ALIASING uint16_t *s = (SHZ_ALIASING uint16_t *)src;

    count >>= 1;

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
    SHZ_ALIASING uint32_t *d = (SHZ_ALIASING uint32_t *)dest;
    SHZ_ALIASING uint32_t *s = (SHZ_ALIASING uint32_t *)src;
    
    count >>= 2;

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
    : [src] "+r" (*src));
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
    : [dst] "+r" (*dst));
}

SHZ_INLINE void *shz_memcpy32(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
          uint32_t* d = dst;
    const uint32_t* s = src;
    size_t        cnt = (bytes >> 5);

    if(cnt) {
        __builtin_prefetch(s);
        asm volatile("fschg");

        while(SHZ_LIKELY(cnt-- > 1)) {
            shz_memcpy32_load_(&s);
            shz_memcpy32_store_(&d);
            __builtin_prefetch(s);
            d += 8;
        }

        shz_memcpy32_load_(&s);
        shz_memcpy32_store_(&d);

        asm volatile("fschg");
    }

    return dst;
}

SHZ_INLINE void shz_memcpy4_16(void *dst, const void *src) {
    SHZ_ALIASING const uint32_t (*d)[16] = (SHZ_ALIASING const uint32_t (*)[16])dst;
    SHZ_ALIASING       uint32_t (*s)[16] = (SHZ_ALIASING       uint32_t (*)[16])src;

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

SHZ_DECLS_END

#endif
