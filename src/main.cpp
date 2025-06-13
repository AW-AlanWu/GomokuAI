// main.cpp
#include <cstring>
#include <filesystem>
#include <fstream>
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
        if (std::strcmp(argv[i], "--history") == 0) {
            std::ifstream ofs("GameResult.log");
            if (!ofs.is_open()) {
                std::cout << "尚無對局紀錄...\n";
                return 0;
            }
            int game_cnt = 0;
            int result, p1_cnt, p2_cnt;
            while (ofs >> result >> p1_cnt >> p2_cnt) {
                std::cout << "第 " << ++game_cnt << " 局: ";
                if (result == 0) {
                    std::cout << "Draw, ";
                } else {
                    std::cout << "player " << (result == 1 ? "1" : "2") << " win, ";
                }
                std::cout << "p1 use " << p1_cnt << " moves, p2 use " << p2_cnt << " moves!\n";
            }
            return 0;
        }
        if (std::strcmp(argv[i], "--clear") == 0) {
            if (std::filesystem::remove("GameResult.log")) {
                std::cout << "對局紀錄已清除\n";
            } else {
                std::cout << "尚無對局紀錄...\n";
            }
            return 0;
        }
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
