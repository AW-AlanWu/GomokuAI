#include <cassert>
#include <iostream>

#include "Board.hpp"

static void horizontalWin() {
    Board b;
    int row = 7;
    for (int c = 0; c < 5; ++c) b.placeStone(row, c, 1);
    assert(b.checkWin() == 1);
}

static void verticalWin() {
    Board b;
    int col = 3;
    for (int r = 0; r < 5; ++r) b.placeStone(r, col, 1);
    assert(b.checkWin() == 1);
}

static void diagonalWin() {
    Board b;
    for (int i = 0; i < 5; ++i) b.placeStone(i, i, 1);
    assert(b.checkWin() == 1);
}

static void antiDiagonalWin() {
    Board b;
    for (int i = 0; i < 5; ++i) b.placeStone(4 - i, i, 1);
    assert(b.checkWin() == 1);
}

static void noWin() {
    Board b;
    b.placeStone(0, 0, 1);
    b.placeStone(0, 1, 1);
    b.placeStone(0, 2, 1);
    b.placeStone(1, 0, 1);
    assert(b.checkWin() == 0);
}

static void fullBoard() {
    Board b;
    for (size_t r = 0; r < Board::N; ++r)
        for (size_t c = 0; c < Board::N; ++c) b.placeStone(r, c, 1);
    assert(b.isFull());
}

static void overwriteCell() {
    Board b;
    b.placeStone(0, 0, 1);
    b.placeStone(0, 0, -1);
    for (size_t r = 0; r < Board::N; ++r)
        for (size_t c = 0; c < Board::N; ++c)
            if (!(r == 0 && c == 0)) b.placeStone(r, c, 1);
    assert(b.isFull());
}

int main() {
    horizontalWin();
    verticalWin();
    diagonalWin();
    antiDiagonalWin();
    noWin();
    fullBoard();
    overwriteCell();
    std::cout << "board tests passed\n";
    return 0;
}
