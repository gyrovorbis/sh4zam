/*! \file
 *  \brief Preprocessor definitions and macro utilities.
 *
 *  This file contains commonly used preprocessor definitions used throughout
 *  the project:
 *      - Compiler attributes
 *      - Miscellaneous utilities
 *
 *  \author Falco Girgis
 * 
 *  \todo
 *      - run-time version function
 */
#ifndef SHZ_CDEFS_H
#define SHZ_CDEFS_H

#include <stdint.h>

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
    ((type *)((char *)(ptr) - offsetof(type, member)))

//! Swaps the contents of two variables
#define SHZ_SWAP(a, b) do { \
        typeof(a) tmp = a; \
        a = b; \
        b = tmp; \
    } while(false)
//! @}

//! \cond
/*! \name Compiler attributes
 *  \brief Defines for commonly-used GCC attributes.
 *  @{
 */
//! Forces a function or type to be aligned by \p N bytes.
#define SHZ_ALIGNAS(N)           __attribute__((aligned((N))))
//! Tells GCC that a scalar type is to be treated as a vector of size \p N.
#define SHZ_SIMD(N)              __attribute__((vector_size((N))))
//! Tells GCC that a particular function should be optimized for performance.
#define SHZ_HOT                  __attribute__((hot))
//! Aligns a function by the size of an icache line (32 bytes).
#define SHZ_ICACHE_ALIGNED       __attribute__((aligned(32)))
//! Forces GCC to inline the given function.
#define SHZ_FORCE_INLINE         __attribute__((always_inline)) SHZ_INLINE
//! Prevents GCC from inlining the given function.
#define SHZ_NO_INLINE            __attribute__((noinline))
//! Forces GCC to inline all calls to other functions made within the tagged function.
#define SHZ_FLATTEN              __attribute__((flatten))
//! Tells GCC not to introduce any extra padding for the given type.
#define SHZ_PACKED               __attribute__((packed))
//! Tells GCC the function has no effects other than returning a value that depends on its arguments and global variables.
#define SHZ_PURE                 __attribute__((pure))
//! Tells GCC that the decorated pointer may be breaking strict aliasing rules for C and C++
#define SHZ_ALIASING             __attribute__((__may_alias__))
//! Tells GCC that the expression is likely to be true (used for conditional and loop optimizations)
#define SHZ_LIKELY(e)            __builtin_expect(!!(e), 1)
//! Tells GCC that the expression is likely to be false (used for conditional and loop optimizations)
#define SHZ_UNLIKELY(e)          __builtin_expect(!!(e), 0)
//! Tells GCC to use its builtin intrinsic for prefetching (better instruction scheduling than pure ASM pref)
#define SHZ_PREFETCH(a)          __builtin_prefetch(a)
//! Tells GCC to issue a prefetch, using inline ASM so that it cannot be reordered
#define SHZ_PREFETCH_VOLATILE(a) asm volatile("pref @%0" : : "r" (a))
//! Put this before a function definition to tell GCC to use fast math optimizations on a specific function.
#define SHZ_FAST_MATH           __attribute__((optimize("fast-math")))
//! Put this before a function definition to tell GCC to NOT use fast math optimizations on a specific function.
#define SHZ_NO_FAST_MATH        __attribute__((optimize("no-fast-math")))

#ifndef __cplusplus
    //! Dummy define provided for C++ compatibility
#   define SHZ_DECLS_BEGIN
    //! Dummy define provided for C++ compatibility
#   define SHZ_DECLS_END
    //! Requests a function or member to be inlined (nonforcibly) OR to have static linkage.
#   define SHZ_INLINE           inline static
    //! Tells GCC the function has no effects other than returning a value that depends only on its arguments.
#   define SHZ_CONST            __attribute__((const))
    //! Tells GCC the pointer paramter is unique and is not aliased by another parameter
#   define SHZ_RESTRICT         restrict
    //! Dummy define provided for C++ compatibility
#   define SHZ_NOEXCEPT
#else
    //! Forces functions declared after this directive to use C linkage.
#   define SHZ_DECLS_BEGIN      extern "C" {
    //! Ends forcing functions to use C linkage.
#   define SHZ_DECLS_END        }
    //! Requests a function or member to be inlined (nonforcibly).
#   define SHZ_INLINE           inline
    //! Tells GCC the function has no effects other than returning a value that depends only on its arguments.
#   define SHZ_CONST            __attribute__((const)) constexpr
    //! Dummy define provided for C compatibility
#   define SHZ_RESTRICT
    //! Tells the compiler that the function does not throw exceptions
#   define SHZ_NOEXCEPT         noexcept
#endif
//! @}
//! \endcond

typedef SHZ_ALIASING int16_t  shz_alias_int16_t;
typedef SHZ_ALIASING uint16_t shz_alias_uint16_t;
typedef SHZ_ALIASING int32_t  shz_alias_int32_t;
typedef SHZ_ALIASING uint32_t shz_alias_uint32_t;
typedef SHZ_ALIASING float    shz_alias_float_t;
typedef SHZ_ALIASING int64_t  shz_alias_int64_t;
typedef SHZ_ALIASING uint64_t shz_alias_uint64_t;
typedef SHZ_ALIASING double   shz_alias_double_t;

//! Namespace containing the C++23 interface of the SH4ZAM API.
namespace shz {
    //! Utility class template used give a derived class compatibility with a value type.
    template<typename P>
    class primitive_compatible {
    protected:
        P value_; //<! Encapsulated inner value of the requested type.
    public:
        //! Allows construction from an existing compatible value or its default value.
        SHZ_FORCE_INLINE primitive_compatible(P value={}) noexcept:
            value_(std::move(value)) {}
        //!Allows a derived class to convert to a value of the given type.
        SHZ_FORCE_INLINE operator P() const noexcept { return value_; }
        //! Allows a derived class to be assigned to an existing value of the given type.
        SHZ_FORCE_INLINE P operator=(P rhs) noexcept { value_ = std::move(rhs); }
    };
}

//! \cond
#define SHZ_STRINGIFY_(a)       #a
//! \endcond

#endif // SHZ_CDEFS_H
