/*! \file
 *  \brief   Memory API 
 *  \ingroup memory
 *
 *  API built around copying, assigning, and working with memory.
 *
 *  \todo
 *      - memset2()
 *      - memset4()
 *      - memset32()
 *      - shz_macw()
 *
 * \author    Falco Girgis
 * \copyright MIT License
 */

#ifndef SHZ_MEM_H
#define SHZ_MEM_H

#include "shz_cdefs.h"
#include "shz_fpscr.h"

/*! \defgroup memory
    \brief    Routines for managing memory.

    This API provides the following types of memory routines:
        - barriers
        - special instruction intrinsics
        - cache operations
        - memcpy()-type routines

    \note
    memcpy()-like routines will typically always check for
    proper alignment and size increments of parameters using
    assert(), so make sure to build a release build (-DNDEBUG)
    for maximal gainz, when not debugging.
 */

/*! \name  Barriers
    \brief Macros for preventing GCC from reordering instructions.
    @{
*/

//! Creates a software memory barrier beyond which any loads or stores may not be reordered
#define SHZ_MEMORY_BARRIER_SOFT()   asm volatile("" : : : "memory")
//! Creates a hardware memory barrier beyond which any loads or stores may not be reordered
#define SHZ_MEMORY_BARRIER_HARD()   __sync_synchronize()

//! @}

SHZ_DECLS_BEGIN

/*! \name Intrinsics
    \brief Intrinsics around memory-related SH4 instructions.
    @{
*/

/*! Intrinsic around the SH4 `XTRCT` instruction.

    Extracts the middle 32 bits from the 64-bit contents of \p a
    combined with \p b.

    \author Paul Cercueil
*/
SHZ_FORCE_INLINE uint32_t shz_xtrct(uint32_t a, uint32_t b) SHZ_NOEXCEPT {
    return (b << 16) | (a >> 16);
}

/*! Instrinsic around the SH4 `CMP/STR` instruction.

    Compares 32-bit values \p a and \p b, returning `true` if any of the 4
    bytes in \p a are equal to the corresponding byte in \p b.
*/
SHZ_FORCE_INLINE bool shz_cmp_str(uint32_t a, uint32_t b) SHZ_NOEXCEPT {
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

/*! Intrinsic around thhe SH4 `MOVCA.L` instruction.

    Preallocates the cache-line containing \p src.

    Zero-initializes all 32-bytes within the \p src cache-line,
    setting the valid bit to `1`.
*/
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

//! @}

/*! Copies an unaligned buffer to another one byte at a time.

    \note
    Typically, unless you know you are copying a tiny number of
    definitely unaligned bytes, you want to use shz_memcpy(),
    which automatically handles arbitrary alignment for you,
    potentially more efficiently than copying byte-by-byte.

    \sa shz_memcpy()
*/
SHZ_FORCE_INLINE void* shz_memcpy1(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
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

/*! Copies from one 2-byte aligned buffer to another two bytes at a time.

    \warning
    \p dst and \p src must both be aligned by at least 2 bytes, and \p bytes
    must be a multiple of 2.
*/
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
        bytes &= 0xf;
    }

    while(SHZ_LIKELY(bytes--))
        d[bytes] = s[bytes];

    return dst;
}

