---
title: Getting Started
description: Install SH4ZAM, include it, link it, and build a first program in C and C++ for the Sega Dreamcast
---

SH4ZAM is a hand-optimized, general-purpose math and linear algebra library for harnessing the floating-point power of the SH4 processor in the Sega Dreamcast. Install it, include one header, link `libsh4zam.a`, and make some `gainz`.

## Install

Pick one:

- **KallistiOS (Dreamcast).** SH4ZAM is an official part of [kos-ports](https://github.com/KallistiOS/kos-ports). If you followed the standard KallistiOS setup, it is already installed. Otherwise run `make install` from `kos-ports/sh4zam`. Details: [Install with KallistiOS](/guides/install-kallistios/).
- **CMake (any platform).** `mkdir build && cd build && cmake .. && make && make install` from the repo root. On anything that is not a Dreamcast this builds the portable software back-end. Details: [Install with CMake](/guides/install-cmake/).

## Include and link

In order to use SH4ZAM within your project, you must do two things:

1. Include the header file(s) within your code.
   - `#include <sh4zam/shz_sh4zam.h>` globally includes the whole SH4ZAM C API.
   - `#include <sh4zam/shz_sh4zam.hpp>` globally includes the whole SH4ZAM C++ API.
2. Link to the statically linked library, `libsh4zam.a`, within your build system.

Unless you link to SH4ZAM, your project will compile correctly, yet fail to link. With a KallistiOS `Makefile`, add `-lsh4zam`:

```make
$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lsh4zam
```

With CMake:

```cmake
target_link_libraries(MyProject PUBLIC -lsh4zam)
```

More on this in [Using Within a Project](/guides/using-in-a-project/).

## First program

These are the usage samples from the upstream README. Both build an XMTRX transform, store it, and transform a vector by it.

### C

```c
#include <sh4zam/shz_sh4zam.h>

int main(int argc, const char *argv[]) {
    shz_vec4_t vec1 = shz_vec4_init(2.0f, 3.0f, 4.0f, 1.0f);
    shz_vec4_t vec2 = shz_vec4_normalize(shz_vec4_scale(vec1, shz_sinf(SHZ_F_PI)));
    shz_mat4x4_t mat = {};

    shz_xmtrx_init_diagonal(vec2.x, vec2.y, vec2.z, vec2.w);
    shz_xmtrx_apply_rotation_x(shz_vec4_dot(vec1, vec2));
    shz_xmtrx_apply_translation(vec1.x, vec1.y, vec1.z);
    shz_xmtrx_store_4x4(&mat);

    shz_vec4_t vec3 = shz_xmtrx_transform_vec4(vec2);

    return 0;
}
```

Where applicable, the C API also includes type-generic routines which resolve to the proper function based on argument type. For example, `shz_vec_dot()` forwards to `shz_vec2_dot()`, `shz_vec3_dot()` or `shz_vec4_dot()` depending on the vectors passed to it.

### C++

```cpp
#include <sh4zam/shz_sh4zam.hpp>

int main(int argc, const char* argv[]) {
    shz::vec4 vec1(2.0f, 3.0f, 4.0f, 1.0f);
    shz::vec4 vec2 = shz::vec4(vec1 * shz::sinf(shz::pi_f)).direction();
    shz::mat4x4 mat {};

    shz::xmtrx::init_diagonal(vec2.x, vec2.y, vec2.z, vec2.w);
    shz::xmtrx::apply_rotation_x(vec1.dot(vec2));
    shz::xmtrx::apply_translation(vec1.x, vec1.y, vec1.z);
    shz::xmtrx::store(&mat);

    shz::vec4 vec3 = shz::xmtrx::transform(vec2);

    return 0;
}
```

Upstream's README passes `vec2.y, vec2.z` to `apply_translation()` in the C++ version; it is fixed to match the C version here. C++ can still use the C API by design, and every C++ type is also compatible with its corresponding C type and the C API, so you can mix and match.

The C sample uses `{}` empty initialization, which is C23. With an older C standard, write `shz_mat4x4_t mat = {0};`.

## Run Bruce's Balls

At this point it is advised that you check out and attempt to run the examples provided with SH4ZAM. If you installed through kos-ports they live in `kos-ports/examples/sh4zam`; type `make` in an example's folder to build it. You can quickly verify the integrity of your SH4ZAM install by playing with Bruce's Balls, which renders 93 high-polygon spheres and reports polygons per second. See [Examples](/guides/examples/) for a walkthrough of what it does.

## Next

- [Matrix Transforms](/guides/matrix-transforms/): where the easiest gainz usually are.
- [Naming and Suffixes](/concepts/naming-and-suffixes/): what `_fsrra`, `_safe`, `_unaligned` and friends mean.
- [Conventions](/concepts/conventions/): column-major matrices, `<W, X, Y, Z>` quaternions, radians.

## Related

- [Install with KallistiOS](/guides/install-kallistios/)
- [Install with CMake](/guides/install-cmake/)
- [API Reference](/api/)
- [Cheatsheet](/cheatsheet/)
