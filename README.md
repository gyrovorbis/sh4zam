<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/gyrovorbis/sh4zam">
    <img src="http://sh4zam.elysianshadows.com/logo.png" alt="SH4ZAM">
  </a>
  <h3 align="center"><strong>SH4ZAM!</strong></h3>
  <p align="center">
    Fast math library for the Sega Dreamcast's SH4 CPU
    <br />
    <a href="http://sh4zam.falcogirgis.net"><strong>Explore the docs »</strong></a>
  </p>
</div>
<hr>

# Overview
`SH4ZAM` is a hand-optimized, general-purpose math and linear algebra library for harnessing the floating-point power of the SH4 processor in the Sega Dreamcast. It originally began as a collection of inline assembly routines developed specifically to accelerate the math in the Grand Theft Auto 3 and Vice City DC ports. These routines, along with many others from various contributors, have been separated and combined into a single standalone library, which can be leveraged to bring gainz to the Dreamcast and SuperH communities at large.

NOTE: <i>This repo is still undergoing rapid changes as we pull in these routines from various places, get them exposed via both C and C++ APIs, and get them unit tested. The majority of the content has now been tested and verified in shipping games; however, not everything has been fully tested, as we are not yet to version 1.0.</i>

# Features
- Dual modern C23 and modern C++23 APIs
- Clean header files, separating interface from implementation
- No external dependencies (can be used with KallistiOS, SH Linux, etc)
- Meticulously optimized for latest SH-GCC version 15.x
- Rigorously unit tested and validated on physical HW

# APIs
- **Preprocessor** directives controlling optimizations
- **Intrinsics** for SH4 instructions not emitted by GCC
- **Memory** routines (memcpy(), memset(), memmove(), etc)
- **Floating-point Environment** control
- **Scalar** math operations, including faster `<math.h>` replacements
- **Trigonomety** functions
- **Vector** types: 2D, 3D, 4D
- **Matrix** types: 2x2, 3x2, 3x3, 3x4, 4x3, and 4x4
- **Quaternion** math operations
- **XMTRX** API for manipulating 4x4 back-bank of FP registers
- **Miscellaneous** optimized routines

# Examples

## C23
For C code, include the header files with the `.h` extensions or `sh4zam/shz_sh4zam.h` to include everything.

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

NOTE: <i>Where applicable, the C API also includes a variety of type-generic routines which will automatically resolve to calling the proper functions based on argument type. for example, `shz_vec_dot()` will automatically forward to the proper routine based on the type of vectors passed to it.</i>

## C++23
For C++ code, include the header files with the `.hpp` extensions or `sh4zam/shz_sh4zam.hpp` to include everything.

```cxx
#include <sh4zam/shz_sh4zam.hpp>

int main(int argc, const char* argv[]) {
    shz::vec4 vec1(2.0f, 3.0f, 4.0f, 1.0f);
    shz::vec4 vec2 = shz::vec4(vec1 * shz::sinf(shz::pi_f)).direction();
    shz::mat4x4 mat {};

    shz::xmtrx::init_diagonal(vec2.x, vec2.y, vec2.z, vec2.w);
    shz::xmtrx::apply_rotation_x(vec1.dot(vec2));
    shz::xmtrx::apply_translation(vec1.x, vec2.y, vec2.z);
    shz::xmtrx::store(&mat);

    shz::vec4 vec3 = shz::xmtrx::transform(vec2);

    return 0;
}
```

NOTE: <i>C++ can still use the C API by design, and every C++ type is also compatible with its corresponding C types and C API, so you can mix and match.</i>

# Installation

## CMake
To quickly build the project with CMake in any environment, run the following from the repo root:
```
mkdir build
cd build
cmake ..
make
make install
```
This will build and install the statically linked library as well as the public headers.

If you would like to also build and run the unit tests, include `-DSHZ_ENABLE_TESTS=on` within the `cmake` command. Now a separate binary for the unit test executable should be built as well.

NOTE: <i>For KOS users, use `kos-cmake` instead of your system `cmake` command!</i>

## Make (KallistiOS)
A `Makefile` has been included to provide a convenient way to install and use SH4ZAM within the standard [KallistiOS environment](https://github.com/KallistiOS/KallistiOS). See [The Wiki](https://dreamcast.wiki/Getting_Started_with_Dreamcast_development) for a guide on how to set up KallistiOS.

Simply run `make <target>` from the root of the repo, optionally passing in one of the following targets:

Target      | Description
------------|------------
`all`       | Builds everything
`clean`     | Cleans everything
`lib`       | Builds just libsh4zam.a
`tests`     | Builds unit test ELF
`run`       | Builds and runs unit tests using ${KOS_LOADER}
`flycast`   | Builds and runs unit tests using Flycast emulator
`install`   | Installs library as a KOS addon
`uninstall` | Uninstalls library from KOS addons
`docs`      | Builds Doxygen documentation
`update`    | Updates git repo to latest

# Contributors

Special thanks to the following individuals who have either directly or indirectly contributed to SH4ZAM:
- Falco Girgis
- Paul Cercueil
- Oleg Endo
- jnmartin64
- Stefanos Kornilios Mitsis Poiitidis
- MoopTheHedgehog
- Mikael Kalms
- Twada
- TapamN
- UnknownShadow
- yomboprime
- David Reichelt
- Luke Benstead
- Ian Michael
- HailToDodongo
- Jesse Stojan
- Jamie Hyland
- Aleios
- John Brooks
