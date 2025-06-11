#include "SimpleRuleStrategy.hpp"

static constexpr int DIR[4][2] = {
    {1,0},{0,1},{1,1},{1,-1}
};

static bool makesFive(const Board &board,int r,int c,int8_t who){
    const int N = static_cast<int>(Board::N);
    for(auto &d:DIR){
        int cnt=1;
        for(int s=1;s<5;++s){
            int nr=r+d[1]*s,nc=c+d[0]*s;
            if(nr<0||nr>=N||nc<0||nc>=N) break;
            if(board.at(nr,nc)==who) cnt++; else break;
        }
        for(int s=1;s<5;++s){
            int nr=r-d[1]*s,nc=c-d[0]*s;
            if(nr<0||nr>=N||nc<0||nc>=N) break;
            if(board.at(nr,nc)==who) cnt++; else break;
        }
        if(cnt>=5) return true;
    }
    return false;
}

std::pair<int,int> SimpleRuleStrategy::computeMove(Board &board,int8_t player){
    int8_t opponent = player==1?-1:1;
    for(size_t r=0;r<Board::N;++r){
        for(size_t c=0;c<Board::N;++c){
            if(!board.isEmpty(r,c)) continue;
            if(makesFive(board,static_cast<int>(r),static_cast<int>(c),player))
                return {static_cast<int>(r),static_cast<int>(c)};
        }
    }
    for(size_t r=0;r<Board::N;++r){
        for(size_t c=0;c<Board::N;++c){
            if(!board.isEmpty(r,c)) continue;
            if(makesFive(board,static_cast<int>(r),static_cast<int>(c),opponent))
                return {static_cast<int>(r),static_cast<int>(c)};
        }
    }
    for(size_t r=0;r<Board::N;++r){
        for(size_t c=0;c<Board::N;++c){
            if(board.isEmpty(r,c)) return {static_cast<int>(r),static_cast<int>(c)};
        }
    }
    return {-1,-1};
}
