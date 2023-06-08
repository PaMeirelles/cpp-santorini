#include "move.h"

std::vector<int> neighbors[] = {
    {1, 5, 6},
    {0, 2, 5, 6, 7},
    {1, 3, 6, 7, 8},
    {2, 4, 7, 8, 9},
    {3, 8, 9},

    {0, 1, 6, 10, 11},
    {0, 1, 2, 5, 7, 10, 11, 12},
    {1, 2, 3, 6, 8, 11, 12, 13},
    {2, 3, 4, 7, 9, 12, 13, 14},
    {3, 4, 8, 13, 14},

    {5, 6, 11, 15, 16},
    {5, 6, 7, 10, 12, 15, 16, 17},
    {6, 7, 8, 11, 13, 16, 17, 18},
    {7, 8, 9, 12, 14, 17, 18, 19},
    {8, 9, 13, 18, 19},

    {10, 11, 16, 20, 21},
    {10, 11, 12, 15, 17, 20, 21, 22},
    {11, 12, 13, 16, 18, 21, 22, 23},
    {12, 13, 14, 17, 19, 22, 23, 24},
    {13, 14, 18, 23, 24},

    {15, 16, 21},
    {15, 16, 17, 20, 22},
    {16, 17, 18, 21, 23},
    {17, 18, 19, 22, 24},
    {18, 19, 23}
};

std::vector<int> getNeighbors(int n){
    return neighbors[n];
}

Move::Move(int f, int t, int b){
    from = f;
    to = t;
    build = b;
}

Move::Move(int f, int t, int b, int fh, int th){
    from = f;
    to = t;
    build = b;
    fromHeight = fh;
    toHeight = th;
}
void Move::printMove() {
        std::cout << "From: " << from << ", To: " << to << ", Build: " << build << std::endl;
    }
std::string Move::toString(){
    return "From: " + std::to_string(from) + ", To: " + std::to_string(to) + ", Build: " + std::to_string(build);
}