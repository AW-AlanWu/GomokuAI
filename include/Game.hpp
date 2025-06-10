// Game.hpp
#pragma once

#include <cstdint>
#include <memory>

#include "Board.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

class Game {
    Terminal &term_;
    Board &board_;
    Renderer &renderer_;
    std::unique_ptr<Player> p1_, p2_;
    int8_t current_player_;  // +1 or -1
public:
    Game(Terminal &term, Board &board, Renderer &renderer, std::unique_ptr<Player> p1,
         std::unique_ptr<Player> p2);
    ~Game();

    void play();
};
