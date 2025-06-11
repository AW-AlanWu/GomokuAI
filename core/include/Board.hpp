// Board.hpp
#pragma once

#include <array>
#include <cstdint>
#include <cstddef>

class Board {
public:
    static constexpr size_t N = 15;
    using Grid = std::array<std::array<int8_t, N>, N>;

    Board();
    void reset();

    bool isEmpty(size_t r, size_t c) const;
    bool isFull() const;
    int8_t at(size_t r, size_t c) const;
    void placeStone(size_t r, size_t c, int8_t player);
    void removeStone(size_t r, size_t c);
    int8_t checkWin() const;

private:
    Grid grid_;
    size_t placedCount_;

    bool inBounds(size_t r, size_t c) const;
    bool checkWinAt(size_t r, size_t c) const;
};
