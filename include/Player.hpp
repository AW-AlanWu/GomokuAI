// Player.hpp
#pragma once

#include <utility>

#include "Board.hpp"

class Player {
public:
    virtual ~Player() = default;
    // Given the current board and player id (1 or -1), return the next move
    // (row, col)
    virtual std::pair<int, int> getMove(const Board &board, int8_t player) = 0;
};

using IPlayer = Player;
