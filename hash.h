#ifndef HASHWORKERS_H
#define HASHWORKERS_H

int hashWorkers(const int (&w)[4]);
void unhashWorkers(int hash, int * w);
bool validateHash(int hash);
int genHash();

# define minHash 0
# define maxHash 390625

#endif // HASHWORKERS_H