#pragma once
#include <vector>
#include <iostream>

#ifndef MOVE_H
#define MOVE_H

using namespace std;


inline vector<int> neighbors[] = {
    {1, 5, 6},
    {0, 2, 5, 6, 7},
    {1, 3, 6, 7, 8},
    {2, 4, 7, 8, 9},
    {3, 8, 9},

    {0, 1, 6, 10, 11},
    {0, 1, 2, 5, 7, 10, 11, 12},
    {1, 2, 3, 6, 8, 11, 12, 13},
    {2, 3, 4, 7, 9, 12, 13, 14},
    {3, 4, 8, 13, 14},

    {5, 6, 11, 15, 16},
    {5, 6, 7, 10, 12, 15, 16, 17},
    {6, 7, 8, 11, 13, 16, 17, 18},
    {7, 8, 9, 12, 14, 17, 18, 19},
    {8, 9, 13, 18, 19},

    {10, 11, 16, 20, 21},
    {10, 11, 12, 15, 17, 20, 21, 22},
    {11, 12, 13, 16, 18, 21, 22, 23},
    {12, 13, 14, 17, 19, 22, 23, 24},
    {13, 14, 18, 23, 24},

    {15, 16, 21},
    {15, 16, 17, 20, 22},
    {16, 17, 18, 21, 23},
    {17, 18, 19, 22, 24},
    {18, 19, 23}
};

struct Move{
    int from;
    int to;
    int build;
    int fromHeight;
    int toHeight;
    int moveOrderingScore;
    int fromN;
    int toN;

    Move();
    Move(int from, int to, int build);
    Move(int from, int to, int build, int fromHeight, int toHeight);

    void printMove() const;

    [[nodiscard]] string toString() const;

    bool operator==(const Move& other) const {
    return from == other.from && to == other.to && build == other.build &&
            fromHeight == other.fromHeight && toHeight == other.toHeight;
    }
    static const Move NO_MOVE;
};

std::vector<int> getNeighbors(int n);
#define HALF_MOVE (-1)
#define WIN (-3)
#define NO_MOVE Move(-2, -2, -2)
#endif

