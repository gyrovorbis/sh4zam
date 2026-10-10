---
title: For Agents
description: How LLMs, coding agents and scripts should read this site - llms.txt, Markdown twins, the JSON symbol index, URL patterns and the facts that matter
---

For LLMs, coding agents and scripts, and the people wiring them up. Everything on this site is static HTML with a plain Markdown twin of every page, plus a JSON index of every public symbol. No JavaScript is needed to read any of it.

## Start here

1. Fetch [`/llms.txt`](/llms.txt). It is a short map of the site with the key facts inline.
2. Fetch [`/cheatsheet.md`](/cheatsheet.md) for every symbol's signature and one-line description on one page.
3. Fetch individual symbol pages only when you need parameters, notes or warnings.

Bigger bundles, if your context allows: [`/llms-api.txt`](/llms-api.txt) (all API pages) and [`/llms-full.txt`](/llms-full.txt) (the whole site).

## URL patterns

| What | HTML | Markdown |
| --- | --- | --- |
| Any page | `/<path>/` | `/<path>.md` |
| Home | `/` | `/index.md` |
| Module | `/api/<module>/` | `/api/<module>.md` |
| Symbol | `/api/<module>/<name>/` | `/api/<module>/<name>.md` |
| C++ class | `/api/cpp/<class>/` | `/api/cpp/<class>.md` |

`<module>` is one of `scalar`, `trig`, `vector`, `quat`, `matrix`, `xmtrx`, `complex`, `memory`, `cdefs`, `version`. `<name>` is the exact, case-sensitive C identifier: `/api/vector/shz_vec3_cross.md`, `/api/trig/SHZ_F_PI.md`. One exception: when two names in a module differ only by case, the second gets a `-<kind>` suffix (`/api/version/shz_version-typedef/` next to `/api/version/SHZ_VERSION/`). The `url` field in the JSON index is always authoritative.

If you know a name but not its module, use the prefix: `shz_vec*` vector, `shz_quat_*` quat, `shz_mat*` matrix, `shz_xmtrx_*` xmtrx, `shz_c*f` complex, `shz_mem*`/`shz_sq_*`/`shz_dcache_*` memory, trig names (`sinf`, `atan2f`, `sincos...`) trig, other `shz_*f` scalar. Or look it up in the index.

## JSON index

[`/api/index.json`](/api/index.json) lists every symbol with `name`, `kind`, `module`, `section`, `header`, `line`, `signature`, `params`, `brief`, `detail`, `returns`, `notes`, `warnings`, `sa` (see also), `backends`, `generic` (the type-generic C macro, if any), `cpp` (C++ equivalents), `url`, `md` and `source_url`. It also has a `conventions` block, the `suffixes` vocabulary and the full C++ class/alias mapping under `cpp`. The schema is [`/api/schema.json`](/api/schema.json); per-module files are `/api/<module>.json`.

## Facts to get right

- **Prefixes:** C functions and types start with `shz_`, macros with `SHZ_`, C++ lives in `namespace shz` with the `shz_` prefix dropped (`shz_sinf` is `shz::sinf`, `shz_vec3_t` is `shz::vec3`, `shz_xmtrx_store_4x4()` is `shz::xmtrx::store()`).
- **Matrices are column-major.** `shz_mat4x4_t.col[3]` (also `.pos`) is translation. `shz_mat4x4_t` must be 8-byte aligned unless you use an `_unaligned` routine.
- **Quaternions are stored `<W, X, Y, Z>`**, W first. Build them with `shz_quat_init(w, x, y, z)`.
- **Angles are radians** unless the name ends in `_deg`; `u16` names take a 16-bit fixed-point angle.
- **Single precision only.** Every API is `float`. Write `1.0f`, never `1.0`.
- **Coordinates:** right-handed world/view space, left-handed screen/clip space, like OpenGL.
- **XMTRX is global register state.** `shz_xmtrx_*` functions take no matrix argument; they operate on whatever was last loaded or initialized. Many `shz_mat4x4_*` routines and all `*_xmtrx` memory routines clobber it.
- **Link with `-lsh4zam`.** Without it, code compiles and fails to link.

