#pragma once
#include <vector>
#include <iostream>

#ifndef MOVE_H
#define MOVE_H

struct Move{
    int from;
    int to;
    int build;
    int fromHeight;
    int toHeight;

    Move();
    Move(int from, int to, int build);
    Move(int from, int to, int build, int fromHeight, int toHeight);

    void printMove() const;

    std::string toString() const;

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

