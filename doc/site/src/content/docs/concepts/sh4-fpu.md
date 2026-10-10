---
title: The SH4 FPU
description: Glossary of the Dreamcast SH4 floating-point hardware SH4ZAM is built on - FIPR, FTRV, FSCA, FSRRA, FMAC, XMTRX, precision modes, store queues, prefetch
---

SH4ZAM's hot paths are hand-written SH4 assembly exploiting the Dreamcast CPU's vector-ish FPU instructions, its back-bank of 16 FP registers, its store queues and its prefetch. Below is a short glossary of that hardware; the deep dives are on [Resources](/resources/).

## Instructions

| Instruction | What it does | Where SH4ZAM uses it |
| --- | --- | --- |
| `FIPR fvA, fvB` | 4-component dot product of two front-bank vectors. Reduced precision. | `shz_vec4_dot()`, magnitudes, `shz_dot8f()`, `shz_mag_sqr4f()`, the `dot2`/`dot3` batch routines. |
| `FTRV XMTRX, fvN` | Multiplies a 4-vector by the 4x4 matrix in the back bank. X, Y, Z, W are ready on cycles 4, 5, 6, 7. | Every `shz_xmtrx_transform_*`, and matrix routines that go through XMTRX. |
| `FSCA FPUL, drN` | Sine and cosine of a 16-bit fixed-point angle (65536 = one turn), both at once. | `shz_sincosf()`, `shz_sinf()`, `shz_cosf()`, `shz_sincosu16()`, rotations. |
| `FSRRA frN` | Approximate 1/sqrt(x) in one instruction. Input must be positive. | Everything ending in `_fsrra`, normalization, fast inverse and division. |
| `FMAC` | Multiply-accumulate: `frN = fr0 * frM + frN`. | Hand-scheduled XMTRX routines (rotation setup, for example). `shz_fmaf()` is written as `a * b + c` so GCC can pick it. |
| `FRCHG` | Swaps the front and back register banks. | Moving data in and out of XMTRX. |

`FSCA` has a maximum error of `SHZ_FSCA_ERROR_MAX`; `FIPR` results are compared in the tests with `shz::fipr_max_error`.

## Register banks and XMTRX

In single-precision mode the FPU has two banks of 16 registers. The front bank (`FR0`..`FR15`, also addressed as the four vectors `fv0`, `fv4`, `fv8`, `fv12`) does ordinary math. The back bank (`XF0`..`XF15`) is XMTRX, the 4x4 matrix `FTRV` multiplies by. SH4ZAM's [XMTRX API](/concepts/xmtrx/) keeps your matrix there across calls.

## Precision modes

The SH4 back-end requires the FPU in single-precision mode: compile with `-m4-single-only` or `-m4-single`. Other FPU modes need the software back-end; see [Back-ends](/concepts/backends/#targets-are-not-back-ends). With `-m4-single`, never let `double` values near SH4ZAM's inline assembly, or the FPU can enter it in double mode. Details in [Optimization Tips](/guides/optimization/#fp-precision-modes).

## Fast trig without -ffast-math

With `-ffast-math`, `-mfsca` and `-mfsrra`, GCC emits `FSCA` and `FSRRA` itself and allocates registers around them well, so SH4ZAM uses the compiler builtins. Without fast math, SH4ZAM falls back to inline assembly that emits the instructions explicitly, so you get the hardware either way.

## Store queues

The SH4 has two 32-byte store queues (SQ). Filling one and flushing it writes a whole 32-byte burst to memory or to the PowerVR's tile accelerator, which is far faster than individual stores to uncached memory. SH4ZAM's `sq_` routines copy through them: `shz_sq_memcpy32()` for buffers, `shz_sq_memcpy32_1()` for exactly one 32-byte block (a PVR polygon header or vertex).

## Cache, prefetch, line allocation

Cache lines are 32 bytes.

- `SHZ_PREFETCH(ptr)` issues `PREF` to start loading a line before you need it. Only one prefetch is in flight at a time.
- `shz_dcache_alloc_line(ptr)` wraps `MOVCA.L`: it allocates the data-cache line for `ptr` without reading RAM first. It is a win when you are about to overwrite all 32 bytes, and it corrupts any other data in that line if you do not own the full line.

## Integer and double gotchas

- `float` with `uint32_t` arithmetic is slow: the SH4 only converts signed integers to float, so both sides get promoted to `double`. `uint16_t` is fine.
- `10.0` is a `double`. Write `10.0f`.

## Related

- [XMTRX](/concepts/xmtrx/)
- [Naming and Suffixes](/concepts/naming-and-suffixes/)
- [Optimization Tips](/guides/optimization/)
- [Resources](/resources/)
