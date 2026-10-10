---
title: C and C++ APIs
description: How SH4ZAM's C and C++ APIs map onto each other - aliases, CRTP vector types, static XMTRX members, type-generic C macros, and mixing both
---

SH4ZAM ships two interoperable APIs over the same implementation: a C17 API (with extras for C23) and a C++20 API (with extras for C++23). They are layout-compatible and freely mixable; C++ can still use the C API by design, and every C++ type is also compatible with its corresponding C type. The tables below map one onto the other.

## The mapping at a glance

| C | C++ | How |
| --- | --- | --- |
| `shz_floorf(x)` | `shz::floorf(x)` | `constexpr auto floorf = shz_floorf;` alias |
| `shz_sincosf(a)` returning `shz_sincos_t` | `shz::sincosf(a)` or `shz::sincos::from_radians(a)` | alias, plus the `shz::sincos` type with `sinf()`, `cosf()`, `tanf()` and friends |
| `shz_vec3_t` | `shz::vec3` | `struct vec3 : vecN<vec3, shz_vec3_t, 3>`, which derives from `shz_vec3_t` |
| `shz_vec3_cross(a, b)` | `a.cross(b)` | member function wrapping the C call |
| `shz_vec3_add(a, b)` | `a + b` | operators |
| `shz_vec3_normalize(v)` | `v.direction()` or `v.normalized()`; `v.normalize()` in place | members |
| `shz_quat_slerp(q, p, t)` | `shz::quat::slerp(q, p, t)` | static member |
| `shz_mat4x4_init_identity(&m)` | `m.init_identity()` | member |
| `shz_xmtrx_apply_rotation_x(a)` | `shz::xmtrx::apply_rotation_x(a)` | `struct xmtrx` with static members only |
| `shz_xmtrx_store_4x4(&m)` | `shz::xmtrx::store(&m)` | overloads replace the size suffixes |
| `shz_vec4_from(x)` / `shz_vec4_to(T, v)` | `shz::vec4::from(x)` / `v.to<T>()` | |
| `SHZ_F_PI` | `shz::pi_f` | `constexpr float` |

Every C function page in the [API reference](/api/) has a "C++ equivalent" line, and every [C++ class page](/api/cpp/) lists, per member, the C function it calls. [`/api/index.json`](/api/index.json) has the same mapping in the `cpp` field of each symbol.

## Free-function aliases

Scalar, trig, complex and memory routines are `constexpr` function-pointer aliases in `namespace shz`: same name minus the `shz_` prefix, same signature, no wrapper. The full list is on the [C++ API](/api/cpp/) page.

## Vector types

`shz::vec2`, `shz::vec3` and `shz::vec4` share a CRTP base, `shz::vecN<CRTP, C, R>`, that provides the common surface once: `from()`, `to<T>()`, `deref()`, `swizzle<I...>()`, `abs`, `neg`, `inv`, `min`/`max`/`minv`/`maxv`, `clamp`, `floor`/`ceil`/`round`/`fract`/`sign`/`saturate`, `dot`, `magnitude` and friends, `direction` / `normalize`, `distance`, `lerp`, `step`, `smoothstep`, `reflect`, `refract`, `project`, and the arithmetic operators. Each derived type adds what only it has, e.g. `vec3::cross()`, `vec3::rotate_x()`, `vec2::from_angle()`.

The base calls the C type-generic macros (`shz_vec_dot()` and so on), which resolve to the per-dimension C function, so `shz::vec3::dot` ends up in `shz_vec3_dot()`.

With C++23, the vector types also get a deducing-this `operator[]`, iterators (`begin()`/`end()`) and a `<=>` comparison.

## XMTRX

`shz::xmtrx` is a struct with only static member functions, because XMTRX is a single piece of global (per-thread) register state. Names drop the `shz_xmtrx_` prefix, and overloads replace the size suffixes: `shz::xmtrx::load()` accepts a `shz_mat4x4_t`, a `float[16]`, a `std::array<float, 16>`, a `shz_mat3x4_t` and more.

## Type-generic C

C has no overloading, so `shz_vector.h` provides 48 `_Generic` macros named `shz_vec_<verb>` that forward to the proper routine based on argument type:

```c
#include <sh4zam/shz_vector.h>

float len3(shz_vec3_t v) { return shz_vec_magnitude(v); }   // -> shz_vec3_magnitude()
float len4(shz_vec4_t v) { return shz_vec_magnitude(v); }   // -> shz_vec4_magnitude()
```

When the same header is compiled as C++, those names become overloaded inline functions instead, so `shz_vec_dot()` works in both languages.

## Mixing

Because the C++ types derive from the C structs, you can pass them straight to C functions, and wrap C results back up:

```cpp
#include <sh4zam/shz_sh4zam.hpp>

shz::vec3 bounce(const shz::vec3& v, const shz::vec3& n) {
    shz_vec3_t r = shz_vec3_reflect(v, n);   // C call on C++ objects
    return shz::vec3(r);                     // back to the C++ type
}
```

## Language levels

- C: the library builds as C11; the API targets C17, with extra API features supported for C23.
- C++: C++20 minimum, with extra API features (the ones above) for C++23.

## Related

- [C++ API](/api/cpp/)
- [Naming and Suffixes](/concepts/naming-and-suffixes/)
- [Types and Memory Layout](/concepts/types-and-layout/)
- [Interop with Existing APIs](/guides/interop/)
