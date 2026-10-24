/*! \file
 *  \brief   Out-of-line SH4 implementation of matrix routines.
 *  \ingroup matrix
 *
 *  Compiles the shared out-of-line matrix routines in shz_matrix.inc.c
 *  for the SH4 back-end, giving them the _sh4 suffix.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 *
 *  \important  Do not copy this file or its routines into other projects. Use the
 *              KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

// Force this translation unit to use the SH4 back-end.
#undef  SHZ_BACKEND
#define SHZ_BACKEND     SHZ_SH4

#define SHZ_IMPL_SUFFIX _sh4

#include "../shz_matrix.inc.c"
