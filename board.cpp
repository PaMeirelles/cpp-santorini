#include "board.h"
#include "hash.h"
#include <iostream>

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
    int w[4];
    int hash = genHash();

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

int Board::getWorkerHeight(int workerId){
    return squares[workers[workerId]];
}
int Board::getWorkerPos(int workerId){
    return workers[workerId];
}