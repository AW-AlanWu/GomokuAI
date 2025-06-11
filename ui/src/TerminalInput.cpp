#include "TerminalInput.hpp"
#include <stdexcept>

std::pair<int,int> TerminalInput::getSelection() {
    while (true) {
        if (board_.isFull()) return {-1, -1};
        TermKey k = term_.readKey();
        if (k == TermKey::Quit) throw std::runtime_error("Game Exited");
        if (renderer_.handleKey(k)) {
            renderer_.draw();
            continue;
        }
        if (k == TermKey::Enter) {
            auto [r, c] = renderer_.cursor();
            if (board_.isEmpty(r, c)) return {r, c};
        }
    }
}
