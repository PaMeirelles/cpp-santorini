#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include "board.h"
#include "hash.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>

int evalPosition(std::function<int(Board)> eval, int hash);

std::set<int> getPositionsForMatch(std::function<int(Board)> evalOne,
                                   std::function<int(Board)> evalTwo, int n);

struct EngineInfo {
  std::function<int(Board, int, std::function<int(Board)>)> search;
  std::function<int(Board)> eval;
  std::function<int(int)> timeManager;
};

enum Engines { Titan, Ruler };
void playMatch(int startingPos, int time, std::string playerA,
               std::string playerB);

#endif /* MATCH_MANAGER_H */
