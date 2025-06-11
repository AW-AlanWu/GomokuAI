// AIPlayer.hpp
#pragma once

#include "AIAlgorithm.hpp"
#include "IPlayer.hpp"
class AIPlayer : public IPlayer {
    AIAlgorithm *strategy_;

public:
    explicit AIPlayer(AIAlgorithm *strategy);
    AIPlayer();
    std::pair<int, int> getMove(const Board &board, int8_t player) override;
};
