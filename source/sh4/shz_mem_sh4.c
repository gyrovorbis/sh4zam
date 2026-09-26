/*! \file
 *  \brief   Out-of-line SH4 implementation of large memory copies.
 *  \ingroup memory
 *
 *  This file contains the block-copy path of shz_memcpy(), kept out of line
 *  so that the inline small-copy path doesn't pay for its stack frame.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

// Force this translation unit to use the SH4 back-end.
#undef  SHZ_BACKEND
#define SHZ_BACKEND     SHZ_SH4

#include "sh4zam/shz_mem.h"

void* shz_memcpy_large_sh4_(      void* SHZ_RESTRICT dst,
                            const void* SHZ_RESTRICT src,
                                 size_t              bytes) SHZ_NOEXCEPT {
    const uint8_t *s = (const uint8_t *)src;
          uint8_t *d = (      uint8_t *)dst;
    size_t copied;

    SHZ_PREFETCH(src);

    // Bring dst to 32-byte alignment. Co-aligned buffers take the word path;
    // otherwise short preludes are cheapest as bytes, long ones via newlib.
    copied = (-(uintptr_t)d) & 31;
    if(copied) {
        if(copied >= 4 && !(((uintptr_t)d ^ (uintptr_t)s) & 3))
            shz_memcpy_small_sh4_(d, s, copied);
        else
            shz_memcpy1_sh4_(d, s, copied);

        bytes -= copied;
        d     += copied;
        s     += copied;
    }

    SHZ_PREFETCH(s);

    copied = 0;
    if(!(((uintptr_t)s) & 0x7)) {
        if(SHZ_LIKELY(bytes >= 32)) {
            copied = bytes & ~31;
            shz_memcpy32_sh4(d, s, copied);
        } else if(bytes >= 8) {
            copied = bytes & ~7;
            shz_memcpy8_sh4(d, s, copied);
        }
    } else if(!(((uintptr_t)s) & 3)) {
        if(bytes >= 4) {
            copied = bytes & ~3;
            shz_memcpy4_sh4(d, s, copied);
        }
    } else if(bytes >= 32) {
        copied = bytes & ~31;
        shz_memcpy_shift_sh4_(d, s, copied >> 5);
    }

    bytes -= copied;
    if(bytes) {
        s += copied;
        d += copied;
        shz_memcpy1_sh4_(d, s, bytes);
    }

    return dst;
}
