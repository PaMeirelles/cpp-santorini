#ifndef SEARCH_H
#define SEARCH_H

#include <functional>
#include "board.h"


struct AlphaBetaInfo{
    int alpha;
    int beta;

    AlphaBetaInfo(int alpha, int beta);
};

int alphabeta(Board b, int depth, std::function<int(Board)> eval);
int negamax(Board b, int depth, std::function<int(Board)> eval);
Move getBestMove(Board b, std::function<int(Board, int, std::function<int(Board)>)> search, std::function<int(Board)> eval, std::function<int(int)> timeManager, int time);
#define VERBOSE false

#endif