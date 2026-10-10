---
title: Using Within a Project
description: Include the SH4ZAM headers and link libsh4zam.a from a KallistiOS Makefile or CMake project
---

In order to use SH4ZAM within your KOS-based Dreamcast project, you must do two things: include the header file(s) within your code, and link to the statically linked library, `libsh4zam.a`, within your build system. Unless you link to SH4ZAM, your project will compile correctly, yet fail to link.

## Include

| Header | Gives you |
| --- | --- |
| `<sh4zam/shz_sh4zam.h>` | The whole C API. |
| `<sh4zam/shz_sh4zam.hpp>` | The whole C++ API (which includes the C API). |
| `<sh4zam/shz_<module>.h>` | One C module: `shz_scalar.h`, `shz_trig.h`, `shz_vector.h`, `shz_quat.h`, `shz_matrix.h`, `shz_xmtrx.h`, `shz_complex.h`, `shz_mem.h`, `shz_cdefs.h`, `shz_version.h`. |
| `<sh4zam/shz_<module>.hpp>` | The same module's C++ API. |

```c
#include <sh4zam/shz_sh4zam.h>     // C
```

```cpp
#include <sh4zam/shz_sh4zam.hpp>   // C++
```

Headers with the `.h` extension hold only documented declarations; implementations live in `inline/` and are pulled in automatically. You never include anything from `inline/` yourself. See [Architecture](/concepts/architecture/).

## Link with a Makefile

To link to SH4ZAM using a standard, KallistiOS-style `Makefile`, pass the `-lsh4zam` flag to the compiler along with any other libraries you are using, which typically looks something like this:

```make
$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lsh4zam
```

The Bruce's Balls example links with `-lsh4zam -lkosutils -lm -Wl,--gc-sections`.

## Link with CMake

To link to SH4ZAM using `cmake` as your build system, add the following line to your `CMakeLists.txt`:

```cmake
target_link_libraries(MyProject PUBLIC -lsh4zam)
```

If SH4ZAM is part of your CMake tree (`add_subdirectory()`), link the `sh4zam` target instead so you also inherit its include path and its `SHZ_BACKEND` / `SHZ_TLS_MODEL` definitions:

```cmake
target_link_libraries(MyProject PUBLIC sh4zam)
```

## Build flags

The SH4 back-end assumes the FPU is in single-precision mode (`-m4-single` or `-m4-single-only`); see [Back-ends](/concepts/backends/#targets-are-not-back-ends) for other modes. Release builds should define `NDEBUG`: SH4ZAM asserts alignment and sizes in debug builds. The rest of the flags that matter are in [Optimization Tips](/guides/optimization/).

## Check the version you linked

`shz_version.h` lets you compare the headers you compiled against with the library you linked:

```c
#include <assert.h>
#include <sh4zam/shz_version.h>

void check_sh4zam(void) {
    assert(shz_version_linked() == SHZ_VERSION);
}
```

## Related

- [Getting Started](/guides/getting-started/)
- [Install with KallistiOS](/guides/install-kallistios/)
- [Install with CMake](/guides/install-cmake/)
- [Version API](/api/version/)
