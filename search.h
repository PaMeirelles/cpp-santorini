#include <functional>
#include "board.h"

int negamax(Board b, int depth, int turn, std::function<int(Board)> eval);
Move getBestMove(Board b, int turn, std::function<int(Board)> eval, std::function<int(int)> timeManager, int time);
