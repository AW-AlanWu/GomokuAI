// main.cpp
#include <cstring>
#include <iostream>
#include <vector>

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

    IPlayer *p1;
    if (p1_ai)
        p1 = new AIPlayer();
    else
        p1 = new HumanPlayer(input, ui);

    IPlayer *p2;
    if (p2_ai)
        p2 = new AIPlayer();
    else
        p2 = new HumanPlayer(input, ui);

    std::vector<IPlayer *> players{p1, p2};

    try {
        GameLogic logic(board, players, ui, input);
        logic.run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
        delete p1;
        delete p2;
        return 1;
    }
    delete p1;
    delete p2;
    return 0;
}
