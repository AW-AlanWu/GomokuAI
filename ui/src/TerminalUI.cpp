#include "TerminalUI.hpp"
#include <unistd.h>
#include <iostream>
#include <string>

void TerminalUI::drawBoard(const Board & /*board*/) {
    renderer_.draw();
}

void TerminalUI::showResult(int8_t winner) {
    term_.moveCursor(Board::N + 6, 1);
    term_.clearLine();
    if (winner == 0)
        std::cout << "Draw!\n";
    else
        std::cout << "Player " << (winner == 1 ? "●" : "○") << " wins!\n";
}
