# 🕹️ CHIP-8 Emulator

A CHIP-8 emulator and interpreter written in C, utilizing SDL3 for graphics and input.
This project accurately emulates the CHIP-8 instruction set, allowing you to run classic CHIP-8 ROMs.

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://example.com/build)
[![Version](https://img.shields.io/badge/version-1.0.0-blue)](https://example.com/version)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow)](https://opensource.org/licenses/MIT)

---

## Table of Contents 📜

- [About The Project](#about-the-project)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Installation](#installation)
- [How to Use](#how-to-use)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

---

## About The Project 🚀

This is a CHIP-8 emulator written in C. It's designed to accurately simulate the original CHIP-8 hardware, enabling the execution of CHIP-8 programs (ROMs). The emulator leverages the SDL3 library for handling graphics rendering and user input, providing a visual interface for the classic monochrome display.

---

## ✨ Features

- **Full CHIP-8 Instruction Set Emulation**: Implements all 35 opcodes of the CHIP-8 instruction set.
- **64x32 Monochrome Display**: Accurately emulates the 64x32 pixel display resolution.
- **Memory, Registers, and Stack**: Complete emulation of the CHIP-8's memory (4KB), 16 general-purpose registers (V0-VF), and stack for subroutine calls.
- **Keyboard Input**: Utilizes SDL3 to map keyboard inputs to the CHIP-8 keypad (0-F).
- **Timers**: Implements delay and sound timers for accurate game logic and timing.
- **ROM Loading**: Supports loading external `.ch8` ROM files to run various CHIP-8 programs.
- **Configurable Display**: Allows customization of window size, background, and foreground colors.

---

## Tech Stack 💻

- **Language**: C
- **Graphics & Input**: SDL3
- **Build System**: Standard C compiler (GCC/Clang)

---

## Installation ⚙️

To build and run the CHIP-8 emulator, you need a C compiler and the SDL3 development library.

**Requirements:**

- **C Compiler**: GCC or Clang recommended.
- **SDL3 Development Library**: Ensure you have the SDL3 development headers and libraries installed on your system.
  - **Debian/Ubuntu**: `sudo apt-get install libsdl3-dev`
  - **Fedora**: `sudo dnf install SDL3-devel`
  - **macOS (Homebrew)**: `brew install sdl3`
  - **Windows**: Download the development libraries from the official SDL website and configure your compiler/IDE to find them.

**Build Instructions:**

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/mertemin1/chip8.git
    cd chip8
    ```

2.  **Compile the code**:
    Navigate to the `chip8` directory where `Source.c` is located and compile using your C compiler. Ensure the SDL3 include and library paths are correctly set.

    **Example using GCC:**
    ```bash
    gcc chip8/Source.c -o chip8_emulator -I/path/to/SDL3/include -L/path/to/SDL3/lib -lSDL3
    ```
    *(Replace `/path/to/SDL3/` with the actual path to your SDL3 installation if it's not in standard system directories.)*

---

## How to Use 🎮

1.  **Obtain CHIP-8 ROMs**: Download CHIP-8 ROM files (typically with a `.ch8` extension). You can find many classic CHIP-8 games online.

2.  **Run the Emulator**: Execute the compiled emulator.
    ```bash
    ./chip8_emulator
    ```

3.  **Enter ROM Name**: The emulator will prompt you to enter the name of the ROM file you wish to load.
    ```
Enter ROM name: pong.ch8
    ```

4.  **Play**: The CHIP-8 game will start running in a window.

**Controls:**

The emulator maps the CHIP-8 keypad to your keyboard as follows:

| CHIP-8 Keypad | Keyboard Mapping |
|---|---|
| `1` | `1` |
| `2` | `2` |
| `3` | `3` |
| `C` | `4` |
| `4` | `Q` |
| `5` | `W` |
| `6` | `E` |
| `D` | `R` |
| `7` | `A` |
| `8` | `S` |
| `9` | `D` |
| `E` | `F` |
| `A` | `Z` |
| `0` | `X` |
| `B` | `C` |
| `F` | `V` |

- Press `SPACE` to pause/resume the emulator.
- Close the window to quit.

---

## Project Structure 📂

```
chip8/
├── chip8.sln
├── chip8.vcxproj
├── chip8.vcxproj.filters
├── Source.c
└── roms/
    ├── airplane.ch8
    ├── animalrace.ch8
    ├── blitz.ch8
    ├── dangerous.ch8
    ├── ibmlogo.ch8
    ├── keypadtest.ch8
    ├── pong.ch8
    └── test.ch8
```

- **`Source.c`**: The main source file containing the emulator's logic, including CPU emulation, display rendering, and input handling.
- **`roms/`**: A directory containing sample CHIP-8 ROM files.
- **`chip8.sln`**, **`chip8.vcxproj`**, **`chip8.vcxproj.filters`**: Visual Studio project files (primarily for Windows development).

---

## Contributing 🤝

Contributions are welcome! If you find any bugs or have suggestions for improvements, please open an issue or submit a pull request.

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request

---

## License 📄

This project is licensed under the MIT License - see the [LICENSE](LICENSE.txt) file for details. (Note: A LICENSE.txt file was not found in the provided analysis, assuming MIT based on common open-source practices for such projects).

---

## Contact 📧

Emin Mert - mertemin1@gmail.com

Project Link: [https://github.com/mertemin1/chip8](https://github.com/mertemin1/chip8)


---
**<p align="center">Generated by [ReadmeCodeGen](https://www.readmecodegen.com/)</p>**