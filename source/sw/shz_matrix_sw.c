/*! \file
 *  \brief   Out-of-line SW implementation of matrix routines.
 *  \ingroup matrix
 *
 *  Compiles the shared out-of-line matrix routines in shz_matrix.inc.c
 *  for the SW back-end, giving them the _sw suffix.
 *
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 *
 *  \important  Do not copy this file or its routines into other projects. Use the
 *              KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

// Force this translation unit to use the SW back-end.
#undef  SHZ_BACKEND
#define SHZ_BACKEND     SHZ_SW

#define SHZ_IMPL_SUFFIX _sw

#include "../shz_matrix.inc.c"
