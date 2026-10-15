---
title: Testing and Contributing
description: Build and run the SH4ZAM unit tests on Dreamcast or on the host, and what to know before opening a PR
---

SH4ZAM's unit tests live in `test/` and build into a single binary, `Sh4zamTests`, which runs on real hardware, in an emulator, or on your host machine with the software back-end. The contribution policy fits in a sentence: pretty casual and easy-going, nothing super formal required.

## How the tests work

- One test suite per module: `shz_scalar_test_suite.cpp`, `shz_trig_test_suite.cpp`, `shz_vector_test_suite.cpp`, `shz_quat_test_suite.cpp`, `shz_matrix_test_suite.cpp`, `shz_xmtrx_test_suite.cpp`, `shz_complex_test_suite.cpp`, `shz_mem_test_suite.cpp`, plus shared helpers in `shz_test.{c,h,hpp}`.
- The harness is [libGimbal](https://github.com/gyrovorbis/libgimbal)'s test framework (`GBL_TEST_CASE`, `GBL_TEST_VERIFY`).
- [cglm](https://github.com/recp/cglm) is linked as a reference implementation that results are compared against. On Dreamcast it is built with `-fno-fast-math`.
- Both are git submodules (`test/libgimbal`, `test/cglm`), so clone with `--recurse-submodules` or run `git submodule update --init`.
- The suites build as C17 and C++23.
- Benchmarks are reported alongside the tests (perf counters on SH4, `std::chrono` elsewhere). Configure with `-DSHZ_DISABLE_BENCHMARKS=ON` to turn them off.

## Run on Dreamcast

From a SH4ZAM checkout with your KOS `environ.sh` sourced:

```sh
make run        # build Sh4zamTests.elf and run it with $(KOS_LOADER)
make flycast    # or run it in the flycast emulator
make rerun      # clean, rebuild and run
```

`make tests` only builds the binary (`build/test/Sh4zamTests.elf`). Pass `SHZ_BACKEND=SW` to test the software back-end on the Dreamcast itself.

## Run on the host

The software back-end runs everywhere, which makes it the quick sanity check:

```sh
make check-sw
```

That configures a fresh `build-sw` directory with `-DSHZ_ENABLE_TESTS=on` using Ninja, builds, and runs `build-sw/test/Sh4zamTests`. Without the Makefile:

```sh
cmake -S . -B build-sw -G Ninja -DSHZ_ENABLE_TESTS=on
ninja -C build-sw
build-sw/test/Sh4zamTests
```

## Before you open a PR

From upstream's contributing guide:

- **Anything that adds value is welcome**: new API routines, optimizing existing implementations, new examples, better test coverage, documentation of any kind, code comments and typo fixes. Don't be shy.
- **Describe your changes** as well as you can; there is no formal PR template. If you want early feedback, open the PR as a draft.
- **Coding style** should stay consistent, but juicy optimizations won't be turned down over little style inconsistencies.
- **Doxygen comments**: everything in the public API should be fully Doxygen commented. If that's too much of a PITA, say so in the PR and you'll be covered.
- **Unit tests**: the project is striving for 100% coverage. At minimum, run the existing tests to make sure you haven't broken anything that does have coverage.
- **C++ bindings** are kept in sync with the C API, but C and SH4 contributors aren't expected to write idiomatic C++ bindings themselves; Falco will handle it.

The [Code of Conduct](https://github.com/gyrovorbis/sh4zam/blob/master/CODE_OF_CONDUCT.md) is one line: don't be a dick.

## Related

- [Contributing](/contributing/)
- [Install with CMake](/guides/install-cmake/)
- [Back-ends](/concepts/backends/)
- [Community](/community/)
