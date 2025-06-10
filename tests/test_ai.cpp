#include <cassert>
#include <iostream>

#include "AIPlayer.hpp"
#include "Board.hpp"

static void winImmediately() {
    Board b;
    int row = 7;
    for (int c = 0; c < 4; ++c) b.place(row, c, 1);
    AIPlayer ai;
    auto move = ai.getMove(b, 1);
    assert(move.first == row && move.second == 4);
}

static void blockOpponent() {
    Board b;
    int row = 5;
    for (int c = 0; c < 4; ++c) b.place(row, c, -1);
    AIPlayer ai;
    auto move = ai.getMove(b, 1);
    assert(move.first == row && move.second == 4);
}

static void fullBoard() {
    Board b;
    for (int r = 0; r < Board::N; ++r)
        for (int c = 0; c < Board::N; ++c) b.place(r, c, 1);
    AIPlayer ai;
    auto move = ai.getMove(b, 1);
    assert(move.first == -1 && move.second == -1);
}

static void validMoveOnEmpty() {
    Board b;
    AIPlayer ai;
    auto move = ai.getMove(b, 1);
    assert(b.isEmpty(move.first, move.second));
}

int main() {
    winImmediately();
    blockOpponent();
    fullBoard();
    validMoveOnEmpty();
    std::cout << "ai tests passed\n";
    return 0;
}
