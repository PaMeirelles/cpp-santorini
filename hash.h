#ifndef HASHWORKERS_H
#define HASHWORKERS_H

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

# define minHash 0
# define maxHash 390625

#endif // HASHWORKERS_H