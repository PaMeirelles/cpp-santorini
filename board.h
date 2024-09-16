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
    int getWorkerHeight (int workerId) const;
    int getWorkerPos (int workerId) const;
    void makeMove (const Move &move);
    void unmakeMove (const Move &move);
    std::vector<Move> gen_moves(int player) const;
    std::vector<Move> gen_half_moves(int player) const;
    bool isFree(int square) const;
    int getHeight(int square) const;

};
#endif
