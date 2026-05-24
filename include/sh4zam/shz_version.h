/*! \file
 *  \brief Compile-Time API Versioning
 *
 *  This file provides the utility macros and constants needed for implementing
 *  compile/link-time version checking of the SH4ZAM library.
 * 
 *  \author     2026 Falco Girgis
 *  \copyright  MIT License
 */

#ifndef SHZ_VERSION_H
#define SHZ_VERSION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \name  Compile-Time Utilities
    \brief Constants and methods for checking the compile-time SH4ZAM version.
    @{
*/

#define SHZ_VERSION_MAJOR   0   //!< Compile-time SH4ZAM major version.
#define SHZ_VERSION_MINOR   7   //!< Compile-time SH4ZAM minor version.
#define SHZ_VERSION_PATCH   0   //!< Compile-time Sh4ZAM patch version.

//! Current SH4ZAM full compile-time version identifier, integer-compatible.
#define SHZ_VERSION \
    SHZ_VERSION_INIT(SHZ_VERSION_MAJOR, SHZ_VERSION_MINOR, SHZ_VERSION_PATCH)

/*! Packs values of individual version components into a SH4ZAM full version value.

    Combines values from individual version components into a single unsigned 32-bit
    integer which can be compared to other version values for equality or inequality.

    \param  major       Unsigned 8-bit field representing major version value.
    \param  minor       Unsigned 16-bit field representing minor version value.
    \param  patch       Unsigned 8-bit field representing patch version value.

    \returns            Unsigned 32-bit value representing the full version value.
*/
#define SHZ_VERSION_INIT(major, minor, patch) \
    ((((major) & 0xff) << 24) | (((minor) & 0xffff) << 8) | (((patch) & 0xff)))

//! @}

/*! \name  Link-Time Utilities
    \brief Methods for checking the link-time SH4ZAM version.
    @{
*/

//! Integer-compatible value type containing a full SH4ZAM version.
typedef uint32_t shz_version_t;

//! Version typedef for those who hate POSIX-style.
typedef shz_version_t shz_version;

//! Returns the full version of SH4ZAM that was linked against.
shz_version_t shz_version_linked(void);

//! Extracts the individual version component values from a full version.
void shz_version_fields(shz_version_t version, uint8_t* major, uint16_t* minor, uint8_t* patch);

//! @}

#ifdef __cplusplus
}
#endif

#endif
