---
title: Resources
description: SH4 instruction set references, manuals, ABI docs, pipeline simulators and optimization guides for Dreamcast development
---

The following is a compilation of additional resources for low-level development and optimizations targeting the SH4 processor. Many of these were used to aid in the development of SH4ZAM itself. The list is upstream's, from `doc/resources.dox`.

| Link | Description |
| --- | --- |
| [SH Instruction Set Summary](https://shared-ptr.com/sh_insns.html) | Easy-to-navigate instruction set reference for the SuperH processor line. |
| [Compiler Explorer Templates](https://dreamcast.wiki/SH4_in_Compiler_Explorer) | Preconfigured templates for viewing SH4 disassembly from KOS-based C/C++ source code within the web browser. |
| [SH4 Programming Manual](https://0x04.net/~mwk/doc/sh/e602156_sh4.pdf) | Detailed information on the SH4's instruction set. |
| [SH4A Hardware Manual](https://www.renesas.cn/zh/document/mah/sh-4a-core-extended-functions-users-manual-hardware) | Detailed information about the SH4's peripherals and HW. |
| [SH4 C ABI Specification](https://www.st.com/resource/en/reference_manual/rm0197-sh4-generic-and-c-specific-application-binary-interface-stmicroelectronics.pdf) | Documents the C binary interface and calling convention. |
| [SH4 Pipeline Simulator](https://sh4-sim.dreamcast.wiki/) | Low-level SH4 pipeline simulator for manually scheduling instructions. |
| [SH4 Optimization Tips](https://dreamcast.wiki/Development#Optimization) | Dreamcast.wiki's list of useful optimization tips. |
| [Reversing SH4 Vector Instructions](https://drk.emudev.org/blog/sh4-fpu/) | Deep-dive into how FIPR, FTRV, FSCA, and FSRRA actually work on the HW. |
| [SH4 Vector FPU Calculator](https://drk.emudev.org/sh4-vector-calculator/) | Interactive calculator, visualizing the circuitry of the SH4 VFPU. |
| [SuperH Manuals](https://github.com/KitsunebiGames/SuperH-Manuals) | GitHub repository containing a collection of manuals for the SuperH architecture. |

## On this site

- [The SH4 FPU](/concepts/sh4-fpu/): a short glossary of `FIPR`, `FTRV`, `FSCA`, `FSRRA`, XMTRX, store queues and prefetch.
- [Optimization Tips](/guides/optimization/): compiler flags and FPU precision modes.
- [Getting Started with Dreamcast development](https://dreamcast.wiki/Getting_Started_with_Dreamcast_development): the community guide to setting up KallistiOS.

## Related

- [The SH4 FPU](/concepts/sh4-fpu/)
- [Optimization Tips](/guides/optimization/)
