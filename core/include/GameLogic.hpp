#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "Board.hpp"
#include "IGameUI.hpp"
#include "IPlayer.hpp"

class GameLogic {
    Board &board_;
    std::vector<IPlayer *> &players_;
    IGameUI &ui_;
    int8_t currentPlayer_;

public:
    GameLogic(Board &b, std::vector<IPlayer *> &players, IGameUI &ui);
    int8_t run();
};
