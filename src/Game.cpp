// Game.cpp
#include <fstream>
#include <iostream>

#include "Game.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

Game::Game(Terminal &term, Board &board, Renderer &renderer, std::unique_ptr<Player> p1,
           std::unique_ptr<Player> p2)
    : term_(term),
      board_(board),
      renderer_(renderer),
      p1_(std::move(p1)),
      p2_(std::move(p2)),
      current_player_(1) {
    term_.hideCursor();
}

Game::~Game() {
    term_.showCursor();
}

void Game::play() {
    int p1_cnt = 0, p2_cnt = 0;
    std::ofstream out("GameResult.log", std::ios::app);
    while (true) {
        renderer_.draw();
        auto &player = (current_player_ == 1 ? p1_ : p2_);
        auto [r, c] = player->getMove(board_, current_player_);
        if (r < 0 || c < 0) {
            term_.moveCursor(Board::N + 6, 1);
            term_.clearLine();
            std::cout << "Draw!\n";
            std::cout << "p1 use " << p1_cnt << " moves, p2 use " << p2_cnt << " moves!\n";
            out << "0 " << p1_cnt << " " << p2_cnt << "\n";
            break;
        }
        board_.place(r, c, current_player_);
        (current_player_ == 1 ? ++p1_cnt : ++p2_cnt);
        if (board_.checkWin(r, c)) {
            renderer_.draw();
            term_.moveCursor(Board::N + 6, 1);
            term_.clearLine();
            std::cout << "Player " << (current_player_ == 1 ? "●" : "○") << " wins!\n";
            std::cout << "p1 use " << p1_cnt << " moves, p2 use " << p2_cnt << " moves!\n";
            out << (int)current_player_ << " " << p1_cnt << " " << p2_cnt << "\n";
            break;
        }
        current_player_ = -current_player_;
    }
    out.close();
}
