#include <iostream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "board.h"
#include "eval.h"
#include "matchManager.h"
#include "search.h"
#include "timeManagement.h"

int main() {
    int w[4] = {0, 4, 20, 24};
    Board b = Board(w);
    b.print();
    Move m = getBestMove(b, 1, nh_s, et_s, 60 * 1000 * 5);
    m.printMove();
}
