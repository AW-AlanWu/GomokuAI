#include "AIPlayer.hpp"
#include "NegamaxStrategy.hpp"

namespace {
NegamaxStrategy defaultStrategy;
}

AIPlayer::AIPlayer(AIAlgorithm *strategy)
    : strategy_(strategy ? strategy : &defaultStrategy) {}

AIPlayer::AIPlayer() : strategy_(&defaultStrategy) {}

std::pair<int,int> AIPlayer::getMove(const Board &board, int8_t player) {
    return strategy_->computeMove(const_cast<Board &>(board), player);
}
