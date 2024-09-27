//
// Created by Rafael on 9/26/2024.
//

#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "board.h"
#include "hashTable.h"
using namespace std;
inline vector<string> numToSquare = {
    "a1", "b1", "c1", "d1", "e1",
    "a2", "b2", "c2", "d2", "e2",
    "a3", "b3", "c3", "d3", "e3",
    "a4", "b4", "c4", "d4", "e4",
    "a5", "b5", "c5", "d5", "e5",
};
void mainLoop(Board * b, HashTable * ht);
#endif //INTERFACE_H
