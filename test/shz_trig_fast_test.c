#include <sh4zam/shz_trig.h>

#ifndef __FAST_MATH__
#error This regression fixture must be compiled with -ffast-math.
#endif

// Keep this in a separate, non-LTO translation unit so a constant argument
// cannot bypass the runtime backend through __builtin_constant_p().
shz_sincos_t shz_test_sincosu16_fast(uint16_t angle) {
    return shz_sincosu16(angle);
}
