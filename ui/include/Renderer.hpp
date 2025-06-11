// Renderer.hpp
#pragma once

#include "Board.hpp"
#include "Terminal.hpp"

class Renderer {
    Terminal &term_;
    const Board &board_;
    int cur_row_, cur_col_;

public:
    Renderer(Terminal &term, const Board &board);

    // Render the board
    void draw();

    // Handle cursor movement command, return true if only moved (no placement)
    bool handleKey(TermKey key);

    // Get current cursor position
    std::pair<int, int> cursor() const;
};
