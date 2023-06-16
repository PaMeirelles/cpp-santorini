#ifndef PRE_MATCH_H
#define PRE_MATCH_H

#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "board.h"
#include "hash.h"
#include "eval.h"
#include "search.h"
#include "timeManagement.h"
#include "hashTable.h"

struct Entry {
    std::string starting_pos;
    std::string player_a;
    std::string player_b;
};
struct EntryHash {
    std::size_t operator()(const Entry& entry) const {
        std::size_t starting_pos_hash = std::hash<std::string>{}(entry.starting_pos);
        std::size_t player_a_hash = std::hash<std::string>{}(entry.player_a);
        std::size_t player_b_hash = std::hash<std::string>{}(entry.player_b);
        return starting_pos_hash ^ (player_a_hash << 1) ^ (player_b_hash << 2);
    }
};

struct EntryEqual {
    bool operator()(const Entry& lhs, const Entry& rhs) const {
        return lhs.starting_pos == rhs.starting_pos &&
               lhs.player_a == rhs.player_a &&
               lhs.player_b == rhs.player_b;
    }
};
int evalPosition(std::function<int(Board)> eval, int hash);
std::vector<int> getValidPositions(const std::string& playerA, const std::string& playerB);
std::set<int> getPositionsForMatch(std::string playerA, std::string playerB, int n);
EngineInfo assemblyEngine(std::string name);
std::unordered_set<Entry, EntryHash, EntryEqual> findEntriesWithNoPair(int targetTime);
#endif  // PRE_MATCH_H