/*! Copies 16 shorts from \p src to \p dst.

    \warning
    \p dst and \p src must both be aligned by at least two bytes.
*/
SHZ_INLINE void shz_memcpy2_16(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    assert(!((uintptr_t)dst & 0x1) && !((uintptr_t)src & 0x1));

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

/*! Sets the values of the 16 shorts pointed to by \p dst to the given \p value.

    \warning
    \p dst must be aligned by at least two bytes.
*/
SHZ_INLINE void shz_memset2_16(void* dst, uint16_t value) SHZ_NOEXCEPT {
    assert(!((uintptr_t)dst & 0x1));

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

/*! Copies a from one 4-byte aligned buffer to another 4 bytes at a time.

    \warning
    \p dst and \p src must both be aligned by at least 4 bytes, and
    \p bytes must be a multiple of 4.
*/
SHZ_INLINE void* shz_memcpy4(void*       SHZ_RESTRICT dst,
                             const void* SHZ_RESTRICT src,
                             size_t                   bytes) SHZ_NOEXCEPT {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t*)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t*)dst;

    assert(!(bytes % 4) && !((uintptr_t)dst & 3) && !((uintptr_t)src & 3));

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

/*! Copies 16 4-byte, long values from \p src to \p dst.

    \warning
    The \p src and \p dst buffers must both be at least 4-byte aligned.
*/
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

/*! Copies a from one 8-byte aligned buffer to another 8 bytes at a time.

    \warning
    \p dst and \p src must both be aligned by at least 8 bytes, and
    \p bytes must be a multiple of 8.
*/
extern void* shz_memcpy8(      void* SHZ_RESTRICT dst,
                         const void *SHZ_RESTRICT src,
                         size_t                   bytes) SHZ_NOEXCEPT;

/*! Assigns the given 8-byte \p value to the \p bytes in \p dst.

    \warning
    \p dst should be at least 8-byte aligned, and \p bytes should be
    a multiple of 8!
*/
extern void* shz_memset8(void*    dst,
                         uint64_t value,
                         size_t   bytes) SHZ_NOEXCEPT;

//! \cond INTERNAL
extern void* shz_memcpy128_(      void* SHZ_RESTRICT dst,
                            const void* SHZ_RESTRICT src,
                            size_t                   bytes) SHZ_NOEXCEPT;

SHZ_FORCE_INLINE void shz_memcpy32_store_(uint64_t* SHZ_RESTRICT* dst) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_memcpy32_load_(const uint64_t* SHZ_RESTRICT* src) SHZ_NOEXCEPT {
    asm volatile(R"(
        fmov.d    @%[src]+, dr4
        fmov.d    @%[src]+, dr6
        fmov.d    @%[src]+, dr8
        fmov.d    @%[src]+, dr10
    )"
    : [src] "+r" (*src)
    : "m" (src[0]), "m" (src[1]), "m" (src[2]), "m" (src[3]));
}
//! \endcond

