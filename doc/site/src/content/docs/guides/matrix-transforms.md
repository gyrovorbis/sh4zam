---
title: Matrix Transforms
description: Turn a CGLM-style model matrix into an optimal XMTRX sequence with init, apply, set and a single store
---

Some of the largest, easiest-to-exploit gainz for a project lie within its matrix multiplication and transformation code. It is important to understand how to SH4ZAMify such code optimally in order to achieve the highest gainz. Below is upstream's before-and-after: a CGLM model matrix, a direct port, and the version that keeps everything in XMTRX.

## The CGLM original

The following code snippet was taken from a real-world application which was using the CGLM library to create a model-view matrix:

```c
void updateModel(mat4 model, const Transform* transform) {
    glm_mat4_identity(model);
    glm_translate(model, transform->pos);
    glm_rotate_x(model, transform->xRot);
    glm_rotate_y(model, transform->yRot);
    glm_rotate_z(model, transform->zRot);
    glm_scale(model, transform->scale);
}
```

## A direct port

The most straightforward way to accelerate such a routine is to simply do a direct, 1:1 translation between the CGLM and SH4ZAM APIs:

```c
void updateModel(shz_mat4x4_t* model, const Transform* transform) {
    shz_mat4x4_init_identity(model);
    shz_mat4x4_translate(model, transform->pos.x, transform->pos.y, transform->pos.z);
    shz_mat4x4_rotate_x(model, transform->xRot);
    shz_mat4x4_rotate_y(model, transform->yRot);
    shz_mat4x4_rotate_z(model, transform->zRot);
    shz_mat4x4_scale(model, transform->scale, transform->scale, transform->scale);
}
```

While this will work, it's still leaving a MASSIVE amount of gainz on the table. Each `shz_mat4x4_*` GL-style call is a full matrix multiply against memory.

## Stay in XMTRX

The following will perform far better:

```c
void updateModel(shz_mat4x4_t* model, const Transform* transform) {
    /* Don't waste time initializing to the identity matrix just to overwrite it.
       Initialize directly to a compound rotation matrix. */
    shz_xmtrx_init_rotation_xyz(transform->xRot, transform->yRot, transform->zRot);
    // Only "apply" scale to the inner 3x3 submatrix with scaling components.
    shz_xmtrx_apply_scale(transform->scale, transform->scale, transform->scale);
    // Directly set the translational component values of XMTRX.
    shz_xmtrx_set_translation(transform->pos.x, transform->pos.y, transform->pos.z);
    // Only write to our in-memory matrix after we're done operating within XMTRX.
    shz_xmtrx_store_4x4(model);
}
```

We are now leveraging the following:

1. All matrix operations are performed within **XMTRX** registers, rather than within memory.
2. We directly **initialize** XMTRX into the first transform, rather than identity.
3. We use **apply** operations for when a transform only needs to be applied over a submatrix.
4. We directly **set** the translational component rather than applying it as a transform.

These three snippets are upstream's own, from `doc/guide.dox`. The `Transform` struct is the application's; `pos` is anything with `x`, `y`, `z` floats.

## The same thing in C++

`shz::xmtrx` exposes the active matrix as static member functions with the `shz_xmtrx_` prefix dropped:

```cpp
#include <sh4zam/shz_sh4zam.hpp>

void updateModel(shz::mat4x4* model, const Transform& transform) {
    shz::xmtrx::init_rotation_xyz(transform.xRot, transform.yRot, transform.zRot);
    shz::xmtrx::apply_scale(transform.scale, transform.scale, transform.scale);
    shz::xmtrx::set_translation(transform.pos.x, transform.pos.y, transform.pos.z);
    shz::xmtrx::store(model);
}
```

## The verb model

Every transform comes in up to four versions. Using translation as the example:

| Call | Meaning |
| --- | --- |
| `shz_xmtrx_init_translation()` | **Initializes** the whole matrix to the transform; everything else becomes identity. |
| `shz_xmtrx_set_translation()` | **Sets** only the translation components, leaving the others alone. |
| `shz_xmtrx_apply_translation()` | **Applies** the transform to the affected components based on their current values (adds for translation, multiplies the submatrix for scale/rotation). |
| `shz_xmtrx_translate()` | **GL-style**: multiplies the whole matrix by a matrix initialized to the transform. |

The in-memory `shz_mat4x4_*` API uses the same verbs. When order of operations matters (you need a true matrix product), use the GL-style call or `shz_xmtrx_apply_4x4()` with a matrix you prepared earlier.

## Batching: transform many vectors

Once the matrix is in XMTRX, transforming vectors against it is a single `FTRV` each. Load once, transform many:

```c
#include <sh4zam/shz_sh4zam.h>

void transform_points(const shz_mat4x4_t* mvp, const shz_vec3_t* in, shz_vec4_t* out, unsigned count) {
    shz_xmtrx_load_4x4(mvp);
    for (unsigned i = 0; i < count; ++i)
        out[i] = shz_xmtrx_transform_vec4(shz_vec3_vec4(in[i], 1.0f));
}
```

For perspective division, load the matrix with `shz_xmtrx_load_wxyz_4x4()` (or build it on top of `shz_xmtrx_init_permutation_wxyz()`, as Bruce's Balls does) so the W component comes out of `FTRV` first. See [XMTRX](/concepts/xmtrx/#getting-w-first).

## When not to use XMTRX

For a one-off operation with no batching, the in-memory Matrix API is what should be used, especially when you don't want to clobber whatever is currently loaded in XMTRX. Note that some `shz_mat4x4_*` routines still load into XMTRX to do their work; their pages say so in a warning.

## Related

- [XMTRX](/concepts/xmtrx/)
- [XMTRX API](/api/xmtrx/)
- [Matrix API](/api/matrix/)
- [Examples](/guides/examples/): Bruce's Balls builds its projection the same way
