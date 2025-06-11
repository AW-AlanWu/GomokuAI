#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Board.hpp"
#include "GameLogic.hpp"
#include "IGameUI.hpp"
#include "IPlayer.hpp"

class DummyPlayer : public IPlayer {
    std::vector<std::pair<int, int>> moves_;
    size_t idx_ = 0;

public:
    explicit DummyPlayer(std::vector<std::pair<int, int>> moves) : moves_(std::move(moves)) {}
    std::pair<int, int> getMove(const Board &, int8_t) override {
        if (idx_ >= moves_.size()) return {-1, -1};
        return moves_[idx_++];
    }
};

class DummyUI : public IGameUI {
public:
    int drawCount = 0;
    int showCount = 0;
    int8_t lastWinner = 0;

    void drawBoard(const Board &) override { drawCount++; }
    void showResult(int8_t winner) override {
        showCount++;
        lastWinner = winner;
    }
};

static void playerOneWins() {
    Board b;
    DummyPlayer p1({{7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}});
    DummyPlayer p2({{0, 0}, {0, 1}, {0, 2}, {0, 3}});
    std::vector<IPlayer *> players{&p1, &p2};
    DummyUI ui;
    GameLogic g(b, players, ui);
    int8_t winner = g.run();
    assert(winner == 1);
    assert(ui.lastWinner == 1);
    assert(ui.drawCount == 10);  // 9 turns + final board
}

static void earlyQuit() {
    Board b;
    DummyPlayer p1({{-1, -1}});
    DummyPlayer p2({});
    std::vector<IPlayer *> players{&p1, &p2};
    DummyUI ui;
    GameLogic g(b, players, ui);
    int8_t winner = g.run();
    assert(winner == 0);
    assert(ui.lastWinner == 0);
    assert(ui.drawCount == 1);  // one turn with early quit
}

static void drawGame() {
    Board b;
    for (size_t r = 0; r < Board::N; ++r) {
        int8_t start = (r % 4 < 2) ? 1 : -1;
        for (size_t c = 0; c < Board::N; ++c) {
            if (r == Board::N - 1 && c == Board::N - 1) break;
            int8_t val = (c % 2 == 0) ? start : -start;
            b.placeStone(r, c, val);
        }
    }
    DummyPlayer p1({{Board::N - 1, Board::N - 1}});
    DummyPlayer p2({});
    std::vector<IPlayer *> players{&p1, &p2};
    DummyUI ui;
    GameLogic g(b, players, ui);
    int8_t winner = g.run();
    assert(winner == 0);
    assert(ui.lastWinner == 0);
    assert(ui.drawCount == 2);  // one turn plus final board
}

static void invalidPlayers() {
    Board b;
    DummyPlayer p1({});
    std::vector<IPlayer *> players{&p1};
    DummyUI ui;
    bool thrown = false;
    try {
        GameLogic g(b, players, ui);
    } catch (const std::invalid_argument &) {
        thrown = true;
    }
    assert(thrown);
}

int main() {
    playerOneWins();
    earlyQuit();
    drawGame();
    invalidPlayers();
    std::cout << "gamelogic tests passed\n";
    return 0;
}
