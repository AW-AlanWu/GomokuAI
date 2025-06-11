#pragma once
#include <utility>
#include "Board.hpp"

struct IPlayer {
    virtual std::pair<int, int> getMove(const Board &, int8_t) = 0;
    virtual ~IPlayer() = default;
};
