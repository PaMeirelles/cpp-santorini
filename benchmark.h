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
void runBenchmark(const Board &board, int matchId, const string &search_engine, const string &eval_function, int depth);
void runBenchmark(const string &search_engine, const string &eval_function, int depth);
void compare_engines(const string &engine_1, const string &engine_2, const string &eval, int depth);
#endif  // BENCHMARK_H
