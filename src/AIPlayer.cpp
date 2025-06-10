// AIPlayer.cpp
#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <unordered_map>
#include <vector>

#include "AIPlayer.hpp"

// Direction vectors for checking lines (dx, dy)
static constexpr int DIR[4][2] = {
    {1, 0},   // horizontal
    {0, 1},   // vertical
    {1, 1},   // down-right diagonal
    {1, -1},  // up-right diagonal
};

// Check if placing piece `who` at (r,c) would form five in a row
bool makesFive(const Board &board, int r, int c, int8_t who) {
    for (auto &d : DIR) {
        int cnt = 1;  // count (r,c) itself
        // forward direction
        for (int step = 1; step < 5; ++step) {
            int nr = r + d[1] * step;
            int nc = c + d[0] * step;
            if (nr < 0 || nr >= Board::N || nc < 0 || nc >= Board::N) break;
            if (board.at(nr, nc) == who)
                cnt++;
            else
                break;
        }
        // backward direction
        for (int step = 1; step < 5; ++step) {
            int nr = r - d[1] * step;
            int nc = c - d[0] * step;
            if (nr < 0 || nr >= Board::N || nc < 0 || nc >= Board::N) break;
            if (board.at(nr, nc) == who)
                cnt++;
            else
                break;
        }
        if (cnt >= 5) return true;
    }
    return false;
}

// Check if cell (r,c) has any neighboring piece (used to prune moves)
bool hasNeighbor(const Board &board, int r, int c) {
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr, nc = c + dc;
            if (nr < 0 || nr >= Board::N || nc < 0 || nc >= Board::N) continue;
            if (!board.isEmpty(nr, nc)) return true;
        }
    }
    return false;
}

