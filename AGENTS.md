# Agent Guide

This repository implements a terminal based Gomoku (Five in a row) game in
C++20.  The code base is small and organised as follows:

```
include/   # headers
src/       # implementations
tests/     # minimal test programs
Makefile   # build logic for game and tests
```

Important components:

* **Board** – manages board state and win detection.
* **Renderer** & **Terminal** – draw to the terminal and handle key input.
* **Game** – coordinates a match using two `Player` objects.
* **AIPlayer** – simple heuristic + negamax search with alpha–beta pruning.
* **HumanPlayer** – reads arrow/enter/Q keys from the terminal.

## Building

Run `make` to produce `bin/gomoku`.  `make clean` removes build artefacts.
`./gen_compile_commands.sh` (requires `bear`) can generate
`compile_commands.json` for tooling.

## Testing

`make test` compiles all `tests/*.cpp` into `bin/tests/` and executes them.
Always run the tests before committing.

## Code Style

* 4 space indentation with no tabs.
* Headers use `#pragma once`.
* Opening braces on the same line.
* Naming: classes/enums in PascalCase, functions/variables in camelCase,
  macros in ALL_CAPS, namespaces in lower_snake_case.
* `.clang-format` defines formatting (100 column limit).

## Miscellaneous

* Do not commit generated files such as `bin/`, `build/`, `.cache/` or
  `compile_commands.json`.
* When adding features, supply matching tests in `tests/`.
* The game accepts optional `--p1=ai` and `--p2=ai` flags to enable AI
  opponents.
