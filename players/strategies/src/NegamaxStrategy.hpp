#pragma once
#include "AIAlgorithm.hpp"
#include <cstdint>
#include <unordered_map>

class NegamaxStrategy : public AIAlgorithm {
public:
    NegamaxStrategy();
    std::pair<int, int> computeMove(Board &board, int8_t player) override;

private:
    struct TTEntry {
        int depth;
        int value;
        uint8_t flag;
    };
    std::unordered_map<uint64_t, TTEntry> tt_;

    bool makesFive(const Board &board, int r, int c, int8_t who) const;
    bool hasNeighbor(const Board &board, int r, int c) const;
};
