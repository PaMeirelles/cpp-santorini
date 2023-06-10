#include "eval.h"
#include <iostream>

int numNeighbors[] = {3, 5, 5, 5, 3,
                      5, 8, 8, 8, 5,
                      5, 8, 8, 8, 5,
                      5, 8, 8, 8, 5,
                      3, 5, 5, 5, 3};

int numDoubleNeighbors[] = {9,  12, 15, 12, 9,
                            12, 16, 20, 16, 12,
                            15, 20, 25, 20, 15,
                            12, 16, 20, 16, 12,
                            9,  12, 15, 12, 9};

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
int doubleNeighborHeightFunc(int pos){
    return numDoubleNeighbors[pos];
}

int neighborHeight(int a, int b, int c, Board board){
    return positionHeight(a, b, c, board, neighborHeightFunc);
}
int doublePH(int a, int b, int c, Board board){
    return positionHeight(a, b, c, board, doubleNeighborHeightFunc);
}
int nh_s(Board board){
    return neighborHeight(6, 2, 1, board);
}

int nh_c(Board board){
    return neighborHeight(4, 3, 3, board);
}

int nh_a(Board board){
    return neighborHeight(7, 4, 1, board);
}

int db_s(Board board){
    return neighborHeight(9, 3, 1, board);
}

