/*! \file
    \brief Routines for manipulating the SH4's FP environment. 
    \ingroup fpscr
    
    This file provides a collection of low-level routines for modifying
    and querying the SH4's floating point environment.

    \todo
        - shz_fschg_scoped()
        - shz_frchg_scoped()

    \author    Falco Girgis
    \copyright MIT License
*/

#ifndef SHZ_FPSCR_H
#define SHZ_FPSCR_H

#include "shz_cdefs.h"

/*! \defgroup fpscr FPU Environment
    \brief          Floating-point environment manipulation.
@{ 
*/

//! Initial value of the floating-point status control register
#define SHZ_FPSCR_VALUE_DEFAULT 0x00040001

//! Swaps the active FP register bank
#define SHZ_FRCHG() asm volatile("frchg")

//! Swaps FMOV size mode (asserting that the previous mode was configured as expected).
#define SHZ_FSCHG(pairwise_mode) do { \
        assert(shz_fpscr_read().sz == !(pairwise_mode) && \
               shz_fpscr_read().pr == !(pairwise_mode));  \
        asm volatile("fschg"); \
    } while(false)

//! Debug build check that the FPU is configured in single-precision mode.
#define SHZ_SINGLE_PRECISION_GUARD() \
    assert(!shz_fpscr_read().sz && !shz_fpscr_read().pr)

SHZ_DECLS_BEGIN

//! Represents the value of the floating-point statuc control register
typedef union shz_fpscr {
    struct {            //!< Bitfield representation of flags
        uint32_t rm        : 1;  //!< 0  Rounding mode
        uint32_t           : 1;  //!< 1  Padding
        // FPU Exception flag fields
        uint32_t flag_i    : 1;  //!< 2  Inexact result
        uint32_t flag_u    : 1;  //!< 3  Underflow
        uint32_t flag_o    : 1;  //!< 4  Overflow
        uint32_t flag_z    : 1;  //!< 5  Division-by-zero
        uint32_t flag_v    : 1;  //!< 6  Invalid Operation
        // FPU Exception enable fields
        uint32_t enable_i  : 1;  //!< 7  Inexact result
        uint32_t enable_u  : 1;  //!< 8  Underflow
        uint32_t enable_o  : 1;  //!< 9  Overflow
        uint32_t enable_z  : 1;  //!< 10 Division-by-zero
        uint32_t enable_v  : 1;  //!< 11 Invalid Operation
        // FPU Exception cause fields
        uint32_t cause_i   : 1;  //!< 12 Inexact result
        uint32_t cause_u   : 1;  //!< 13 Underflow
        uint32_t cause_o   : 1;  //!< 14 Overflow
        uint32_t cause_z   : 1;  //!< 15 Division-by-zero
        uint32_t cause_v   : 1;  //!< 16 Invalid Operation
        uint32_t cause_e   : 1;  //!< 17 FPU Error
        // Control bits
        uint32_t dn        : 1;  //!< 18 Denormalization mode (0 => Denormals; 1 => Zero)
        uint32_t pr        : 1;  //!< 19 Precision mode
        uint32_t sz        : 1;  //!< 20 Transfer size mode
        uint32_t fr        : 1;  //!< 21 FP register bank
        uint32_t           : 10; //!< Padding
    };
    uint32_t value;     //!< Raw 32-bit uint representation of all fields
} shz_fpscr_t;

static_assert(sizeof(shz_fpscr_t) == sizeof(uint32_t),
              "Incorrect size for shz_fpscr_t struct!");

//! Returns the current value of the floating-point status control register
SHZ_FORCE_INLINE shz_fpscr_t shz_fpscr_read(void) SHZ_NOEXCEPT {
    return (shz_fpscr_t) {
        .value = __builtin_sh_get_fpscr()
    };
}

//! Sets the current value of the floating-point satus control register to the given value
SHZ_FORCE_INLINE void shz_fpscr_write(shz_fpscr_t new_value) SHZ_NOEXCEPT {
    __builtin_sh_set_fpscr(new_value.value);
}

SHZ_DECLS_END

// @}

#endif