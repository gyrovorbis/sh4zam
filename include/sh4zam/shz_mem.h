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

/*! \defgroup memory
 *  \brief    Routines for managing memory
 */

SHZ_DECLS_BEGIN

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

SHZ_INLINE void *shz_memcpy32(void *SHZ_RESTRICT dst, const void *SHZ_RESTRICT src, size_t bytes) {
    size_t cnt = (bytes >> 5);
    
    if(cnt) {
        asm volatile(R"( 
            pref      @%[src]           ! Prefetch src start 
            fschg                       ! Swap to double FMOV mode
        1:
            movca.l   r0, @%[dst]       ! Preallocate cache line (overwriting existing)  
            dt        %[cnt]            ! Decrement + test if r6 is zero       
            add       #32, %[dst]       ! Pre-increment dst pointer by 4 doubles            
            fmov.d    @%[src]+, dr4
            fmov.d    @%[src]+, dr6   
            fmov.d    @%[src]+, dr8     ! Load 4 8-byte doubles into FP regs,
            fmov.d    @%[src]+, dr10    !   incrementing src by 8 bytes each     
            fmov.d    dr10, @-%[dst]  
            fmov.d    dr8,  @-%[dst]     
            fmov.d    dr6,  @-%[dst]    ! Store 4 8-byte doubles from FP regs,          
            fmov.d    dr4,  @-%[dst]    !   decrementing dst by 8 bytes each 
            bt/s      2f
            add       #32, %[dst]       ! Increment dst to next block   
            bra       1b                ! Continue looping until we only have 1 iteration
            pref      @%[src]           ! Prefetch src start             
        2:    
            fschg                       ! Swap back to float FMOVs
        )"
        : [dst] "+r" (dst), [src] "+r" (src), [cnt] "+r" (cnt)
        :
        : "memory");
    }

    return dst;
}

SHZ_DECLS_END

#endif
