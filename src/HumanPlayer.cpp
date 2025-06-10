// HumanPlayer.cpp
#include <stdexcept>

#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer(Terminal &t, Renderer &r) : term_(t), renderer_(r) {}

std::pair<int, int> HumanPlayer::getMove(const Board &board, int8_t player) {
    (void)player;
    // Continuously read keys and update the cursor until Enter is pressed on a
    // valid cell
    while (true) {
        if (board.isFull()) return {-1, -1};
        TermKey k = term_.readKey();
        if (k == TermKey::Quit) {
            throw std::runtime_error("Game Exited");
        }
        if (renderer_.handleKey(k)) {
            renderer_.draw();
            continue;
        }
        if (k == TermKey::Enter) {
            auto [r, c] = renderer_.cursor();
            if (board.isEmpty(r, c)) return {r, c};
            // Otherwise ignore and keep waiting
        }
    }
}
