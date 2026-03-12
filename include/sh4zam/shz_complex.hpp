/*! \file
    \brief Complex number C++ API
    \ingroup complex

    This file contains the C++ layer for wrapping the Complex
    Number C API.

    \author 2026 Falco Girgis

    \copyright MIT License
*/

#ifndef SHZ_COMPLEX_HPP
#define SHZ_COMPLEX_HPP

#include "shz_complex.h"

namespace shz {
    class complex: public shz_complex_t {

    };

}

#endif
