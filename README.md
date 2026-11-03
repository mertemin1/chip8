# CHIP-8 Emulator & Virtual Machine 👾

**Developer:** Mert Emin İlhan  
**Repository:** [https://github.com/mertemin1/chip8](https://github.com/mertemin1/chip8)  

A cycle-accurate CHIP-8 virtual machine and interpreter written from scratch in **C**, utilizing **SDL3** for hardware-accelerated 2D graphics rendering and real-time keyboard input handling. This project emulates the classic CHIP-8 architecture, enabling the execution of original `.ch8` game and test ROMs.

[![Language: C](https://img.shields.io/badge/Language-C99-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Library: SDL3](https://img.shields.io/badge/Library-SDL3-orange.svg)](https://github.com/libsdl-org/SDL)

---

## ✨ Technical Features & Architecture

- **Full Instruction Set Emulation:** Implements all **35 standard opcodes** of the CHIP-8 instruction set (arithmetic, bitwise operations, conditional branching, subroutine calls, and sprite drawing).
- **Virtual Hardware Architecture:**
  - **Memory:** Complete emulation of the 4KB (4096 bytes) RAM layout, loading ROM instructions starting at memory address `0x200` and built-in hexadecimal font sprites in reserved lower memory.
  - **Registers:** 16 general-purpose 8-bit data registers (`V0`–`VF`, with `VF` serving as the carry/borrow and sprite collision flag), a 16-bit index register (`I`), and a Program Counter (`PC`).
  - **Stack Management:** 16-level stack array with a dedicated Stack Pointer (`SP`) to manage nested subroutine calls (`0x2NNN`) and returns (`0x00EE`).
- **64x32 Monochrome Framebuffer:** Accurately emulates the 64x32 pixel XOR-based display buffer with pixel-collision detection and configurable window scaling via SDL3 (`SDL_INIT_VIDEO`).
- **Hardware Timers:** Implements 60Hz tick decrements for both the `delayTimer` and `soundTimer` registers.
- **Hexadecimal Keypad Mapping:** Maps modern QWERTY keyboard events (`SDL_Event`) to the original 16-key hexadecimal (`0x0`–`0xF`) CHIP-8 input layout.

---

## 🛠️ Tech Stack

- **Language:** C
- **Graphics & Input API:** SDL3 (`SDL_Init(SDL_INIT_VIDEO)`)
- **IDE / Build Support:** Visual Studio Solution (`.sln`, `.vcxproj`) & GCC/Clang CLI compatible

---

## 🚀 Building the Project

To compile and run the emulator, ensure you have a C compiler and the **SDL3** development libraries installed.

### Option 1: Using GCC (Linux / MinGW)
Clone the repository and compile `Source.c` by linking against SDL3:

```bash
git clone [https://github.com/mertemin1/chip8.git](https://github.com/mertemin1/chip8.git)
cd chip8/chip8

# Compile with GCC (adjust include/library paths if SDL3 is installed in a custom directory)
gcc Source.c -o chip8_emulator -lSDL3