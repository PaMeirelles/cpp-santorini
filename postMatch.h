#ifndef POST_MATCH_H
#define POST_MATCH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "move.h"
#include <algorithm>

struct MatchResult {
    int result;
    std::vector<Move> moves;
};

void appendToCSV(const std::string& filename, const std::string& row);
int findLowestUnusedID();
void saveMoves(const std::vector<Move>& moves, int id);
void registerMatch(int startingPos, std::string playerA, std::string playerB, int timeA, int timeB, MatchResult result);
#endif // POST_MATCH_H
