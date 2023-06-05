#ifndef EVAL_H
#define EVAL_H

#include "board.h"
#include <functional>
#include <math.h>

int positionHeight(int a, int b, int c, Board board, std::function<int(int)> p);
int neighborHeightFunc(int pos);
int neighborHeight(int a, int b, int c, Board board);
int nh_s(Board board);
int nh_c(Board board);

#define MAX_SCORE 10000

int numNeighbors[] = {3, 5, 5, 5, 3,
                  5, 9, 9, 9, 5,
                  5, 9, 9, 9, 5,
                  5, 9, 9, 9, 5,
                  3, 5, 5, 5, 3};

#endif /* EVAL_H */