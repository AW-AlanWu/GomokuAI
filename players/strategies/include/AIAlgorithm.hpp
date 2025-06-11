#pragma once
#include <utility>
#include "Board.hpp"

struct AIAlgorithm {
    virtual std::pair<int, int> computeMove(Board &board, int8_t player) = 0;
    virtual ~AIAlgorithm() = default;
};
