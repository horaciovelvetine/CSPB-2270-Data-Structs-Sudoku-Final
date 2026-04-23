# CSPB-2270-Data-Structs-Sudoku-Final

This is a final project for the 2270 Data Structures class as a part of the computer science program at the University of Colorado Boulder. The structure of the project is an open ended exploration of any data structure or algorithm in C++.

# Goal

Determine the number of valid layouts of a Sudoku puzzle exist. Sudoku's are a puzzle consisting of a 3 by 3 grid of squares each containing 9 cells, also arranged in 3 by 3. Each cell contains a single number 1 through 9, and each square must contain all of the numbers only a single time. A valid Sudoku board doesn't allow duplicates cell values in any row or column spanning 3 squares horizontally or vertically.

## Build & run

From the project root, configure an out-of-source build in `build/`, compile the `sudoku-structs` executable, then run it.

**One line (copy everything below):**

```sh
cmake -S . -B build && cmake --build build && ./build/sudoku-structs
```

- `cmake -S . -B build` — generates build files under `build/`
- `cmake --build build` — compiles the project
- `./build/sudoku-structs` — runs the binary (macOS/Linux)

