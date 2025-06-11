#include "NegamaxStrategy.hpp"
#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <vector>

// Direction vectors for checking lines (dx, dy)
static constexpr int directions[4][2] = {
    {1, 0},   // horizontal
    {0, 1},   // vertical
    {1, 1},   // down-right diagonal
    {1, -1},  // up-right diagonal
};

NegamaxStrategy::NegamaxStrategy() {
    transpositionTable_.reserve(100000);
}

bool NegamaxStrategy::makesFive(const Board &board, int r, int c, int8_t who) const {
    const int N = static_cast<int>(Board::N);
    for (auto &d : directions) {
        int count = 1;
        for (int step = 1; step < 5; ++step) {
            int nr = r + d[1] * step;
            int nc = c + d[0] * step;
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) break;
            if (board.at(nr, nc) == who)
                count++;
            else
                break;
        }
        for (int step = 1; step < 5; ++step) {
            int nr = r - d[1] * step;
            int nc = c - d[0] * step;
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) break;
            if (board.at(nr, nc) == who)
                count++;
            else
                break;
        }
        if (count >= 5) return true;
    }
    return false;
}

bool NegamaxStrategy::hasNeighbor(const Board &board, int r, int c) const {
    const int N = static_cast<int>(Board::N);
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr, nc = c + dc;
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            if (!board.isEmpty(nr, nc)) return true;
        }
    }
    return false;
}

void NegamaxStrategy::applyMove(Board &board, std::pair<int, int> pos, int8_t player) {
    board.placeStone(pos.first, pos.second, player);
}

void NegamaxStrategy::undoMove(Board &board, std::pair<int, int> pos) {
    board.removeStone(pos.first, pos.second);
}

