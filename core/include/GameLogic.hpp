#pragma once

#include <cstdint>
#include <vector>

#include "Board.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

class GameLogic {
    Board &board_;
    std::vector<Player *> &players_;
    Renderer &ui_;
    Terminal &input_;
    int8_t current_;

public:
    GameLogic(Board &b, std::vector<Player *> &players, Renderer &ui,
              Terminal &input);
    int8_t run();
};

