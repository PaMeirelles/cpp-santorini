#ifndef SEARCH_H
#define SEARCH_H

#include <functional>
#include "board.h"
#include "hashTable.h"
#include <chrono>

using namespace std;


struct SearchResult{
    Move move;
    int score{};
    vector<Move> pvLine{};
};

struct SearchInfo{
    Board * b{};
    int depth{};
    U64 * nodes{};
    bool quit{};
    HashTable * hashTable{};
    chrono::high_resolution_clock::time_point end;
    SearchInfo(Board * b, int depth, U64 * nodes, bool quit, HashTable * hashTable, chrono::high_resolution_clock::time_point end);
};

Move getBestMove(Board * b, int remaining_time, HashTable * hashTable);
Move getBestMove(Board * b, int remaining_time, HashTable * hashTable, int maxDepth);
#endif