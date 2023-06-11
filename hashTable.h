#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "move.h"
#include "board.h"
#include "hash.h"
#include <math.h>

struct HashEntry {
    U64 hashKey;
    Move move;
    int depth;
    int score;
    char flag;
};

struct HashTable {
    HashEntry *pTable;
    int numEntries;
    int newWrite;
    int overWrite;
    int hit;
    int cut;
};

void clearHashTable(HashTable *table);
void allocateHashTable(HashTable *hashTable, const int MB);
void storeHashEntry(Board b, Move m, int score, int depth, char flag, HashTable *hashTable);
bool probeHashEntry(HashEntry *hashEntry, Board b, HashTable *hashTable);
void freeHashTable(HashTable * hashTable);
#define DEBUG false
#endif // HASHTABLE_H
