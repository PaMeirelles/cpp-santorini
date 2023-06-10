#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"

int main() { 
    // play(1 * 60 * 1000, 1200, "Ascendant", "Whisper");
    //  testUpTo("climb mo v10", "alphabeta", 10, 6);
    int w[4] = {11, 13, 7, 12};
    Board b = Board(w);
    Move m = Move(11, 10, 11);
    HashTable ht;
    HashEntry he;
    allocateHashTable(&ht, 400);
    storeHashEntry(b, m, 10, 2, 'e', &ht);
    probeHashEntry(&he, b, &ht);
    he.move.printMove();
 }
