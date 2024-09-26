#ifndef HASHWORKERS_H
#define HASHWORKERS_H
#define U64 unsigned long long
#include <cmath>
#include <random>
#include "hash.h"
#include "board.h"
int hashWorkers(const int (&w)[4]);
void unhashWorkers(int hash, int * w);
bool validateHash(int hash);
int genHash();
void fillAlternativeHashes(int hash, int * alternativeHashes);
int reduceHash(int hash);

struct Coord{
    int x;
    int y;
};

Coord cordFromTwoInt(int x, int y);
Coord makeCoord(int n);
int breakCoord(Coord coord);
U64 hashBoard (const Board * b);
#endif // HASHWORKERS_H