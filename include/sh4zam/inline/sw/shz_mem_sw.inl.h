//! \cond INTERNAL
/*! \file
    \brief Software implementation of Quaternion API
    \ingroup quat

    This file contains the generic implementation routines for
    the memory API, which are platform-independent, shared,
    and may be run anywhere. They're offered as part of the
    SW back-end.

    \author 2026 Falco Girgis

    \copyright MIT License
*/

#ifndef SHZ_MEM_SW_INL_H
#define SHZ_MEM_SW_INL_H

#include <string.h>

SHZ_FORCE_INLINE void shz_dcache_alloc_line(void* src) SHZ_NOEXCEPT {}

SHZ_FORCE_INLINE void* shz_memcpy(      void* SHZ_RESTRICT dst,
                                  const void* SHZ_RESTRICT src,
                                  size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_memcpy1(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_memcpy2(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_memcpy4(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_memcpy8(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_memcpy32(      void* SHZ_RESTRICT dst,
                                    const void* SHZ_RESTRICT src,
                                    size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_sq_memcpy32(      void* SHZ_RESTRICT dst,
                                       const void* SHZ_RESTRICT src,
                                       size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_sq_memcpy32_xmtrx(      void* SHZ_RESTRICT dst,
                                             const void* SHZ_RESTRICT src,
                                             size_t                   bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_memcpy64(      void* SHZ_RESTRICT dst,
                                    const void* SHZ_RESTRICT src,
                                    size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void* shz_memcpy128(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src,
                                      size_t              bytes) SHZ_NOEXCEPT {
    return memcpy(dst, src, bytes);
}

SHZ_FORCE_INLINE void shz_memcpy2_16(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    memcpy(dst, src, sizeof(uint16_t) * 16);

}

SHZ_FORCE_INLINE void* shz_memset8(void* dst, uint64_t value, size_t bytes) SHZ_NOEXCEPT {
    shz_alias_uint64_t* d = (shz_alias_uint64_t*)dst;

    for(unsigned i = 0; i < bytes >> 8; ++i)
        d[i] = value;

    return dst;
}

SHZ_FORCE_INLINE void shz_memset2_16(void* dst, uint16_t value) SHZ_NOEXCEPT {
    shz_alias_int16_t* d = (shz_alias_int16_t*)dst;

    for(unsigned i = 0; i < 16; ++i)
        d[i] = value;
}

SHZ_FORCE_INLINE void shz_memcpy4_16(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    memcpy(dst, src, sizeof(uint32_t) * 16);
}

SHZ_FORCE_INLINE void shz_memcpy32_1(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    memcpy(dst, src, 32);
}

SHZ_FORCE_INLINE void shz_memswap32_1(void* SHZ_RESTRICT p1,
                                      void* SHZ_RESTRICT p2) SHZ_NOEXCEPT {
    alignas(32) uint8_t buffer[32];

    memcpy(buffer, p1, 32);
    memcpy(p1, p2, 32);
    memcpy(p2, buffer, 32);
}

SHZ_FORCE_INLINE void shz_memswap32_1_xmtrx(void* SHZ_RESTRICT p1,
                                            void* SHZ_RESTRICT p2) SHZ_NOEXCEPT {
    alignas(32) uint8_t buffer[32];

    memcpy(buffer, p1, 32);
    memcpy(p1, p2, 32);
    memcpy(p2, buffer, 32);
}

SHZ_FORCE_INLINE void* shz_sq_memcpy32_1(      void* SHZ_RESTRICT dst,
                                         const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    return memcpy(dst, src, 32);
}


SHZ_FORCE_INLINE void* shz_sq_memcpy32_1_xmtrx(      void* SHZ_RESTRICT dst,
                                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
    return memcpy(dst, src, 32);
}

#endif
