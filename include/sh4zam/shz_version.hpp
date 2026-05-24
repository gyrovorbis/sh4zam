/*! \file
 *  \brief Compile-Time API Versioning
 *
 *  This file provides the utility macros and constants needed for implementing
 *  compile/link-time version checking of the SH4ZAM library using C++.
 * 
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

#ifndef SHZ_VERSION_HPP
#define SHZ_VERSION_HPP

#include "shz_version.h"

#include <string>

namespace shz {
    
    //! Class representing a full SH4ZAM API version.
    class version {
        shz_version_t value_;    //!< Private packed value member.

    public:
        //! Constructs a version from the given full value.
        constexpr version(shz_version_t value) noexcept:
            value_(value) {}

        //! Constructs a full version from the given component values.
        constexpr version(uint8_t major, uint16_t minor, uint8_t patch) noexcept:
            version(SHZ_VERSION_INIT(major, minor, patch)) {}

        //! Compile-time version of SH4ZAM headers which are being included.
        constexpr static version compiled() noexcept { return SHZ_VERSION; }

        //! Link-time version of SH4ZAM library which was linked against.
        static version linked() noexcept { return shz_version_linked(); }

        //! Implicit conversion operator to make this class automatically comparable to a `uint32_t`.
        constexpr operator shz_version_t() const noexcept { return value_; }

        //! Assignment operator for assigning a version to a full version ID value.
        constexpr version operator=(uint32_t rhs) noexcept { value_ = rhs; return *this; }

        //! Extracts the major component value from a full version.
        constexpr uint8_t major() const noexcept { return (value_ >> 24) & 0xff; }
        
        //! Extracts the minor component value from a full version.
        constexpr uint16_t minor() const noexcept { return (value_ >> 8) & 0xffff; }
        
        //! Extracts the patch component value from a full version.
        constexpr uint8_t patch() const noexcept { return value_ & 0xff; }
    };

    //! Alias for those who prefer POSIX-style.
    using version_t = version;
}

#endif
