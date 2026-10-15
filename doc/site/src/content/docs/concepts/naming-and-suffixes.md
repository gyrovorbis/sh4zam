---
title: Naming and Suffixes
description: How SH4ZAM names functions, types and C++ wrappers, and what every suffix (_fsrra, _safe, _deg, _unaligned, _wxyz, sq_) means
---

Every SH4ZAM name is built from the same few parts: a module prefix, a verb, and optional suffixes that change precision, safety, units, alignment or register layout. Once you know the parts, you can guess most names correctly.

## Functions, types, C++

| Kind | Pattern | Example |
| --- | --- | --- |
| C function | `shz_<module>_<verb>[_<qualifier>]` | `shz_vec3_cross()`, `shz_xmtrx_apply_rotation_x()` |
| Scalar/trig C function | `shz_<name>` (usually the `<math.h>` name) | `shz_floorf()`, `shz_sinf()` |
| C type | `shz_<name>_t` | `shz_vec3_t`, `shz_mat4x4_t`, `shz_quat_t` |
| C type, alternate | `shz_<name>` (same type, "for those who hate POSIX-style") | `shz_vec3`, `shz_mat4x4` |
| Macro / constant | `SHZ_<NAME>` | `SHZ_F_PI`, `SHZ_FLT_EPSILON` |
| Type-generic C macro | `shz_vec_<verb>` | `shz_vec_dot()` picks `shz_vec2_dot()`, `shz_vec3_dot()` or `shz_vec4_dot()` |
| C++ namespace | `shz::` | |
| C++ scalar/trig | `shz::<name>`, a `constexpr` alias of the C function | `shz::floorf` is `shz_floorf` |
| C++ types | `shz::<name>`, deriving from the C struct | `shz::vec3 : shz_vec3_t` |
| C++ XMTRX | `shz::xmtrx::<verb>`, static member functions | `shz::xmtrx::apply_rotation_x()` |

Module prefixes: `vec2`/`vec3`/`vec4` (vector), `quat`, `mat4x4`/`mat3x3`/... (matrix), `xmtrx`, complex functions are C99-style (`shz_caddf()`, `shz_cabsf()`), memory functions are libc-style (`shz_memcpy()`).

## Suffixes

### `_fsrra`

Uses the SH4 `FSRRA` instruction, a single-instruction approximation of 1/sqrt(x). Faster, less precise, and the argument or denominator must be **positive**.

- `shz_inv_sqrtf_fsrra(x)`: fast 1/sqrt(x).
- `shz_sqrtf_fsrra(x)`: fast square root.
- `shz_invf_fsrra(x)`: fast 1/x. It is computed as 1/sqrt(x*x), so it returns 1/|x|: a negative input comes back with the wrong sign. `shz_invf()` is the variant that safely handles negative values.
- `shz_divf_fsrra(num, denom)`: fast division; requires `denom` to be positive. `shz_divf()` otherwise.
- `shz_normalizef_fsrra()`, `shz_remapf_fsrra()`, `shz_wrapf_fsrra()`: range mapping, valid when the range difference is positive.

The classic use is perspective division, where W is known to be positive.

### `_safe`

Guards against degenerate input at a small cost. `shz_vec3_normalize_safe()` returns a zero vector when the magnitude is not greater than zero, where `shz_vec3_normalize()` would divide by zero. Also `shz_smoothstepf_safe()`, `shz_quat_normalize_safe()`, `shz_vec3_project_safe()` and others. (`shz_xmtrx_init_identity_safe()` is deprecated: the plain `shz_xmtrx_init_identity()` is always safe now.)

### `_deg`

Takes the angle in degrees. Everything else is radians. `shz_sinf_deg()`, `shz_sincosf_deg()`, `shz_vec2_from_angle_deg()`. `SHZ_DEG_TO_RAD()` and `SHZ_RAD_TO_DEG()` convert.

### `u16`

Takes the angle as a 16-bit fixed-point value where 0..65535 covers a full turn, which is the native input format of `FSCA`. `shz_sincosu16()`.

### `_unaligned`

Works on matrix data that is not 8-byte aligned, usually taken as a plain `float[16]` array. `shz_mat4x4_t` and `shz_mat2x2_t` must otherwise be 8-byte aligned because the SH4 routines load and store register pairs with 64-bit `FMOV`. `shz_xmtrx_load_unaligned_4x4(const float matrix[16])`, `shz_xmtrx_store_unaligned_4x4(float matrix[16])`, `shz_mat4x4_copy_unaligned()`.

### `_transpose`

Works with the transpose of the matrix: loading a row-major matrix, or applying the inverse of a pure rotation. `shz_xmtrx_load_transpose_4x4()`, `shz_mat4x4_transform_vec3_transpose()`.

### `_reverse`

Reverses the multiplication order (pre-multiply instead of post-multiply). `shz_xmtrx_apply_reverse_4x4()`, `shz_xmtrx_translate_reverse()`.

### `_wxyz`, `_yzwx`, `_wzyx`

Register or column permutations. `FTRV` produces X, Y, Z, W on cycles 4, 5, 6, 7; a permutation puts W first so perspective division can start three cycles earlier. `shz_xmtrx_load_wxyz_4x4()`, `shz_xmtrx_init_permutation_wxyz()`. See [XMTRX](/concepts/xmtrx/#getting-w-first).

### `_xmtrx`

Memory routines with this suffix use the XMTRX register bank as a 64-byte scratch buffer, so they **clobber XMTRX**. `shz_sq_memcpy32_xmtrx()`, `shz_memswap32_1_xmtrx()`.

### `sq_`

Writes through the SH4 store queues: burst writes of 32 bytes at a time, used for submitting to the PVR or copying to VRAM. `shz_sq_memcpy32()`, `shz_sq_memcpy32_1()`.

### Numbers on memory routines

The number is the granularity or count. `shz_memcpy4()` copies 4-byte units, `shz_memcpy32()` 32-byte chunks, `shz_memcpy2_16()` sixteen 2-byte units, and the `_1` routines copy exactly one 32-byte line (`shz_memcpy32_1()`).

### `dot2`, `dot3`

Compute 2 or 3 dot products of one left-hand vector against several right-hand vectors in one pipelined `FIPR` sequence, returning a `shz_vec2_t` or `shz_vec3_t`. `shz_vec3_dot2()`, `shz_vec4_dot3()`, `shz_quat_dot3()`.

## Verbs on matrices and XMTRX

| Verb | Meaning |
| --- | --- |
| `init_*` | Initialize the whole matrix to the transform; other components become identity. |
| `set_*` | Set only the components the transform touches; leave the rest alone. |
| `apply_*` | Apply the transform to the affected components based on their current values (add for translation, multiply the submatrix for scale and rotation). |
| `translate`, `scale`, `rotate_*` | GL-style: full multiply by a matrix initialized to the transform. The most general form. |
| `load_*` / `store_*` | Memory to XMTRX and back. `load_apply_*` and `load_apply_store_*` fuse them. |
| `transform_vec*` / `transform_point*` | Transform vectors; for 3D, `point` implies w = 1 and `vec` implies w = 0. |
| `get_*`, `read*`, `write*`, `swap_*` | Element and row/column access. |

Worked example in [Matrix Transforms](/guides/matrix-transforms/#the-verb-model).

## Related

- [Conventions](/concepts/conventions/)
- [C and C++ APIs](/concepts/c-and-cpp/)
- [Cheatsheet](/cheatsheet/)
- [For Agents](/for-agents/)
