// Board.cpp
#include "Board.hpp"

Board::Board() : placed_count_(0) { reset(); }

void Board::reset() {
    for (auto &row : grid_) row.fill(0);
    placed_count_ = 0;
}

bool Board::isEmpty(size_t r, size_t c) const {
    return inBounds(r, c) && grid_[r][c] == 0;
}

bool Board::isFull() const { return placed_count_ == N * N; }

void Board::placeStone(size_t r, size_t c, int8_t player) {
    if (!inBounds(r, c)) return;
    if (grid_[r][c] == 0) ++placed_count_;
    grid_[r][c] = player;
}

void Board::removeStone(size_t r, size_t c) {
    if (!inBounds(r, c)) return;
    if (grid_[r][c] != 0) {
        grid_[r][c] = 0;
        --placed_count_;
    }
}

int8_t Board::at(size_t r, size_t c) const {
    return inBounds(r, c) ? grid_[r][c] : 0;
}

// Efficient win check (scans in four directions)
bool Board::checkWinAt(size_t r, size_t c) const {
    static constexpr std::array<std::pair<int, int>, 4> dirs = {{{1, 0}, {0, 1}, {1, 1}, {1, -1}}};
    int8_t player = at(r, c);
    if (player == 0) return false;
    for (auto [dr, dc] : dirs) {
        int count = 1;
        for (int step = 1;
             inBounds(r + dr * step, c + dc * step) && at(r + dr * step, c + dc * step) == player;
             ++step)
            ++count;
        for (int step = 1;
             inBounds(r - dr * step, c - dc * step) && at(r - dr * step, c - dc * step) == player;
             ++step)
            ++count;
        if (count >= 5) return true;
    }
    return false;
}

int8_t Board::checkWin() const {
    for (size_t r = 0; r < N; ++r) {
        for (size_t c = 0; c < N; ++c) {
            if (checkWinAt(r, c)) return at(r, c);
        }
    }
    return 0;
}

bool Board::inBounds(size_t r, size_t c) const { return r < N && c < N; }
