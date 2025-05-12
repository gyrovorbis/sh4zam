<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/gyrovorbis/sh4zam">
    <img src="doc/logo.png" alt="Sh4zam">
  </a>
  <h3 align="center"><strong>Sh4zam</strong></h1>
  <p align="center">
    Fast math library for the Sega Dreamcast's SH4 CPU.
    <br />
  </p>
</div>
<hr>
NOTE: <i>This repo is still undergoing rapid changes as we pull in these routines from various places, get them exposed via both C and C++ APIs, and get them unit tested. It is not advised that you use it just yet, as we have yet to rigorously validate things; however, please do open tickets if you are in need of any math routines or operations that you aren't seeing as part of the API!</i>

# Overview

`Sh4zam` is a hand-optimized, general-purpose math and linear algebra library for harnessing the floating-point power of the SH4 processor in the Sega Dreamcast. It originally began as a collection of inline assembly routines developed specifically to accelerate the math in the Grand Theft Auto 3 and Vice City DC ports. These routines, along with many others from various contributors, have been separated and combined into a single standalone library, which can be leveraged to bring gainz to the Dreamcast community at large.

# Features

- floating-point math intrinsics
- API for directly manipulating the `XMTRX` FP registers
- 2D, 3D, 4D vector APIs
- 2x2, 3x3, 3x4, and 4x4 matrix APIs
- quaternion API

# Contributors

Special thanks to the following individuals who have either directly or indirectly contributed to `sh4zam`:
- Falco Girgis
- Paul Cercuei
- Oleg Endo
- jnmartin64
- skmp
- MoopTheHedgehog
- Mikael Kalms
- Twada
- yomboprime
- freakdave
- Ian Michael
- HailToDodongo
