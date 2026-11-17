/*! \file
 *  \brief   C++ Memory API
 *  \ingroup memory
 *
 *  C++ wrapper API for copying, assigning, and working with memory.
 *
 *  \author    2025, 2026 Falco Girgis
 *  \copyright MIT License
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#ifndef SHZ_MEM_HPP
#define SHZ_MEM_HPP

#include "shz_mem.h"

namespace shz {
    //! C++ wrapper around shz_dcache_alloc_line().
    SHZ_FORCE_INLINE void dcache_alloc_line(void* src) noexcept { shz_dcache_alloc_line(src); }

    //! C++ wrappers sharing names with libc, kept out of argument-dependent lookup.
    namespace libc {
        //! C++ wrapper around shz_memmove().
        SHZ_FORCE_INLINE void* memmove(void* dst, const void* src, size_t bytes) noexcept {
            return shz_memmove(dst, src, bytes);
        }

        //! C++ wrapper around shz_memcpy().
        SHZ_FORCE_INLINE void* memcpy(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept {
            return shz_memcpy(dst, src, bytes);
        }
    }

    using namespace libc;

    //! C++ wrapper around shz_memcpy1().
    SHZ_FORCE_INLINE void* memcpy1(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_memcpy1(dst, src, bytes); }
    //! C++ wrapper around shz_memcpy2().
    SHZ_FORCE_INLINE void* memcpy2(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_memcpy2(dst, src, bytes); }
    //! C++ wrapper around shz_memcpy4().
    SHZ_FORCE_INLINE void* memcpy4(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_memcpy4(dst, src, bytes); }
    //! C++ wrapper around shz_memcpy8().
    SHZ_FORCE_INLINE void* memcpy8(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_memcpy8(dst, src, bytes); }
    //! C++ wrapper around shz_memset8().
    SHZ_FORCE_INLINE void* memset8(void* dst, uint64_t value, size_t bytes) noexcept { return shz_memset8(dst, value, bytes); }
    //! C++ wrapper around shz_memcpy32().
    SHZ_FORCE_INLINE void* memcpy32(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_memcpy32(dst, src, bytes); }
    //! C++ wrapper around shz_memcpy64().
    SHZ_FORCE_INLINE void* memcpy64(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_memcpy64(dst, src, bytes); }

    //! C++ wrapper around shz_memcpy128().
    SHZ_FORCE_INLINE void* memcpy128(void* dst, const void* src, size_t bytes) noexcept {
        return shz_memcpy128(dst, src, bytes);
    }

    //! C++ wrapper around shz_memcpy2_8().
    SHZ_FORCE_INLINE void memcpy2_8(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src) noexcept { shz_memcpy2_8(dst, src); }
    //! C++ wrapper around shz_memcpy2_16().
    SHZ_FORCE_INLINE void memcpy2_16(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src) noexcept { shz_memcpy2_16(dst, src); }
    //! C++ wrapper around shz_memset2_16().
    SHZ_FORCE_INLINE void memset2_16(void* dst, uint16_t value) noexcept { shz_memset2_16(dst, value); }
    //! C++ wrapper around shz_memcpy4_16().
    SHZ_FORCE_INLINE void memcpy4_16(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src) noexcept { shz_memcpy4_16(dst, src); }

    //! C++ wrapper around shz_memswap32_1().
    SHZ_FORCE_INLINE void memswap32_1(void* SHZ_RESTRICT p1, void* SHZ_RESTRICT p2) noexcept { shz_memswap32_1(p1, p2); }
    //! C++ wrapper around shz_memswap32_1_xmtrx().
    SHZ_FORCE_INLINE void memswap32_1_xmtrx(void* SHZ_RESTRICT p1, void* SHZ_RESTRICT p2) noexcept { shz_memswap32_1_xmtrx(p1, p2); }

    //! C++ wrapper around shz_sq_memcpy32().
    SHZ_FORCE_INLINE void* sq_memcpy32(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_sq_memcpy32(dst, src, bytes); }
    //! C++ wrapper around shz_sq_memcpy32_xmtrx().
    SHZ_FORCE_INLINE void* sq_memcpy32_xmtrx(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src, size_t bytes) noexcept { return shz_sq_memcpy32_xmtrx(dst, src, bytes); }
    //! C++ wrapper around shz_sq_memcpy32_1().
    SHZ_FORCE_INLINE void* sq_memcpy32_1(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src) noexcept { return shz_sq_memcpy32_1(dst, src); }
    //! C++ wrapper around shz_sq_memcpy32_1_xmtrx().
    SHZ_FORCE_INLINE void* sq_memcpy32_1_xmtrx(void* SHZ_RESTRICT dst, const void* SHZ_RESTRICT src) noexcept { return shz_sq_memcpy32_1_xmtrx(dst, src); }
}

#endif
