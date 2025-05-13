#ifndef SHZ_CDEFS_H
#define SHZ_CDEFS_H

#include <stdint.h>

#define SHZ_VERSION_MAJOR       0
#define SHZ_VERSION_MINOR       1
#define SHZ_VERSION_PATCH       0

#define SHZ_VERSION             ((uint32_t)((SHZ_VERSION_MAJOR << 24) | (SHZ_VERSION_MINOR << 8) | (SHZ_VERSION_PATCH)))
#define SHZ_VERSION_STR         SHZ_STRINGIFY(SHZ_VERSION_MAJOR) "." SHZ_STRINGIFY(SHZ_VERSION_MINOR) "." SHZ_STRINGIFY(SHZ_VERSION_PATCH)

#define SHZ_STRINGIFY(a)        SHZ_STRINGIFY_(a)
#define SHZ_STRINGIFY_(a)       #a

#define SHZ_COUNT_OF(array)    (sizeof(array) / sizeof((array)[0]))

#define SHZ_CONTAINER_OF(ptr, type, member) \
    ((type*)((char*)(ptr) - offsetof(type, member)))

#define SHZ_SWAP(a, b) do { \
        auto tmp = a; \
        a = b; \
        b = tmp; \
    } while(false)

#define SHZ_ALIGNAS(N)          __attribute__((aligned((N))))
#define SHZ_SIMD(N)             __attribute__((vector_size((N))))
#define SHZ_HOT                 __attribute__((hot))
#define SHZ_ICACHE_ALIGNED      __attribute__((aligned(32)))
#define SHZ_FORCE_INLINE        __attribute__((always_inline)) inline static
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

#endif // SHZ_CDEFS_H
