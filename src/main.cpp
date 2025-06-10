// main.cpp
#include <cstring>
#include <iostream>

#include "AIPlayer.hpp"
#include "Game.hpp"
#include "HumanPlayer.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

int main(int argc, char *argv[]) {
    bool p1_ai = false, p2_ai = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--p1=ai") == 0) p1_ai = true;
        if (std::strcmp(argv[i], "--p2=ai") == 0) p2_ai = true;
    }

    Terminal term;
    Board board;
    Renderer renderer(term, board);

    std::unique_ptr<Player> p1;
    if (p1_ai)
        p1 = std::make_unique<AIPlayer>();
    else
        p1 = std::make_unique<HumanPlayer>(term, renderer);

    std::unique_ptr<Player> p2;
    if (p2_ai)
        p2 = std::make_unique<AIPlayer>();
    else
        p2 = std::make_unique<HumanPlayer>(term, renderer);

    try {
        Game g(term, board, renderer, std::move(p1), std::move(p2));
        g.play();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
