#include <iostream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "board.h"
#include "eval.h"
#include "matchManager.h"

int main() {
    int w[4] = {0, 4, 20, 24};
    Board b = Board(w);
    b.print();
    Move m = Move(0, 1, 7);
    b.makeMove(m);
    b.print();
    b.unmakeMove(m);
    b.print();
    return 0;
}
