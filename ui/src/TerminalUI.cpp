#include "TerminalUI.hpp"
#include <unistd.h>
#include <string>

void TerminalUI::drawBoard(const Board & /*board*/) {
    renderer_.draw();
}

void TerminalUI::showResult(int8_t winner) {
    term_.moveCursor(Board::N + 6, 1);
    term_.clearLine();
    if (winner == 0)
        term_.writeLine("Draw!");
    else
        term_.writeLine(std::string("Player ") + (winner == 1 ? "●" : "○") + " wins!");
}
