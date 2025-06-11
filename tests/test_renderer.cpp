#include <cassert>
#include <iostream>

#include "Board.hpp"
#include "Renderer.hpp"
#include "Terminal.hpp"

static void basicMovement() {
    Terminal t;
    Board b;
    Renderer r(t, b);
    auto pos = r.cursor();
    assert(pos.first == Board::N / 2 && pos.second == Board::N / 2);
    r.handleKey(TermKey::Up);
    r.handleKey(TermKey::Left);
    pos = r.cursor();
    assert(pos.first == Board::N / 2 - 1 && pos.second == Board::N / 2 - 1);
    r.handleKey(TermKey::Down);
    r.handleKey(TermKey::Right);
    pos = r.cursor();
    assert(pos.first == Board::N / 2 && pos.second == Board::N / 2);
}

static void boundaryChecks() {
    Terminal t;
    Board b;
    Renderer r(t, b);
    for (size_t i = 0; i < Board::N * 2; ++i) r.handleKey(TermKey::Up);
    for (size_t i = 0; i < Board::N * 2; ++i) r.handleKey(TermKey::Left);
    auto pos = r.cursor();
    assert(pos.first == 0 && pos.second == 0);
    r.handleKey(TermKey::Up);
    r.handleKey(TermKey::Left);
    pos = r.cursor();
    assert(pos.first == 0 && pos.second == 0);

    for (size_t i = 0; i < Board::N * 2; ++i) r.handleKey(TermKey::Down);
    for (size_t i = 0; i < Board::N * 2; ++i) r.handleKey(TermKey::Right);
    pos = r.cursor();
    assert(pos.first == Board::N - 1 && pos.second == Board::N - 1);
    r.handleKey(TermKey::Down);
    r.handleKey(TermKey::Right);
    pos = r.cursor();
    assert(pos.first == Board::N - 1 && pos.second == Board::N - 1);
}

static void returnValues() {
    Terminal t;
    Board b;
    Renderer r(t, b);
    assert(r.handleKey(TermKey::Up));
    assert(!r.handleKey(TermKey::Enter));
}

int main() {
    basicMovement();
    boundaryChecks();
    returnValues();
    std::cout << "renderer tests passed\n";
    return 0;
}
