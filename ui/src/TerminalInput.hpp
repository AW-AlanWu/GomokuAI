#pragma once
#include "Board.hpp"
#include "IInput.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"
class TerminalInput : public IInput {
    Terminal &term_;
    Renderer &renderer_;
    const Board &board_;

public:
    TerminalInput(Terminal &t, Renderer &r, const Board &b) : term_(t), renderer_(r), board_(b) {}
    std::pair<int, int> getSelection() override;
};
