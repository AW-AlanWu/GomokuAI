#include "GameLogic.hpp"

GameLogic::GameLogic(Board &b, std::vector<Player *> &players, IGameUI &ui,
                     IInput &input)
    : board_(b), players_(players), ui_(ui), input_(input), current_(1) {}

int8_t GameLogic::run() {
    while (true) {
        ui_.drawBoard(board_);
        Player *p = players_[(current_ == 1 ? 0 : 1)];
        auto [r, c] = p->getMove(board_, current_);
        if (r < 0 || c < 0) return 0;
        board_.placeStone(r, c, current_);
        int8_t winner = board_.checkWin();
        if (winner != 0 || board_.isFull()) {
            ui_.drawBoard(board_);
            ui_.showResult(winner);
            return winner;
        }
        current_ = -current_;
    }
}
