#pragma once
#include "AIAlgorithm.hpp"

class SimpleRuleStrategy : public AIAlgorithm {
public:
    std::pair<int,int> computeMove(Board &board, int8_t player) override;
};
