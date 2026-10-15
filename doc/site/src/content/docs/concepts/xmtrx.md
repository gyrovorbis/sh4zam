---
title: XMTRX
description: The SH4's register-resident 4x4 active matrix, how SH4ZAM's XMTRX API uses it, FTRV timing, the WXYZ trick and thread safety
---

`XMTRX` is the name given to the 16 FP registers contained within the back-bank of the SH4's FPU. Together they represent the SH4's 4x4 "active matrix", which vectors can be transformed against with a single `FTRV` instruction. For maximum FP performance on the SH4, strategic usage of XMTRX to batch transform operations together, without having to reload FP registers, is key. SH4ZAM's [XMTRX API](/api/xmtrx/) is a set of functions that operate on this global register state directly.

## The register banks

```text
SH4 FPU, single-precision mode
+---------------------------------------------+
| front bank  FR0 .. FR15                     |  ordinary float math,
|             fv0  fv4  fv8  fv12             |  four 4-vectors for FIPR/FTRV
+---------------------------------------------+
| back bank   XF0 .. XF15   = XMTRX           |  the 4x4 active matrix
|             col 0: XF0  XF1  XF2  XF3       |
|             col 1: XF4  XF5  XF6  XF7       |
|             col 2: XF8  XF9  XF10 XF11      |
|             col 3: XF12 XF13 XF14 XF15      |
+---------------------------------------------+
FRCHG swaps the banks.  FTRV XMTRX, fvN  computes  fvN = XMTRX * fvN.
```

Because the matrix lives in registers, a sequence like "load, apply rotation, apply scale, set translation, transform 500 vertices" never touches memory for the matrix. Every function in `shz_xmtrx.h` reads or writes this state; the only ones taking a matrix argument are the `load_*`, `store_*`, `apply_*` (2x2, 3x3, 3x4, 4x4) and fused `load_apply_*` families that move data in and out. The individual registers are named by the `shz_xmtrx_reg` enum (`SHZ_XMTRX_XF0` .. `SHZ_XMTRX_XF15`) for `shz_xmtrx_read()` and `shz_xmtrx_write()`.

## When to use XMTRX vs the Matrix API

Typically, for one-off operations, the in-memory [Matrix API](/api/matrix/) is what should be used. When operations can be batched, using XMTRX to hold your matrix within registers gives the best performance. Examples of such scenarios:

- Applying multiple operations to a source matrix before storing the result.
- Transforming batches of vectors against a single matrix.

Some `shz_mat4x4_*` routines load into XMTRX to do their work and so clobber whatever was there. Their pages carry the warning "This routine clobbers XMTRX." So do the memory routines ending in `_xmtrx`.

## A typical sequence

```c
#include <sh4zam/shz_sh4zam.h>

void draw_mesh(const shz_mat4x4_t* view_proj, shz_vec3_t pos, float yaw,
               const shz_vec3_t* verts, shz_vec4_t* out, unsigned count) {
    shz_xmtrx_load_4x4(view_proj);              // memory -> XMTRX
    shz_xmtrx_translate(pos.x, pos.y, pos.z);   // GL-style multiply, in registers
    shz_xmtrx_apply_rotation_y(yaw);            // only touches the rotation submatrix

    for (unsigned i = 0; i < count; ++i)        // one FTRV per vertex
        out[i] = shz_xmtrx_transform_vec4(shz_vec3_vec4(verts[i], 1.0f));
}
```

The same in C++, where `shz::xmtrx` has only static members:

```cpp
#include <sh4zam/shz_sh4zam.hpp>

void draw_mesh(const shz::mat4x4& view_proj, shz::vec3 pos, float yaw,
               const shz::vec3* verts, shz::vec4* out, unsigned count) {
    shz::xmtrx::load(view_proj);
    shz::xmtrx::translate(pos.x, pos.y, pos.z);
    shz::xmtrx::apply_rotation_y(yaw);

    for (unsigned i = 0; i < count; ++i)
        out[i] = shz::xmtrx::transform(shz::vec4(verts[i], 1.0f));
}
```

For building a model matrix, see the init / apply / set / store pattern in [Matrix Transforms](/guides/matrix-transforms/).

## FTRV timing

`FTRV` produces each component of the result one cycle after another:

| Component | Ready on cycle |
| --- | --- |
| X | 4 |
| Y | 5 |
| Z | 6 |
| W | 7 |

## Getting W first

Transform-and-lighting needs W first, to divide the other components by it, but W comes out last. Two ways around it:

1. `shz_xmtrx_load_wxyz_4x4()` loads a 4x4 matrix with the W column coming first, allowing you to use its result on cycle 4 rather than waiting until cycle 7.
2. Build the projection on top of a permutation matrix, as Bruce's Balls does, then swizzle each result back:

```c
#include <sh4zam/shz_sh4zam.h>

void setup_projection(shz_mat4x4_t* out, float fov, float aspect, float near_z) {
    shz_xmtrx_init_permutation_wxyz();
    shz_xmtrx_apply_screen(640.0f, 480.0f);
    shz_xmtrx_apply_perspective(fov, aspect, near_z);
    shz_xmtrx_store_4x4(out);
}

shz_vec4_t project(shz_vec3_t p) {   // XMTRX holds the matrix built above
    shz_vec4_t v = shz_xmtrx_transform_vec4(shz_vec3_vec4(p, 1.0f));
    v = shz_vec4_swizzle(v, 1, 2, 3, 0);   // undo the WXYZ permutation
    v.w = shz_invf_fsrra(v.w);             // W > 0 for visible points
    return shz_vec4_init(v.x * v.w, v.y * v.w, v.w, 1.0f);
}
```

The full version, with store-queue submission, is in [Examples](/guides/examples/#bruces-balls).

## Thread safety and the software back-end

Unless TLS has been disabled, the XMTRX API is thread-safe, with each thread getting its own unique copy of XMTRX. On the software back-end, XMTRX is emulated as a 16-float state block declared with `SHZ_TLS_DECL()`, so how "per thread" works depends on `SHZ_TLS_MODEL`: compiler `thread_local`, pthread keys, C11 `tss_t`, or disabled (one shared copy). See [Back-ends](/concepts/backends/).

## Related

- [XMTRX API](/api/xmtrx/), [`shz::xmtrx`](/api/cpp/xmtrx/)
- [Matrix Transforms](/guides/matrix-transforms/)
- [The SH4 FPU](/concepts/sh4-fpu/)
- [Matrix API](/api/matrix/)
