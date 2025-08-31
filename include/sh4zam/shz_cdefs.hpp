#ifndef SHZ_CDEFS_HPP
#define SHZ_CDEFS_HPP

#include "shz_cdefs.h"

//! Namespace enclosing the SH4ZAM C++ API.
namespace shz {
    /*! \name  Aliasing Types
     *  \brief Types which may break C/C++'s strict aliasing rules
     *  @{
     */
    //! int16_t type whose value may be aliased as another type.
    using alias_int16_t  = shz_alias_int16_t;
    //! uint16_t type whose value may be aliased as another type.
    using alias_uint16_t = shz_alias_uint16_t;
    //! int32_t type whose value may be aliased as another type.
    using alias_int32_t  = shz_alias_int32_t;
    //! uint32_t type whose value may be aliased as another type.
    using alias_uint32_t = shz_alias_uint32_t;
    //! float type whose value may be aliased as another type.
    using alias_float_t  = shz_alias_float_t;
    //! int64_t type whose value may be aliased as another type.
    using alias_int64_t  = shz_alias_int64_t;
    //! uint64_t type whose value may be aliased as another type.
    using alias_uint64_t = shz_alias_uint64_t;
    //! double type whose value may be aliased as another type.
    using alias_double_t = shz_alias_double_t;
    //! @}
}

#endif
