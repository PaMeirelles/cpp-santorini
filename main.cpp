#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"

using namespace std;

int main() { 
    play(1000 * 60 * 30, 1, "Cosmic", "Pilot");

    while (true){
        system("python core.py");
        system(".\\pushToGit.bat");
        heal(1000 * 60);
        play(1000 * 60 * 1, 24, "Eclipse", "Cosmic");
        play(1000 * 60 * 1, 24, "Pilot", "Cosmic");
    }
    // int squares[25] = 
    //     {1, 0, 4, 2, 0,
    //      1, 0, 1, 3, 1,
    //      1, 4, 0, 1, 0,
    //      0, 2, 0, 0, 1,
    //      0, 0, 0, 2, 4};

    // Board b = Board({12, 7, 23, 1}, squares);
    // auto moves = b.gen_moves(1);
    // for(Move move: moves){
    //     move.printMove();
    // }
    // cout << ss_h(b);
 }
