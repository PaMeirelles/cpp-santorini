#ifndef SEARCH_H
#define SEARCH_H

#include <functional>
#include "board.h"
#include "hashTable.h"
#include <chrono>

struct AlphaBetaInfo{
    int alpha;
    int beta;

    AlphaBetaInfo(int alpha, int beta);
};

struct SearchResult{
    Move move;
    int score;
    bool outOftime;
    bool keepResult;
    SearchResult(Move move, int score, bool oot, bool keepResult);
    SearchResult(Move move, int score, bool oot);
    SearchResult();
};

struct SearchInfo{
    Board b;
    int depth;
    std::function<int(Board)> eval;
    int time;
    HashTable hashTable;
    std::chrono::_V2::system_clock::time_point start;

    SearchInfo(Board b, int depth, std::function<int(Board)> eval, int time, HashTable hashTable, std::chrono::_V2::system_clock::time_point start);
};

struct TimeInfo{
    int * diveCheck;
    int time;
    std::chrono::_V2::system_clock::time_point start;
    bool * oot;

    TimeInfo(int * dc, int t, std::chrono::_V2::system_clock::time_point s, bool * o);
};

SearchResult alphabeta(SearchInfo searchInfo);
SearchResult negamax(SearchInfo searchInfo);
SearchResult mvb3(SearchInfo si);
Move getBestMove(
    Board b, std::function<SearchResult(SearchInfo)> search,
    std::function<int(Board)> eval, std::function<int(int)> timeManager,
    int time);
#define VERBOSE true

#endif