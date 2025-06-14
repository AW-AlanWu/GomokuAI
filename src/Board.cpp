// Board.cpp
#include "Board.hpp"

Board::Board() : placed_count_(0) {
    reset();
}

void Board::reset() {
    for (auto &row : grid_) row.fill(0);
    placed_count_ = 0;
}

bool Board::isEmpty(int r, int c) const {
    return inBounds(r, c) && grid_[r][c] == 0;
}

bool Board::isFull() const {
    return placed_count_ == N * N;
}

void Board::place(int r, int c, int8_t player) {
    if (!inBounds(r, c)) return;
    if (grid_[r][c] == 0) ++placed_count_;
    grid_[r][c] = player;
}

int8_t Board::at(int r, int c) const {
    return inBounds(r, c) ? grid_[r][c] : 0;
}

// Efficient win check (scans in four directions)
bool Board::checkWin(int r, int c) const {
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

bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < N && c >= 0 && c < N;
}
