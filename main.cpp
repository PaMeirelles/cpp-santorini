#include <iostream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "board.h"
#include "eval.h"
#include "matchManager.h"

int main() {
    Board b = Board();
    std::vector<Move> moves = b.gen_moves(1);

    b.print();
    for(Move move: moves){
        move.printMove();
    }
    return 0;
}
