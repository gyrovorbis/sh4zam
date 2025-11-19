/*! \file
    \brief C++ bindings to SH4ZAM C-based Circle API.
    \ingroup misc

    \author 2025 Falco Girgis

    \copyright MIT License
*/

#ifndef SHZ_CIRCLE_HPP
#define SHZ_CIRCLE_HPP

#include "shz_circle.h"

namespace shz {
    //! C++ wrapper around shz_circle_matrix.
    constexpr auto circle_matrix = shz_circle_matrix;
}

#endif
