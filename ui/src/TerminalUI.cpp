#include "TerminalUI.hpp"
#include <unistd.h>
#include <iostream>
#include <string>

void TerminalUI::drawBoard(const Board &board) {
    std::string buffer;
    buffer.reserve(4096);
    buffer += "\x1b[H";   // move cursor home
    buffer += "\x1b[2J";  // clear screen
    buffer += "=================  Gomoku (五子棋) =================\n\n";
    buffer += "     ";
    for (size_t c = 1; c <= Board::N; ++c) {
        buffer += std::to_string(c);
        if (c < 10u) buffer += ' ';
        buffer += ' ';
    }
    buffer += '\n';

    auto [curRow, curCol] = renderer_.cursor();
    for (size_t r = 0; r < Board::N; ++r) {
        if (r + 1 < 10u) buffer += ' ';
        buffer += std::to_string(r + 1);
        buffer += " | ";
        for (size_t c = 0; c < Board::N; ++c) {
            bool highlight = (r == static_cast<size_t>(curRow) &&
                              c == static_cast<size_t>(curCol));
            if (highlight) buffer += "\x1b[7m";
            int cell = board.at(r, c);
            if (cell == 0)
                buffer += '.';
            else if (cell == 1)
                buffer += "\xe2\x97\x8f";  // ●
            else
                buffer += "\xe2\x97\x8b";  // ○
            if (highlight) buffer += "\x1b[0m";
            buffer += "  ";
        }
        buffer += "|\n";
    }
    buffer += "\nUse Arrow keys, Enter to place, Q to quit.\n";

    term_.writeBuffer(buffer);
}

void TerminalUI::showResult(int8_t winner) {
    term_.moveCursor(Board::N + 6, 1);
    term_.clearLine();
    if (winner == 0)
        std::cout << "Draw!\n";
    else
        std::cout << "Player " << (winner == 1 ? "●" : "○") << " wins!\n";
}
