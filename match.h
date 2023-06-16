#ifndef MATCH_H
#define MATCH_H

#include "search.h"
#include "postMatch.h"
#include "preMatch.h"
#include "move.h"
#include <chrono>
#include <iomanip>
#include <map>

void printClocks(int clockA, int clockB);

MatchResult playMatch(int time, int startingPos, EngineInfo engineA, EngineInfo engineB);

void play(int time, int numMatches, std::string playerA, std::string playerB);
void playSingleMatch(int time, std::string playerA, std::string playerB, int pos);
void heal(int time);
#define CLOCK true
#define BOARD true
#endif  // MATCH_H