std::pair<int, int> AIPlayer::getMove(const Board &board, int8_t player) {
    int8_t opponent = (player == 1 ? -1 : 1);

    // 1. If placing here wins immediately, do it.
    for (int r = 0; r < Board::N; ++r) {
        for (int c = 0; c < Board::N; ++c) {
            if (!board.isEmpty(r, c)) continue;
            if (makesFive(board, r, c, player)) {
                return {r, c};  // win immediately
            }
        }
    }
    // 2. Block the opponent if they can win next move
    for (int r = 0; r < Board::N; ++r) {
        for (int c = 0; c < Board::N; ++c) {
            if (!board.isEmpty(r, c)) continue;
            if (makesFive(board, r, c, opponent)) {
                return {r, c};  // stop opponent from winning
            }
        }
    }

    // 3. Use iterative deepening Negamax with alpha-beta pruning and a transposition table
    // Check if the board is full
    if (board.isFull()) {
        return {-1, -1};  // draw
    }

    // Initialize Zobrist table for hashing
    static bool zobristInitialized = false;
    static std::array<std::array<std::array<uint64_t, 2>, Board::N>, Board::N> zobrist;
    if (!zobristInitialized) {
        std::mt19937_64 rng(0xdeadbeef);  // fixed seed for reproducibility
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                // random bits for each cell and player
                zobrist[r][c][0] = rng();
                zobrist[r][c][1] = rng();
            }
        }
        zobristInitialized = true;
    }

    // Transposition table entry
    struct TTEntry {
        int depth;
        int value;
        uint8_t flag;  // 0 = exact, 1 = alpha bound, 2 = beta bound
    };
    static std::unordered_map<uint64_t, TTEntry> transpositionTable;
    transpositionTable.clear();  // clear before each search

    // Board evaluation: own pieces add score, opponent pieces subtract
    auto evaluateBoard = [&](const Board &b) {
        int score = 0;
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                int8_t val = b.at(r, c);
                if (val == 1)
                    score += 1;
                else if (val == -1)
                    score -= 1;
            }
        }
        return score;
    };

    // Negamax recursion with alpha-beta and transposition table
    std::function<int(const Board &, int, int, int, int8_t, uint64_t)> negamax =
        [&](const Board &b, int depth, int alpha, int beta, int8_t currentPlayer,
            uint64_t hash) -> int {
        // Look up the table; if depth is sufficient return the stored value
        auto it = transpositionTable.find(hash);
        if (it != transpositionTable.end()) {
            const TTEntry &entry = it->second;
            if (entry.depth >= depth) {
                if (entry.flag == 0) {  // exact value
                    return entry.value;
                } else if (entry.flag == 1 && entry.value <= alpha) {  // upper bound
                    return entry.value;
                } else if (entry.flag == 2 && entry.value >= beta) {  // lower bound
                    return entry.value;
                }
            }
        }
        // Stop when depth is zero or board is full
        if (depth == 0 || b.isFull()) {
            int eval = evaluateBoard(b);
            // Score from the viewpoint of currentPlayer
            return currentPlayer * eval;
        }

        int maxVal = -1000000000;  // negative infinity
        int originalAlpha = alpha;
        // Generate moves near existing stones only
        std::vector<std::pair<int, int>> moves;
        moves.reserve(Board::N * Board::N);
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                if (!b.isEmpty(r, c)) continue;
                if (!hasNeighbor(b, r, c)) continue;  // skip distant cells
                moves.emplace_back(r, c);
            }
        }
        if (moves.empty()) {
            // If none found (e.g. empty board) use any empty cell
            for (int r = 0; r < Board::N; ++r) {
                for (int c = 0; c < Board::N; ++c) {
                    if (b.isEmpty(r, c)) {
                        moves.emplace_back(r, c);
                    }
                }
            }
        }
        // Sort moves by neighbor count to improve pruning
        std::sort(moves.begin(), moves.end(),
                  [&](const std::pair<int, int> &m1, const std::pair<int, int> &m2) {
                      int count1 = 0, count2 = 0;
                      for (int dr = -1; dr <= 1; ++dr) {
                          for (int dc = -1; dc <= 1; ++dc) {
                              if (dr == 0 && dc == 0) continue;
                              int nr1 = m1.first + dr, nc1 = m1.second + dc;
                              int nr2 = m2.first + dr, nc2 = m2.second + dc;
                              if (nr1 >= 0 && nr1 < Board::N && nc1 >= 0 && nc1 < Board::N &&
                                  !b.isEmpty(nr1, nc1))
                                  count1++;
                              if (nr2 >= 0 && nr2 < Board::N && nc2 >= 0 && nc2 < Board::N &&
                                  !b.isEmpty(nr2, nc2))
                                  count2++;
                          }
                      }
                      return count1 > count2;
                  });

        // Try every move
        for (auto [r, c] : moves) {
            if (alpha >= beta) break;  // branch cannot improve
            // try the move
            Board newBoard = b;
            newBoard.place(r, c, currentPlayer);
            // update hash for new board
            uint64_t newHash = hash ^ zobrist[r][c][(currentPlayer == 1 ? 0 : 1)];
            // check for a win
            if (newBoard.checkWin(r, c)) {
                // winning move, reward quicker wins
                int winVal = 1000000 - 10 * depth;
                // store as beta bound because it is winning
                transpositionTable[hash] = {depth, winVal, 2};
                return winVal;
            }
            // recursive search of opponent via negamax
            int val = -negamax(newBoard, depth - 1, -beta, -alpha, (int8_t)-currentPlayer, newHash);
            if (val > maxVal) {
                maxVal = val;
            }
            if (val > alpha) {
                alpha = val;  // raise alpha
            }
            if (alpha >= beta) {
                // beta cutoff
                break;
            }
        }

        // store result in table
        TTEntry entry;
        entry.depth = depth;
        entry.value = maxVal;
        if (maxVal <= originalAlpha) {
            entry.flag = 1;  // no alpha improvement => upper bound
        } else if (maxVal >= beta) {
            entry.flag = 2;  // exceeds beta => lower bound
        } else {
            entry.flag = 0;  // exact value in (alpha, beta)
        }
        transpositionTable[hash] = entry;
        return maxVal;
    };

    // Iterative deepening search loop
    std::pair<int, int> bestMove = {-1, -1};
    int bestValue = -1000000000;
    int maxDepth = 6;  // maximum search depth
    auto startTime = std::chrono::steady_clock::now();
    const int timeLimitMs = 1000;  // time limit in ms
    // Compute hash for the current board
    uint64_t rootHash = 0ULL;
    for (int r = 0; r < Board::N; ++r) {
        for (int c = 0; c < Board::N; ++c) {
            int8_t val = board.at(r, c);
            if (val != 0) {
                // player 1 -> index 0, player -1 -> index 1
                rootHash ^= zobrist[r][c][(val == 1 ? 0 : 1)];
            }
        }
    }
    for (int depth = 1; depth <= maxDepth; ++depth) {
        int alpha = -1000000000, beta = 1000000000;
        std::pair<int, int> currentBest = {-1, -1};
        int currentBestVal = -1000000000;
        // Generate and sort root moves using neighbor heuristic
        std::vector<std::pair<int, int>> rootMoves;
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                if (!board.isEmpty(r, c)) continue;
                if (!hasNeighbor(board, r, c)) continue;
                rootMoves.emplace_back(r, c);
            }
        }
        if (rootMoves.empty()) {
            for (int r = 0; r < Board::N; ++r) {
                for (int c = 0; c < Board::N; ++c) {
                    if (board.isEmpty(r, c)) {
                        rootMoves.emplace_back(r, c);
                    }
                }
            }
        }
        std::sort(rootMoves.begin(), rootMoves.end(),
                  [&](const std::pair<int, int> &m1, const std::pair<int, int> &m2) {
                      int count1 = 0, count2 = 0;
                      for (int dr = -1; dr <= 1; ++dr) {
                          for (int dc = -1; dc <= 1; ++dc) {
                              if (dr == 0 && dc == 0) continue;
                              int nr1 = m1.first + dr, nc1 = m1.second + dc;
                              int nr2 = m2.first + dr, nc2 = m2.second + dc;
                              if (nr1 >= 0 && nr1 < Board::N && nc1 >= 0 && nc1 < Board::N &&
                                  !board.isEmpty(nr1, nc1))
                                  count1++;
                              if (nr2 >= 0 && nr2 < Board::N && nc2 >= 0 && nc2 < Board::N &&
                                  !board.isEmpty(nr2, nc2))
                                  count2++;
                          }
                      }
                      return count1 > count2;
                  });
        bool outOfTime = false;
        // Evaluate each move at this depth
        for (auto [r, c] : rootMoves) {
            // check if time limit exceeded
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() >
                timeLimitMs) {
                outOfTime = true;
                break;
            }
            Board newBoard = board;
            newBoard.place(r, c, player);
            uint64_t newHash = rootHash ^ zobrist[r][c][(player == 1 ? 0 : 1)];
            // check for win (should not happen)
            if (newBoard.checkWin(r, c)) {
                return {r, c};
            }
            // search opponent response
            int val = -negamax(newBoard, depth - 1, -beta, -alpha, (int8_t)-player, newHash);
            if (val > currentBestVal) {
                currentBestVal = val;
                currentBest = {r, c};
            }
            if (val > alpha) {
                alpha = val;
            }
            if (alpha >= beta) {
                // prune: found good enough move
                break;
            }
        }
        if (outOfTime) {
            // stop search if out of time
            break;
        }
        // update best move found so far
        bestMove = currentBest;
        bestValue = currentBestVal;
        // stop early if a winning move is found
        if (bestValue > 500000) {
            // values above 500k indicate near certain win
            break;
        }
    }

    // Fallback to a nearby empty cell if no best move found
    if (bestMove.first == -1 || bestMove.second == -1) {
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                if (board.isEmpty(r, c) && hasNeighbor(board, r, c)) {
                    return {r, c};
                }
            }
        }
        // Otherwise return the first empty cell
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                if (board.isEmpty(r, c)) {
                    return {r, c};
                }
            }
        }
        return {-1, -1};
    }

    return bestMove;
}
