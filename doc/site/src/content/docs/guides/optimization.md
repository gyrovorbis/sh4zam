---
title: Optimization Tips
description: Compiler flags, mixed -O levels, selective fast math, SH4 FPU precision modes, double promotion and fast division for SH4ZAM
---

Before we even begin discussing how to make the most out of this library, it is *imperative* that you know how to get the most out of the SH GCC toolchain to get your regular C and C++ code as fast as possible.

## Compiler flags

Only a total newb would leave free, universal gainz on the table before jumping directly into hand-optimizing critical code, as it's possible your bottleneck may not even exist with the proper flags. If you are using KallistiOS, you can control which of these flags are enabled or disabled globally for your entire environment by toggling them within your `environ.sh` file.

| Flag(s) | Description |
| --- | --- |
| `-ffast-math` | Allows the compiler to perform optimizations which break strict IEEE compliance of floating-point numbers. |
| `-mfsca` | Allows the compiler to replace `sinf()` and `cosf()` stdlib calls with the `FSCA` instruction. Requires `-ffast-math` to take effect. |
| `-mfsrra` | Allows the compiler to replace `1.0f / sqrtf(x)` patterns with the `FSRRA` instruction. Requires `-ffast-math` to take effect. |
| `-O[0-3, s]` | Sets the optimization level. Default is typically `-O2`, with `-O3` typically offering speed for more bloated code. `-Os` prioritizes smaller code size above performance. Below `-O2` is useless for release performance but is easier to debug during development. |
| `-flto` | Enables link-time optimizations, which allows the linker to perform function inlining of regular out-of-line functions. This is *extremely* important for performance but increases build times. |
| `-fipa-pta` | Enables interprocedural pointer analysis optimizations in GCC, which allows GCC to analyze beyond the boundaries of a single function body when optimizing. |
| `-fomit-frame-pointer` | Tells the compiler not to reserve one of the general-purpose registers for holding the frame pointer, allowing it to be used for other things. |
| `-m4-single(-only)` | Sets the SH4 FPU mode. `-m4-single-only` implicitly converts every single `double` to `float`. `-m4-single` still supports `double` precision but defaults to single precision upon function entry. |
| `-fno-pic/-fno-pie` | Disables position-independent code, which can lose a bit of performance due to indirection and relative offsetting. |
| `-DNDEBUG` | Denotes a release build; disables assertion checks within codebases. |

For reference, the Bruce's Balls example builds with `-std=gnu2x -O3 -fomit-frame-pointer -flto -fbuiltin -ffast-math -ffp-contract=fast -mfsrra -mfsca -fmerge-all-constants -funroll-loops -fno-PIC -fipa-pta -ftree-vectorize`.

## Mixing optimization levels

While it may seem obvious that your entire project should be compiled at the highest optimization level for maximum performance (`-O3`), real-world projects can rarely afford such a luxury due to this bloating the code segment size and wasting space. For this reason, Falco typically opts to use `-Os` for the entire codebase as the default optimization level, favoring small code size, then creates an explicit list of "hot path" files within the build which get the `-O3` treatment. Good candidates are typically translation units involving rendering, collision, and physics.

By using `-Os` globally with a static list of hand-picked translation units getting `-O3`, the GTA3 and Vice City ports were able to achieve within 1 FPS of the performance from compiling everything with `-O3` while simultaneously saving nearly a megabyte of RAM on code size.

## Selective fast math

Ideally, you would enable `-ffast-math` along with `-mfsca` and `-mfsrra` globally for KOS, any kos-ports you are using, SH4ZAM, and within your codebase. Unfortunately, because it no longer ensures strict IEEE floating-point compliance, sometimes doing so can result in very broken builds, typically manifesting in rendering, collision, physics, or other FP-heavy code.

Should this happen, and you need to disable `-ffast-math`, SH4ZAM is smart enough to fall back to inline asm for `FSCA` and `FSRRA` rather than relying on the compiler to emit them. It is still advised that you isolate the offending source files and add them to an explicit list that does not use `-ffast-math` within your build system, allowing everything else that doesn't break to still use it.

## FP precision modes

Typically, a regular C or C++ programmer should never have to care about the precision modes, unless they are relying on some high-precision `double` value that gets inadvertently truncated to `float` when using `-m4-single-only`.

