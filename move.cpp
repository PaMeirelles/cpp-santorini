#include "move.h"

using namespace std;

vector<int> getNeighbors(const int n){
    return neighbors[n];
}

string getSquareName(const int sq) {
    return squareNames[sq];
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

string Move::toString() const{
    string bd;
    if(build == -3) {
        bd = "WIN";
    }
    else {
        bd = getSquareName(build);
    }
    return getSquareName(from) + "|" + getSquareName(to) + "|" + bd;
}