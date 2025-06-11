#include "NegamaxStrategy.hpp"
#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <vector>

// Direction vectors for checking lines (dx, dy)
static constexpr int DIR[4][2] = {
    {1, 0},   // horizontal
    {0, 1},   // vertical
    {1, 1},   // down-right diagonal
    {1, -1},  // up-right diagonal
};

NegamaxStrategy::NegamaxStrategy() {
    tt_.reserve(100000);
}

bool NegamaxStrategy::makesFive(const Board &board, int r, int c, int8_t who) const {
    for (auto &d : DIR) {
        int cnt = 1;
        for (int step = 1; step < 5; ++step) {
            int nr = r + d[1] * step;
            int nc = c + d[0] * step;
            if (nr < 0 || nr >= Board::N || nc < 0 || nc >= Board::N) break;
            if (board.at(nr, nc) == who)
                cnt++;
            else
                break;
        }
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

bool NegamaxStrategy::hasNeighbor(const Board &board, int r, int c) const {
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

std::pair<int,int> NegamaxStrategy::computeMove(Board &board, int8_t player) {
    int8_t opponent = (player == 1 ? -1 : 1);
    for (int r = 0; r < Board::N; ++r) {
        for (int c = 0; c < Board::N; ++c) {
            if (!board.isEmpty(r, c)) continue;
            if (makesFive(board, r, c, player)) return {r, c};
        }
    }
    for (int r = 0; r < Board::N; ++r) {
        for (int c = 0; c < Board::N; ++c) {
            if (!board.isEmpty(r, c)) continue;
            if (makesFive(board, r, c, opponent)) return {r, c};
        }
    }
    if (board.isFull()) return {-1, -1};

    static bool zobristInitialized = false;
    static std::array<std::array<std::array<uint64_t, 2>, Board::N>, Board::N> zobrist;
    if (!zobristInitialized) {
        std::mt19937_64 rng(0xdeadbeef);
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                zobrist[r][c][0] = rng();
                zobrist[r][c][1] = rng();
            }
        }
        zobristInitialized = true;
    }

    tt_.clear();

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

    std::function<int(const Board&,int,int,int,int8_t,uint64_t)> negamax =
        [&](const Board &b, int depth, int alpha, int beta, int8_t curr, uint64_t hash) -> int {
        auto it = tt_.find(hash);
        if (it != tt_.end()) {
            const TTEntry &e = it->second;
            if (e.depth >= depth) {
                if (e.flag == 0) return e.value;
                else if (e.flag == 1 && e.value <= alpha) return e.value;
                else if (e.flag == 2 && e.value >= beta) return e.value;
            }
        }
        if (depth == 0 || b.isFull()) {
            int eval = evaluateBoard(b);
            return curr * eval;
        }
        int maxVal = -1000000000;
        int origAlpha = alpha;
        std::vector<std::pair<int,int>> moves;
        moves.reserve(Board::N*Board::N);
        for (int r = 0; r < Board::N; ++r) {
            for (int c = 0; c < Board::N; ++c) {
                if (!b.isEmpty(r,c)) continue;
                if (!hasNeighbor(b,r,c)) continue;
                moves.emplace_back(r,c);
            }
        }
        if (moves.empty()) {
            for (int r = 0; r < Board::N; ++r) {
                for (int c = 0; c < Board::N; ++c) {
                    if (b.isEmpty(r,c)) moves.emplace_back(r,c);
                }
            }
        }
        std::sort(moves.begin(), moves.end(), [&](const auto &m1, const auto &m2){
            int count1 = 0, count2 = 0;
            for (int dr=-1; dr<=1; ++dr) {
                for (int dc=-1; dc<=1; ++dc) {
                    if (dr==0 && dc==0) continue;
                    int nr1 = m1.first+dr, nc1 = m1.second+dc;
                    int nr2 = m2.first+dr, nc2 = m2.second+dc;
                    if (nr1>=0 && nr1<Board::N && nc1>=0 && nc1<Board::N && !b.isEmpty(nr1,nc1)) count1++;
                    if (nr2>=0 && nr2<Board::N && nc2>=0 && nc2<Board::N && !b.isEmpty(nr2,nc2)) count2++;
                }
            }
            return count1 > count2;
        });
        for (auto [r,c] : moves) {
            if (alpha >= beta) break;
            Board nb = b;
            nb.placeStone(r,c,curr);
            uint64_t nhash = hash ^ zobrist[r][c][(curr==1?0:1)];
            if (nb.checkWin() == curr) {
                int winVal = 1000000 - 10*depth;
                tt_[hash] = {depth, winVal, 2};
                return winVal;
            }
            int val = -negamax(nb, depth-1, -beta, -alpha, (int8_t)-curr, nhash);
            if (val > maxVal) maxVal = val;
            if (val > alpha) alpha = val;
            if (alpha >= beta) break;
        }
        TTEntry e; e.depth = depth; e.value = maxVal;
        if (maxVal <= origAlpha) e.flag = 1;
        else if (maxVal >= beta) e.flag = 2;
        else e.flag = 0;
        tt_[hash] = e;
        return maxVal;
    };

    std::pair<int,int> bestMove{-1,-1};
    int bestVal = -1000000000;
    int maxDepth = 6;
    auto start = std::chrono::steady_clock::now();
    const int limitMs = 1000;
    uint64_t rootHash = 0ULL;
    for (int r=0; r<Board::N; ++r) {
        for (int c=0; c<Board::N; ++c) {
            int8_t val = board.at(r,c);
            if (val != 0) rootHash ^= zobrist[r][c][(val==1?0:1)];
        }
    }
    for (int depth=1; depth<=maxDepth; ++depth) {
        int alpha=-1000000000, beta=1000000000;
        std::pair<int,int> currentBest{-1,-1};
        int currentBestVal=-1000000000;
        std::vector<std::pair<int,int>> rootMoves;
        for (int r=0; r<Board::N; ++r) {
            for (int c=0; c<Board::N; ++c) {
                if (!board.isEmpty(r,c)) continue;
                if (!hasNeighbor(board,r,c)) continue;
                rootMoves.emplace_back(r,c);
            }
        }
        if (rootMoves.empty()) {
            for (int r=0; r<Board::N; ++r) {
                for (int c=0; c<Board::N; ++c) {
                    if (board.isEmpty(r,c)) rootMoves.emplace_back(r,c);
                }
            }
        }
        std::sort(rootMoves.begin(), rootMoves.end(), [&](const auto &m1, const auto &m2){
            int count1=0,count2=0;
            for (int dr=-1; dr<=1; ++dr){
                for (int dc=-1; dc<=1; ++dc){
                    if (dr==0 && dc==0) continue;
                    int nr1=m1.first+dr,nc1=m1.second+dc;
                    int nr2=m2.first+dr,nc2=m2.second+dc;
                    if (nr1>=0&&nr1<Board::N&&nc1>=0&&nc1<Board::N&&!board.isEmpty(nr1,nc1)) count1++;
                    if (nr2>=0&&nr2<Board::N&&nc2>=0&&nc2<Board::N&&!board.isEmpty(nr2,nc2)) count2++;
                }
            }
            return count1>count2;
        });
        bool outOfTime=false;
        for (auto [r,c] : rootMoves) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count() > limitMs) {
                outOfTime=true;
                break;
            }
            Board nb = board;
            nb.placeStone(r,c,player);
            uint64_t nhash = rootHash ^ zobrist[r][c][(player==1?0:1)];
            if (nb.checkWin() == player) return {r,c};
            int val = -negamax(nb, depth-1, -beta, -alpha, (int8_t)-player, nhash);
            if (val > currentBestVal) { currentBestVal = val; currentBest = {r,c}; }
            if (val > alpha) alpha = val;
            if (alpha >= beta) break;
        }
        if (outOfTime) break;
        bestMove = currentBest;
        bestVal = currentBestVal;
        if (bestVal > 500000) break;
    }

    if (bestMove.first == -1 || bestMove.second == -1) {
        for (int r=0; r<Board::N; ++r) {
            for (int c=0; c<Board::N; ++c) {
                if (board.isEmpty(r,c) && hasNeighbor(board,r,c)) return {r,c};
            }
        }
        for (int r=0; r<Board::N; ++r) {
            for (int c=0; c<Board::N; ++c) {
                if (board.isEmpty(r,c)) return {r,c};
            }
        }
        return {-1,-1};
    }
    return bestMove;
}
