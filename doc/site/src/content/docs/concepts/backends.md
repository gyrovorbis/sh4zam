---
title: Back-ends
description: SH4ZAM's SH4, software and SPU back-ends, how one is selected, targets versus back-ends, and the precision you can expect from each
---

Every SH4ZAM routine has a portable C implementation, and the hot ones also have a hand-written SH4 implementation. Which one you get is decided at compile time by `SHZ_BACKEND`. This is what lets cross-platform engines such as Simulant and the Meese Engine use one API everywhere: the same calls run as SH4 assembly on a Dreamcast and as plain C on a PC or GameCube.

## The three back-ends

| `SHZ_BACKEND` | What it is | Used when |
| --- | --- | --- |
| `SHZ_SH4` | Hand-optimized SH4 inline asm and `.s` files using `FIPR`, `FTRV`, `FSCA`, `FSRRA`, XMTRX, store queues. | CMake and the KOS Makefile default to it on Dreamcast. Header auto-detection (no build-system define) picks it only when the target is SH4 **and** the FPU is in single-precision mode (`__SH4_SINGLE__` or `__SH4_SINGLE_ONLY__`). |
| `SHZ_SW` | Generic C-based software back-end. XMTRX is emulated in memory. | Everything else, including SH4 builds in other FPU modes. |
| `SHZ_SPU` | Cell Broadband Engine SPU implementation. Partial: only some vector routines have SPU-specific code; the rest use the shared C. | Target is the SPU. |

Each function's API page shows its "Back-ends" line: whether a dedicated SH4 or SPU implementation exists, or whether it uses shared C code on that back-end. The same information is in the `backends` field of [`/api/index.json`](/api/index.json).

## Targets are not back-ends

`SHZ_TARGET` is the architecture the compiler is building for, auto-detected from compiler macros: `SHZ_SH4`, `SHZ_PPC`, `SHZ_MIPS`, `SHZ_ARM`, `SHZ_X86_64`, `SHZ_SPU` or `SHZ_WASM`. `SHZ_BACKEND` is the implementation. Since v0.9.1 they are decoupled: PowerPC, MIPS, ARM, x86_64 and WebAssembly are detected targets, but they all use the software back-end. There are no optimized back-ends for them.

Both macros are only auto-detected when not already defined, so you can force either one. With CMake, set the `SHZ_BACKEND` cache variable (`SH4`, `SPU` or `SW`); the build exports the matching `SHZ_BACKEND=SHZ_<value>` definition to everything that links it. With the KOS Makefile, `make SHZ_BACKEND=SW`.

If you build for SH4 with `-m4` or `-m4-double-only`, set `SHZ_BACKEND` to `SW` yourself: the CMake and Makefile defaults select the SH4 back-end on Dreamcast regardless of FPU mode, and its inline assembly assumes single precision.

The software back-end is built for all targets, including Dreamcast, so a Dreamcast project can switch between the accelerated and software back-ends without rebuilding `libsh4zam.a` for the other one. That's handy for checking whether a bug is in your code or in an SH4 routine.

## What the software back-end guarantees

- Same API, same types, same layouts. Code compiles unchanged.
- XMTRX is a block of 16 floats in thread-local storage (see `SHZ_TLS_MODEL` below), so the XMTRX API behaves the same.
- Results are computed with ordinary C float math, so they are not bit-identical to the SH4 hardware approximations. Store queue routines fall back to `memcpy()` plus barriers.

## Precision differences

The SH4 back-end trades precision for speed in specific places:

- `FSCA` sine/cosine has a maximum error of `SHZ_FSCA_ERROR_MAX`.
- `FSRRA` (all `_fsrra` routines, normalization) is an approximation.
- `FIPR` dot products are reduced precision; C++ exposes a tolerance constant `shz::fipr_max_error`.

That is why `SHZ_FLT_EPSILON` is a deliberately loose `0.01f`, and why `shz_equalf()` and friends compare with it. The unit tests check both back-ends against cglm within these tolerances.

## TLS model

`SHZ_TLS_MODEL` controls how per-thread state (the emulated XMTRX) is stored. CMake always defines it. Without CMake, `shz_cdefs.h` picks `SHZ_TLS_IMPLICIT` for an SH4 target and `SHZ_TLS_DISABLED` for SPU; on other targets nothing is picked, which leaves the model at `SHZ_TLS_DISABLED`, so define `SHZ_TLS_MODEL` yourself (or build with CMake) if more than one thread uses XMTRX.

| Value | Mechanism | CMake default for |
| --- | --- | --- |
| `SHZ_TLS_IMPLICIT` | compiler `thread_local` / `_Thread_local` | Dreamcast, MSVC |
| `SHZ_TLS_PTHREAD` | pthread keys | everything else |
| `SHZ_TLS_CTHREAD` | C11 `tss_t` | opt-in |
| `SHZ_TLS_DISABLED` | plain `static`, shared by all threads | the SPU back-end |

## Related

- [Architecture](/concepts/architecture/)
- [Install with CMake](/guides/install-cmake/)
- [XMTRX](/concepts/xmtrx/)
- [Compiler Definitions API](/api/cdefs/)
