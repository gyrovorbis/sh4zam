---
title: Examples
description: Walkthrough of Bruce's Balls, the 4.5 million polygons per second SH4ZAM benchmark, and the PVR DMA renderer example
---

Examples can be found within the `example` subdirectory of the SH4ZAM repo, and new examples are always welcome to be contributed. If you installed SH4ZAM through kos-ports, the examples are installed to `kos-ports/examples/sh4zam`; type `make` in an example's folder to build it.

| Example | Description |
| --- | --- |
| Bruce's Balls | Pushes up to 4.5 million PPS, rendering Bruce's balls. |
| PVR DMA | Real-world, complex renderer using KOS's PVR DMA API. |

## Bruce's Balls

Source: [`example/bruces_balls/bruces_balls.c`](https://github.com/gyrovorbis/sh4zam/blob/master/example/bruces_balls/bruces_balls.c) (C23).

The program renders a series of high-polygon balls (93 by default, 20 stacks by 20 slices each) and reports back the rendering performance in polygons per second. It uses KOS's "Direct Rendering" PVR API, which submits geometry to the PowerVR GPU by filling and flushing the two store queues to the TA in alternation. This is the fastest rendering path, and with the right build flags it pushes 4.5 million polygons per second.

### Cache-line-sized data

The secret to high performance is keeping your data cache-friendly. Each ball is exactly one 32-byte cache line:

```c
typedef struct {
    alignas(32)             // Align to cache-line sizes
        shz_vec3_t pos;     //!< 3D Position of a ball.
        shz_vec2_t rot;     //!< X/Y orientation of a ball.
        shz_vec2_t vel;     //!< X/Y velocity of a ball.
        uint32_t   color;   //!< ARGB8888 color of a ball.
} Ball;

static_assert(sizeof(Ball)  == 32); // Ensure we are exactly the size of a cache-line.
static_assert(alignof(Ball) == 32); // Ensure we are exactly aligned to start on cache-line boundaries.
```

Because a `Ball` fills a whole line, initialization calls `shz_dcache_alloc_line(&balls[idx])` first: it allocates the cache line without fetching the old contents from RAM. You can only do this when your data fits perfectly into cache lines, or you will corrupt whatever else shares the line.

### Projection matrix, built once

```c
static void setup_projection_view(shz_mat4x4_t* mat) {
    constexpr float screen_width  = 640.0f;
    constexpr float screen_height = 480.0f;
    constexpr float near_z        = 0.0f;
    constexpr float fov           = SHZ_DEG_TO_RAD(60.0f);
    constexpr float aspect        = screen_width / screen_height;

    shz_dcache_alloc_line(mat);
    shz_xmtrx_init_permutation_wxyz();
    shz_xmtrx_apply_screen(screen_width, screen_height);
    shz_dcache_alloc_line(((void*)mat) + 32);
    shz_xmtrx_apply_perspective(fov, aspect, near_z);
    shz_xmtrx_store_4x4(mat);
}
```

XMTRX starts as a permutation matrix, so every vector transformed by it comes out with W first, which is what perspective division needs immediately. A screen-space transform and the perspective projection are applied on top, and the result is stored once. (KOS's `mat_perspective()` does the screen mapping for you; SH4ZAM does it explicitly for fine-grained control.)

### Per ball: load, translate, apply

```c
static void apply_model_matrix(shz_vec3_t pos, shz_vec2_t rot, const shz_mat4x4_t* proj_view) {
    shz_xmtrx_load_4x4(proj_view);
    shz_xmtrx_translate(pos.x, pos.y, -pos.z);
    shz_xmtrx_apply_rotation_x(rot.x);
    shz_xmtrx_apply_rotation_y(rot.y);
}
```

### Per vertex: sincos, FTRV, FSRRA, store queues

```c
shz_sincos_t sc_slice = shz_sincosf(sliceAngle);
shz_vec3_t local_pos1 = shz_vec3_scale(shz_vec3_init(sc_slice.cos * r2, sc_slice.sin * r2, z2), radius);

shz_vec4_t trans_pos1 = shz_xmtrx_transform_vec4(shz_vec3_vec4(local_pos1, 1.0f));
// Undo the WXYZ permutation baked into the projection matrix.
trans_pos1 = shz_vec4_swizzle(trans_pos1, 1, 2, 3, 0);

pvr_vertex_t* vert1 = pvr_dr_target(*dr_state);
SHZ_MEMORY_BARRIER_SOFT();

trans_pos1.w = shz_invf_fsrra(trans_pos1.w);
vert1->x = trans_pos1.x * trans_pos1.w;
vert1->y = trans_pos1.y * trans_pos1.w;
vert1->z = trans_pos1.w;
pvr_dr_commit(vert1);
```

- `shz_sincosf()` gets sine and cosine from one `FSCA`.
- `shz_xmtrx_transform_vec4()` is one `FTRV` against the matrix held in XMTRX.
- `SHZ_MEMORY_BARRIER_SOFT()` stops GCC from reordering memory accesses so it doesn't read the transformed position before it's ready.
- `shz_invf_fsrra()` is the fast inverse. Upstream's comment is worth repeating: this inversion trick only returns the ABSOLUTE value, so it has the wrong sign when inverting negatives. It is fine here because Z is positive.
- Polygon headers go out with `SHZ_PREFETCH()` followed by `shz_sq_memcpy32_1()`, a single 32-byte copy through the store queues.

### Build flags

```make
CFLAGS += -std=gnu2x -O3 \
          -fomit-frame-pointer -flto -fbuiltin -ffast-math -ffp-contract=fast -mfsrra -mfsca \
          -fmerge-all-constants -funroll-loops -fno-PIC -fipa-pta  \
          -ftree-vectorize -I$(KOS_BASE)/utils
```

It links with `-lsh4zam -lkosutils -lm -Wl,--gc-sections`.

## PVR DMA

Source: [`example/pvr_dma/`](https://github.com/gyrovorbis/sh4zam/tree/master/example/pvr_dma), by jnmartin64.

A real-world renderer that loads an OBJ model with textures from a romdisk and draws it through KOS's PVR DMA path:

- Uses KOS's `pvr_vertbuf_tail()`/`pvr_vertbuf_written()` API for DMA scene submission.
- Uses SH4ZAM to perform all matrix math and transforms.
- Provides a real-world tested implementation of near-Z clipping.
- Shows off PVR hardware fog.

To build: compile and install KOS, install `sh4zam`, `zlib`, `libjpeg`, `libpng` and `libkmg` from kos-ports, then run `make`.

Controls: analog stick moves and turns, d-pad up/down moves vertically, d-pad left/right and the L/R triggers adjust the fog far and near planes, A toggles texturing off for debug vertex coloring, Y resets the scene, Start exits.

## More references

The projects on the [Showcase](/showcase/) page have been accelerated with SH4ZAM and make great references, especially [SH4ZAM PVR](https://github.com/dfchil/sh4zam_pvr), a set of advanced PVR Direct Rendering examples.

## Related

- [Matrix Transforms](/guides/matrix-transforms/)
- [XMTRX](/concepts/xmtrx/)
- [Memory API](/api/memory/)
- [Showcase](/showcase/)
