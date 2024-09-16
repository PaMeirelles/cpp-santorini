#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "move.h"
#include "board.h"
#include "hash.h"
#include <cmath>

struct HashEntry {
    U64 hashKey{};
    Move move;
    int depth{};
    int score{};
    char flag{};
};

struct HashTable {
    HashEntry *pTable;
    unsigned long long numEntries;
    unsigned long long newWrite;
    unsigned long long overWrite;
    unsigned long long hit;
    unsigned long long cut;
};

void clearHashTable(HashTable *table);
void allocateHashTable(HashTable *hashTable, int MB);
void storeHashEntry(const Board &b, const Move &m, int score, int depth, char flag, HashTable *hashTable);
bool probeHashEntry(const Board &b, HashTable * hashTable, Move * move, int * score, int alpha, int beta, int depth);
void freeHashTable(const HashTable * hashTable);
void printHashTable(const HashTable& table);
void printHashEntry(const HashEntry& entry);
Move probePvMove(const Board &b, const HashTable * hashTable, int * score);
#define DEBUG false
#endif // HASHTABLE_H
