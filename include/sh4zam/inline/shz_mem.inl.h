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

#if SHZ_BACKEND == SHZ_DREAMCAST
#   include "dc/shz_mem_dc.inl.h"
#else
#   include "sw/shz_mem_sw.inl.h"
#endif

//! \endcond