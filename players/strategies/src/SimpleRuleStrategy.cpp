#include "SimpleRuleStrategy.hpp"

static constexpr int DIR[4][2] = {
    {1,0},{0,1},{1,1},{1,-1}
};

static bool makesFive(const Board &board,int r,int c,int8_t who){
    for(auto &d:DIR){
        int cnt=1;
        for(int s=1;s<5;++s){
            int nr=r+d[1]*s,nc=c+d[0]*s;
            if(nr<0||nr>=Board::N||nc<0||nc>=Board::N) break;
            if(board.at(nr,nc)==who) cnt++; else break;
        }
        for(int s=1;s<5;++s){
            int nr=r-d[1]*s,nc=c-d[0]*s;
            if(nr<0||nr>=Board::N||nc<0||nc>=Board::N) break;
            if(board.at(nr,nc)==who) cnt++; else break;
        }
        if(cnt>=5) return true;
    }
    return false;
}

std::pair<int,int> SimpleRuleStrategy::computeMove(Board &board,int8_t player){
    int8_t opponent = player==1?-1:1;
    for(int r=0;r<Board::N;++r){
        for(int c=0;c<Board::N;++c){
            if(!board.isEmpty(r,c)) continue;
            if(makesFive(board,r,c,player)) return {r,c};
        }
    }
    for(int r=0;r<Board::N;++r){
        for(int c=0;c<Board::N;++c){
            if(!board.isEmpty(r,c)) continue;
            if(makesFive(board,r,c,opponent)) return {r,c};
        }
    }
    for(int r=0;r<Board::N;++r){
        for(int c=0;c<Board::N;++c){
            if(board.isEmpty(r,c)) return {r,c};
        }
    }
    return {-1,-1};
}
