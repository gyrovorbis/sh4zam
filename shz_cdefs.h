/*! \file
 *  \brief Preprocessor definitions and macro utilities.
 *
 *  This file contains commonly used preprocessor definitions used throughout
 *  the project:
 *      - Version information
 *      - Compiler attributes
 *      - Miscellaneous utilities
 * 
 *  \author Falco Girgis
 */
#ifndef SHZ_CDEFS_H
#define SHZ_CDEFS_H

#include <stdint.h>

/*! \name   Versioning
 *  \brief  Current library version information.
 *  @{ 
 */
#define SHZ_VERSION_MAJOR       0   //!< Major release version number
#define SHZ_VERSION_MINOR       1   //!< Minor release version number
#define SHZ_VERSION_PATCH       0   //!< Patch release version number

//! Full release version number
#define SHZ_VERSION             ((uint32_t)((SHZ_VERSION_MAJOR << 24) | (SHZ_VERSION_MINOR << 8) | (SHZ_VERSION_PATCH)))

//! Full release version number string representation
#define SHZ_VERSION_STR         SHZ_STRINGIFY(SHZ_VERSION_MAJOR) "." SHZ_STRINGIFY(SHZ_VERSION_MINOR) "." SHZ_STRINGIFY(SHZ_VERSION_PATCH)
//! @}

/*! \name   Utilities 
 *  \brief  Miscellaneous function-like macros
 *  @{
 */
 //! Stringifies an expression \i after preprocessing
#define SHZ_STRINGIFY(a)        SHZ_STRINGIFY_(a)

//! Returns the number of elements within a statically sized array
#define SHZ_COUNT_OF(array)    (sizeof(array) / sizeof((array)[0]))

//! Returns the containing structure from a pointer to one of its members
#define SHZ_CONTAINER_OF(ptr, type, member) \
    ((type*)((char*)(ptr) - offsetof(type, member)))

//! Swaps the contents of two variables
#define SHZ_SWAP(a, b) do { \
        auto tmp = a; \
        a = b; \
        b = tmp; \
    } while(false)
//! @}

#define SHZ_ALIGNAS(N)          __attribute__((aligned((N))))
#define SHZ_SIMD(N)             __attribute__((vector_size((N))))
#define SHZ_HOT                 __attribute__((hot))
#define SHZ_ICACHE_ALIGNED      __attribute__((aligned(32)))
#define SHZ_FORCE_INLINE        __attribute__((always_inline)) SHZ_INLINE
#define SHZ_FLATTEN             __attribute__((flatten))
#define SHZ_PACKED              __attribute__((packed))

#ifdef __cplusplus
#   define SHZ_BEGIN_DECLS      extern "C" {
#   define SHZ_END_DECLS        }
#   define SHZ_INLINE           inline
#else
#   define SHZ_BEGIN_DECLS
#   define SHZ_END_DECLS
#   define SHZ_INLINE           inline static
#endif

//! \cond
#define SHZ_STRINGIFY_(a)       #a
//! \endcond

#endif // SHZ_CDEFS_H
