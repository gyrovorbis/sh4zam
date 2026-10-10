---
title: Install with CMake
description: Build and install SH4ZAM with CMake on any platform, with the SH4, SPU or portable software back-end
---

SH4ZAM builds with plain CMake in any environment, independently of KallistiOS. On a Dreamcast toolchain it builds the SH4 back-end; everywhere else it builds the portable software back-end, so cross-platform engines can use one math API on every target.

## Build and install

To quickly build the project with CMake in any environment, run the following from the repo root:

```sh
mkdir build
cd build
cmake ..
make
make install
```

This builds and installs the statically linked library as well as the public headers.

If you would like to also build and run the unit tests, include `-DSHZ_ENABLE_TESTS=on` within the `cmake` command. A separate unit test executable, `Sh4zamTests`, is built as well.

For KOS users, use `kos-cmake` instead of your system `cmake` command.

## Options

| Option | Values | Default | Notes |
| --- | --- | --- | --- |
| `SHZ_BACKEND` | `SH4`, `SPU`, `SW` | `SH4` when `PLATFORM_DREAMCAST`, `SPU` when `SPU`, else `SW` | Back-end implementation. Exported to users as `SHZ_BACKEND=SHZ_<value>`. |
| `SHZ_TLS_MODEL` | `DISABLED`, `IMPLICIT`, `PTHREAD`, `CTHREAD` | `IMPLICIT` on Dreamcast or MSVC, `DISABLED` for SPU, else `PTHREAD` | How the per-thread XMTRX state is stored. Exported as `SHZ_TLS_MODEL=SHZ_TLS_<value>`. `PTHREAD`/`CTHREAD` link `Threads::Threads`. |
| `SHZ_ENABLE_TESTS` | `ON`/`OFF` | `OFF` | Adds the `test/` directory and the `Sh4zamTests` target. |
| `SHZ_SAVE_TEMPS` | `ON`/`OFF` | `OFF` | GCC/Clang only. Keeps intermediate files (`-save-temps=obj`) and disables LTO. |
| `SHZ_ENABLE_PIC` | `ON`/`OFF` | `OFF` | Position-independent code. |
| `SHZ_LIBRARY_TYPE` | `STATIC`, `OBJECT`, ... | `STATIC` | Set by a parent project. Simulant builds SH4ZAM as an `OBJECT` library embedded in its own archive; in that case nothing is installed. |

`SHZ_BACKEND` and `SHZ_TLS_MODEL` are added to the library's `PUBLIC` compile definitions, so anything linking the `sh4zam` target sees the same values the library was built with.

The project builds as C11 and C++20 (`CMAKE_C_STANDARD 11`, `CMAKE_CXX_STANDARD 20`, GNU extensions on). Your own code can use C17, C23, C++20 or C++23; some API features are only available with C23 or C++23.

## Software back-end on desktop

On a desktop compiler `SHZ_BACKEND` defaults to `SW`: every routine runs portable C, and XMTRX is emulated in a thread-local array, so the same code compiles and runs on x86_64, ARM, PowerPC and friends. The Meese Engine runs on GameCube this way. See [Back-ends](/concepts/backends/) for what the software back-end guarantees.

## As a subproject

Because the target exports its include directory and compile definitions, adding SH4ZAM with `add_subdirectory()` and linking the `sh4zam` target is enough:

```cmake
add_subdirectory(external/sh4zam)
target_link_libraries(MyProject PUBLIC sh4zam)
```

## Related

- [Install with KallistiOS](/guides/install-kallistios/)
- [Using Within a Project](/guides/using-in-a-project/)
- [Back-ends](/concepts/backends/)
- [Testing and Contributing](/guides/testing-and-contributing/)
