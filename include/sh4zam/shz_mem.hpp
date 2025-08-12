#ifndef SHZ_MEM_HPP
#define SHZ_MEM_HPP

#include "shz_mem.h"

namespace shz {
    constexpr auto ipv4_checksum     = shz_ipv4_checksum;

    constexpr auto cmp_str           = shz_cmp_str;
    constexpr auto xtrct             = shz_xtrct;
    constexpr auto dcache_alloc_line = shz_dcache_alloc_line;

    constexpr auto memcpy            = shz_memcpy;
    constexpr auto memcpy1           = shz_memcpy1;
    constexpr auto memcpy2           = shz_memcpy2;
    constexpr auto memcpy4           = shz_memcpy4;
    constexpr auto memcpy8           = shz_memcpy8;
    constexpr auto memset8           = shz_memset8;
    constexpr auto memcpy32          = shz_memcpy32;
    constexpr auto memcpy64          = shz_memcpy64;
    constexpr auto memcpy128         = shz_memcpy128;

    constexpr auto memcpy2_16        = shz_memcpy2_16;
    constexpr auto memset2_16        = shz_memset2_16;
    constexpr auto memcpy4_16        = shz_memcpy4_16;
    constexpr auto memswap32_1       = shz_memswap32_1;

    constexpr auto sq_memcpy32       = shz_sq_memcpy32;
    constexpr auto sq_memcpy32_1     = shz_sq_memcpy32_1;
}

#endif
