<div align="center">
  <a href="https://www.sh4zam.com">
    <img src="http://sh4zam.falcogirgis.net/logo.png" alt="SH4ZAM" />
  </a>
  <h1 align="center">SH4ZAM!</h1>
  <p align="center">
    Fast math library for the Sega Dreamcast's SH4 CPU
    <br />
    <a href="https://www.sh4zam.com"><strong>Explore the docs »</strong></a>
  </p>
</div>

<hr />

# Overview

**SH4ZAM** is a hand-optimized, general-purpose math and linear algebra library designed to fully harness the floating-point power of the SH4 processor in the Sega Dreamcast.

It originally began as a collection of inline assembly routines developed to accelerate math operations in the Grand Theft Auto 3 and Vice City Dreamcast ports. These routines—along with contributions from the community—have been unified into a standalone library.

---

# Features

- Dual, modern, interoperable **C17** and **C++20** APIs  
- Additional features for **C23** and **C++23**  
- Ergonomic, zero-overhead composition  
- Clean headers with strict interface/implementation separation  
- `constexpr` evaluation support  
- Zero-cost interop conversions  
- Optimized for **SH-GCC 15.x**  
- Tested on real Dreamcast hardware  
- Fully documented headers + Doxygen site  
- Software backend for cross-platform usage  

---

# APIs

SH4ZAM provides:

- **Preprocessor** optimization controls  
- **Intrinsics** for SH4-specific instructions  
- **Memory routines** (`memcpy`, `memset`, `memmove`, etc.)  
- **Floating-point environment** control  
- **Scalar math** (faster `<math.h>` alternatives)  
- **Trigonometry** (including inverse trig)  
- **Vector types** (2D, 3D, 4D)  
- **Matrix types** (2x2 → 4x4)  
- **Quaternion math**  
- **XMTRX API** (FP register matrix manipulation)  
- **Complex math + FFT acceleration**  
- Miscellaneous optimized utilities  

🔗 Full API reference: https://www.sh4zam.com

---

# Usage

## C23

```c
#include <sh4zam/shz_sh4zam.h>

int main(int argc, const char *argv[]) {
    shz_vec4_t vec1 = shz_vec4_init(2.0f, 3.0f, 4.0f, 1.0f);
    shz_vec4_t vec2 = shz_vec4_normalize(
        shz_vec4_scale(vec1, shz_sinf(SHZ_F_PI))
    );
    shz_mat4x4_t mat = {};

    shz_xmtrx_init_diagonal(vec2.x, vec2.y, vec2.z, vec2.w);
    shz_xmtrx_apply_rotation_x(shz_vec4_dot(vec1, vec2));
    shz_xmtrx_apply_translation(vec1.x, vec1.y, vec1.z);
    shz_xmtrx_store_4x4(&mat);

    shz_vec4_t vec3 = shz_xmtrx_transform_vec4(vec2);

    return 0;
}
```

> The C API supports type-generic dispatch.  
> Example: `shz_vec_dot()` resolves automatically based on input types.

---

## C++23

```cpp
#include <sh4zam/shz_sh4zam.hpp>

int main(int argc, const char* argv[]) {
    shz::vec4 vec1(2.0f, 3.0f, 4.0f, 1.0f);
    shz::vec4 vec2 = (vec1 * shz::sinf(shz::pi_f)).direction();
    shz::mat4x4 mat {};

    shz::xmtrx::init_diagonal(vec2.x, vec2.y, vec2.z, vec2.w);
    shz::xmtrx::apply_rotation_x(vec1.dot(vec2));
    shz::xmtrx::apply_translation(vec1.x, vec2.y, vec2.z);
    shz::xmtrx::store(&mat);

    shz::vec4 vec3 = shz::xmtrx::transform(vec2);

    return 0;
}
```

> C++ types are fully compatible with the C API—mix and match freely.

---

# Installation

## KallistiOS

SH4ZAM is part of **kos-ports** and integrates directly with the KallistiOS ecosystem.

- Installed automatically during KOS setup  
- Available via **DreamSDK**  
- Included in `kos-ports/examples/sh4zam`

📘 Setup guide:  
https://dreamcast.wiki/Getting_Started_with_Dreamcast_development  

---

## CMake

```bash
mkdir build
cd build
cmake ..
make
make install
```

Enable tests:

```bash
cmake -DSHZ_ENABLE_TESTS=on ..
```

> KOS users: use `kos-cmake` instead of `cmake`.

---

# Examples

Examples are located in the `example/` directory.

| Example        | Description |
|----------------|------------|
| Bruce's Balls  | Renders up to 4.5 million PPS |
| PVR DMA        | Complex real-world renderer |

---

# Projects Using SH4ZAM

| Project | Description |
|--------|------------|
| [SH4ZAM PVR](https://github.com/dfchil/sh4zam_pvr) | Advanced PVR rendering |
| [Mario Kart 64 DC](https://github.com/jnmartin84/mk64-dc) | MK64 Dreamcast port |
| [Star Fox 64 DC](https://github.com/jnmartin84/sf64-dc) | SF64 Dreamcast port |
| [Super Mario 64 DC](https://github.com/jnmartin84/sm64-dc) | SM64 Dreamcast port |

---

# Conventions

| Convention  | Description |
|------------|------------|
| Coordinates | Right-handed (world), left-handed (screen) |
| Matrices    | Column-major |
| Quaternions | `<W, X, Y, Z>` |

---

# Community

Join the **#sh4zam** channel on the Simulant Discord:

https://discord.gg/G2Ay9kxec2  

Or visit: https://www.sh4zam.com

---

# Contributors

Falco Girgis, 
Paul Cercueil, 
Oleg Endo, 
jnmartin64,  
Stefanos Kornilios Mitsis Poiitidis, MoopTheHedgehog,  
Mikael Kalms, 
Twada, 
TapamN, 
UnknownShadow, 
yomboprime,  
David Reichelt, 
Luke Benstead, 
Ian Michael, 
HailToDodongo,  
Jesse Stojan, 
Jamie Hyland, 
Aleios, 
John P, 
Daniel Fairchild,  
BruceLeet, 
Eric Fradella, 
Donald Haase

---

# 🔗 Links

- 🌐 Website: https://www.sh4zam.com  
- 📘 Docs: https://www.sh4zam.com  
- 💬 Community: https://discord.gg/G2Ay9kxec2
