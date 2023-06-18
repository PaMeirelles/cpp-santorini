#include "move.h"

#ifndef BOARD_H
#define BOARD_H

struct Board {
    int squares[25];
    int workers[4];
    int turn;
    int ply;

    Board(int);
    Board(const int (&w)[4], const int (&s)[25]);
    Board(const int (&w)[4]);
    Board();

    void print() const;
    int getWorkerHeight (int workerId);
    int getWorkerPos (int workerId);
    void makeMove (Move move);
    void unmakeMove (Move move);
    std::vector<Move> gen_moves(int player);
    bool isFree(int square);
    int getHeight(int square);

};
#define WIN -1
#endif
