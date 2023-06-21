#include "eval.h"
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

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

int communistPositionHeight(int a, int b, int c, Board board, std::function<int(int)> p){
    int scoreA = 1;
    int scoreB = 1;
    for(int i=0; i < 2; i++){
        scoreA *= (a * pow(b, board.getWorkerHeight(i)) + c * p(board.getWorkerPos(i))); 
    }  
    for(int i=2; i < 4; i++){
        scoreB *= (a * pow(b, board.getWorkerHeight(i)) + c * p(board.getWorkerPos(i))); 
    }   
    return scoreA - scoreB;  
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

int communistNeighborHeight(int a, int b, int c, Board board){
    return communistPositionHeight(a, b, c, board, neighborHeightFunc);
}

int doublePH(int a, int b, int c, Board board){
    return positionHeight(a, b, c, board, doubleNeighborHeightFunc);
}
int nh_s(Board board){
    return neighborHeight(6, 2, 1, board);
}
int nh_s_1(Board board){
    return communistNeighborHeight(6, 2, 1, board);
}
int nh_c(Board board){
    return neighborHeight(4, 3, 3, board);
}

int nh_a(Board board){
    return neighborHeight(7, 4, 1, board);
}

int db_s(Board board){
    return doublePH(9, 3, 1, board);
}

int dist(int posW1, int posW2){
    return (max(abs((posW1 % 5) - (posW2 % 5)), abs(posW1 / 5 - posW2 / 5))) - 1;
}

int scorer(int positionScorer, int enemyOne, int enemyTwo, int a, int b, int height){
    return a * min(dist(positionScorer, enemyOne), dist(positionScorer, enemyTwo)) + b * height;
}

int disruptor(int positionDisruptor, int enemyOne, int enemyTwo, int a, int b, int height){
    return b * height - a * max(dist(positionDisruptor, enemyOne), dist(positionDisruptor, enemyTwo));
}

int staticSpacer(int a, int b, int c, int d, Board board){
    int scoreWhite1 =
         scorer(board.workers[0], board.workers[2], board.workers[3], a, b, board.getHeight(board.workers[0])) +
         disruptor(board.workers[1], board.workers[2], board.workers[3], c, d, board.getHeight(board.workers[1]));

    int scoreWhite2 =
         scorer(board.workers[1], board.workers[2], board.workers[3], a, b, board.getHeight(board.workers[1])) +
         disruptor(board.workers[0], board.workers[2], board.workers[3], c, d, board.getHeight(board.workers[0]));

    int scoreGray1 =
         scorer(board.workers[2], board.workers[0], board.workers[1], a, b, board.getHeight(board.workers[2])) +
         disruptor(board.workers[3], board.workers[0], board.workers[1], c, d, board.getHeight(board.workers[3]));

    int scoreGray2 =
         scorer(board.workers[3], board.workers[0], board.workers[1], a, b, board.getHeight(board.workers[3])) +
         disruptor(board.workers[2], board.workers[0], board.workers[1], c, d, board.getHeight(board.workers[2]));
    return max(scoreWhite1, scoreWhite2) - max(scoreGray1, scoreGray2);
}

int ss_h(Board b){
    return staticSpacer(1, 4, 3, 1, b);
}