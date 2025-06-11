#pragma once
#include "IGameUI.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"
class TerminalUI : public IGameUI {
    Renderer &renderer_;
    Terminal &term_;

public:
    TerminalUI(Renderer &r, Terminal &t) : renderer_(r), term_(t) {}
    void drawBoard(const Board &board) override;
    void showResult(int8_t winner) override;
};
