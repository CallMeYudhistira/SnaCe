# 🐍 SnaCe — Snake Game Built in C

A classic Snake game that runs entirely in the terminal, built with pure C (C11 standard). No game engine, no external libraries — just standard C and platform APIs.

## ✨ Features

- **Classic Snake gameplay** — eat food, grow longer, don't crash!
- **Real-time keyboard input** — responsive WASD + arrow key controls
- **Main menu system** — Start Game, How to Play, High Score, Exit
- **3 difficulty levels** — Easy, Normal, Hard with increasing speed
- **Progressive difficulty** — speed increases as your score grows
- **Persistent high score** — saved to `data/highscore.dat`
- **Pause & resume** — press ESC during gameplay
- **Cross-platform** — works on Windows (`conio.h`) and Linux/macOS (`termios`)
- **Clean, modular code** — separated into logical modules

## 📸 Preview

```
  ================================
         C CLI SNAKE GAME
  ================================

    1. Start Game
    2. How to Play
    3. High Score
    4. Exit

  Select option:
```

```
+--------------------------------+
| Score: 40                      |
|--------------------------------|
|                                |
|         oooO                   |
|                                |
|                    *           |
|                                |
|                                |
+--------------------------------+
```

## 🛠️ Requirements

- **C Compiler**: GCC, Clang, or any C11-compatible compiler
- **OS**: Windows, Linux, or macOS
- **Make** (optional): GNU Make for using the Makefile

## 🚀 Getting Started

### Installing a C Compiler

**Windows:**
```bash
# Option 1: Install MinGW-w64 via winget
winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT

# Option 2: Install Dev-C++ (includes MinGW)
# Download from: https://sourceforge.net/projects/orwelldevcpp/

# Option 3: Use MSYS2
# Download from: https://www.msys2.org/
pacman -S mingw-w64-x86_64-gcc
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt update && sudo apt install gcc make
```

**macOS:**
```bash
xcode-select --install
```

### Compiling

**Using Make:**
```bash
make            # Build the game
make run        # Build and run
make debug      # Build with debug symbols
make clean      # Remove build artifacts
```

**Manual compilation (GCC):**
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Isrc \
    src/main.c src/game.c src/snake.c src/food.c src/input.c src/ui.c \
    -o snace
```

**Windows (without Make):**
```cmd
gcc -std=c11 -Wall -Wextra -Wpedantic -Isrc src/main.c src/game.c src/snake.c src/food.c src/input.c src/ui.c -o snace.exe
```

### Running

```bash
./snace        # Linux/macOS
snace.exe      # Windows
```

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W` / `↑` | Move Up |
| `A` / `←` | Move Left |
| `S` / `↓` | Move Down |
| `D` / `→` | Move Right |
| `ESC` | Pause / Menu |

> The snake cannot reverse direction (e.g., you can't go from UP to DOWN directly).

## 📁 Project Structure

```
SnaCe/
├── src/
│   ├── main.c       # Entry point, menu flow, game loop orchestration
│   ├── game.c       # Core game logic, state management, high score I/O
│   ├── game.h       # Game state struct, constants, difficulty settings
│   ├── snake.c      # Snake movement, direction changes, collision checks
│   ├── snake.h      # Snake struct, Position, Direction enum
│   ├── food.c       # Random food spawning logic
│   ├── food.h       # Food struct definition
│   ├── input.c      # Platform-specific keyboard input (Win/Linux/Mac)
│   ├── input.h      # Input API: get_key, wait_key, sleep
│   ├── ui.c         # All terminal rendering (menus, arena, game over)
│   └── ui.h         # UI function declarations, display characters
├── data/
│   └── highscore.dat  # Persistent high score (auto-created)
├── Makefile         # Build automation
├── LICENSE          # MIT License
├── PROMPT.md        # Original project specification
└── README.md        # This file
```

## 🏗️ Architecture

The codebase follows a modular design with clear separation of concerns:

| Module | Responsibility |
|--------|---------------|
| **main.c** | Program entry, menu navigation, game session orchestration |
| **game** | Game state initialization, update tick (move → collide → eat → score), high score persistence |
| **snake** | Snake data structure (array of positions), movement, direction validation, collision detection |
| **food** | Random food placement ensuring no overlap with snake body |
| **input** | Platform-abstracted non-blocking keyboard input and sleep |
| **ui** | All terminal output: menus, arena rendering, game over screen |

**Key design decisions:**
- `struct` for Position, Snake, Food, GameState — no global mutable state
- `enum` for Direction, GameStatus, Difficulty — no magic numbers
- Platform-specific code isolated in `input.c` with `#ifdef _WIN32`
- Snake body stored as array of positions; head at index 0

<!-- ## 🗺️ Roadmap

- [ ] Add color support (ANSI escape codes)
- [ ] Add special food types (bonus points, speed boost)
- [ ] Add walls/obstacles in higher difficulties
- [ ] Add a two-player mode
- [ ] Add sound effects (terminal bell or platform audio)
- [ ] Add a scoreboard with player names
- [ ] Improve rendering with double-buffering to reduce flicker
- [ ] Add unit tests for game logic -->

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

*Built as a C learning project. Contributions and suggestions welcome!*
