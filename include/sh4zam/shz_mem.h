/*! \file
 *  \brief   Memory API 
 *  \ingroup memory
 *
 *  API built around copying, assigning, and working with memory.
 *
 *  \todo
 *      - shz_macw()
 *      - shz_memset2()
 *      - shz_memset4()
 *      - shz_memset32()
 *      - shz_memset()
 *      - shz_memmove()
 *
 * \author 2025, 2026 Falco Girgis
 * \author 2020 MoopTheHedgehog
 *
 * \copyright MIT License
 */

#ifndef SHZ_MEM_H
#define SHZ_MEM_H

#include "shz_cdefs.h"

#include <stdbool.h>

/*! \defgroup memory Memory
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

/*! Intrinsic around the SH4 `MOVCA.L` instruction.

    Preallocates the cache-line containing \p src.

    Zero-initializes all 32-bytes within the \p src cache-line,
    setting the valid bit to `1`.
*/
SHZ_FORCE_INLINE void shz_dcache_alloc_line(void* src) SHZ_NOEXCEPT;

/*! Generic drop-in fast memcpy() replacement.

    Copies \p bytes from \p src to \p dst, determining the most efficient
    specialization to call into at run-time, returning \p dst.

    There are no alignment or size requirements for this routine.

    \note
    When you know of and can control the \p src and \p dst alignments and
    batch sizes, you can micro-optimize by calling into the most specific
    memcpy() specialization for your given scenario, over just using this
    generic implementation, which must choose which one to use at run-time.

    \warning
    \p dst and \p src buffers should not be overlapping.

    \warning
    This routine may clobber the `XMTRX` matrix back-bank!

    \sa shz_memcpy1(), shz_memcpy2(), shz_memcpy4(), shz_memcpy8(), shz_memcpy32(),
    shz_memcpy64(), shz_memcpy128()
*/
SHZ_INLINE void* shz_memcpy(      void* SHZ_RESTRICT dst,
                            const void* SHZ_RESTRICT src,
                                 size_t              bytes) SHZ_NOEXCEPT;

/*! \name  Specializations
    \brief Specialized routines for specific sizes + alignments.
    @{
*/

