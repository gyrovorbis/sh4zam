/*! \file
 *  \brief Preprocessor definitions and macro utilities.
 *
 *  This file contains commonly used preprocessor definitions used throughout
 *  the project:
 *      - Compiler attributes
 *      - Miscellaneous utilities
 *
 *  \author    Falco Girgis
 *  \copyright MIT License
 */

#ifndef SHZ_CDEFS_H
#define SHZ_CDEFS_H

#ifndef __cplusplus
#include <stdint.h>
#include <assert.h>
#else
#include <cstdint>
#include <cassert>
#endif// C else C++

//================================================================

#ifdef __has_include
#define SHZ_HAS_INCLUDE(x)        __has_include(x)
#else
#define SHZ_HAS_INCLUDE(x)        0
#endif//__has_include

//================================================================

#if defined(_MSC_VER)
// <intrin.h> includes { immintrin.h | mm3dnow.h | armintr.h | arm_neon.h | arm64intr.h | arm64_neon.h }
// depening on supported target architecture
// Also includes { vcruntime.h | setjmp.h | **softintrin.h** }
// (**) only #ifndef {defined(_M_CEE_PURE)} and #if (defined(_M_ARM64EC) && !defined(_DISABLE_SOFTINTRIN_))
#include <intrin.h>
//----------------------------------------------------------------
//TODO: Maybe keep this, though I didn't find the declarations/definitions for
//      __check_arch_support nor __check_isa_support, aside from documentation on them existing.
//      They're supposedly inside <immintrin.h>, but their declaration/definition was not located
//      in my recursive search in all included headers.
//----------------------------------------------------------------
// #if defined(_INCLUDED_IMM)
// #include <isa_availability.h>
// // `isa_feature`    Bit flags for checking availability of (enum ISA_AVAILABILITY) 
// // `avx10_version`  Only for AVX10.x, 0 when AVX10 version check isn't required.
// #define SHZ_MSVC_CHECK_ISA_SUPPORT(isa_feature,   avx10_version)  ( \
//         __check_arch_support((isa_feature), (avx10_version)) || \
//         __check_isa_support ((isa_feature), (avx10_version))    \
//     )
//
// // __check_arch_support - detects if the arch flag supports the specified ISA feature and AVX10 version at compile time.
// // __check_isa_support  - detects if the processor supports the specified ISA feature and AVX10 version at run time.
// #else
// #define SHZ_MSVC_CHECK_ISA_SUPPORT(isa_feature, avx10_version)  0
// #endif
// #else
// #define SHZ_MSVC_CHECK_ISA_SUPPORT(isa_feature, avx10_version)  0
#endif// MSVC Headers

//================================================================

#define SHZ_C_23        202311L // C23
#define SHZ_C_17        201710L // C17
#define SHZ_C_11        201112L // C11
#define SHZ_C_99        199901L // C99
#define SHZ_C_95        199409L // C95
#define SHZ_C_89        198999L // C89 (fill-in version, __STDC_VERSION__ was undefined before C95)

//================================================================

#define SHZ_CXX_26      202401L // C++26, NOTE: Current Experimental Version, update when standardized.
#define SHZ_CXX_23      202302L // C++23
#define SHZ_CXX_20      202002L // C++20
#define SHZ_CXX_17      201703L // C++17
#define SHZ_CXX_14      201402L // C++14
#define SHZ_CXX_11      201103L // C++11
#define SHZ_CXX_98      199711L // C++98

//================================================================
// C Version
//================================================================
#ifdef  __STDC_VERSION__
//----------------------------------------------------------------
#   if defined(_MSC_VER) && !defined(__cplusplus)
//TODO: Going to need to add feature testing for acquiring the C version
//      without relying on an external macro definition/configuration.
#   define shz_c_version    SHZ_C_11
#   else
#   define shz_c_version    __STDC_VERSION__
#   endif//shz_c_version (temporary)
//----------------------------------------------------------------
#   if   (shz_c_version >=  SHZ_C_23)
    // C23
#   define SHZ_C_VERSION    SHZ_C_23
#   elif (shz_c_version >=  SHZ_C_17)
    // C17
#   define SHZ_C_VERSION    SHZ_C_17
#   elif (shz_c_version >=  SHZ_C_11)
    // C11
#   define SHZ_C_VERSION    SHZ_C_11
#   elif (shz_c_version >=  SHZ_C_99)
    // C99
#   define SHZ_C_VERSION    SHZ_C_99
#   elif (shz_c_version >=  SHZ_C_95)
    // C95
#   define SHZ_C_VERSION    SHZ_C_95
#   endif//SHZ_C_VERSION
//----------------------------------------------------------------
#   undef shz_c_version
//----------------------------------------------------------------
#elif !defined(__cplusplus)
    // C89
#   define SHZ_C_VERSION    SHZ_C_89 
#endif//__STDC_VERSION__

