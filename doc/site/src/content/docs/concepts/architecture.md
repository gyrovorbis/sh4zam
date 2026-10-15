---
title: Architecture
description: How SH4ZAM is laid out (interface headers, inline implementations, per-back-end code, out-of-line sources) and how a call reaches its back-end
---

SH4ZAM is a header-heavy library: almost everything is `inline`, so the compiler sees the implementation at every call site and can schedule SH4 instructions across your code. The headers are split so the part you read stays clean: interface headers hold only documented declarations, and the implementations live in an `inline/` tree with one folder per back-end. A small static library, `libsh4zam.a`, holds the few routines that are better out of line.

## File tree

```text
include/sh4zam/
  shz_sh4zam.h / .hpp        umbrella: includes everything
  shz_cdefs.h / .hpp         configuration, target/back-end detection, attributes, TLS, SHZ_CONVERT
  shz_version.h / .hpp       compile-time and link-time version
  shz_scalar.h / .hpp        scalar math
  shz_trig.h / .hpp          trigonometry
  shz_vector.h / .hpp        vec2 / vec3 / vec4
  shz_quat.h / .hpp          quaternions
  shz_matrix.h / .hpp        in-memory matrices
  shz_xmtrx.h / .hpp         the register-resident active matrix
  shz_complex.h / .hpp       complex numbers + FFT
  shz_mem.h / .hpp           memcpy family, store queues, cache
  inline/
    shz_<module>.inl.h       generic implementation; picks a back-end
    sh4/shz_<module>_sh4.inl.h    SH4 inline asm
    sw/shz_<module>_sw.inl.h      portable C
    spu/shz_vector_spu.inl.h      Cell SPU (vector only)
source/
  shz_matrix.c  shz_quat.c  shz_version.c  shz_xmtrx.c      out-of-line, any back-end
  sh4/shz_xmtrx_sh4.s  sh4/shz_mem_sh4.s  sh4/shz_complex_sh4.c
  sw/shz_xmtrx_sw.c    sw/shz_complex_sw.c
example/   bruces_balls/  pvr_dma/
test/      one *_test_suite.cpp per module, libGimbal + cglm submodules
```

## The pattern per module

For each module `X`:

- `shz_X.h` is the **interface**: types, constants and documented declarations only. This is the file to read, and the file this site's API reference is generated from.
- `inline/shz_X.inl.h` is the **implementation**, included at the bottom of `shz_X.h`. Routines that are the same on every back-end are written here once. Routines that differ call `shz_foo_sh4()` or `shz_foo_sw()`.
- `inline/sh4/shz_X_sh4.inl.h` and `inline/sw/shz_X_sw.inl.h` hold those back-end-specific versions.
- `shz_X.hpp` is the **C++ API** in `namespace shz`, a thin layer over the C functions.

## How a call is dispatched

Each `inline/shz_X.inl.h` picks its back-end at compile time. For most modules that is exactly one include:

```c
#if SHZ_BACKEND == SHZ_SH4
#   include "sh4/shz_scalar_sh4.inl.h"
#else
#   include "sw/shz_scalar_sw.inl.h"
#endif
```

The back-end is chosen at compile time, with no function pointers: the chosen implementation is inlined into your code. `SHZ_BACKEND` comes from `shz_cdefs.h` (auto-detected) or from your build system; see [Back-ends](/concepts/backends/).

Some routines are written once in the generic file on top of other SH4ZAM routines. `shz_invf()`, for example, is `shz_invf_fsrra()` with the sign restored, so it is fast on every back-end that has a fast `shz_invf_fsrra()`.

## What libsh4zam.a contains

The static library holds what is better out of line: the larger matrix and quaternion routines (`source/shz_matrix.c`, `source/shz_quat.c`), the version query, the XMTRX routines that are big enough to be called rather than inlined (`shz_xmtrx_sh4.s` / `shz_xmtrx_sw.c`), the memory routines in `shz_mem_sh4.s`, and the FFT. That's why you need `-lsh4zam` even though most of the API is inline: without it, your project compiles and then fails to link.

## Configuration in shz_cdefs.h

`shz_cdefs.h` is included by everything. It defines:

- `SHZ_TARGET` and `SHZ_BACKEND` (see [Back-ends](/concepts/backends/)).
- `SHZ_TLS_MODEL` and `SHZ_TLS_DECL()` / `SHZ_TLS_REF()`, used for the per-thread XMTRX state of the software back-end.
- Compiler detection: `SHZ_GNUC`, `SHZ_GCC`, `SHZ_CLANG`, `SHZ_MINGW32`, `SHZ_MINGW64`, `SHZ_MSVC`.
- Attribute macros: `SHZ_INLINE`, `SHZ_FORCE_INLINE`, `SHZ_NO_INLINE`, `SHZ_HOT`, `SHZ_COLD`, `SHZ_FAST_MATH`, `SHZ_NO_FAST_MATH`, `SHZ_PURE`, `SHZ_CONST`, `SHZ_ALIGNAS()`, `SHZ_SIMD()`, `SHZ_PACKED`, `SHZ_ALIASING`, `SHZ_RESTRICT`, `SHZ_LIKELY()`, `SHZ_UNLIKELY()`, `SHZ_DEPRECATED()`, barriers (`SHZ_MEMORY_BARRIER_SOFT()`, `SHZ_MEMORY_BARRIER_HARD()`, `SHZ_INSTR_BARRIER()`), and `SHZ_DECLS_BEGIN` / `SHZ_DECLS_END` for `extern "C"`.
- Utilities: `SHZ_STRINGIFY()`, `SHZ_COUNT_OF()`, `SHZ_CONTAINER_OF()`, `SHZ_SWAP()`, `SHZ_DECLARE_STRUCT()`, `SHZ_INIT()`, and `SHZ_CONVERT()` for zero-cost conversions.
- Aliasing typedefs `shz_alias_int16_t` .. `shz_alias_double_t` for type punning.

Upstream keeps the attribute macros out of its own Doxygen reference; the [Compiler Definitions](/api/cdefs/) page lists them under "Not in upstream reference".

## Versioning

`shz_version.h` defines `SHZ_VERSION_MAJOR`, `SHZ_VERSION_MINOR`, `SHZ_VERSION_PATCH` and the packed `SHZ_VERSION`. Upstream keeps the version in sync across `shz_version.h`, `CMakeLists.txt` and `doc/Doxyfile` with `util/shz_versioning.py`.

## Related

- [Back-ends](/concepts/backends/)
- [C and C++ APIs](/concepts/c-and-cpp/)
- [Using Within a Project](/guides/using-in-a-project/)
- [Compiler Definitions API](/api/cdefs/)
