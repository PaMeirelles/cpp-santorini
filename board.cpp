#include "board.h"
#include "hash.h"
#include <iostream>
#include <random>

Board::Board(const int (&w)[4]) {
    // Fill the squares array with zeros
    for (int i = 0; i < 25; i++) {
        squares[i] = 0;
    }
  
    // Copy the worker array to the workers array
    for (int i = 0; i < 4; i++) {
        workers[i] = w[i];
    }
}
void Board::print() const {
    std::cout << "Squares:\n";
    for (int i = 0; i < 25; i++) {
        std::cout << squares[i] << " ";
        if ((i + 1) % 5 == 0) {
            std::cout << "\n";
        }
    }

    std::cout << "\nWorkers:\n";
    for (int i = 0; i < 4; i++) {
        std::cout << workers[i] << " ";
    }
    std::cout << std::endl;
}
Board::Board() {
    // Create a random device
    std::random_device rd;

    // Create a random engine

    std::mt19937 eng(rd());

    // Create a uniform distribution
    std::uniform_int_distribution<> dist(minHash, maxHash);

    int hash = dist(eng);
    int w[4];
    unhashWorkers(hash, w);

    // Fill the squares array with zeros
    for (int i = 0; i < 25; i++) {
        squares[i] = 0;
    }

    // Copy the worker array to the workers array
    for (int i = 0; i < 4; i++) {
        workers[i] = w[i];
    }
}