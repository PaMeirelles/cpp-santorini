//
// Created by Rafael on 10/14/2024.
//

#include "board.h"
#include "hashTable.h"
#include "search.h"

void profile() {
    auto b = Board();
    auto ht = HashTable();
    allocateHashTable(&ht, 1000);
    getBestMove(&b, 60 * 1000 * 10, &ht);
}
