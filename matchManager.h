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
#include <sstream>
#include <string>
#include <unordered_set>

int evalPosition(std::function<int(Board)> eval, int hash);

std::set<int> getPositionsForMatch(std::string playerA,
                                   std::string playerB, int n);

struct EngineInfo {
  std::function<int(Board, int, std::function<int(Board)>)> search;
  std::function<int(Board)> eval;
  std::function<int(int)> timeManager;
};

int playMatch(int time, int startingPos, EngineInfo engineA, EngineInfo engineB);
EngineInfo assemblyEngine(std::string name);
void registerMatch(int starting_pos, std::string player_a, std::string player_b, int time_a, int time_b, int result);
int readCounter();
void updateCounter();
void play(int time, int numMatches, std::string playerA, std::string playerB);

#define DETAILED true

#endif /* MATCH_MANAGER_H */
