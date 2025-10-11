/*! \file
    \brief Algorithm to speed up circle drawing.
    \ingroup misc

    \author Paul Cercueil

    \copyright MIT License
*/

#ifndef SHZ_CIRCLE_H
#define SHZ_CIRCLE_H

#include "shz_cdefs.h"
#include "shz_vector.h"

SHZ_DECLS_BEGIN

/*! Get the transform matrix to speed up circle drawing

    \note
    This function can be used to obtain the transform matrix that can be used
    to replace the following circle-drawing algorithm:
        for (i = 0; i < order; i++) {
            x += y / factor;
            y -= x / factor;
        }

    To compute the final X/Y from the matrix, all you need to do then is:
        x1 = x0 * matrix.x + y0 * matrix.y;
        y1 = x0 * matrix.z + y0 * matrix.w;
*/
shz_vec4_t shz_circle_matrix(unsigned int order, float factor) SHZ_NOEXCEPT;

SHZ_DECLS_END

#endif
