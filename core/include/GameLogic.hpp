#pragma once

#include <cstdint>
#include <vector>

#include "Board.hpp"
#include "IGameUI.hpp"
#include "IInput.hpp"
#include "IPlayer.hpp"

class GameLogic {
    Board &board_;
    std::vector<IPlayer *> &players_;
    IGameUI &ui_;
    IInput &input_;
    int8_t currentPlayer_;

public:
    GameLogic(Board &b, std::vector<IPlayer *> &players, IGameUI &ui, IInput &input);
    int8_t run();
};
