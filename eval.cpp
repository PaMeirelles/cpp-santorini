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

int positionHeight(const int a, const int b, const int c, const Board &board, const function<int(int)>& p){
    double score = 0;
    double multiplier = 1;
    for(int i=0; i < 4; i++){
        score += multiplier * (a * pow(b, board.getWorkerHeight(i)) + c * p(board.getWorkerPos(i))); 
        if(i == 1){
            multiplier *= -1;
        }
    }
    return static_cast<int>(score);
}

int communistPositionHeight(const int a, const int b, const int c, const Board &board, const function<int(int)>& p){
    double scoreA = 1;
    double scoreB = 1;
    for(int i=0; i < 2; i++){
        scoreA *= (a * pow(b, board.getWorkerHeight(i)) + c * p(board.getWorkerPos(i))); 
    }  
    for(int i=2; i < 4; i++){
        scoreB *= (a * pow(b, board.getWorkerHeight(i)) + c * p(board.getWorkerPos(i))); 
    }   
    return static_cast<int>(scoreA - scoreB);
}

int neighborHeightFunc(const int pos){
    return numNeighbors[pos];
}
int doubleNeighborHeightFunc(const int pos){
    return numDoubleNeighbors[pos];
}

int neighborHeight(const int a, const int b, const int c, const Board &board){
    return positionHeight(a, b, c, board, neighborHeightFunc);
}

int communistNeighborHeight(const int a, const int b, const int c, const Board &board){
    return communistPositionHeight(a, b, c, board, neighborHeightFunc);
}

int doublePH(const int a, const int b, const int c, const Board &board){
    return positionHeight(a, b, c, board, doubleNeighborHeightFunc);
}
int nh_s(const Board &board){
    return neighborHeight(6, 2, 1, board);
}
int nh_s_1(const Board &board){
    return communistNeighborHeight(6, 2, 1, board);
}
int nh_c(const Board &board){
    return neighborHeight(4, 3, 3, board);
}

int nh_a(const Board &board){
    return neighborHeight(7, 4, 1, board);
}

int db_s(const Board &board){
    return doublePH(9, 3, 1, board);
}

int dist(const int posW1, const int posW2){
    return max(abs(posW1 % 5 - posW2 % 5), abs(posW1 / 5 - posW2 / 5)) - 1;
}

int scorer(const int positionScorer, const int enemyOne, const int enemyTwo, const int a, const int b, const int height){
    return a * min(dist(positionScorer, enemyOne), dist(positionScorer, enemyTwo)) + b * height;
}

int disruptor(const int positionDisruptor, const int enemyOne, const int enemyTwo, const int a, const int b, const int height){
    return b * height - a * max(dist(positionDisruptor, enemyOne), dist(positionDisruptor, enemyTwo));
}

int staticSpacer(const int a, const int b, const int c, const int d, const Board &board){
    const int scoreWhite1 =
         scorer(board.workers[0], board.workers[2], board.workers[3], a, b, board.getHeight(board.workers[0])) +
         disruptor(board.workers[1], board.workers[2], board.workers[3], c, d, board.getHeight(board.workers[1]));

    const int scoreWhite2 =
         scorer(board.workers[1], board.workers[2], board.workers[3], a, b, board.getHeight(board.workers[1])) +
         disruptor(board.workers[0], board.workers[2], board.workers[3], c, d, board.getHeight(board.workers[0]));

    const int scoreGray1 =
         scorer(board.workers[2], board.workers[0], board.workers[1], a, b, board.getHeight(board.workers[2])) +
         disruptor(board.workers[3], board.workers[0], board.workers[1], c, d, board.getHeight(board.workers[3]));

    const int scoreGray2 =
         scorer(board.workers[3], board.workers[0], board.workers[1], a, b, board.getHeight(board.workers[3])) +
         disruptor(board.workers[2], board.workers[0], board.workers[1], c, d, board.getHeight(board.workers[2]));
    return max(scoreWhite1, scoreWhite2) - max(scoreGray1, scoreGray2);
}

int ss_h(const Board &b){
    return staticSpacer(1, 4, 3, 1, b);
}

int cyan(const Board &b) {
    constexpr int posScore[] =   {-50, -30, -10, -30, -50,
                                  -30,  10,  30,  10, -30,
                                  -10,  30,  50,  30,  10,
                                  -30,  10,  30,  10, -30,
                                  -50, -30, -10, -30, -50,};

    constexpr int heightScore[] = {0, 100, 400};

    constexpr int sameHeightSupport[] = {-30, 0, 55};
    constexpr int nextHeightSupport[] = {0, 35, 120};

    auto scoreWorker = [&](const int worker_pos) {
        const int square = b.workers[worker_pos];
        const int height = b.squares[square];

        const int pScore = posScore[square];
        const int hScore = heightScore[height];

        int support = 0;

        if (height > 0) {
            int sameH = 0;
            int nextH = 0;
            for(const auto n: neighbors[square]) {
                if(b.isFree(n)) {
                    if(b.squares[n] == height) sameH++;
                    else if (b.squares[n] == height + 1) nextH++;
                }
            }
            sameH = min(sameH, 2);
            nextH = min(nextH, 2);
            support = sameHeightSupport[sameH] + nextHeightSupport[nextH];
        }
        cout << pScore << " " << hScore << " " << support << endl;
        return pScore + hScore + support;
    };
    cout <<  scoreWorker(0) << " " << scoreWorker(1) << " " <<  scoreWorker(2) << " " <<  scoreWorker(3) << endl;
    return scoreWorker(0) + scoreWorker(1) - scoreWorker(2) - scoreWorker(3);
}