#pragma once
#include "Board.hpp"
struct IGameUI {
    virtual void drawBoard(const Board &board) = 0;
    virtual void showResult(int8_t winner) = 0;
    virtual ~IGameUI() = default;
};
