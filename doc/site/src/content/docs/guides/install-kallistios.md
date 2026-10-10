---
title: Install with KallistiOS
description: Install SH4ZAM through kos-ports or DreamSDK, update it, and use the upstream Makefile targets
---

SH4ZAM is an official part of [kos-ports](https://github.com/KallistiOS/kos-ports), so it is integrated with the [KallistiOS](https://github.com/KallistiOS/KallistiOS) ecosystem and is set up as part of the regular KOS installation process.

## Standard KallistiOS setup

If you're following the standard community guide for [Setting up KallistiOS](https://dreamcast.wiki/Getting_Started_with_Dreamcast_development), SH4ZAM will be installed as part of the [Building KOS Ports](https://dreamcast.wiki/Getting_Started_with_Dreamcast_development#Building_kos-ports) step. Nothing else to do.

## DreamSDK

If you're using [DreamSDK](https://dreamsdk.org/) to manage your KallistiOS development environment, SH4ZAM may be installed from the `KallistiOS Ports` tab.

## Manual install or update

To get started within a preexisting KallistiOS environment, use `git pull` from within the folder containing the `kos-ports` git repository, which is typically installed to `/opt/toolchains/dc/kos-ports`. Then `cd` into the `sh4zam` folder and run `make install` to build and install the latest version of SH4ZAM as a statically linked library:

```sh
cd /opt/toolchains/dc/kos-ports
git pull
cd sh4zam
make install
```

Source your `environ.sh` first; the Makefile refuses to run without `KOS_BASE` set. `make install` symlinks `libsh4zam.a` into `$(KOS_PORTS)/lib` and the `sh4zam` header folder into `$(KOS_PORTS)/include`.

Once this succeeds, your KOS application can begin leveraging SH4ZAM to make big performance `gainz`.

## Makefile targets

The upstream `Makefile` wraps `kos-cmake`. Run these from a SH4ZAM checkout:

| Target | What it does |
| --- | --- |
| `lib` | Builds the static library only. |
| `tests` | Builds the unit test binary. |
| `all` | Builds library and unit test binary. |
| `run` | Runs the unit tests using KOS's default loader (`$(KOS_LOADER)`). |
| `rerun` | Cleans, rebuilds and reruns the unit tests. |
| `flycast` | Runs the unit tests with the flycast emulator. |
| `install` | Installs the static library as a KOS port. |
| `uninstall` | Removes the library and headers from KOS ports. |
| `reinstall` | `uninstall` then `install`. |
| `clean` | Deletes all build artifacts. |
| `rebuild` | Cleans, then rebuilds the static library. |
| `check-sw` | Rebuilds and runs the software back-end unit tests on the host (needs Ninja). |
| `docs` | Regenerates the Doxygen documentation. |
| `update` | Checks out `master` and pulls the latest. |

Two variables can be overridden on the command line: `SHZ_BACKEND` (default `SH4`) and `SHZ_SAVE_TEMPS` (default `off`). For example, `make clean lib SHZ_BACKEND=SW` builds the portable software back-end with the Dreamcast toolchain. The variables only take effect when `build/` is configured, so run `make clean` before switching.

## Using kos-cmake

For KOS users building with CMake, use `kos-cmake` instead of your system `cmake` command. It sets up the Dreamcast toolchain so `PLATFORM_DREAMCAST` is defined and the SH4 back-end is selected. The options are listed in [Install with CMake](/guides/install-cmake/).

## Examples

The examples are installed to `kos-ports/examples/sh4zam`. You should be able to type `make` to build any example. See [Examples](/guides/examples/).

## Related

- [Getting Started](/guides/getting-started/)
- [Using Within a Project](/guides/using-in-a-project/)
- [Install with CMake](/guides/install-cmake/)
- [Optimization Tips](/guides/optimization/): compiler flags to set in `environ.sh`
