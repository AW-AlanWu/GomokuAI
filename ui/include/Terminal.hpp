// Terminal.hpp
#pragma once

#include <termios.h>
#include <unistd.h>
#include <string>

enum class TermKey { None, Up, Down, Left, Right, Enter, Quit };

class Terminal {
    termios orig_;
    void enableRawMode();
    void disableRawMode();

public:
    Terminal();
    ~Terminal();

    TermKey readKey();
    void writeBuffer(const std::string &s);

    // ANSI operations
    void clearLine();
    void moveCursor(int row, int col);
};
