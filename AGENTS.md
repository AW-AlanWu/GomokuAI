# Agent Guide

This repository contains a small terminal-based Gomoku game written in C++20.  
The directory layout is:

```
core/                  # Board logic and GameLogic engine
players/               # Human and AI players
players/strategies/    # Search algorithms (Negamax)
ui/                    # Terminal I/O helpers
src/                   # main.cpp
tests/                 # unit tests
Makefile               # build rules
```

Key pieces:

* **Board** – manages a 15x15 grid and win detection.
* **GameLogic** – runs a match between two `IPlayer` instances.
* **Renderer** and **Terminal** – draw the board and read keys.
* **TerminalUI** and **TerminalInput** – UI helpers for `HumanPlayer`.
* **AIPlayer** – uses an iterative deepening negamax search with a transposition table.

## Building

Run `make` to produce `bin/gomoku`; use `make clean` to remove artifacts.  
`./gen_compile_commands.sh` (requires `bear`) rebuilds the project and writes
`compile_commands.json` for tools.  Launch the game via:

```
./bin/gomoku [--p1=ai] [--p2=ai]
```

## Testing

Execute `make test` to build all programs in `tests/` and run them.  Always run
these tests before committing.

## Code Style

* 4‑space indentation, no tabs.
* `#pragma once` in headers.
* Opening braces on the same line.
* PascalCase for types, camelCase for functions and variables.
* `.clang-format` enforces a 100‑column limit.

## Miscellaneous

* Never commit generated files such as `bin/`, `build/`, `.cache/`, `.vscode/`
  or `compile_commands.json`.
* New features should come with matching tests under `tests/`.
