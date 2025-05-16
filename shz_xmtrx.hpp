#ifndef SHZ_XMTRX_HPP
#define SHZ_XMTRX_HPP

#include "shz_xmtrx.h"
#include "shz_matrix.hpp"

#include <mdspan>

namespace shz {
    static class {
    public:
        float operator[](size_t r, size_t c);
    
        void load(auto matrix &mat);
        void store(auto matrix *mat);


    } xmtrx;

    
}

#endif

