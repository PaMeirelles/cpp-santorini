#include <functional>
#include "board.h"

int negamax(Board b, int depth, std::function<int(Board)> eval);
Move getBestMove(Board b, std::function<int(Board)> eval, std::function<int(int)> timeManager, int time);
#define VERBOSE true