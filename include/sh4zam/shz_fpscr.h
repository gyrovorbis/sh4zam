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

/*! \defgroup fpscr Floating Point Status and Control
    \brief          Floating point environment manipulation.
@{ 
*/

#define SHZ_FPSCR_INITIAL_VALUE 0x00040001

#define SHZ_FSCHG(pairwise_mode) do { \
        assert(shz_fpscr_read().sz == !(pairwise_mode)); \
        asm volatile("fschg"); \
    } while(false)

SHZ_DECLS_BEGIN

typedef union shz_fpscr {
    struct {
        struct {
            bool    : 10;
            bool fr :  1;   //< FP register bank
            bool sz :  1;   //< Transfer size mode
            bool pr :  1;   //< Precision mode
            bool dn :  1;   //< Denormalization mode (0 => Denormals; 1 => Zero)
        };
        struct {
            bool e  :  1;   //< FPU Error
            bool v  :  1;   //< Invalid Operation
            bool z  :  1;   //< Division-by-zero
            bool o  :  1;   //< Overflow
            bool u  :  1;   //< Underflow
            bool i  :  1;   //< Inexact result
        } cause;            //< FPU Exception cause field
        struct {
            bool v  :  1;   //< Invalid Operation
            bool z  :  1;   //< Division-by-zero
            bool o  :  1;   //< Overflow
            bool u  :  1;   //< Underflow
            bool i  :  1;   //< Inexact result
        } enable,           //< FPU Exception enable field
          flag;             //< FPU Exception flag field
        bool        :  1;
        bool     rm :  1;   //< Rounding Mode (0 => Nearest; 1 => Zero)
    };
    uint32_t value;
} shz_fpscr_t;

static_assert(sizeof(shz_fpscr_t) == sizeof(uint32_t),
              "Incorrect size for shz_fscr_t struct!")

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