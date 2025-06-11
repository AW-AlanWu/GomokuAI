#pragma once
#include <cstdint>
#include <unordered_map>
#include "AIAlgorithm.hpp"

class NegamaxStrategy : public AIAlgorithm {
public:
    NegamaxStrategy();
    std::pair<int, int> computeMove(Board &board, int8_t player) override;

private:
    struct TranspositionEntry {
        int depth;
        int value;
        uint8_t flag;
    };
    std::unordered_map<uint64_t, TranspositionEntry> transpositionTable_;

    bool makesFive(const Board &board, int r, int c, int8_t who) const;
    bool hasNeighbor(const Board &board, int r, int c) const;
    void applyMove(Board &board, std::pair<int, int> pos, int8_t player);
    void undoMove(Board &board, std::pair<int, int> pos);
};
