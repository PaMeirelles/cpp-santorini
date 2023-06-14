#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"

int main() { 
    // while(true){
    //     play(1000 * 60 * 1, 12, "Ascendant", "Whisper");
    //     heal(60 * 1000 * 1);

    // }
    int w[4] = {0, 1, 12, 13};
    int squares[25] = 
        {0, 0, 0, 0, 0,
         1, 1, 1, 1, 1,
         2, 1, 1, 1, 2,
         1, 1, 1, 1, 1,
         0, 0, 0, 0, 0};
    Board b = Board(w, squares);
    HashTable ht;
    allocateHashTable(&ht, 40);
    auto si = SearchInfo(b, 4, nh_s, 1000 * 15 * 60, ht);
    auto r = alphabeta(si);

    auto alphaBeta = AlphaBetaInfo(-MAX_SCORE, MAX_SCORE);
    int dc = 0;
    bool oot = false;
    auto start = std::chrono::high_resolution_clock::now();
    auto ti = TimeInfo(&dc, 1000 * 15 * 60, start, &oot);
    auto r2 = mvb3Recur(alphaBeta, 4, b, ti, nh_s, &ht);
    std::cout << r.score << " " << r2 << std::endl;
    printHashTable(ht);
 }