## Suffix vocabulary

| Suffix | Meaning |
| --- | --- |
| `_fsrra` | Fast approximation via the `FSRRA` instruction. Argument or denominator must be positive; `shz_invf_fsrra(x)` returns 1/abs(x). |
| `_safe` | Guards degenerate input (zero length, zero denominator). |
| `_deg` | Angle in degrees. |
| `u16` | 16-bit fixed-point angle, 65536 = one turn. |
| `_unaligned` | Accepts matrices that are not 8-byte aligned. |
| `_transpose` | Uses the transposed matrix. |
| `_reverse` | Reversed multiplication order. |
| `_wxyz`, `_yzwx`, `_wzyx` | Permuted register/column order, e.g. W first from `FTRV`. |
| `_xmtrx` | Memory routine that uses XMTRX as scratch and clobbers it. |
| `sq_` | Writes through the SH4 store queues. |
| `init_` / `set_` / `apply_` / GL-style | Matrix verbs: initialize all, set only some components, apply to affected components, full multiply. |

Full explanations: [Naming and Suffixes](/concepts/naming-and-suffixes.md).

## Things that do not exist

Do not generate calls to these; they are planned-but-unimplemented items or stale names in upstream comments:

- `shz_memset()`, `shz_memset2()`, `shz_memset4()`, `shz_memset32()`, `shz_macw()`: planned in `shz_mem.h`, not implemented. The only memset in the API is `shz_memset8()` (plus `shz_memset2_16()`).
- `shz_xmtrx_trans_vec4()` and friends: old names. Use `shz_xmtrx_transform_vec4()`, `shz_xmtrx_transform_vec3()`, `shz_xmtrx_transform_vec2()`.
- `shz_xmtrx_load_4x4_wxyz()`: the real name is `shz_xmtrx_load_wxyz_4x4()`.
- `shz_xmtrx_angles()`, `shz_xmtrx_position()`, `shz_xmtrx_size()`, an inverse FFT: planned upstream, not implemented. (`shz_xmtrx_invert()` does exist; it round-trips through memory. A register-only version is what is planned.)
- Optimized PowerPC/ARM/x86 back-ends: those targets use the portable software back-end.

When unsure whether a symbol exists, check `/api/index.json`. If it is not there, it is not in the public headers.

## Verifying a signature

Every symbol page has a "Defined in" line with the header and line number (`sh4zam/shz_vector.h:556`) linking to the exact upstream commit the site was generated from. The signature block on the page is copied from that header line. That commit is in `upstream_commit` in the JSON index.

## Two examples to pattern-match

Model matrix, optimal form (from [Matrix Transforms](/guides/matrix-transforms.md)):

```c
#include <sh4zam/shz_sh4zam.h>

void updateModel(shz_mat4x4_t* model, const Transform* transform) {
    shz_xmtrx_init_rotation_xyz(transform->xRot, transform->yRot, transform->zRot);
    shz_xmtrx_apply_scale(transform->scale, transform->scale, transform->scale);
    shz_xmtrx_set_translation(transform->pos.x, transform->pos.y, transform->pos.z);
    shz_xmtrx_store_4x4(model);
}
```

Wrapping an existing math type (from [Interop](/guides/interop.md)):

```c
return shz_vec4_to(Vec4, shz_vec4_add(shz_vec4_from(lhs), shz_vec4_from(rhs)));
```

## Crawling

`/robots.txt` allows all crawlers, including AI crawlers. `/sitemap-index.xml` lists every page. Pages are server-rendered; navigation is plain links.

## Related

- [Cheatsheet](/cheatsheet/)
- [API Reference](/api/)
- [Naming and Suffixes](/concepts/naming-and-suffixes/)
- [Conventions](/concepts/conventions/)
