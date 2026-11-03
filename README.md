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
- **64x32 Monochrome Framebuffer:** Accurately emulates the 64x32 pixel XOR-based display buffer with pixel-collision detection and window rendering via SDL3 (`SDL_INIT_VIDEO`).
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
git clone https://github.com/mertemin1/chip8.git
cd chip8/chip8

# Compile with GCC (adjust include/library paths if SDL3 is installed in a custom directory)
gcc Source.c -o chip8_emulator -lSDL3
```

### Option 2: Using Visual Studio (Windows)
1. Open `chip8/chip8.sln` in Visual Studio.
2. Verify that the SDL3 `include` and `lib` directories are configured in the project properties.
3. Build and run the solution in `Release` or `Debug` mode.

---

## 📖 Usage & Controls

### Loading a ROM
The repository includes a collection of classic `.ch8` ROMs inside the `roms/` directory (e.g., `pong.ch8`, `ibmlogo.ch8`, `blitz.ch8`, `keypadtest.ch8`).

1. Run the compiled executable from the directory containing the `roms/` folder:
   ```bash
   ./chip8_emulator
   ```
2. Type the path/filename of the ROM into the standard input (`stdin`) and press **Enter**:
   ```text
   roms/pong.ch8
   ```

### Keypad Mapping
The original 16-key hexadecimal CHIP-8 keypad is mapped to the left side of a standard QWERTY keyboard in a 4x4 grid:

| Original CHIP-8 Keypad | QWERTY Keyboard Mapping |
| :---: | :---: |
| `1` `2` `3` `C` | `1` `2` `3` `4` |
| `4` `5` `6` `D` | `Q` `W` `E` `R` |
| `7` `8` `9` `E` | `A` `S` `D` `F` |
| `A` `0` `B` `F` | `Z` `X` `C` `V` |

---

## 📁 Project Structure

```text
chip8/
├── Source.c                 # Core emulator implementation (CPU cycle, opcodes, memory & SDL3 loop)
├── chip8.sln                # Visual Studio Solution file
├── chip8.vcxproj            # Visual Studio Project configuration
├── chip8.vcxproj.filters    # Visual Studio filters
└── roms/                    # Sample CHIP-8 ROM binaries for testing
    ├── airplane.ch8
    ├── animalrace.ch8
    ├── blitz.ch8
    ├── dangerous.ch8
    ├── ibmlogo.ch8
    ├── keypadtest.ch8
    ├── pong.ch8
    └── test.ch8
```

---

## 📌 Roadmap & Future Improvements

- **Audio Output:** Bind the `soundTimer` register decrement logic to an active SDL3 audio stream to generate a square-wave beep tone when the timer is non-zero.
- **Execution State Toggle:** Hook the `PAUSED` state machine flag into the main instruction fetch-decode-execute cycle to support live pausing via the `SPACE` key.
- **CLI Arguments:** Support passing the ROM file path directly via `argv[1]` alongside `stdin`.

---

## 📄 License

This project is open-source and available under the MIT License.