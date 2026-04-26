/*! \file
 *  \brief Preprocessor definitions and macro utilities.
 *
 *  This file contains commonly used preprocessor definitions used throughout
 *  the project:
 *      - Library-wide configuration defines
 *      - Compiler detection
 *      - Compiler attributes
 *      - TLS model
 *      - Conversion utilities
 *
 *  \author    2025, 2026 Falco Girgis
 *  \copyright MIT License
 */

#ifndef SHZ_CDEFS_H
#define SHZ_CDEFS_H

#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
#   include <type_traits>
#endif

/*! \name  Back-Ends
    \brief Platform-specific SH4ZAM implementations.
    @{
*/
#define SHZ_SH4   1     //!< Back-end for the Dreamcast's SH4.
#define SHZ_PPC   2     //!< Back-end for the Gamecube/Wii's Gekko PPC.
#define SHZ_MIPS  3     //!< Back-end for MIPS architectures
#define SHZ_ARM   4     //!< Back-end for ARM architectures.
#define SHZ_X86   5     //!< Back-end for x86/64 architectures.
#define SHZ_WASM  8     //!< Back-end for WebAssembly.
#define SHZ_SW    ~0    //!< Generic C-based software back-end.
//! @}

// Attempt to detect default back-end for the given build environment.
#ifndef SHZ_BACKEND
#   ifdef __DREAMCAST__
#       define SHZ_BACKEND SHZ_SH4  // Dreamcast builds use SH4 back-end.
#   else
#       define SHZ_BACKEND SHZ_SW   // Everything else uses SW C back-end.
#   endif
#endif

/*! \name  TLS Models
    \brief Defines for different mechanisms for thread-local storage.
    @{
*/
#define SHZ_TLS_DISABLED    0   //!< No thread-local variables.
#define SHZ_TLS_IMPLICIT    1   //!< Use `thread_local` keyword for TLS.
#define SHZ_TLS_PTHREAD     2   //!< Use pthread unique keys for TLS.
#define SHZ_TLS_CTHREAD     3   //!< Use C11 thread TSS keys for TLS.
//! @}

// Attempt to detect default TLS model for the given back-end.
#ifndef SHZ_TLS_MODEL
#   if SHZ_BACKEND == SHZ_SH4
#       define SHZ_TLS_MODEL    SHZ_TLS_IMPLICIT    // SH4 back-end supports compiler-level TLS.
#   elif SHZ_BACKEND == SHZ_SW
#       define SHZ_TLS_MODEL    SHZ_TLS_PTHREAD     // SW back-end uses pthread-based TLS for compatibilty.
#   endif
#endif

/*! \name  Compiler Detection
    \brief Defines for identifying the detected compiler.
    @{
*/
#ifdef _MSC_VER
#   define SHZ_MSVC         1   //!< Defined when building with Microsoft Visual C++.
#elif defined(__GNUC__)
#   define SHZ_GNUC         1   //!< Defined when building with a GCC-compatible compiler.
#   if defined(__clang__)
#       define SHZ_CLANG    1   //!< Defined when bulding with Clang.
#   elif defined(__MINGW64__)
#       define SHZ_MINGW64  1   //!< Defined when bulding with MinGW-w64.
#   elif defined(__MINGW32__)
#       define SHZ_MINGW32  1   //!< Defined when building for 32-bit MinGW or other MinGW compiler(s).
#   else
#       define SHZ_GCC      1   //!< Defined when building for GCC.
#   endif
#else
#   define SHZ_GNUC         1   // Unknown compiler -- assume it's GCC-compatible?
#endif
//! @}

/*! \name   Utilities
 *  \brief  Miscellaneous function-like macros
 *  @{
 */
//! Stringifies a literal expression.
#define SHZ_STRINGIFY_LITERAL(a)            #a
 //! Stringifies an expression _after_ preprocessing, supporting macro expansion.
#define SHZ_STRINGIFY(a)                    SHZ_STRINGIFY_LITERAL(a)
//! Returns the number of elements within a statically sized array
#define SHZ_COUNT_OF(array)                 (sizeof(array) / sizeof((array)[0]))
//! Returns the containing structure from a pointer to one of its members
#define SHZ_CONTAINER_OF(ptr, type, member) ((type*)((char*)(ptr) - offsetof(type, member)))
//! Macro which swaps the two values held by \p a and \p b.
#define SHZ_SWAP(a, b)                      do { typeof(a) tmp = a; a = b; b = tmp; } while(false)
//! Macro which forward declares a struct and its typedef.
#define SHZ_DECLARE_STRUCT(n, t)            struct n; typedef struct n t
//! Macro which forward declares a manually aligned struct and its typedef.
#define SHZ_DECLARE_STRUCT_ALIGNED(n, t, a) struct SHZ_ALIGNAS(a) n; typedef struct n t
//! @}

