#ifndef EVAL_H
#define EVAL_H

#include "board.h"
#include <functional>
int dist(int posW1, int posW2);
int positionHeight(int a, int b, int c, const Board &board, const std::function<int(int)>& p);
int neighborHeightFunc(int pos);
int neighborHeight(int a, int b, int c, const Board &board);
int nh_s(const Board &board);
int nh_c(const Board &board);
int nh_a(const Board &board);
int db_s(const Board &board);
int ss_h(const Board &b);
int nh_s_1(const Board &board);
int cyan(const Board &b);
#define MAX_SCORE 10000

#endif /* EVAL_H */