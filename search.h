#ifndef SEARCH_H
#define SEARCH_H

#include <functional>
#include "board.h"
#include "hashTable.h"
#include "timeManagement.h"
#include <chrono>

using namespace std;

struct AlphaBetaInfo{
    int alpha;
    int beta;

    AlphaBetaInfo(int alpha, int beta);
};

struct SearchResult{
    Move move;
    int score{};
    bool outOftime{};
    bool keepResult{};
    SearchResult(const Move &move, int score, bool oot);

    SearchResult(Move &m, int s, bool o, bool k);

    SearchResult();
};

struct SearchInfo{
    Board b;
    int depth;
    function<int(Board)> eval;
    int time;
    HashTable hashTable;
    std::chrono::_V2::system_clock::time_point start;

    SearchInfo(const Board &b, int depth, std::function<int(Board)> eval, int time, const HashTable &hashTable, std::chrono::_V2::system_clock::time_point start);
};

struct TimeInfo{
    int * diveCheck;
    int time;
    std::chrono::_V2::system_clock::time_point start;
    bool * oot;

    TimeInfo(int * dc, int t, std::chrono::_V2::system_clock::time_point s, bool * o);
};
struct EngineInfo {
  std::function<SearchResult(SearchInfo)> search;
  std::function<int(Board)> eval;
  std::function<int(TMInfo)> timeManager;
  HashTable hashTable;
};

SearchResult alphabeta(SearchInfo searchInfo);
SearchResult negamax(const SearchInfo& searchInfo);
SearchResult mvb15(SearchInfo si);
SearchResult mvb127(SearchInfo si);
SearchResult mvb143(SearchInfo si);
SearchResult properMO(SearchInfo si);
SearchResult properMOV2(SearchInfo si);
SearchResult properMOV3(SearchInfo si);
SearchResult creator(SearchInfo si);
Move getBestMove(Board b, EngineInfo engineInfo, int time);
std::vector<Move> bucketOrder(const std::vector<Move>& moves, HashTable * hashTable, Board b, int depth);
#define VERBOSE true
#define DIVE_CHECK 1000
#endif