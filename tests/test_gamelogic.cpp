#include <cassert>
#include <iostream>
#include <vector>

#include "Board.hpp"
#include "GameLogic.hpp"
#include "IPlayer.hpp"
#include "IGameUI.hpp"
#include "IInput.hpp"

class DummyPlayer : public IPlayer {
    std::vector<std::pair<int, int>> moves_;
    size_t idx_ = 0;

public:
    explicit DummyPlayer(std::vector<std::pair<int, int>> moves)
        : moves_(std::move(moves)) {}
    std::pair<int, int> getMove(const Board &, int8_t) override {
        if (idx_ >= moves_.size())
            return {-1, -1};
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

class DummyInput : public IInput {
public:
    std::pair<int, int> getSelection() override { return {-1, -1}; }
};

static void playerOneWins() {
    Board b;
    DummyPlayer p1({{7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}});
    DummyPlayer p2({{0, 0}, {0, 1}, {0, 2}, {0, 3}});
    std::vector<IPlayer *> players{&p1, &p2};
    DummyUI ui;
    DummyInput in;
    GameLogic g(b, players, ui, in);
    int8_t winner = g.run();
    assert(winner == 1);
    assert(ui.lastWinner == 1);
}

static void earlyQuit() {
    Board b;
    DummyPlayer p1({{-1, -1}});
    DummyPlayer p2({});
    std::vector<IPlayer *> players{&p1, &p2};
    DummyUI ui;
    DummyInput in;
    GameLogic g(b, players, ui, in);
    int8_t winner = g.run();
    assert(winner == 0);
    assert(ui.lastWinner == 0);
}

int main() {
    playerOneWins();
    earlyQuit();
    std::cout << "gamelogic tests passed\n";
    return 0;
}