//================================================================
// C++ Version
//================================================================
#ifdef __cplusplus
//----------------------------------------------------------------
#   if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
#   define shz_cxx_version  _MSVC_LANG
#   else
#   define shz_cxx_version  __cplusplus
#   endif//shz_cxx_version (temporary)
//----------------------------------------------------------------
#   if   shz_cxx_version >= SHZ_CXX_26
    // C++26 (experimental)
#   define SHZ_CXX_VERSION  SHZ_CXX_26
#   elif shz_cxx_version >= SHZ_CXX_23
    // C++23
#   define SHZ_CXX_VERSION  SHZ_CXX_23
#   elif shz_cxx_version >= SHZ_CXX_20
    // C++20
#   define SHZ_CXX_VERSION  SHZ_CXX_20
#   elif shz_cxx_version >= SHZ_CXX_17
    // C++17
#   define SHZ_CXX_VERSION  SHZ_CXX_17
#   elif shz_cxx_version >= SHZ_CXX_14
    // C++14
#   define SHZ_CXX_VERSION  SHZ_CXX_14
#   elif shz_cxx_version >= SHZ_CXX_11
    // C++11
#   define SHZ_CXX_VERSION  SHZ_CXX_11
#   elif shz_cxx_version >= SHZ_CXX_98
    // C++98
#   define SHZ_CXX_VERSION  SHZ_CXX_98
#   endif//SHZ_CXX_VERSION
//----------------------------------------------------------------
#   undef  shz_cxx_version
//----------------------------------------------------------------
#endif//__cplusplus

//================================================================
// Architectures { 0 = NOT_TARGET, 32/64/NNN = IS_TARGET Architecture}
//================================================================

#if defined(__DREAMCAST__) || defined(__SH4__)
// 32-bit SH4 architecture
#define SHZ_ARCH_SH4            32
#endif//SH4/SuperH 4

#if defined(_M_IX86) || defined(__i386__)
// 32-bit x86 architecture
#define SHZ_ARCH_X86            32
#endif//x86

#if defined(_M_X64) || defined(__x86_64__)
// 64-bit x86_64 architecture
#define SHZ_ARCH_X64            64
#endif//x86_64/amd64

#if defined(_M_ARM) || defined(__arm__)
// 32-bit ARM architecture
#define SHZ_ARCH_ARM            32
#endif//arm (armv7-a)

#if defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__)
// 64-bit ARM64 architecture
#define SHZ_ARCH_ARM64          64
#endif//arm64/aarch64

#if defined(__riscv__) && (defined(__riscv_lp64) || defined(__riscv_lp64d))
// 64-bit RISC-V 64 (RV64) architecture
#define SHZ_ARCH_RISCV64        64
#endif//riscv64/rv64

//TODO: Add in MIPS target architectures

//----------------------------------------------------------------
// #ifndef, then it is NOT the TARGET Architecture
//----------------------------------------------------------------

#ifndef SHZ_ARCH_SH4
#define SHZ_ARCH_SH4            0
#endif//SHZ_ARCH_SH4

#ifndef SHZ_ARCH_X86
#define SHZ_ARCH_X86            0
#endif//SHZ_ARCH_X86

#ifndef SHZ_ARCH_X64
#define SHZ_ARCH_X64            0
#endif//SHZ_ARCH_X64

#ifndef SHZ_ARCH_ARM
#define SHZ_ARCH_ARM            0
#endif//SHZ_ARCH_ARM

#ifndef SHZ_ARCH_ARM64
#define SHZ_ARCH_ARM64          0
#endif//SHZ_ARCH_ARM64

#ifndef SHZ_ARCH_RISCV64
#define SHZ_ARCH_RISCV64        0
#endif//SHZ_ARCH_RISCV64

#ifndef SHZ_ARCH_MIPS
#define SHZ_ARCH_MIPS           0
#endif//SHZ_ARCH_MIPS

//================================================================
// Utilities
//================================================================

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
#if defined(_MSC_VER)

//! Forces a function or type to be aligned by \p N bytes.
#define SHZ_ALIGNAS(N)              __declspec(align(N))

//! (UNSUPPORTED) Tells MSVC that a scalar type is to be treated as a vector of size \p N.
#define SHZ_SIMD(N)                 //NOTE: No MSVC equivalent available.

//! (UNSUPPORTED) Tells MSVC that a particular function should be optimized for performance.
#define SHZ_HOT                     //NOTE: No MSVC equivalent, only MSVC equivalent is a pragma region.

//! Aligns a function by the size of an icache line (32 bytes).
#define SHZ_ICACHE_ALIGNED          __declspec(align(32))

//! Forces MSVC to inline the given function.
#define SHZ_FORCE_INLINE            __forceinline SHZ_INLINE

//! Prevents MSVC from inlining the given function.
#define SHZ_NO_INLINE               __declspec(noinline)

//! (UNSUPPORTED) Forces MSVC to inline all calls to other functions made within the tagged function.
#define SHZ_FLATTEN                 //NOTE: No MSVC equivalent available.

