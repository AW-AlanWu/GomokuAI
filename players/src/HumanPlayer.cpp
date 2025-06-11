// HumanPlayer.cpp
#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer(IInput &input, IGameUI &ui) : input_(input), ui_(ui) {}

std::pair<int, int> HumanPlayer::getMove(const Board &board, int8_t player) {
    (void)board;
    (void)player;
    return input_.getSelection();
}
