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
    int w[4] = {12, 13, 11, 0};
    int s[25] = 
        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         2, 1, 0, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0};

    int w1[4] = {6, 13, 11, 1};
    int s1[25] = 
        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         2, 1, 0, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0};

    int w2[4] = {12, 18, 10, 0};
    int s2[25] = 
        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         2, 1, 0, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0};

    Board b = Board(w, s);
    Board b1 = Board(w1, s1);
    Board b2 = Board(w2, s2);

    std::cout << nh_s(b1) << std::endl;
    std::cout << nh_s(b2) << std::endl;

    getBestMove(b, negamax, nh_s, et_s, 1000 * 15 * 10);
}