//! \cond
/*! \name  Compiler attributes
 *  \brief Defines for commonly-used GCC attributes.
 *  @{
 */
#ifdef SHZ_GNUC
    //! Forces a function or type to be aligned by \p N bytes.
#   define SHZ_ALIGNAS(N)              __attribute__((aligned((N))))
    //! Tells GCC that a scalar type is to be treated as a vector of size \p N.
#   define SHZ_SIMD(N)                 __attribute__((vector_size((N))))
    //! Tells GCC that a particular function should be optimized for performance.
#   define SHZ_HOT                     __attribute__((hot))
    //! Tells GCC that a particular function should be optimized for size.
#   define SHZ_COLD                    __attribute__((cold))
    //! Put this before a function definition to tell GCC to use fast math optimizations on a specific function.
#   define SHZ_FAST_MATH               __attribute__((optimize("fast-math")))
    //! Put this before a function definition to tell GCC to NOT use fast math optimizations on a specific function.
#   define SHZ_NO_FAST_MATH            __attribute__((optimize("no-fast-math")))
    //! Put this before a function definition to tell GCC to NOT unroll loops in a specific function.
#   define SHZ_NO_UNROLL_LOOPS         __attribute__((optimize("no-unroll-loops")))
    //! Tells GCC to disable any optimizations when compiling a function.
#   define SHZ_NO_OPTIMIZATION         __attribute__((optimize("O0")))
    //! Aligns a function within the .text segment by \p N bytes.
#   define SHZ_FUNC_ALIGNAS(N)         __attribute__((aligned(N)))
    //! Forces GCC to inline the given function.
#   define SHZ_FORCE_INLINE            __attribute__((always_inline)) SHZ_INLINE
    //! Prevents GCC from inlining the given function.
#   define SHZ_NO_INLINE               __attribute__((noinline))
    //! Forces GCC to inline all calls to other functions made within the tagged function.
#   define SHZ_FLATTEN                 __attribute__((flatten))
    //! Tells GCC not to introduce any extra padding for the given type.
#   define SHZ_PACKED                  __attribute__((packed))
    //! Tells GCC the function has no effects other than returning a value that depends on its arguments and global variables.
#   define SHZ_PURE                    __attribute__((pure))
    //! Tells GCC the function has no effects other than returning a value that depends only on its arguments.
#   define SHZ_CONST                   __attribute__((const))
    //! Tells GCC that the decorated pointer may be breaking strict aliasing rules for C and C++
#   define SHZ_ALIASING                __attribute__((__may_alias__))
    //! Tells GCC that the expression is likely to be true (used for conditional and loop optimizations)
#   define SHZ_LIKELY(e)               __builtin_expect(!!(e), 1)
    //! Tells GCC that the expression is likely to be false (used for conditional and loop optimizations)
#   define SHZ_UNLIKELY(e)             __builtin_expect(!!(e), 0)
    //! Tells GCC to use its builtin intrinsic for prefetching (better instruction scheduling than pure ASM pref)
#   define SHZ_PREFETCH(a)             __builtin_prefetch(a)
    //! Tells GCC the pointer paraemter is unique and is not aliased by another parameter
#   define SHZ_RESTRICT                __restrict__
    //! Creates a software memory barrier beyond which any loads or stores may not be reordered
#   define SHZ_MEMORY_BARRIER_SOFT()   asm volatile("" : : : "memory")
    //! Creates a hardware memory barrier beyond which any loads or stores may not be reordered
#   define SHZ_MEMORY_BARRIER_HARD()   __sync_synchronize()
#elif defined(SHZ_MSVC)
    //! Forces a function or type to be aligned by \p N bytes.
#   define SHZ_ALIGNAS(N)              __declspec(align(N))
    //! Unsupported by MSVC.
#   define SHZ_SIMD(N)
    //! Unsupported by MSVC.
#   define SHZ_HOT
    //! Unsupported by MSVC.
#   define SHZ_COLD
    //! Unsupported by MSVC.
