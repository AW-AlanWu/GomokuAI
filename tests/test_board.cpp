#include <cassert>
#include <iostream>

#include "Board.hpp"

static void horizontalWin() {
    Board b;
    int row = 7;
    for (int c = 0; c < 5; ++c) b.place(row, c, 1);
    assert(b.checkWin(row, 2));
}

static void verticalWin() {
    Board b;
    int col = 3;
    for (int r = 0; r < 5; ++r) b.place(r, col, 1);
    assert(b.checkWin(2, col));
}

static void diagonalWin() {
    Board b;
    for (int i = 0; i < 5; ++i) b.place(i, i, 1);
    assert(b.checkWin(2, 2));
}

static void antiDiagonalWin() {
    Board b;
    for (int i = 0; i < 5; ++i) b.place(4 - i, i, 1);
    assert(b.checkWin(2, 2));
}

static void noWin() {
    Board b;
    b.place(0, 0, 1);
    b.place(0, 1, 1);
    b.place(0, 2, 1);
    b.place(1, 0, 1);
    assert(!b.checkWin(1, 0));
}

static void fullBoard() {
    Board b;
    for (int r = 0; r < Board::N; ++r)
        for (int c = 0; c < Board::N; ++c) b.place(r, c, 1);
    assert(b.isFull());
}

static void overwriteCell() {
    Board b;
    b.place(0, 0, 1);
    b.place(0, 0, -1);
    for (int r = 0; r < Board::N; ++r)
        for (int c = 0; c < Board::N; ++c)
            if (!(r == 0 && c == 0)) b.place(r, c, 1);
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
