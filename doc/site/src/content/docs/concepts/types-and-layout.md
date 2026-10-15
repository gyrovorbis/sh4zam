---
title: Types and Memory Layout
description: Memory layout of SH4ZAM vector, quaternion, matrix, sincos and complex types, alignment rules, and how the C++ types relate
---

SH4ZAM's interop story rests on bit-compatibility: every type is a plain struct of `float`s with a documented layout, the C++ types derive from the C structs without adding data, and conversion to and from your own types is a zero-cost reinterpretation.

## Vectors

```text
shz_vec2_t   8 bytes    [ x | y ]
shz_vec3_t  12 bytes    [ x | y | z ]
shz_vec4_t  16 bytes    [ x | y | z | w ]
```

Each is a union, so the same storage is reachable several ways:

| Type | Members |
| --- | --- |
| `shz_vec2_t` | `x`, `y`, `e[2]` |
| `shz_vec3_t` | `x`, `y`, `z`, `e[3]`, `xy` (a `shz_vec2_t`) |
| `shz_vec4_t` | `x`, `y`, `z`, `w`, `e` (indexable like `e[4]`), `xyz` (a `shz_vec3_t`), `xy` and `zw` (two `shz_vec2_t`) |

`shz_vec4_t`'s `e` is declared `float e SHZ_SIMD(16)`: a 16-byte GCC vector (`vector_size`) on GCC-compatible compilers, a plain `float[4]` elsewhere. The exact definitions are on the [`shz_vec2_t`](/api/vector/shz_vec2_t/), [`shz_vec3_t`](/api/vector/shz_vec3_t/) and [`shz_vec4_t`](/api/vector/shz_vec4_t/) pages.

## Quaternion

```text
shz_quat_t  16 bytes    [ w | x | y | z ]      W first
```

Members: `w`, `x`, `y`, `z`, `axis` (a `shz_vec3_t` over x, y, z), `e[4]`. The W-first order is the one to watch when converting from libraries that store X, Y, Z, W.

## Matrices

All matrices are column-major. Columns are vectors:

```text
shz_mat4x4_t  64 bytes, 8-byte aligned
  col[0] = left     [ x | y | z | w ]
  col[1] = up       [ x | y | z | w ]
  col[2] = forward  [ x | y | z | w ]
  col[3] = pos      [ x | y | z | w ]   translation lives here
  elem[16], elem2D[4][4]  indexed [column][row]
```

| Type | Size | Alignment | Columns | Named columns |
| --- | --- | --- | --- | --- |
| `shz_mat4x4_t` | 64 | 8 | `shz_vec4_t col[4]` | `left`, `up`, `forward`, `pos` |
| `shz_mat4x3_t` | 48 | natural | `shz_vec4_t col[3]` | `left`, `up`, `forward` |
| `shz_mat3x4_t` | 48 | natural | `shz_vec3_t col[4]` | `left`, `up`, `forward`, `pos` |
| `shz_mat3x3_t` | 36 | natural | `shz_vec3_t col[3]` | `left`, `up`, `forward` |
| `shz_mat2x2_t` | 16 | 8 | `shz_vec2_t col[2]` | |

`shz_mat4x4_t` and `shz_mat2x2_t` are declared `SHZ_ALIGNAS(8)` and must stay 8-byte aligned, because the SH4 routines move them with 64-bit paired `FMOV`. If a matrix comes from somewhere that doesn't guarantee that (a packed file format, another library's struct), use the `_unaligned` routines such as `shz_xmtrx_load_unaligned_4x4()`.

The 2x2, 3x3, 3x4 and 4x3 types sit in a block upstream marks as undocumented "until API is complete"; they are usable, but expect them to change.

## Small types

```text
shz_sincos_t   8 bytes   [ sin | cos ]         one FSCA result
shz_complex_t  8 bytes   [ real | imag ]
```

`shz_sincos_t` is what `shz_sincosf()` returns: the SH4 computes both values in one `FSCA`, so when you need both, grab them as a pair. `shz_complex_t` is built with `SHZ_CMPLXF(x, y)`; `SHZ_I` is the unit imaginary.

## Alternate names

Every struct has two C names: `shz_vec3_t` and `shz_vec3` (the "for those who hate POSIX-style" typedef). They are the same type.

## C++ types

`shz::vec2`, `shz::vec3`, `shz::vec4`, `shz::quat`, `shz::mat4x4`, `shz::sincos` and `shz::complex` derive from the matching C struct and add only methods and operators, so a `shz::vec3*` is usable anywhere a `shz_vec3_t*` is expected and the C API accepts C++ objects directly. The vector types share a CRTP base, [`shz::vecN`](/api/cpp/vecn/), which provides `from()`, `to<T>()`, `deref()`, `swizzle<...>()` and the common math.

## Cache lines

The SH4 has 32-byte cache lines. A `shz_mat4x4_t` is exactly two of them, which is why Bruce's Balls calls `shz_dcache_alloc_line()` on each half before storing a freshly built matrix into it. See [Examples](/guides/examples/#cache-line-sized-data).

## Related

- [Interop with Existing APIs](/guides/interop/)
- [Conventions](/concepts/conventions/)
- [C and C++ APIs](/concepts/c-and-cpp/)
- [Vector API](/api/vector/), [Matrix API](/api/matrix/)
