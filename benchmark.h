#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <map>
#include <functional>
#include <chrono>
#include "search.h"
#include "eval.h"

struct Record {
    int id;
    int starting_pos;
    std::string player_a;
    std::string player_b;
    int time_a;
    int time_b;
    std::string result;
};

std::map<std::string, std::function<SearchResult(Board, int, std::function<int(Board)>, int)>> getEngineMap();
std::vector<int> getRandomStartingPos(int n);

std::vector<int> runTest(std::vector<int> positions, std::string search, int depth);
void twinTest(std::string search1, std::string search2, int n, int depth);
void testUpTo(std::string search1, std::string search2, int n, int depth);
#endif  // BENCHMARK_H
