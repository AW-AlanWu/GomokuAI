#pragma once

#include <cstdint>
#include <vector>

#include "Board.hpp"
#include "Player.hpp"
#include "IGameUI.hpp"
#include "IInput.hpp"

class GameLogic {
    Board &board_;
    std::vector<Player *> &players_;
    IGameUI &ui_;
    IInput &input_;
    int8_t current_;

public:
    GameLogic(Board &b, std::vector<Player *> &players, IGameUI &ui,
              IInput &input);
    int8_t run();
};