//! Tells MSVC not to introduce any extra padding for the given type.
#define SHZ_PACKED                  __pragma(pack(1))

//! Tells MSVC the function has no effects other than returning a value that depends on its arguments and global variables.
#define SHZ_PURE                    __declspec(noalias)

//! (UNSUPPORTED) Tells MSVC that the decorated pointer may be breaking strict aliasing rules for C and C++
#define SHZ_ALIASING                //NOTE: No MSVC equivalent available.

#if SHZ_CXX_VERSION >= SHZ_CXX_20
//! Tells MSVC that the expression is likely to be true (used for conditional and loop optimizations)
#define SHZ_LIKELY(e)               (e) [[likely]]

//! Tells MSVC that the expression is likely to be false (used for conditional and loop optimizations)
#define SHZ_UNLIKELY(e)             (e) [[unlikely]]
#else
//! (UNSUPPORTED) Tells MSVC that the expression is likely to be true (used for conditional and loop optimizations)
#define SHZ_LIKELY(e)               (e)

//! (UNSUPPORTED) Tells MSVC that the expression is likely to be false (used for conditional and loop optimizations)
#define SHZ_UNLIKELY(e)             (e)
#endif// >=C++20 else (UNSUPPORTED)

#if SHZ_ARCH_ARM || SHZ_ARCH_ARM64
//! Tells MSVC to use its builtin intrinsic for prefetching (better instruction scheduling than pure ASM pref)
#define SHZ_PREFETCH(a)             __prefetch(a, _MM_HINT_T0)

//! Tells MSVC to issue a prefetch, using _ReadWriteBarrier() so that it cannot be reordered
#define SHZ_PREFETCH_VOLATILE(a)    do { \
                                        _ReadWriteBarrier(); \
                                        __prefetch(a, _MM_HINT_T0); \
                                        _ReadWriteBarrier(); \
                                    } while(0)
#else
//! Tells MSVC to use its builtin intrinsic for prefetching (better instruction scheduling than pure ASM pref)
#define SHZ_PREFETCH(a)             _mm_prefetch(a, _MM_HINT_T0)

//! Tells MSVC to issue a prefetch, using _ReadWriteBarrier() so that it cannot be reordered
#define SHZ_PREFETCH_VOLATILE(a)    do { \
                                        _ReadWriteBarrier(); \
                                        _mm_prefetch(a, _MM_HINT_T0); \
                                        _ReadWriteBarrier(); \
                                    } while(0)
#endif// arm/arm64 else x86/x64

//! Put this before a function definition to tell MSVC to use fast math optimizations on a specific function.
#define SHZ_FAST_MATH               __pragma(fp_contract(on)) \
                                    __pragma(fenv_access(off)) \
                                    __pragma(float_control(except, off)) \
                                    __pragma(float_control(precise, off))

//! Put this before a function definition to tell MSVC to NOT use fast math optimizations on a specific function.
#define SHZ_NO_FAST_MATH            __pragma(fp_contract(off)) \
                                    __pragma(fenv_access(on)) \
                                    __pragma(float_control(except, on)) \
                                    __pragma(float_control(precise, on))

//----------------------------------------------------------------
#else
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
#endif//MSVC else (GCC/CLANG)

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

#if defined(_MSC_VER)
    //! Tells MSVC the function has no effects other than returning a value that depends only on its arguments.
#   define SHZ_CONST            SHZ_PURE SHZ_FORCE_INLINE constexpr
    //! (SUPPORTED IN MSVC), define provided for C compatibility and macro non-/aliasing.
#   define SHZ_RESTRICT         __restrict
#else
    //! Tells GCC the function has no effects other than returning a value that depends only on its arguments.
#   define SHZ_CONST            __attribute__((const)) constexpr
    //! Dummy define provided for C compatibility
#   define SHZ_RESTRICT
#endif

    //! Tells the compiler that the function does not throw exceptions
#   define SHZ_NOEXCEPT         noexcept
#endif//C else C++
//! @}
//! \endcond

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

/*! \cond Forward Declarations */
struct SHZ_ALIGNAS(8) shz_mat2x2;
typedef struct shz_mat2x2 shz_mat2x2_t;

struct mat3x3;
typedef struct shz_mat3x3 shz_mat3x3_t;

struct shz_mat4x3;
typedef struct shz_mat4x3 shz_mat4x3_t;

struct shz_mat3x4;
typedef struct shz_mat3x4 shz_mat3x4_t;

struct SHZ_ALIGNAS(8) shz_mat4x4;
typedef struct shz_mat4x4 shz_mat4x4_t;
/*! \endcond */

//! \cond
#define SHZ_STRINGIFY_(a)       #a  // surround with double quotes (")
//! \endcond

//! \cond
#define SHZ_CHARIFY_(a)         #@a // surround with single quotes (')
//! \endcond

#endif // SHZ_CDEFS_H