#   define SHZ_FAST_MATH
    //! Unsupported by MSVC.
#   define SHZ_NO_FAST_MATH
    //! Unsupported by MSVC.
#   define SHZ_NO_UNROLL_LOOPS
    //! Unsupported by MSVC.
#   define SHZ_NO_OPTIMIZATION
    //! Unsupported by MSVC.
#   define SHZ_FUNC_ALIGNAS(N)
    //! Forces MSVC to inline the given function.
#   define SHZ_FORCE_INLINE            __forceinline
    //! Prevents MSVC from inlining the given function.
#   define SHZ_NO_INLINE               __declspec(noinline)
    //! Unsupported by MSVC.
#   define SHZ_FLATTEN
    //! Unimplemented for MSVC.
#   define SHZ_PACKED
    //! Unsupported by MSVC.
#   define SHZ_PURE                    SHZ_CONST
    //! Tells MSVC that a function may be evaluated at compile-time.
#   define SHZ_CONST                   constexpr
    //! MSVC already doesn't optimize for strict aliasing rules.
#   define SHZ_ALIASING
    //! Unimplemented for MSVC.
#   define SHZ_LIKELY(e)              (e)
    //! Unimplemented for MSVC.
#   define SHZ_UNLIKELY(e)            (e)
    //! Unimplemented for MSVC.
#   define SHZ_PREFETCH(a)
    //! Tells MSVC the pointer paraemter is unique and is not aliased by another parameter.
#   define SHZ_RESTRICT               __restrict
    //! Unimplemented for MSVC.
#   define SHZ_MEMORY_BARRIER_SOFT()
    //! Unimpemented for MSVC.
#   define SHZ_MEMORY_BARRIER_HARD()
#endif

#ifndef __cplusplus
    //! Dummy define provided for C++ compatibility
#   define SHZ_DECLS_BEGIN
    //! Dummy define provided for C++ compatibility
#   define SHZ_DECLS_END
    //! Requests a function or member to be inlined (nonforcibly) OR to have static linkage.
#   define SHZ_INLINE                   inline static
    //! Dummy define provided for C++ compatibility
#   define SHZ_NOEXCEPT
    //! Temporary struct initialization statement to use within C.
#   define SHZ_INIT(type, ...)         ((type){ __VA_ARGS__ })
    //! Conversion macro for zero-overhead conversions, taking the given \p value to a value of the given \p type.
#   define SHZ_CONVERT(type, value) \
        (((struct { \
            union { \
                typeof(value) from; \
                type to; \
            }; \
            static_assert(sizeof(type) == sizeof(value), "SHZ_CONVERT: Cannot convert between types of differing sizes."); \
        }){ (value) }).to)
#else
    //! Forces functions declared after this directive to use C linkage.
#   define SHZ_DECLS_BEGIN              extern "C" {
    //! Ends forcing functions to use C linkage.
#   define SHZ_DECLS_END                }
    //! Requests a function or member to be inlined (nonforcibly).
#   define SHZ_INLINE                   inline
    //! Tells the compiler that the function does not throw exceptions
#   define SHZ_NOEXCEPT                 noexcept
    //! Temporary struct initialization statement to use within C++.
#   define SHZ_INIT(type, ...)         (type{ __VA_ARGS__ })
    //! Conversion macro for zero-overhead conversions that handles pointers/references \p from and \p type.
#   define SHZ_CONVERT(type, from) \
        [&]<typename To, typename V>(V&& value) -> To { \
            using TNR = std::remove_reference_t<To>; \
            using VNR = std::remove_reference_t<V>; \
            if constexpr (std::is_pointer_v<To>) { \
                if constexpr (std::is_pointer_v<VNR>) { \
                    return reinterpret_cast<To>(value); \
                } else { \
                    return reinterpret_cast<To>(&value); \
                } \
            } else if constexpr (std::is_reference_v<To>) { \
                static_assert(sizeof(VNR) == sizeof(TNR), "SHZ_CONVERT: Cannot convert between types of differing sizes when converting to a reference type."); \
                return reinterpret_cast<To>(value); \
            } else { \
                static_assert(sizeof(VNR) == sizeof(TNR), "SHZ_CONVERT: Cannot convert between types of differing sizes."); \
                return reinterpret_cast<To&>(value); \
            } \
        }.template operator()<type>(from)
#endif
//! @}
//! \endcond

