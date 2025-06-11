#include "GameLogic.hpp"

GameLogic::GameLogic(Board &b, std::vector<IPlayer *> &players, IGameUI &ui, IInput &input)
    : board_(b), players_(players), ui_(ui), input_(input), currentPlayer_(1) {}

int8_t GameLogic::run() {
    while (true) {
        ui_.drawBoard(board_);
        IPlayer *p = players_[(currentPlayer_ == 1 ? 0 : 1)];
        auto [r, c] = p->getMove(board_, currentPlayer_);
        if (r < 0 || c < 0) return 0;
        board_.placeStone(r, c, currentPlayer_);
        int8_t winner = board_.checkWin();
        if (winner != 0 || board_.isFull()) {
            ui_.drawBoard(board_);
            ui_.showResult(winner);
            return winner;
        }
        currentPlayer_ = -currentPlayer_;
    }
}
