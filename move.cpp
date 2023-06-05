#include "move.h"

std::vector<int> getNeighbors(int n){
    return neighbors[n];
}

Move::Move(int f, int t, int b){
    from = f;
    to = t;
    build = b;
}

void Move::printMove() {
        std::cout << "From: " << from << ", To: " << to << ", Build: " << build << std::endl;
    }