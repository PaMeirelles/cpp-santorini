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
};
std::vector<int> getNeighbors(int n);
#endif

