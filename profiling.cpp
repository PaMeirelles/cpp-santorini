//
// Created by Rafael on 10/14/2024.
//

#include "board.h"
#include "hashTable.h"
#include "search.h"

void profile() {
    auto b = Board(1200);
    auto ht = HashTable();
    allocateHashTable(&ht, 1000);

    auto start = std::chrono::high_resolution_clock::now(); // Start timing
    getBestMove(&b, 60 * 1000 * 10 * 1000, &ht, 6);
    auto end = std::chrono::high_resolution_clock::now(); // End timing

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // Calculate duration
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl; // Print duration
}
