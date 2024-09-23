#include "move.h"

using namespace std;

std::vector<int> getNeighbors(const int n){
    return neighbors[n];
}

Move::Move(int f, int t, int b): fromHeight(0), toHeight(0) {
    from = f;
    to = t;
    build = b;
}

Move::Move()= default;

Move::Move(int f, int t, int b, int fh, int th){
    from = f;
    to = t;
    build = b;
    fromHeight = fh;
    toHeight = th;
    fromN = static_cast<int>(neighbors[f].size());
    toN = static_cast<int>(neighbors[t].size());
}

void Move::printMove() const {
        std::cout << "From: " << from << ", To: " << to << ", Build: " << build << std::endl;
    }
std::string Move::toString() const{
    return "From: " + std::to_string(from) + ", To: " + std::to_string(to) + ", Build: " + std::to_string(build);
}