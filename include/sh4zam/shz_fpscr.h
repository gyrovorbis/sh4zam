/*! \file
    \brief Routines for manipulating the SH4's FP environment. 
    \ingroup fpscr
    
    This file provides a collection of low-level routines for modifying
    and querying the SH4's floating point environment. 

    \author Falco Girgis 
*/
#ifndef SHZ_FPSCR_H
#define SHZ_FPSCR_H

#include "shz_cdefs.h"

/*! \defgroup fpscr FPU Environment
    \brief          Floating-point environment manipulation.
@{ 
*/

#define SHZ_FPSCR_INITIAL_VALUE 0x00040001

#define SHZ_FSCHG(pairwise_mode) do { \
        assert(shz_fpscr_read().sz == !(pairwise_mode)); \
        asm volatile("fschg"); \
    } while(false)

#define SHZ_FRCHG() asm volatile("frchg")

SHZ_DECLS_BEGIN

typedef union shz_fpscr {
    struct {            //< Bitfield representation of flags
        uint32_t          : 10; //< Padding
        uint32_t fr       :  1; //< FP register bank
        uint32_t sz       :  1; //< Transfer size mode
        uint32_t pr       :  1; //< Precision mode
        uint32_t dn       :  1; //< Denormalization mode (0 => Denormals; 1 => Zero)
        // FPU Exception cause fields
        uint32_t cause_e  :  1; //< FPU Error
        uint32_t cause_v  :  1; //< Invalid Operation
        uint32_t cause_z  :  1; //< Division-by-zero
        uint32_t cause_o  :  1; //< Overflow
        uint32_t cause_u  :  1; //< Underflow
        uint32_t cause_i  :  1; //< Inexact result
        // FPU Exception enable fields
        uint32_t enable_v :  1; //< Invalid Operation
        uint32_t enable_z :  1; //< Division-by-zero
        uint32_t enable_o :  1; //< Overflow
        uint32_t enable_u :  1; //< Underflow
        uint32_t enable_i :  1; //< Inexact result
        // FPU Exception flag fields
        uint32_t flag_v   :  1; //< Invalid Operation
        uint32_t flag_z   :  1; //< Division-by-zero
        uint32_t flag_o   :  1; //< Overflow
        uint32_t flag_u   :  1; //< Underflow
        uint32_t flag_i   :  1; //< Inexact result
        uint32_t          :  1; //< Padding
        uint32_t rm       :  1; //< Rounding Mode (0 => Nearest; 1 => Zero)
    };
    uint32_t value;     //< Raw 32-bit uint representation of all fields
} shz_fpscr_t;

static_assert(sizeof(shz_fpscr_t) == sizeof(uint32_t),
              "Incorrect size for shz_fpscr_t struct!");

SHZ_FORCE_INLINE shz_fpscr_t shz_fpscr_read(void) {
    return (shz_fpscr_t) {
        .value = __builtin_sh_get_fpscr()
    };
}

SHZ_FORCE_INLINE void shz_fpscr_write(shz_fpscr_t new_value) {
    __builtin_sh_set_fpscr(new_value.value);
}

SHZ_DECLS_END

// @}

#endif