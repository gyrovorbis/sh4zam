/*! \file
 *  \brief C++ wrappers for preprocessor definitions and macro utilities.
 *
 *  This file contains the C++ API around the C preprocessor and macro API.
 *
 *  \author    2025, 2026 Falco Girgis
 *  \copyright MIT License
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#ifndef SHZ_CDEFS_HPP
#define SHZ_CDEFS_HPP

#include <type_traits>

#include "shz_cdefs.h"

#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || (defined(__cplusplus) && __cplusplus >= 202002L)
    //! Defined if building with C++20 to enable optional features.
#   define SHZ_CPP20    1
#endif

#if (defined(_MSVC_LANG) && _MSVC_LANG > 202002L) || (defined(__cplusplus) && __cplusplus > 202002L)
    //! Defined if building with C++23 to enable optional features.
#   define SHZ_CPP23    1
#endif

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

    /*! \name  Type Traits
     *  \brief Compile-time predicates on argument types, for constraining overloads.
     *  @{
     */

    /*! Whether \p Ts are the types of arguments in radians.

        A routine overloaded on `float` radians and on a `uint16_t` angle is ambiguous for a
        `double`, which converts to either. A template beside the `float` overload, enabled
        where this is true, takes it as radians. \p Ts are radians where each is arithmetic,
        at least one is floating-point and none is a `uint16_t`, so that a call with a 16-bit
        angle keeps the overload it had.
    */
    template<typename... Ts>
    inline constexpr bool is_radians_v =
        (std::is_arithmetic_v<Ts> && ...) &&
        (std::is_floating_point_v<Ts> || ...) &&
        !(std::is_same_v<Ts, uint16_t> || ...);

    //! @}
}

#endif
