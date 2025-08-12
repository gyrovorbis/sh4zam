#ifndef SHZ_XMTRX_HPP
#define SHZ_XMTRX_HPP

#include "shz_xmtrx.h"

namespace shz {

    struct xmtrx {
        enum class reg: uint8_t {
            XF0  = SHZ_XMTRX_XF0,
            XF1  = SHZ_XMTRX_XF1,
            XF2  = SHZ_XMTRX_XF2,
            XF3  = SHZ_XMTRX_XF3,
            XF4  = SHZ_XMTRX_XF4,
            XF5  = SHZ_XMTRX_XF5,
            XF6  = SHZ_XMTRX_XF6,
            XF7  = SHZ_XMTRX_XF7,
            XF8  = SHZ_XMTRX_XF8,
            XF9  = SHZ_XMTRX_XF9,
            XF10 = SHZ_XMTRX_XF10,
            XF11 = SHZ_XMTRX_XF11,
            XF12 = SHZ_XMTRX_XF12,
            XF13 = SHZ_XMTRX_XF13,
            XF14 = SHZ_XMTRX_XF14,
            XF15 = SHZ_XMTRX_XF15
        };

        inline static float read(reg xf) noexcept {
            return shz_xmtrx_read_reg(static_cast<shz_xmtrx_reg>(xf));
        }

        inline static void write(reg xf, float value) noexcept {
            shz_xmtrx_write_reg(static_cast<shz_xmtrx_reg>(xf), value);
        }
    };
}

#endif
