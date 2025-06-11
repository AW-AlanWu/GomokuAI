// Renderer.cpp
#include "Renderer.hpp"

Renderer::Renderer(Terminal &t, const Board &b)
    : term_(t), board_(b), cur_row_(Board::N / 2), cur_col_(Board::N / 2) {}


void Renderer::draw() {
    std::string out;
    out.reserve(4096);
    out += "\x1b[H";   // move home before clear
    out += "\x1b[2J";  // clear screen
    out += "=================  Gomoku (五子棋) =================\n\n";
    out += "     ";
    for (int c = 1; c <= Board::N; ++c) {
        out += std::to_string(c);
        if (c < 10) out += ' ';
        out += ' ';
    }
    out += "\n";
    for (int r = 0; r < Board::N; ++r) {
        if (r + 1 < 10) out += ' ';
        out += std::to_string(r + 1);
        out += " | ";
        for (int c = 0; c < Board::N; ++c) {
            bool highlight = (r == cur_row_ && c == cur_col_);
            if (highlight) out += "\x1b[7m";
            int cell = board_.at(r, c);
            if (cell == 0)
                out += '.';
            else if (cell == 1)
                out += "●";
            else
                out += "○";
            if (highlight) out += "\x1b[0m";
            out += "  ";
        }
        out += "|\n";
    }
    out += "\nUse Arrow keys, Enter to place, Q to quit.\n";
    term_.writeBuffer(out);
}

// move cursor logically; return true if moved (not Enter/Quit)
bool Renderer::handleKey(TermKey k) {
    switch (k) {
    case TermKey::Up:
        cur_row_ = std::max(0, cur_row_ - 1);
        return true;
    case TermKey::Down:
        cur_row_ = std::min(Board::N - 1, cur_row_ + 1);
        return true;
    case TermKey::Left:
        cur_col_ = std::max(0, cur_col_ - 1);
        return true;
    case TermKey::Right:
        cur_col_ = std::min(Board::N - 1, cur_col_ + 1);
        return true;
    default:
        return false;
    }
}

std::pair<int, int> Renderer::cursor() const {
    return {cur_row_, cur_col_};
}
