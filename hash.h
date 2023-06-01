#ifndef HASHWORKERS_H
#define HASHWORKERS_H

int hashWorkers(const int (&w)[4]);
void unhashWorkers(int hash, int * w);

int minHash = hashWorkers({0, 1, 2, 3});
int maxHash = hashWorkers({21, 22, 23, 24});


#endif // HASHWORKERS_H