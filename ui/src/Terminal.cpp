// Terminal.cpp
#include <unistd.h>
#include <stdexcept>

#include "Terminal.hpp"

void Terminal::enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_) == -1) throw std::runtime_error("tcgetattr failed");
    termios raw = orig_;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        throw std::runtime_error("tcsetattr failed");
}

void Terminal::disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_);
}

Terminal::Terminal() {
    enableRawMode();
}

Terminal::~Terminal() {
    disableRawMode();
}

void Terminal::clearLine() {
    writeBuffer("\x1b[2K");
}
void Terminal::moveCursor(int r, int c) {
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", r, c);
    writeBuffer(std::string(buf, buf + len));
}

TermKey Terminal::readKey() {
    char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return TermKey::None;
    if (c == '\x1b') {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return TermKey::None;
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return TermKey::None;
        if (seq[0] == '[') {
            switch (seq[1]) {
            case 'A':
                return TermKey::Up;
            case 'B':
                return TermKey::Down;
            case 'C':
                return TermKey::Right;
            case 'D':
                return TermKey::Left;
            }
        }
        return TermKey::None;
    }
    if (c == '\r' || c == '\n') return TermKey::Enter;
    if (c == 'q' || c == 'Q') return TermKey::Quit;
    return TermKey::None;
}

void Terminal::writeBuffer(const std::string &s) {
    auto _ = ::write(STDOUT_FILENO, s.data(), s.size());
    (void)_;
}
