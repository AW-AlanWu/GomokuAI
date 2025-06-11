// HumanPlayer.hpp
#pragma once

#include "IGameUI.hpp"
#include "IInput.hpp"
#include "IPlayer.hpp"

class HumanPlayer : public IPlayer {
    IInput &input_;
    IGameUI &ui_;

public:
    HumanPlayer(IInput &input, IGameUI &ui);
    std::pair<int, int> getMove(const Board &board, int8_t player) override;
};
