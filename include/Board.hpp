// Board.hpp
#pragma once

#include <array>
#include <cstdint>

class Board {
public:
    static constexpr int N = 15;
    using Grid = std::array<std::array<int8_t, N>, N>;

    Board();
    void reset();

    bool isEmpty(int r, int c) const;
    bool isFull() const;
    void place(int r, int c, int8_t player);
    int8_t at(int r, int c) const;
    bool checkWin(int r, int c) const;

private:
    Grid grid_;
    int placed_count_;
    bool inBounds(int r, int c) const;
};
