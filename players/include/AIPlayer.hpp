// AIPlayer.hpp
#pragma once

#include <memory>
#include "AIAlgorithm.hpp"
#include "IPlayer.hpp"
class AIPlayer : public IPlayer {
    std::unique_ptr<AIAlgorithm> strategy_;

public:
    explicit AIPlayer(std::unique_ptr<AIAlgorithm> strategy);
    AIPlayer();
    std::pair<int, int> getMove(const Board &board, int8_t player) override;
};
