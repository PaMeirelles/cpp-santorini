#ifndef SEARCH_H
#define SEARCH_H

#include <functional>
#include "board.h"
#include "hashTable.h"

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

struct SearchInfo{
    Board b;
    int depth;
    std::function<int(Board)> eval;
    int time;
    HashTable hashTable;

    SearchInfo(Board b, int depth, std::function<int(Board)> eval, int time, HashTable hashTable);
};

SearchResult alphabeta(SearchInfo searchInfo);
SearchResult negamax(SearchInfo searchInfo);

Move getBestMove(
    Board b, std::function<SearchResult(SearchInfo)> search,
    std::function<int(Board)> eval, std::function<int(int)> timeManager,
    int time);
#define VERBOSE false

#endif