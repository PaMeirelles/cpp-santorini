//
// Created by Rafael on 9/26/2024.
//

#include "board.h"

Board parse_position(const string &position) {
    if(position.size() != 50) throw runtime_error("Invalid position");

    int blocks[25];
    int workers[4];

    int numGrayWorkers = 0;
    int numBlueWorkers = 0;

    for(int i=0; i < 25; i++) {
        char buffer = position[2 * i];
        const int height = buffer - '0';
        if (height < 0 || height > 4) throw runtime_error("Invalid position");
        blocks[i] = height;
        buffer = position[2 * i + 1];
        if (buffer == 'G') {
            if (numGrayWorkers == 2) throw runtime_error("Invalid position");
            workers[numGrayWorkers] = i;
            numGrayWorkers++;
        }
        else if (buffer == 'B') {
            if (numBlueWorkers == 2) throw runtime_error("Invalid position");
            workers[2 + numBlueWorkers] = i;
            numBlueWorkers++;
        }
    }
    if(numGrayWorkers != 2 || numBlueWorkers != 2) throw runtime_error("Invalid position");
    return {workers, blocks};
}