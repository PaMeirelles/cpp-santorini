#ifndef SEARCH_H
#define SEARCH_H

#include <functional>
#include "board.h"


struct AlphaBetaInfo{
    int alpha;
    int beta;

    AlphaBetaInfo(int alpha, int beta);
};

struct SearchResult{
    Move move;
    int score;
    bool outOftime;

    SearchResult(Move move, int score, bool oot);
    SearchResult();
};

SearchResult alphabetaWitClimbhMo(Board b, int depth, std::function<int(Board)> eval, int time);
SearchResult alphabeta(Board b, int depth, std::function<int(Board)> eval, int time);
SearchResult negamax(Board b, int depth, std::function<int(Board)> eval, int time);

Move getBestMove(
    Board b, std::function<SearchResult(Board, int, std::function<int(Board)>, int)> search,
    std::function<int(Board)> eval, std::function<int(int)> timeManager,
    int time);
#define VERBOSE false

#endif