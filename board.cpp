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
void Board::makeMove(Move move) {
    bool workerFound = false;

    for (int i = 0; i < 4; i++) {
        if (workers[i] == move.from) {
            workerFound = true;
            
            if (workers[i] == move.to) {
                throw std::runtime_error("Invalid move: 'from' and 'to' positions cannot be the same.");
            }

            if (workers[i] == move.build) {
                throw std::runtime_error("Invalid move: a worker cannot be at the 'build' position.");
            }
            
            workers[i] = move.to;
            break;
        }
    }

    if (!workerFound) {
        throw std::runtime_error("Worker not found at the specified position.");
    }

    if (move.from > 24 || move.to > 24 || move.build > 24) {
        throw std::runtime_error("Invalid position specified in the move.");
    }

    if (squares[move.build] == 4) {
        throw std::runtime_error("Maximum number of squares already built at the specified position.");
    }

    squares[move.build]++;
}
