#ifndef SH4_CDEFS_H
#define SH4_CDEFS_H

#include <stdint.h>

#define SH4_VERSION_MAJOR       0
#define SH4_VERSION_MINOR       1
#define SH4_VERSION_PATCH       0

#define SH4_VERSION             ((uint32_t)((SH4_VERSION_MAJOR << 24) | (SH4_VERSION_MINOR << 8) | (SH4_VERSION_PATCH)))
#define SH4_VERSION_STR         GBL_STRINGIFY(SH4_VERSION_MAJOR) "." GBL_STRINGIFY(SH4_VERSION_MINOR) "." GBL_STRINGIFY(SH4_VERSION_PATCH)

#define SH4_STRINGIFY(a)        SH4_STRINGIFY_(a)
#define SH4_STRINGIFY_(a)       #a

#define SH4_COUNT_OF(array)    (sizeof(array) / sizeof((array)[0]))

#define SH4_CONTAINER_OF(ptr, type, member) \
    ((type*)((char*)(ptr) - offsetof(type, member)))

#define SH4_SWAP(a, b) do { \
        auto tmp = a; \
        a = b; \
        b = tmp; \
    } while(false)

#define SH4_ALIGNAS(N)          __attribute__((aligned((N))))
#define SH4_SIMD(N)             __attribute__((vector_size((N))))
#define SH4_HOT                 __attribute__((hot))
#define SH4_ICACHE_ALIGNED      __attribute__((aligned(32)))
#define SH4_FORCE_INLINE        __attribute__((always_inline)) inline static
#define SH4_FLATTEN             __attribute__((flatten))
#define SH4_PACKED              __attribute__((packed))

#ifdef __cplusplus
#   define SH4_BEGIN_DECLS      extern "C" {
#   define SH4_END_DECLS        }
#else
#   define SH4_BEGIN_DECLS
#   define SH4_END_DECLS
#endif

#endif // SH4_CDEFS_H
