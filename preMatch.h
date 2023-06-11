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
#include "board.h"
#include "hash.h"
#include "eval.h"
#include "search.h"
#include "timeManagement.h"

struct EngineInfo {
  std::function<SearchResult(SearchInfo)> search;
  std::function<int(Board)> eval;
  std::function<int(int)> timeManager;
};
int evalPosition(std::function<int(Board)> eval, int hash);
std::vector<int> getValidPositions(const std::string& playerA, const std::string& playerB);
std::set<int> getPositionsForMatch(std::string playerA, std::string playerB, int n);
EngineInfo assemblyEngine(std::string name);

#endif  // PRE_MATCH_H