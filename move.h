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

    void printMove();
    std::string toString();
    bool operator==(const Move& other) const {
    return from == other.from && to == other.to && build == other.build &&
            fromHeight == other.fromHeight && toHeight == other.toHeight;
    }
};
std::vector<int> getNeighbors(int n);
#define NO_MOVE Move(-2, -2, -2)
#endif