std::pair<int, int> NegamaxStrategy::computeMove(Board &board, int8_t player) {
    int8_t opponent = (player == 1 ? -1 : 1);
    for (size_t r = 0; r < Board::N; ++r) {
        for (size_t c = 0; c < Board::N; ++c) {
            if (!board.isEmpty(r, c)) continue;
            if (makesFive(board, static_cast<int>(r), static_cast<int>(c), player))
                return {static_cast<int>(r), static_cast<int>(c)};
        }
    }
    for (size_t r = 0; r < Board::N; ++r) {
        for (size_t c = 0; c < Board::N; ++c) {
            if (!board.isEmpty(r, c)) continue;
            if (makesFive(board, static_cast<int>(r), static_cast<int>(c), opponent))
                return {static_cast<int>(r), static_cast<int>(c)};
        }
    }
    if (board.isFull()) return {-1, -1};

    static bool zobristInitialized = false;
    static std::array<std::array<std::array<uint64_t, 2>, Board::N>, Board::N> zobrist;
    if (!zobristInitialized) {
        std::mt19937_64 rng(0xdeadbeef);
        for (size_t r = 0; r < Board::N; ++r) {
            for (size_t c = 0; c < Board::N; ++c) {
                zobrist[r][c][0] = rng();
                zobrist[r][c][1] = rng();
            }
        }
        zobristInitialized = true;
    }

    transpositionTable_.clear();

    auto evaluateBoard = [&](const Board &b) {
        int score = 0;
        for (size_t r = 0; r < Board::N; ++r) {
            for (size_t c = 0; c < Board::N; ++c) {
                int8_t val = b.at(r, c);
                if (val == 1)
                    score += 1;
                else if (val == -1)
                    score -= 1;
            }
        }
        return score;
    };

    std::function<int(Board &, int, int, int, int8_t, uint64_t)> negamax =
        [&](Board &b, int depth, int alpha, int beta, int8_t curr, uint64_t hash) -> int {
        auto it = transpositionTable_.find(hash);
        if (it != transpositionTable_.end()) {
            const TranspositionEntry &e = it->second;
            if (e.depth >= depth) {
                if (e.flag == 0)
                    return e.value;
                else if (e.flag == 1 && e.value <= alpha)
                    return e.value;
                else if (e.flag == 2 && e.value >= beta)
                    return e.value;
            }
        }
        if (depth == 0 || b.isFull()) {
            int eval = evaluateBoard(b);
            return curr * eval;
        }
        int maxVal = -1000000000;
        int origAlpha = alpha;
        std::vector<std::pair<int, int>> moves;
        moves.reserve(Board::N * Board::N);
        for (size_t r = 0; r < Board::N; ++r) {
            for (size_t c = 0; c < Board::N; ++c) {
                if (!b.isEmpty(r, c)) continue;
                if (!hasNeighbor(b, static_cast<int>(r), static_cast<int>(c))) continue;
                moves.emplace_back(static_cast<int>(r), static_cast<int>(c));
            }
        }
        if (moves.empty()) {
            for (size_t r = 0; r < Board::N; ++r) {
                for (size_t c = 0; c < Board::N; ++c) {
                    if (b.isEmpty(r, c))
                        moves.emplace_back(static_cast<int>(r), static_cast<int>(c));
                }
            }
        }
        std::sort(moves.begin(), moves.end(), [&](const auto &m1, const auto &m2) {
            const int N = static_cast<int>(Board::N);
            int count1 = 0, count2 = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr1 = m1.first + dr, nc1 = m1.second + dc;
                    int nr2 = m2.first + dr, nc2 = m2.second + dc;
                    if (nr1 >= 0 && nr1 < N && nc1 >= 0 && nc1 < N && !b.isEmpty(nr1, nc1))
                        count1++;
                    if (nr2 >= 0 && nr2 < N && nc2 >= 0 && nc2 < N && !b.isEmpty(nr2, nc2))
                        count2++;
                }
            }
            return count1 > count2;
        });
        for (auto [r, c] : moves) {
            if (alpha >= beta) break;
            applyMove(b, {r, c}, curr);
            uint64_t newHash = hash ^ zobrist[r][c][(curr == 1 ? 0 : 1)];
            if (b.checkWin() == curr) {
                int winVal = 1000000 - 10 * depth;
                undoMove(b, {r, c});
                transpositionTable_[hash] = {depth, winVal, 2};
                return winVal;
            }
            int val = -negamax(b, depth - 1, -beta, -alpha, (int8_t)-curr, newHash);
            undoMove(b, {r, c});
            if (val > maxVal) maxVal = val;
            if (val > alpha) alpha = val;
            if (alpha >= beta) break;
        }
        TranspositionEntry e;
        e.depth = depth;
        e.value = maxVal;
        if (maxVal <= origAlpha)
            e.flag = 1;
        else if (maxVal >= beta)
            e.flag = 2;
        else
            e.flag = 0;
        transpositionTable_[hash] = e;
        return maxVal;
    };

    std::pair<int, int> bestMove{-1, -1};
    int bestVal = -1000000000;
    int maxDepth = 6;
    auto start = std::chrono::steady_clock::now();
    const int limitMs = 1000;
    uint64_t rootHash = 0ULL;
    for (size_t r = 0; r < Board::N; ++r) {
        for (size_t c = 0; c < Board::N; ++c) {
            int8_t val = board.at(r, c);
            if (val != 0) rootHash ^= zobrist[r][c][(val == 1 ? 0 : 1)];
        }
    }
    for (int depth = 1; depth <= maxDepth; ++depth) {
        int alpha = -1000000000, beta = 1000000000;
        std::pair<int, int> currentBest{-1, -1};
        int currentBestVal = -1000000000;
        std::vector<std::pair<int, int>> rootMoves;
        for (size_t r = 0; r < Board::N; ++r) {
            for (size_t c = 0; c < Board::N; ++c) {
                if (!board.isEmpty(r, c)) continue;
                if (!hasNeighbor(board, static_cast<int>(r), static_cast<int>(c))) continue;
                rootMoves.emplace_back(static_cast<int>(r), static_cast<int>(c));
            }
        }
        if (rootMoves.empty()) {
            for (size_t r = 0; r < Board::N; ++r) {
                for (size_t c = 0; c < Board::N; ++c) {
                    if (board.isEmpty(r, c))
                        rootMoves.emplace_back(static_cast<int>(r), static_cast<int>(c));
                }
            }
        }
        std::sort(rootMoves.begin(), rootMoves.end(), [&](const auto &m1, const auto &m2) {
            const int N = static_cast<int>(Board::N);
            int count1 = 0, count2 = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr1 = m1.first + dr, nc1 = m1.second + dc;
                    int nr2 = m2.first + dr, nc2 = m2.second + dc;
                    if (nr1 >= 0 && nr1 < N && nc1 >= 0 && nc1 < N && !board.isEmpty(nr1, nc1))
                        count1++;
                    if (nr2 >= 0 && nr2 < N && nc2 >= 0 && nc2 < N && !board.isEmpty(nr2, nc2))
                        count2++;
                }
            }
            return count1 > count2;
        });
        bool outOfTime = false;
        for (auto [r, c] : rootMoves) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >
                limitMs) {
                outOfTime = true;
                break;
            }
            applyMove(board, {r, c}, player);
            uint64_t newHash = rootHash ^ zobrist[r][c][(player == 1 ? 0 : 1)];
            if (board.checkWin() == player) {
                undoMove(board, {r, c});
                return {r, c};
            }
            int val = -negamax(board, depth - 1, -beta, -alpha, (int8_t)-player, newHash);
            undoMove(board, {r, c});
            if (val > currentBestVal) {
                currentBestVal = val;
                currentBest = {r, c};
            }
            if (val > alpha) alpha = val;
            if (alpha >= beta) break;
        }
        if (outOfTime) break;
        bestMove = currentBest;
        bestVal = currentBestVal;
        if (bestVal > 500000) break;
    }

    if (bestMove.first == -1 || bestMove.second == -1) {
        for (size_t r = 0; r < Board::N; ++r) {
            for (size_t c = 0; c < Board::N; ++c) {
                if (board.isEmpty(r, c) &&
                    hasNeighbor(board, static_cast<int>(r), static_cast<int>(c)))
                    return {static_cast<int>(r), static_cast<int>(c)};
            }
        }
        for (size_t r = 0; r < Board::N; ++r) {
            for (size_t c = 0; c < Board::N; ++c) {
                if (board.isEmpty(r, c)) return {static_cast<int>(r), static_cast<int>(c)};
            }
        }
        return {-1, -1};
    }
    return bestMove;
}
