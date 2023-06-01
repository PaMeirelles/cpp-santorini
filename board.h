#ifndef BOARD_H
#define BOARD_H

struct Board {
    int squares[25];
    int workers[4];

    Board(const int (&w)[4]);
    Board();

    void print() const;
};

#endif
