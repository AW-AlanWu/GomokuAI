// main.cpp
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>

#include "AIPlayer.hpp"
#include "GameLogic.hpp"
#include "HumanPlayer.hpp"
#include "IPlayer.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"
#include "TerminalInput.hpp"
#include "TerminalUI.hpp"

int main(int argc, char *argv[]) {
    bool p1_ai = false, p2_ai = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--p1=ai") == 0) p1_ai = true;
        if (std::strcmp(argv[i], "--p2=ai") == 0) p2_ai = true;
    }

    Terminal term;
    Board board;
    Renderer renderer(term, board);
    TerminalUI ui(renderer, term);
    TerminalInput input(term, renderer, board);

    std::unique_ptr<IPlayer> p1;
    if (p1_ai)
        p1 = std::make_unique<AIPlayer>();
    else
        p1 = std::make_unique<HumanPlayer>(input, ui);

    std::unique_ptr<IPlayer> p2;
    if (p2_ai)
        p2 = std::make_unique<AIPlayer>();
    else
        p2 = std::make_unique<HumanPlayer>(input, ui);

    std::vector<std::unique_ptr<IPlayer>> players;
    players.push_back(std::move(p1));
    players.push_back(std::move(p2));

    std::vector<IPlayer *> playerPtrs;
    for (auto &ptr : players) playerPtrs.push_back(ptr.get());

    try {
        GameLogic logic(board, playerPtrs, ui, input);
        logic.run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
