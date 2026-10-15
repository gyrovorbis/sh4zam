---
title: Conventions
description: Coordinate systems, column-major matrices, W-first quaternions, radians, single precision and epsilon in SH4ZAM
---

The following conventions are used throughout the SH4ZAM API by default, unless a function's own page says otherwise. They match GL, so code ported from GL-style math libraries such as CGLM or GLM usually keeps its meaning.

## Summary

| Convention | Description |
| --- | --- |
| Coordinates | Right-handed for world/view space, left-handed for screen/clip space, as with GL. |
| Matrices | Stored in column-major order, as with GL. `mat.col[3]` (also `mat.pos`) is the translation. |
| Quaternions | Stored in `<W, X, Y, Z>` order. W comes first. Identity is (1, 0, 0, 0). |
| Angles | Radians. `_deg` variants take degrees; `u16` variants take a 16-bit fixed-point angle (65536 = one full turn). |
| Precision | Single-precision `float` everywhere. No `double` API. |
| Epsilon | `SHZ_FLT_EPSILON` is `0.01f`, deliberately loose because `FSCA`, `FSRRA` and `FIPR` are approximations. |

## Matrices are column-major

`shz_mat4x4_t` stores 16 floats column by column. The same storage is visible as `elem[16]`, `elem2D[4][4]` (indexed `[column][row]`), four `shz_vec4_t` columns `col[4]`, or the named columns `left`, `up`, `forward`, `pos`:

```c
#include <sh4zam/shz_matrix.h>

shz_vec3_t position_of(const shz_mat4x4_t* m) {
    return m->pos.xyz;           // same as m->col[3].xyz
}
```

`shz_mat4x4_t` must be 8-byte aligned. Use the `_unaligned` routines for matrices that might not be.

## Quaternions store W first

```c
#include <sh4zam/shz_quat.h>

shz_quat_t q = shz_quat_identity();   // w = 1, x = y = z = 0
float w = q.w;                        // also q.e[0]
shz_vec3_t axis = q.axis;             // x, y, z as a shz_vec3_t
```

Watch this when converting from libraries that store `<X, Y, Z, W>` (GLM's `glm::quat` storage, many engines): a straight bit-cast reorders the components. Build the SH4ZAM quaternion with `shz_quat_init(w, x, y, z)` instead.

## Radians, degrees, fixed point

`shz_sinf(x)` takes radians, `shz_sinf_deg(x)` takes degrees, `shz_sincosu16(a)` takes the raw 16-bit `FSCA` angle. `SHZ_DEG_TO_RAD()` and `SHZ_RAD_TO_DEG()` convert; `SHZ_FSCA_RAD_FACTOR` (10430.37835f, which is 65536/2π) and `SHZ_FSCA_DEG_FACTOR` (182.04444443f) scale to the fixed-point format.

## Single precision only

Every API takes and returns `float`. Use the `f` suffix on literals (`1.0f`, not `1.0`), since a `double` near SH4ZAM's inline assembly can leave the FPU in the wrong precision mode, and `float`-with-`double` arithmetic is slow on the SH4. Details in [Optimization Tips](/guides/optimization/#fp-precision-modes).

## Comparing floats

`shz_equalf()` checks for equality based on either the absolute or the relative tolerance, using `SHZ_FLT_EPSILON`; `shz_equalf_abs()` and `shz_equalf_rel()` apply just one of the two. Vector, quaternion and matrix `equal` routines follow the same idea. In C++, `shz::fipr_max_error` is the tolerance constant for results that went through `FIPR`; upstream marks its current value (`0.1f`) as not accurate yet.

## Related

- [Types and Memory Layout](/concepts/types-and-layout/)
- [Naming and Suffixes](/concepts/naming-and-suffixes/)
- [Trigonometry API](/api/trig/)
- [Quaternion API](/api/quat/)
