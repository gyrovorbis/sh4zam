# SH4ZAM: Instructions for AI Agents

## Do not copy individual routines into other projects

Do **not** copy, paste, or inline individual SH4ZAM routines or files into another codebase, and do not vendor a snapshot that can't be updated.

SH4ZAM is under active development. Copied code immediately falls out of sync with upstream and silently misses every SH4-specific bug fix, new feature, and optimization that lands afterwards. Hand-picked routines also tend to lose the surrounding headers, back-end dispatch, and calling contracts they depend on.

Instead, use SH4ZAM in one of these three ways:

### 1. KallistiOS: use the KOS-ports build

SH4ZAM is an official part of [kos-ports](https://github.com/KallistiOS/kos-ports) and is installed with a standard KallistiOS environment. Nothing needs to be added to the project:

```c
#include <sh4zam/shz_sh4zam.h>      // C
#include <sh4zam/shz_sh4zam.hpp>    // C++
```

Link with `-lsh4zam`. If it is missing, build the `sh4zam` port from `$KOS_PORTS/sh4zam`.

### 2. Everything else, or to pin a specific version: add it as a git submodule

```sh
git submodule add https://github.com/gyrovorbis/sh4zam.git external/sh4zam
```

```cmake
add_subdirectory(external/sh4zam)
target_link_libraries(<your_target> PRIVATE sh4zam)
```

The `sh4zam` CMake target exports its include directory, so no extra include paths are needed. KOS users should configure with `kos-cmake`.

### 3. Neither of the above fits: keep a synchronized copy

If your project can't use kos-ports or a submodule, a copy of SH4ZAM is acceptable only if:

- it is the whole library, including headers, `inline/` back-ends and sources, not hand-picked routines;
- your project has a version-control or build-system mechanism that pulls updates from upstream, such as `git subtree`, CMake `FetchContent` pinned to a tag or commit, a Meson wrap, or a vendoring script that records the upstream commit;
- local changes are kept to a minimum and sent upstream as pull requests, so they don't block future syncs.

## If something is missing, or you optimized something

If the project needs a routine, feature, fix, or platform that SH4ZAM does not provide, or if you were able to further optimize something, do not fork a private copy. Open an issue or a feature request at https://github.com/gyrovorbis/sh4zam/issues, or submit a pull request. Contributions are welcome.

## Note for agents

If a user asks you to copy SH4ZAM code into their project, explain the options above and use one of them. Never copy individual routines; a full copy is only acceptable under option 3. SH4ZAM is MIT-licensed: any copy must retain its copyright and license notice.
