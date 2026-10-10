---
title: Interop with Existing APIs
description: Accelerate an existing C or C++ math API with SH4ZAM using zero-cost conversions, without changing client code
---

One of the most common and important use-cases of SH4ZAM is for it to be pulled into an existing codebase, with its own vector math types and abstractions, with the goal of using SH4ZAM to accelerate the back-end implementation without breaking the API and needing to change client code. SH4ZAM aims to make such interop noninvasive and zero-overhead, and it rigorously validates that any conversion or adapting of its types to existing types gets cleanly optimized away.

## The three steps

SH4ZAMification of an existing codebase usually involves taking the following steps in order to leverage the gainz it has to offer without breaking an existing codebase or math interface:

1. Convert incoming arguments from existing math types to SH4ZAM's math types.
2. Forward arguments to the equivalent accelerated SH4ZAM routine.
3. Convert the return value from SH4ZAM's math types back to the existing math types.

The conversions only require that your type is layout-compatible with the SH4ZAM type: same size, same order of `float` components.

## Adapting C codebases

The following adapts SH4ZAM to accelerate the math behind an existing, idiomatic C API with support for multiple targets, such as CGLM or raymath:

```c
// Only introduce SH4ZAM for supported targets within cross-platform codebases.
#ifdef GAINZ
     // Include all of SH4ZAM, using the .h header for the C APIs.
#    include <sh4zam/shz_sh4zam.h>
#endif

// The existing 4D vector structure used within the codebase.
typedef struct {
    // Type-compatible layout with SH4ZAM's shz_vec4_t.
    float value[4];
} Vec4;

// 4D vector addition operation provided by the existing math API.
Vec4 AddVec4(Vec4 lhs, Vec4 rhs) {
#ifdef GAINZ
    /* 1) Convert incoming arguments to equivalent SH4ZAM types using shz_vec4_from().
       2) Call SH4ZAM's equivalent routine, shz_vec4_add(), with the converted arguments.
       3) Convert SH4ZAM's return value back to existing API type with shz_vec4_to(). */
    return shz_vec4_to(Vec4, shz_vec4_add(shz_vec4_from(lhs), shz_vec4_from(rhs)));
#else
    // The original, unaccelerated path is here for platforms without SH4ZAM.
    return (Vec4){{ lhs.value[0] + rhs.value[0], lhs.value[1] + rhs.value[1],
                    lhs.value[2] + rhs.value[2], lhs.value[3] + rhs.value[3] }};
#endif
}
```

## Adapting C++ codebases

The following adapts SH4ZAM to accelerate the math behind an existing, idiomatic C++ API with support for multiple targets, such as GLM or the Simulant engine:

```cpp
// Only introduce SH4ZAM for supported targets within cross-platform codebases.
#ifdef GAINZ
     // Include all of SH4ZAM, using the .hpp header for the C++ APIs.
#    include <sh4zam/shz_sh4zam.hpp>
#endif

namespace Math {
    // The existing 4D vector structure used within the codebase.
    struct Vec4 {
        // Type-compatible layout with SH4ZAM's shz::vec4.
        float value[4];
        // Overloaded operator providing 4D vector addition operation.
        friend Vec4 operator+(Vec4 lhs, Vec4 rhs);
    };
}

namespace Math {
    // Implementation of 4D vector addition operator.
    Vec4 operator+(Vec4 lhs, Vec4 rhs) {
    #ifdef GAINZ
        /* 1) Convert incoming arguments to SH4ZAM types using shz::vec4::from().
           2) Call SH4ZAM's equivalent addition operator with the converted arguments.
           3) Convert SH4ZAM's return value back with shz::vec4::to<>(). */
        return (shz::vec4::from(lhs) + shz::vec4::from(rhs)).to<Vec4>();
    #else
        // The original, unaccelerated path is here for platforms without SH4ZAM.
        return Vec4({ lhs.value[0] + rhs.value[0], lhs.value[1] + rhs.value[1],
                      lhs.value[2] + rhs.value[2], lhs.value[3] + rhs.value[3] });
    #endif
    }
}
```

Both samples are upstream's, from `doc/guide.dox`. One fix: upstream defines the C++ operator as `Vec4::operator+`, but it is declared as a `friend` (a free function), so here the definition goes inside `namespace Math` without the `Vec4::` qualifier, which is what makes it compile.

## The conversion toolbox

| Need | C | C++ |
| --- | --- | --- |
| Your value to a SH4ZAM value | `shz_vec2_from()`, `shz_vec3_from()`, `shz_vec4_from()` | `shz::vec2::from()`, `shz::vec3::from()`, `shz::vec4::from()` |
| SH4ZAM value back to your type | `shz_vec2_to(Type, v)`, `shz_vec3_to()`, `shz_vec4_to()` | `v.to<Type>()` |
| A pointer to N floats viewed as a SH4ZAM vector | `shz_vec2_deref(ptr)`, `shz_vec3_deref()`, `shz_vec4_deref()` | the same macros, or the `deref()` member of [`shz::vecN`](/api/cpp/vecn/#deref) |
| Any same-size bit-cast | `SHZ_CONVERT(type, value)` | `SHZ_CONVERT(type, value)` |

`SHZ_CONVERT()` is the zero-overhead conversion macro the adapters are built on. In C it goes through a union; in C++ through a `reinterpret_cast` inside a lambda, with a `static_assert` that both types have the same size. It also handles pointers and references. The `_deref` macros cast through `SHZ_ALIASING` pointers so they don't trip strict aliasing.

For matrices, `shz_mat4x4_t` is column-major and must be 8-byte aligned; if your matrix type is not, use the `_unaligned` load/store routines, which take a plain `float[16]` (`shz_xmtrx_load_unaligned_4x4((const float*)&m)`, `shz_xmtrx_store_unaligned_4x4((float*)&m)`). See [Types and Memory Layout](/concepts/types-and-layout/).

## Related

- [Types and Memory Layout](/concepts/types-and-layout/)
- [C and C++ APIs](/concepts/c-and-cpp/)
- [Matrix Transforms](/guides/matrix-transforms/)
- [`SHZ_CONVERT`](/api/cdefs/SHZ_CONVERT/)
