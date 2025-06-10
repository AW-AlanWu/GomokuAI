// AIPlayer.hpp
#pragma once

#include "Player.hpp"

class AIPlayer : public Player {
public:
    std::pair<int, int> getMove(const Board &board, int8_t player) override;
};
