#include <iostream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "board.h"
#include "eval.h"
#include "matchManager.h"

int main() {
    int workers[4] = {0, 4, 20, 24};
    Board b = Board(workers);
    Move m;
    m.from = 1;
    m.to = 1;
    m.build = 0;

    b.makeMove(m);
    b.print();
}
