#ifndef EVAL_H
#define EVAL_H

#include "board.h"
#include <functional>
#include <math.h>
int dist(int posW1, int posW2);
int positionHeight(int a, int b, int c, Board board, std::function<int(int)> p);
int neighborHeightFunc(int pos);
int neighborHeight(int a, int b, int c, Board board);
int nh_s(Board board);
int nh_c(Board board);
int nh_a(Board board);
int db_s(Board board);
int ss_h(Board b);
int nh_s_1(Board board);
#define MAX_SCORE 10000

#endif /* EVAL_H */