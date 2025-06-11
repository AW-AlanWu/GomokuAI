#include "AIPlayer.hpp"
#include "NegamaxStrategy.hpp"

AIPlayer::AIPlayer(std::unique_ptr<AIAlgorithm> strategy) : strategy_(std::move(strategy)) {
    if (!strategy_) strategy_ = std::make_unique<NegamaxStrategy>();
}

AIPlayer::AIPlayer() : strategy_(std::make_unique<NegamaxStrategy>()) {}

std::pair<int, int> AIPlayer::getMove(const Board &board, int8_t player) {
    return strategy_->computeMove(board, player);
}
