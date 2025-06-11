#include "GameLogic.hpp"

GameLogic::GameLogic(Board &b, std::vector<IPlayer *> &players, IGameUI &ui)
    : board_(b), players_(players), ui_(ui), currentPlayer_(1) {
    if (players_.size() != 2) {
        throw std::invalid_argument("GameLogic requires exactly two players");
    }
}

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
