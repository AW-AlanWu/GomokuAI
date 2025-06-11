// AIPlayer.hpp
#pragma once

#include "IPlayer.hpp"
#include "AIAlgorithm.hpp"
class AIPlayer : public IPlayer {
    AIAlgorithm &algorithm_;
public:
    explicit AIPlayer(AIAlgorithm &algorithm);
    AIPlayer();
    std::pair<int, int> getMove(const Board &board, int8_t player) override;
};
