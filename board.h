#pragma once
#include "move.h"

#ifndef BOARD_H
#define BOARD_H

struct Board {
    int squares[25]{};
    int workers[4]{};
    int turn;
    int ply;

    explicit Board(int);
    Board(const int (&w)[4], const int (&s)[25]);
    explicit Board(const int (&w)[4]);
    Board();

    void print() const;
    [[nodiscard]] int getWorkerHeight (int workerId) const;
    [[nodiscard]] int getWorkerPos (int workerId) const;
    void makeMove (const Move &move);
    void unmakeMove (const Move &move);
    [[nodiscard]] vector<Move> gen_moves() const;
    [[nodiscard]] bool isFree(int square) const;
    [[nodiscard]] int getHeight(int square) const;

};
#endif
