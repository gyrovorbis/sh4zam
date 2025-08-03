<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/gyrovorbis/sh4zam">
    <img src="http://sh4zam.elysianshadows.com/logo.png" alt="SH4ZAM">
  </a>
  <h3 align="center"><strong>SH4ZAM!</strong></h1>
  <p align="center">
    Fast math library for the Sega Dreamcast's SH4 CPU.
    <br />
    <a href="http://sh4zam.elysianshadows.com"><strong>Explore the docs »</strong></a>
  </p>
</div>
<hr>
NOTE: <i>This repo is still undergoing rapid changes as we pull in these routines from various places, get them exposed via both C and C++ APIs, and get them unit tested. It is not advised that you use it just yet, as we have yet to rigorously validate things; however, please do open tickets if you are in need of any math routines or operations that you aren't seeing as part of the API!</i>

# Overview

`Sh4zam` is a hand-optimized, general-purpose math and linear algebra library for harnessing the floating-point power of the SH4 processor in the Sega Dreamcast. It originally began as a collection of inline assembly routines developed specifically to accelerate the math in the Grand Theft Auto 3 and Vice City DC ports. These routines, along with many others from various contributors, have been separated and combined into a single standalone library, which can be leveraged to bring gainz to the Dreamcast community at large.

# Features
- Dual modern C and modern C++ APIs for everything
- Scalar Math and Trigonometry
- Vectors: 2D, 3D, 4D
- Matrices: 2x2, 3x3, 3x4, and 4x4
- Quaternions
- `XMTRX`: FP register back-bank manipulation
- Memory: optimized specialized `memcpy()` and `memset()` variants

# Installation

## KallistiOS

To quickly build the project with the latest version of the [KallistiOS SDK](https://github.com/KallistiOS/KallistiOS) installed, use the following commands from the root of the repository:

```
mkdir build
cd build
kos-cmake ..
make
make install
```
This will build and install the statically linked library as well as the public headers as a standard kos-port which should automatically work with the KOS build system.

# Contributors

Special thanks to the following individuals who have either directly or indirectly contributed to `sh4zam`:
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
