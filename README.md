<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/gyrovorbis/sh4zam">
    <img src="http://sh4zam.falcogirgis.net/logo.png" alt="SH4ZAM">
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
- **Trigonometry** functions
- **Vector** types: 2D, 3D, 4D
- **Matrix** types: 2x2, 3x2, 3x3, 3x4, 4x3, and 4x4
- **Quaternion** math operations
- **XMTRX** API for manipulating 4x4 back-bank of FP registers
- **Miscellaneous** optimized routines

# Usage

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

## KallistiOS
SH4ZAM is now an official part of [kos-ports](https://github.com/KallistiOS/kos-ports), meaning that it is now directly integrated within the [KallistiOS](https://github.com/KallistiOS/KallistiOS) ecosystem and will be automatically set up as part of the regular KOS installation process. If you're following the standard community guide for [Setting up KallistiOS](https://dreamcast.wiki/Getting_Started_with_Dreamcast_development), SH4ZAM will be installed as part of the [Building KOS Ports](https://dreamcast.wiki/Getting_Started_with_Dreamcast_development#Building_kos-ports) step. If you're using [DreamSDK](https://dreamsdk.org/) to manage your KallistiOS development environment, SH4ZAM may be installed from the `KallistiOS Ports` tab.


## CMake
To quickly build the project with CMake in any environment, independently of KOS, run the following from the repo root:
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

# Examples
Examples can be found within the `example` subdirectory. New examples are always welcome to be contributed!

NOTE:  <i>If you've installed SH4ZAM through KOS-ports, the examples should get installed to `kos-ports/examples/sh4zam`.</i>

Example       | Description
--------------|------------
Bruce's Balls | Pushes up to 4.5 million PPS, rendering Bruce's balls.

# Projects
The following projects have been successfully accelerated with SH4ZAM and make great references:

Project                                                   | Description
----------------------------------------------------------|------------
[SH4ZAM PVR](https://github.com/dfchil/sh4zam_pvr)        | Advanced PVR DR rendering examples with SH4ZAM acceleration.
[Mario Kart 64 DC](https://github.com/jnmartin84/mk64-dc) | jnmartin's Mario Kart 64 DC port with SH4ZAM acceleration.
[Star Fox 64 DC](https://github.com/jnmartin84/sf64-dc)   | jnmartin's Star Fox 63 DC port with SH4ZAM acceleration.

# Conventions
The following conventions are used throughout the API by default, unless otherwise specified:

Convention   | Description
-------------|------------
Coordinates  | Right-handed for world/view space, Left-handed for screen/clip-space, as with GL.
Matrices     | Stored in column-major order, as with GL.
Quaternions  | Stored in `<W, X, Y, Z>` order.

# Community
The `#sh4zam` channel within the [Simulant Discord Server](https://discord.gg/G2Ay9kxec2) for Dreamcast development is the best place to ask questions, get help, give feedback, or discuss anything relating to SH4ZAM.

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
- Daniel Fairchild
- BruceLeet
- Eric Fradella
- Donald Haase
