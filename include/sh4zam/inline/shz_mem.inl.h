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

//! \endcond