/*! Copies an unaligned buffer to another one byte at a time.

    The \p dst pointer is returned.

    \note
    Typically, unless you know you are copying a tiny number of
    definitely unaligned bytes, you want to use shz_memcpy(),
    which automatically handles arbitrary alignment for you,
    potentially more efficiently than copying byte-by-byte.

    \warning
    \p dst and \p src buffers should not be overlapping.

    \sa shz_memcpy()
*/
SHZ_FORCE_INLINE void* shz_memcpy1(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT;

/*! Copies from one 2-byte aligned buffer to another two bytes at a time.

    The \p dst pointer is returned.

    \warning
    \p dst and \p src must both be aligned by at least 2 bytes, and \p bytes
    must be a multiple of 2.

    \warning
    \p dst and \p src buffers should not be overlapping.
*/
SHZ_INLINE void* shz_memcpy2(      void* SHZ_RESTRICT dst,
                             const void* SHZ_RESTRICT src,
                                  size_t              bytes) SHZ_NOEXCEPT;

/*! Copies a from one 4-byte aligned buffer to another 4 bytes at a time.

    The \p dst buffer is returned.

    \warning
    \p dst and \p src must both be aligned by at least 4 bytes, and
    \p bytes must be a multiple of 4.

    \warning
    \p dst and \p src buffers should not be overlapping.
*/
SHZ_INLINE void* shz_memcpy4(      void* SHZ_RESTRICT dst,
                             const void* SHZ_RESTRICT src,
                                  size_t              bytes) SHZ_NOEXCEPT;

/*! Copies a from one 8-byte aligned buffer to another 8 bytes at a time.

    The \p dst buffer is returned.

    \warning
    \p dst and \p src must both be aligned by at least 8 bytes, and
    \p bytes must be a multiple of 8.

    \warning
    \p src and \p dst should not overlap.
*/
extern void* shz_memcpy8(      void* SHZ_RESTRICT dst,
                         const void* SHZ_RESTRICT src,
                              size_t              bytes) SHZ_NOEXCEPT;

/*! Assigns the given 8-byte \p value to the \p bytes in \p dst.

    \warning
    \p dst should be at least 8-byte aligned, and \p bytes should be
    a multiple of 8!
*/
extern void* shz_memset8(void* dst, uint64_t value, size_t bytes) SHZ_NOEXCEPT;

/*! Copies \p bytes from the \p src to the \p dst buffer in 32-byte chunks.

    Transfers from 8-byte aligned buffer, \p src to 32-byte aligned buffer, \p dst,
    32 bytes at a time. Returns the \p dst address.

    \warning
    \p dst must be 32-byte aligned, while \p src can be only 8-byte aligned. \p bytes must
    be a multiple of 32.

    \warning
    \p src and \p dst buffers must not overlap.

    \warning
    This routine may clobber the `XMTRX` matrix back-bank.

    \note
    This is the quickest way to move 32-byte chunks of data around *within memory*, but
    the shz_sq_memcpy32() will be faster when writing through the cache to external memory.

    \sa shz_sq_memcpy32()
*/
SHZ_INLINE void* shz_memcpy32(      void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                                   size_t              bytes) SHZ_NOEXCEPT;

/*! Copies \p bytes from \p src to \p dst in 32-byte chunks, using the Store Queues.

    Transfers from 8-byte aligned buffer, \p src to 4-byte aligned address, \p dst,
    32 bytes at a time, writing through the cache, using the SH4's Store Queues.
    Returns the \p dst address.

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
SHZ_INLINE void* shz_sq_memcpy32(      void* SHZ_RESTRICT dst,
                                 const void* SHZ_RESTRICT src,
                                      size_t              bytes) SHZ_NOEXCEPT;

/*! Copies \p bytes from \p src to \p dst in 32-byte chunks, using the Store Queues and XMTRX.

    Equiavalent to shz_sq_memcpy32(), except copying is done through XMTRX.

    \warning
    This routine clobbers XMTRX.
*/
SHZ_INLINE void* shz_sq_memcpy32_xmtrx(      void* SHZ_RESTRICT dst,
                                       const void* SHZ_RESTRICT src,
                                       size_t                   bytes) SHZ_NOEXCEPT;

/*! Specialized memcpy() variant for copying multiples of 64-bytes.

    Copies a from an 8-byte aligned buffer to a 32-byte aligned buffer, 64 bytes at a time.
    Returns the \p dst address.

    \warning
    \p src and \p dst buffers must not overlap.

    \warning
    This routine may clobber the `XMTRX` matrix back-bank.

    \warning
    \p dst must be 32-byte aligned, while \p src can be only 8-byte aligned. \p bytes must
    be a multiple of 64.
*/
SHZ_INLINE void* shz_memcpy64(      void* SHZ_RESTRICT dst,
                              const void* SHZ_RESTRICT src,
                                   size_t              bytes) SHZ_NOEXCEPT;

/*! Specialized memcpy() variant for copying multiples of 128 bytes.

    Copies a from an 8-byte aligned buffer to a 32-byte aligned buffer, 128 bytes at a time.
    Returns the \p dst address.

    \warning
    \p src and \p dst buffers must not overlap.

    \warning
    This routine may clobber the `XMTRX` matrix back-bank.

    \warning
    \p dst must be 32-byte aligned, while \p src can be only 8-byte aligned. \p bytes must
    be a multiple of 128.
*/
SHZ_INLINE void* shz_memcpy128(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src,
                                    size_t              bytes) SHZ_NOEXCEPT;

//! @}

/*! \name  Constant-sized Operations
    \brief Specialized routines for operating on statically sized buffers.
    @{
*/

/*! Copies 16 shorts from \p src to \p dst.

    \warning
    \p src and \p dst buffers must not overlap.

    \warning
    \p dst and \p src must both be aligned by at least two bytes.
*/
SHZ_INLINE void shz_memcpy2_16(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT;

/*! Sets the values of the 16 shorts pointed to by \p dst to the given \p value.

    \warning
    \p dst must be aligned by at least two bytes.
*/
SHZ_INLINE void shz_memset2_16(void* dst, uint16_t value) SHZ_NOEXCEPT;

/*! Copies 16 4-byte, long values from \p src to \p dst.

    \warning
    \p src and \p dst buffers must not overlap.

    \warning
    The \p src and \p dst buffers must both be at least 4-byte aligned.
*/
SHZ_INLINE void shz_memcpy4_16(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT;

/*! Copies 32 bytes from \p p1 to \p p2 as a single chunk.

    \warning
    This routine may clobber the `XMTRX` matrix back-bank.

    \warning
    \p dst must be 32-byte aligned, while \p src can be only 8-byte aligned.
*/
SHZ_INLINE void shz_memcpy32_1(      void* SHZ_RESTRICT dst,
                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT;

/*! Swaps the values within the given 32-byte buffers.

    \warning
    \p p1 and \p p2 must be at least 8-byte aligned.
*/
SHZ_INLINE void shz_memswap32_1(void* SHZ_RESTRICT p1,
                                void* SHZ_RESTRICT p2) SHZ_NOEXCEPT;

/*! Swaps the values within the given 32-byte buffers, using XMTRX.

    Equivalent to shz_memcpy32_1(), except copying is done through XMTRX.

    \warning
    This routine clobbers XMTRX!
*/
SHZ_INLINE void shz_memswap32_1_xmtrx(void* SHZ_RESTRICT p1,
                                      void* SHZ_RESTRICT p2) SHZ_NOEXCEPT;

/*! Copies \p src to \p dst in a single 32-byte transaction using the Store Queues.

    \note
    The Store Queues bypass the SH4's data-cache! They are typically used to
    transfer to *external memory* and are slower for memory-to-memory transactions.

    \warning
    \p dst must be at least 4-byte aligned, while \p src must be at least 8-byte aligned.

    \sa shz_memcpy32()
*/
SHZ_INLINE void* shz_sq_memcpy32_1(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src) SHZ_NOEXCEPT;


/*! Copies \p src to \p dst in a single 32-byte transaction using the Store Queues and XMTRX.

    Equivalent to shz_sq_memcpy32_1(), except copying is done through XMTRX.

    \warning
    This routine clobberx XMTRX.

    \sa shz_memcpy32()
*/
SHZ_INLINE void* shz_sq_memcpy32_1_xmtrx(      void* SHZ_RESTRICT dst,
                                        const void* SHZ_RESTRICT src) SHZ_NOEXCEPT;

//! @}

#include "inline/shz_mem.inl.h"

SHZ_DECLS_END

#endif
