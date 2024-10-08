#ifndef POST_MATCH_H
#define POST_MATCH_H

#include <vector>
#include <string>
#include "move.h"

using namespace std;

struct MatchResult {
    // positive = player1, negative = player 2
    // 1 = ascension, 2 = suffocation, 3 = timeout, 4 = illegal move
    int result;
    vector<Move> moves;
};
void saveMoves(const vector<Move>& moves, int id);
void registerMatch(int startingPos, const string& playerA, const string& playerB, int timeA, int timeB, const MatchResult& result);
#endif // POST_MATCH_H