/*! Copies \p bytes from the \p src to the \p dst buffer in 32-byte chunks.

    Transfers from 8-byte aligned buffer, \p src to 32-byte aligned buffer, \p dst,
    32 bytes at a time.

    \warning
    \p dst must be 32-byte aligned, while \p src can be only 8-byte aligned. \p bytes must
    be a multiple of 32.

    \note
    This is the quickest way to move 32-byte chunks of data around *within memory*, but
    the shz_sq_memcpy32() will be faster when writing through the cache to external memory.

    \sa shz_sq_memcpy32()
*/
SHZ_INLINE void* shz_memcpy32(      void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                              size_t                   bytes) SHZ_NOEXCEPT {
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;

    assert(!(bytes % 32) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

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

/*! Swaps the values within the given 32-byte buffers.

    \warning
    \p p1 and \p p2 must be at least 8-byte aligned.
*/
SHZ_INLINE void shz_memswap32_1(void* SHZ_RESTRICT p1,
                                void* SHZ_RESTRICT p2) SHZ_NOEXCEPT {
    shz_alias_uint32_t (*a)[8] = (shz_alias_uint32_t (*)[8])p1;
    shz_alias_uint32_t (*b)[8] = (shz_alias_uint32_t (*)[8])p2;

    assert(!((uintptr_t)p1 & 7) && ((uintptr_t)p2 & 7));

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
    : [a] "+r" (a), [b] "+r" (b), "+m" (*a), "+m" (*b));

    SHZ_FSCHG(false);
}

/*! Copies \p bytes from \p src to \p dst in 32-byte chunks, using the Store Queues.

    Transfers from 8-byte aligned buffer, \p src to 4-byte aligned address, \p dst,
    32 bytes at a time, writing through the cache, using the SH4's Store Queues.

    \warning
    \p src must be at least 8-byte aligned, while \p dst can be only 4-byte aligned.
    \p bytes must be a multiple of 32.

    \note
    This is the quickest way to move 32-byte chunks of data to *external memory*.
    When copying to cached memory, you must invalidate the cache lines containing
    \p dst before initiating the copy... Which means this routine becomes slower
    than doing memory-to-memory copies with shz_memcpy32().

    \sa shz_memcpy32(), shz_sq_memcpy32_1()
*/
SHZ_INLINE void* shz_sq_memcpy32(     void* SHZ_RESTRICT dst,
                                const void* SHZ_RESTRICT src,
                                size_t                   bytes) SHZ_NOEXCEPT {
    void* ret = dst;

    assert(!(bytes % 32) && !((uintptr_t)dst & 7) && !((uintptr_t)src & 7));

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

/*! Copies \p src to \p dst in a single 32-byte transaction using the Store Queues.

    \note
    The Store Queues bypass the SH4's data-cache! They are typically used to
    transfer to *external memory* and are slower for memory-to-memory transactions.

    \warning
    \p dst must be at least 4-byte aligned, while \p src must be at least 8-byte aligned.

    \sa shz_memcpy32()
*/
SHZ_INLINE void* shz_sq_memcpy32_1(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    const shz_alias_uint32_t* s = (const shz_alias_uint32_t*)src;
          shz_alias_uint32_t* d = (      shz_alias_uint32_t*)dst;

    assert(!((uintptr_t)s & 7) && !((uintptr_t)d & 7));

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

//! \cond INTERNAL
SHZ_FORCE_INLINE void shz_memcpy64_load_(const uint64_t* SHZ_RESTRICT* src) SHZ_NOEXCEPT {
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

SHZ_FORCE_INLINE void shz_memcpy64_store_(uint64_t* SHZ_RESTRICT* dst) SHZ_NOEXCEPT {
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
//! \endcond

/*! Specialized memcpy() variant for copying multiples of 64-bytes.

    Copies a from an 8-byte aligned buffer to a 32-byte aligned buffer, 64 bytes at a time

    \warning
    \p dst must be 32-byte aligned, while \p src can be only 8-byte aligned. \p bytes must
    be a multiple of 64.
*/
SHZ_INLINE void* shz_memcpy64(      void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                                  size_t               bytes) SHZ_NOEXCEPT {
    const shz_alias_uint64_t* s = (const shz_alias_uint64_t*)src;
          shz_alias_uint64_t* d = (      shz_alias_uint64_t*)dst;

    assert(!(bytes % 64) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

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

/*! Specialized memcpy() variant for copying multiples of 128 bytes.

    Copies a from an 8-byte aligned buffer to a 32-byte aligned buffer, 128 bytes at a time

    \warning
    \p dst must be 32-byte aligned, while \p src can be only 8-byte aligned. \p bytes must
    be a multiple of 128.
*/
SHZ_INLINE void* shz_memcpy128(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src,
                                   size_t               bytes) SHZ_NOEXCEPT {
    assert(!(bytes % 128) && !((uintptr_t)dst & 31) && !((uintptr_t)src & 7));

    if(bytes & ~0x7f) {
        SHZ_FSCHG(true);
        shz_memcpy128_(dst, src, bytes);
        SHZ_FSCHG(false);
    }

    return dst;
}

/*! Generic drop-in memcpy() replacement.

    Copies \p bytes from \p src to \p dst, determining the most efficient
    specialization to call into at run-time, returning \p dst.

    There are no alignment or size requirements for this routine.
*/
SHZ_INLINE void* shz_memcpy(      void* SHZ_RESTRICT dst,
                            const void* SHZ_RESTRICT src,
                                size_t               bytes) SHZ_NOEXCEPT {
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
