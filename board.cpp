#include "board.h"

Board::Board(const int (&w)[4]) {
    // Fill the squares array with zeros
    for (int i = 0; i < 25; ++i) {
        squares[i] = 0;
    }
  
    // Copy the worker array to the workers array
    for (int i = 0; i < 4; ++i) {
        workers[i] = w[i];
    }
}
