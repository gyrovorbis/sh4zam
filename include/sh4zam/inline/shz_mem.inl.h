//! \cond INTERNAL
/*! \file
 *  \brief   Memory API Implementation
 *  \ingroup memory
 *
 *  Implementation of inlined memory API routines,
 *  simply delegating to each back-end to implement
 *  their own, as there is currently no shared code.
 *
 *  \copyright MIT License
 */

#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_mem_sh4.inl.h"
#else
#   include "sw/shz_mem_sw.inl.h"
#endif

SHZ_FORCE_INLINE void shz_dcache_alloc_line(void* src) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_dcache_alloc_line_sh4(src);
#else
    shz_dcache_alloc_line_sw(src);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy(      void* SHZ_RESTRICT dst,
                                  const void* SHZ_RESTRICT src,
                                  size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy_sh4(dst, src, bytes);
#else
    return shz_memcpy_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy1(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy1_sh4(dst, src, bytes);
#else
    return shz_memcpy1_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy2(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy2_sh4(dst, src, bytes);
#else
    return shz_memcpy2_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy4(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy4_sh4(dst, src, bytes);
#else
    return shz_memcpy4_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy8(      void* SHZ_RESTRICT dst,
                                   const void* SHZ_RESTRICT src,
                                        size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy8_sh4(dst, src, bytes);
#else
    return shz_memcpy8_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy32(      void* SHZ_RESTRICT dst,
                                    const void* SHZ_RESTRICT src,
                                    size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy32_sh4(dst, src, bytes);
#else
    return shz_memcpy32_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_sq_memcpy32(      void* SHZ_RESTRICT dst,
                                       const void* SHZ_RESTRICT src,
                                       size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_sq_memcpy32_sh4(dst, src, bytes);
#else
    return shz_sq_memcpy32_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_sq_memcpy32_xmtrx(      void* SHZ_RESTRICT dst,
                                             const void* SHZ_RESTRICT src,
                                             size_t                   bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_sq_memcpy32_xmtrx_sh4(dst, src, bytes);
#else
    return shz_sq_memcpy32_xmtrx_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy64(      void* SHZ_RESTRICT dst,
                                    const void* SHZ_RESTRICT src,
                                    size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy64_sh4(dst, src, bytes);
#else
    return shz_memcpy64_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void* shz_memcpy128(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src,
                                      size_t              bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memcpy128_sh4(dst, src, bytes);
#else
    return shz_memcpy128_sw(dst, src, bytes);
#endif
}

SHZ_FORCE_INLINE void shz_memcpy2_16(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_memcpy2_16_sh4(dst, src);
#else
    shz_memcpy2_16_sw(dst, src);
#endif
}

SHZ_FORCE_INLINE void* shz_memset8(void* dst, uint64_t value, size_t bytes) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_memset8_sh4(dst, value, bytes);
#else
    return shz_memset8_sw(dst, value, bytes);
#endif
}

SHZ_FORCE_INLINE void shz_memset2_16(void* dst, uint16_t value) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_memset2_16_sh4(dst, value);
#else
    shz_memset2_16_sw(dst, value);
#endif
}

SHZ_FORCE_INLINE void shz_memcpy4_16(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_memcpy4_16_sh4(dst, src);
#else
    shz_memcpy4_16_sw(dst, src);
#endif
}

SHZ_FORCE_INLINE void shz_memcpy32_1(      void* SHZ_RESTRICT dst,
                                     const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_memcpy32_1_sh4(dst, src);
#else
    shz_memcpy32_1_sw(dst, src);
#endif
}

SHZ_FORCE_INLINE void shz_memswap32_1(void* SHZ_RESTRICT p1,
                                      void* SHZ_RESTRICT p2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_memswap32_1_sh4(p1, p2);
#else
    shz_memswap32_1_sw(p1, p2);
#endif
}

SHZ_FORCE_INLINE void shz_memswap32_1_xmtrx(void* SHZ_RESTRICT p1,
                                            void* SHZ_RESTRICT p2) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    shz_memswap32_1_xmtrx_sh4(p1, p2);
#else
    shz_memswap32_1_xmtrx_sw(p1, p2);
#endif
}

SHZ_FORCE_INLINE void* shz_sq_memcpy32_1(      void* SHZ_RESTRICT dst,
                                         const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_sq_memcpy32_1_sh4(dst, src);
#else
    return shz_sq_memcpy32_1_sw(dst, src);
#endif
}


SHZ_FORCE_INLINE void* shz_sq_memcpy32_1_xmtrx(      void* SHZ_RESTRICT dst,
                                               const void* SHZ_RESTRICT src) SHZ_NOEXCEPT {
#if SHZ_BACKEND == SHZ_SH4
    return shz_sq_memcpy32_1_xmtrx_sh4(dst, src);
#else
    return shz_sq_memcpy32_1_xmtrx_sw(dst, src);
#endif
}

//! \endcond