/*! \name TLS Utilities
    \brief Macros for declaring and managing thread-local variables.
    @{
*/
#if SHZ_TLS_MODEL == SHZ_TLS_DISABLED
    //! Declares a TLS variable with disabled model (so not thread-local).
#   define SHZ_TLS_DECL(type, name, ...) static type name = __VA_ARGS__;
    //! References a TLS variable, with disabled model.
#   define SHZ_TLS_REF(name)             (&name)
#elif SHZ_TLS_MODEL == SHZ_TLS_IMPLICIT
#   include <threads.h>
    //! Declares a TLS variable with compiler-driven implicit TLS model.
#   define SHZ_TLS_DECL(type, name, ...) static thread_local type name = __VA_ARGS__;
    //! References a TLS variable with compiler-driven implicit TLS model.
#   define SHZ_TLS_REF(name)             (&name)
#elif SHZ_TLS_MODEL == SHZ_TLS_PTHREAD
#   include <pthread.h>
#   include <stdlib.h>
    //! Declares a TLS variable using pthread thread-specific data.
#   define SHZ_TLS_DECL(type, name, ...) \
        static pthread_key_t name; \
        static void tls_##name##_init_(void) { \
            int res = pthread_key_create(&name, free); \
            (void)res; assert(!res); \
        } \
        static type* tls_##name##_ref_(void) { \
            static pthread_once_t once_ctrl = PTHREAD_ONCE_INIT; \
            int ret = pthread_once(&once_ctrl, tls_##name##_init_); \
            (void)ret; assert(!ret); \
            type* ptr = pthread_getspecific(name); \
            if(!ptr) { \
                ptr = (type *)malloc(sizeof(type)); \
                assert(ptr); \
                type temp = __VA_ARGS__; \
                memcpy(ptr, &temp, sizeof(type)); \
                ret = pthread_setspecific(name, ptr); \
                assert(!ret); \
            } \
            return ptr; \
        }
    //! References a TLS variable using pthread thread-specific data.
#   define SHZ_TLS_REF(name) tls_##name##_ref_()
#elif SHZ_TLS_MODEL == SHZ_TLS_CTHREAD
#   include <threads.h>
#   include <stdlib.h>
    //! Declares a TLS variable using C11 thread-specific storage.
#   define SHZ_TLS_DECL(type, name, ...) \
        static tss_t name; \
        static void tls_##name##_init_(void) { \
            int res = tss_create(&name, free); \
            (void)res; assert(res == thrd_success); \
        } \
        static type* tls_##name##_ref_(void) { \
            static once_flag once_ctrl = ONCE_FLAG_INIT; \
            call_once(&once_ctrl, tls_##name##_init_); \
            type* ptr = (type *)tss_get(name); \
            if(!ptr) { \
                ptr = (type *)malloc(sizeof(type)); \
                assert(ptr); \
                type temp = __VA_ARGS__; \
                memcpy(ptr, &temp, sizeof(type)); \
                int res = tss_set(name, ptr); \
                (void)res; assert(res == thrd_success); \
            } \
            return ptr; \
        }
    //! References a TLS variable using C11 thread-specific storage.
#   define SHZ_TLS_REF(name) tls_##name##_ref_()
#endif
//! @}

/*! \name  Aliasing Types
 *  \brief Types which may break C/C++'s strict aliasing rules
 *  @{
 */
//! int16_t type whose value may be aliased as another type.
typedef SHZ_ALIASING int16_t  shz_alias_int16_t;
//! uint16_t type whose value may be aliased as another type.
typedef SHZ_ALIASING uint16_t shz_alias_uint16_t;
//! int32_t type whose value may be aliased as another type.
typedef SHZ_ALIASING int32_t  shz_alias_int32_t;
//! uint32_t type whose value may be aliased as another type.
typedef SHZ_ALIASING uint32_t shz_alias_uint32_t;
//! float type whose value may be aliased as another type.
typedef SHZ_ALIASING float    shz_alias_float_t;
//! int64_t type whose value may be aliased as another type.
typedef SHZ_ALIASING int64_t  shz_alias_int64_t;
//! uint64_t type whose value may be aliased as another type.
typedef SHZ_ALIASING uint64_t shz_alias_uint64_t;
//! double type whose value may be aliased as another type.
typedef SHZ_ALIASING double   shz_alias_double_t;
//! @}

#endif // SHZ_CDEFS_H
