# Agent Guide

This repository implements a terminal based Gomoku (Five in a row) game in
C++20.  The project is split into a few small modules:

```
core/                  # board logic and game loop
players/               # AI/Human player implementations
players/strategies/    # AI algorithms (Negamax)
ui/                    # terminal I/O and rendering helpers
src/                   # main entry point
tests/                 # minimal test programs
Makefile               # build logic for game and tests
```

Important components:

* **Board** – manages board state and win detection.
* **GameLogic** – orchestrates turns between two `IPlayer` objects.
* **Renderer** & **Terminal** – draw the board and handle key input.
* **TerminalUI** & **TerminalInput** – user interface helpers used by
  `HumanPlayer`.
* **AIPlayer** – uses a simple heuristic and negamax search with
  alpha–beta pruning.
* **HumanPlayer** – reads arrow/enter/Q keys from the terminal.

## Building

Run `make` to produce `bin/gomoku` (the main executable).  `make clean` removes
build artefacts.  `./gen_compile_commands.sh` (requires `bear`) can generate
`compile_commands.json` for tooling.  Launch the game with `./bin/gomoku`
optionally adding `--p1=ai` and/or `--p2=ai` to enable AI players.

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

* Do not commit generated files such as `bin/`, `build/`, `.cache/`, `.vscode/`
  or `compile_commands.json`.
* When adding features, supply matching tests in `tests/`.
* The game accepts optional `--p1=ai` and `--p2=ai` flags to enable AI
  opponents.
