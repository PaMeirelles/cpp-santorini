#include "eval.h"

int numNeighbors[] = {3, 5, 5, 5, 3,
                  5, 9, 9, 9, 5,
                  5, 9, 9, 9, 5,
                  5, 9, 9, 9, 5,
                  3, 5, 5, 5, 3};

int positionHeight(int a, int b, int c, Board board, std::function<int(int)> p){
    int score = 0;
    int multiplier = 1;
    for(int i=0; i < 4; i++){
        score += multiplier * (a * pow(b, board.getWorkerHeight(i)) + c * p(board.getWorkerPos(i))); 
        if(i == 1){
            multiplier *= -1;
        }
    }
    return score;
}

int neighborHeightFunc(int pos){
    return numNeighbors[pos];
}

int neighborHeight(int a, int b, int c, Board board){
    return positionHeight(a, b, c, board, neighborHeightFunc);
}

int nh_s(Board board){
    return neighborHeight(6, 2, 1, board);
}

int nh_c(Board board){
    return neighborHeight(4, 3, 3, board);
}