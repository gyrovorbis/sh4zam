---
title: Changelog
description: SH4ZAM release notes - v0.9.1, v0.9.0 and earlier - and the upstream revision this documentation was generated from
---

Release notes summarized from the upstream tags and release commits. The full history is on [GitHub](https://github.com/gyrovorbis/sh4zam/commits/master); tagged releases are listed under [tags](https://github.com/gyrovorbis/sh4zam/tags).

This site documents upstream commit [`7747e5d`](https://github.com/gyrovorbis/sh4zam/commit/7747e5d281a75dd4ffce10df2758748feb421c66) (2026-10-07), which is v0.9.1 plus 23 commits.

## Since v0.9.1 (unreleased)

- Initial Cell Broadband Engine SPU back-end (`SHZ_BACKEND=SHZ_SPU`), vector routines only, added to CMake and to `SHZ_TLS_MODEL` auto-detection.
- `shz_vec_inv_sqrtf()` and `shz_vec_sqrtf()` added to the type-generic routines and the C++ wrappers.
- XMTRX, `shz_fmodf()` and `shz_memcpyN()` bug fixes; fixes to `load_apply_store_4x4` and `store_aligned32_4x4`.
- Build fixes for the KOS + SH toolchain stable versions and for Clang.

## v0.9.1 (2026-09-25): XMTRX gainz, bug fixes, SW back-end on SH4

- **CMake:** a configuration option chooses the back-end rather than deciding by architecture. The software back-end is now built for all targets, so a Dreamcast project can switch between the accelerated and software back-ends without rebuilding `libsh4zam.a`.
- **Makefile:** `SHZ_BACKEND` variable to pick the SW or SH4 back-end.
- **XMTRX:** faster SH4 versions of `load_transpose_3x3`, `load_2x2`, `store_3x3`, `store_transpose_3x3`, `init_one`, `init_rotation_x/y/z`, `apply_translation`, `apply_scale`, `apply_rotation_x` and `init_screen`. Software back-end fixes in `apply_3x3`, `apply_transpose_3x3`, `apply_rotation_x/y/z`, `apply_rotation`, `apply_rotation_quat` and `rotate`. New C++ wrappers for loading, storing and applying 2x2 and 3x3 matrices.
- **Memory:** `shz_sq_memcpy32()` and `shz_sq_memcpy32_1()` software versions now work on real Dreamcast store queues.
- **cdefs:** `SHZ_TARGET` is decoupled from `SHZ_BACKEND`, since any target can use either back-end; every major architecture is auto-detected as a target. `-m4` and `-m4-double-only` SH4 builds are supported (with the software back-end).
- **Tests:** benchmark gainz reported for the SW back-end too; coverage for every newly wrapped C++ routine and every routine that had a bug.

## v0.9.0 (2026-09-22): XMTRX 3x4 API expansion

- XMTRX 3x4 API for C and C++ on both back-ends: `shz_xmtrx_load_transpose_3x4()`, `shz_xmtrx_store_transpose_3x4()`, `shz_xmtrx_apply_transpose_3x4()`, `shz_xmtrx_apply_reverse_3x4()`, `shz_xmtrx_apply_reverse_transpose_3x4()` and the fused `shz_xmtrx_load_apply_3x4()`.
- The C++ API covers the whole 3x4 family.
- Faster SH4 `shz_xmtrx_get_scale()`, `shz_mat4x4_get_scale()` (also fixing a real bug for non-affine matrices) and `shz_mat4x4_transform_vec3()`, which is now hand-written `FIPR` assembly.
- More stable benchmark harness.

## v0.8.1 (2026-09-20)

- One-off rotations of a single 3D vector about the X, Y and Z axes: `shz_vec3_rotate_x()`, `shz_vec3_rotate_y()`, `shz_vec3_rotate_z()` (and `shz::vec3::rotate_x()` etc.), faster than going through XMTRX for a single rotation.
- Resources page additions, including the deep-dive into `FSCA`, `FSRRA`, `FTRV` and `FIPR`.

## v0.8.0 (2026-08-18)

"We're WAY overdue for a minor release." Lots of new features, many bug fixes for both back-ends, and no known regressions. Upstream recommends it as a stable tag to stay on before the 1.0 API.

## v0.7.0 (2026-05-24)

Fix for software back-end unit test builds.

## Related

- [Contributing](/contributing/)
- [Back-ends](/concepts/backends/)