`-m4-single` allows you to mix `double` and `float` variables, swapping the SH4's FPU mode as needed, allowing you to use `float` for speed and `double` for precision, with the compiler automatically switching back to single-precision `float` mode upon each function entry.

There is a trap with `-m4-single` when doing low-level programming with inline assembly routines such as those in SH4ZAM. All of SH4ZAM's inline asm routines assume they are called with the SH4 in single-precision mode, which is the default on function entry. But since these functions are largely inlined, a `double` used within the function making the SH4ZAM call can leave the CPU in double-precision mode, breaking the program.

The golden rule when mixing `-m4-single`, `double` variables and inline asm: NEVER let `double` precision variables get anywhere near your inline SH4 assembly routines which do FP calculations, ensuring the SH4 is always in single-precision mode when entering such routines.

Upstream's tips describe `assert()` checks of the FP precision mode around the inline asm routines. The debug asserts in the SH4 routines cover alignment, sizes and indices, not the FPU mode, so don't count on a debug build to catch a stray `double`.

### Double promotion

Be aware of the circumstances under which single-precision floating-point values get promoted to double precision, as this is an insidious way to lose performance without realizing it. ALWAYS use the `f` suffix with floating-point literals, such as `10.0f` rather than `10.0`, which is a `double`, not a `float`.

Beware of binary arithmetic between a `float` and a `uint32_t`: the SH4's integer-to-float conversion can only convert *signed* integers, so math between a `float` and a 32-bit *unsigned* integer takes a slow path where both values are promoted to `double` first. Since the SH4 has an instruction, `extu.w`, to extend a 16-bit unsigned integer to a 32-bit signed integer, arithmetic between a `float` and a `uint16_t` does not have the same penalty.

## Non-debug builds

KallistiOS, SH4ZAM, and many other libraries use `assert()` to add extra validation and sanity checks, so that potential bugs are caught immediately in debug builds for the price of a little performance. When profiling and releasing, build everything with `-DNDEBUG` to disable those checks. If you run into issues and need help debugging, rebuild a debug binary with `assert()` enabled to see if it catches anything bad.

## Fast division and inversion

When you know the denominator will always be positive, and some precision loss is acceptable, multiply the numerator by `shz_invf_fsrra(denominator)`. When the denominator is not guaranteed to be positive, use `shz_invf(denominator)`. This trick is most commonly leveraged for perspective division during T&L.

```c
#include <sh4zam/shz_scalar.h>

// w is known to be > 0 here (points in front of the camera).
float inv_w = shz_invf_fsrra(w);
float sx = x * inv_w;
float sy = y * inv_w;
```

Bruce's Balls warns about why the precondition matters: the `FSRRA` trick only returns the absolute value, so it has the wrong sign when inverting negatives. See [Naming and Suffixes](/concepts/naming-and-suffixes/#_fsrra).

## Faster perspective division

The `FTRV` instruction for transforming a vector by a matrix produces each component of the resulting vector, going from X to W, one cycle after another:

| Component | Cycle |
| --- | --- |
| X | 4 |
| Y | 5 |
| Z | 6 |
| W | 7 |

Unfortunately, T&L typically necessitates dividing each component by W, which means its result is needed first, despite it coming last.

You can use `shz_xmtrx_load_wxyz_4x4()` to load a 4x4 matrix with the W column coming first, allowing you to use its result first, on cycle 4, rather than having to wait until cycle 7. Bruce's Balls gets the same effect by building its projection on top of `shz_xmtrx_init_permutation_wxyz()` and swizzling each result back with `shz_vec4_swizzle(v, 1, 2, 3, 0)`. The [XMTRX concept page](/concepts/xmtrx/#getting-w-first) walks through it.

## Cache and FPU details

For prefetching and cache-line allocation see [`SHZ_PREFETCH`](/api/memory/SHZ_PREFETCH/) and [`shz_dcache_alloc_line`](/api/memory/shz_dcache_alloc_line/); for `FIPR` and `FTRV` behavior see [The SH4 FPU](/concepts/sh4-fpu/) and the deep dives on [Resources](/resources/).

## Related

- [The SH4 FPU](/concepts/sh4-fpu/)
- [Matrix Transforms](/guides/matrix-transforms/)
- [Examples](/guides/examples/)
- [Resources](/resources/)
