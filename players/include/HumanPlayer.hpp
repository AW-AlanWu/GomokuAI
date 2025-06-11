// HumanPlayer.hpp
#pragma once

#include "Player.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

class HumanPlayer : public Player {
    Terminal &term_;
    Renderer &renderer_;

public:
    HumanPlayer(Terminal &t, Renderer &r);
    std::pair<int, int> getMove(const Board &board, int8_t player) override;
};
