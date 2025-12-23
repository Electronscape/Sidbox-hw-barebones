# SidBox 5.6 Firmware

SidBox 5.6 is a custom embedded hardware platform built around the STM32H7
series, designed for audio, graphics, and real-time experimentation
(emulation, UI, and hardware-driven fun).

This repository contains the **base firmware and hardware abstraction layer**
for the SidBox 5.6 system.

---

## Why a Rewrite?

The original SidBox firmware *works*, but over time it became:

- difficult to extend
- hard to reason about
- tightly coupled in places it shouldn’t be
- full of “it works, don’t touch it” logic

This version is a **clean re-architecture**, not a feature-for-feature port.

The goals of this rewrite are:
- clearer hardware abstractions
- deterministic timing where it matters
- clean memory and DMA usage
- a structure that allows future expansion without turning into spaghetti

Things **will** break during development — that’s expected.
The focus is correctness, clarity, and long-term maintainability.

---

## Hardware Overview

- MCU: STM32H743 (Cortex-M7)
- External SDRAM (used for VRAM and application memory)
- DMA2D-based graphics pipeline
- LCD driven via FMC (8080-style bus)
- UART for debug and system I/O
- Designed with real-time graphics and audio workloads in mind

---

## Graphics System (Current)

The graphics pipeline is built around:
- Double-buffered VRAM
- DMA2D for fast framebuffer transfer and blending
- Cache-aware buffer management
- VSYNC-synchronised frame updates

The system prioritises:
- tear-free output
- predictable timing
- allowing the CPU to prepare the next frame while DMA is active

---

## Project Status

This is **active development**.

### Implemented
- [x] UART (debug + basic I/O)
- [x] LCD driver (FMC-based)
- [x] DMA2D graphics engine
- [x] Double-buffered framebuffer
- [x] VSYNC-synchronised rendering
- [x] Cache-coherent VRAM handling
- [X] SDCard File I/O access
- [X] Audio using either DMA or Interrupt (undecided)
- [x] Higher-level graphics primitives
- [x] Mouse and Joystick port
- [x] Demo CRASH reporter
- [x] Audio capture (sampling) (working in basic feed back form)
- [X] Improved debug and diagnostics
- [X] Basic command line instructions


### In Progress
- [x] Language selection
  - [x] Ongoing English
  - [ ] French


### Planned
- [ ] OS-level services
- [ ] Performance profiling tools

---

## Design Notes

- Performance-critical paths are documented where the code looks “odd”
- DMA and cache interactions are handled explicitly
- Comments focus on *why* something exists, not just *what* it does
- Hardware quirks are documented when discovered (usually after pain)

---

## Disclaimer

This firmware is not production-ready.
It is a living codebase, and some areas may intentionally favour clarity
over completeness while systems are being brought up.

If something looks strange, there is probably a reason — and ideally a comment.

---

## Author

**Electronscape**

SidBox 5.6 Firmware  
A system built to be fast, hackable, and understood.

---

## Contributions

This is a project that is not accepting pull requests.

You are welcome to:
- read the code
- clone it
- experiment locally
- offer suggestions or feedback

All design decisions and code changes remain with me ;)
