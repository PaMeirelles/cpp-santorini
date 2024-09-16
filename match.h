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

void play(int time, int numMatches, const std::string& playerA, const std::string& playerB);
void playSingleMatch(int time, const std::string& playerA, const std::string& playerB, int pos);
void heal(int time);
void playHighestConfidence(int time, int blockSize);
#define CLOCK true
#define BOARD false
#endif  // MATCH_H
