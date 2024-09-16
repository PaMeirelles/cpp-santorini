#include "hash.h"

#include <unordered_set>

using namespace std;

int hashWorkers(const int (&w)[4]){
    int sum = 0;
    for(int i=0; i < 4; i++){
        sum += w[i] * static_cast<int>(pow(25, i));
    }
    return sum;
}

void unhashWorkers(int hash, int * w){
    for(int i=0; i < 4; i++){
        w[i] = hash % 25;
        hash /= 25;
    }
}

bool validateHash(const int hash){
    int w[4];
    unhashWorkers(hash, w);
    unordered_set<int> uniqueElements;
    
    for (int i : w) {
        // If the element is already in the set, it is a duplicate
        if (uniqueElements.find(i) != uniqueElements.end()) {
            return false;
        }
        
        // Add the element to the set
        uniqueElements.insert(i);
    }
    
    return true;
}

int genHash(){
    // Create a random device
    random_device rd;

    // Create a random engine
    mt19937 eng(rd());

    // Create a uniform distribution
    uniform_int_distribution<> dist(minHash, maxHash);

    const int hash = dist(eng);
    if(validateHash(hash)){
        return reduceHash(hash);
    }
    return genHash();
}

Coord cordFromTwoInt(const int x, const int y){
    Coord coord{};
    coord.x = x;
    coord.y = y;
    return coord;
}

Coord makeCoord(const int n){
    return cordFromTwoInt(n % 5, n / 5);
}

int breakCoord(const Coord coord){
    return 5 * coord.y + coord.x;
}

void fillSymmetryTable(const Coord coord, Coord * symmetryTable){
    const int x = coord.x;
    const int y = coord.y;
    const Coord st[] = {cordFromTwoInt(x, y), cordFromTwoInt(y, 4-x),
                  cordFromTwoInt(4-x, 4-y), cordFromTwoInt(4-y, x),
                  cordFromTwoInt(4-x, y), cordFromTwoInt(4-y, 4-x),
                  cordFromTwoInt(x, 4-y), cordFromTwoInt(y, x),
                 };
    for(int i=0; i < 8; i++){
        symmetryTable[i] = st[i];
    }
}

void fillAlternativeHashes(const int hash, int * alternativeHashes){
    int w[4];
    Coord symmetryTable[4][8];
    unhashWorkers(hash, w);
    for(int i=0; i < 4; i++){
        fillSymmetryTable(makeCoord(w[i]), symmetryTable[i]);
    }
    for(int i=0; i < 8; i++){
        for(int j =0; j < 4; j++){
            w[j] = breakCoord(symmetryTable[j][i]);
        }
        alternativeHashes[i] = hashWorkers(w);
    }
}

int semiReduce(const int hash){
    int alternative_hashes[8];
    fillAlternativeHashes(hash, alternative_hashes);
    for(int i=0; i < 8; i++) {
        int wah[8][4];
        unhashWorkers(alternative_hashes[i], wah[i]);
    }
    int min = alternative_hashes[0];

    for (int i = 1; i < 8; i++) {
        if (alternative_hashes[i] < min) {
            min = alternative_hashes[i];
        }
    }
    return min;
}
int reduceHash(const int hash){
    int w[4];
    unhashWorkers(hash, w);
    const int alternateStarts[3][4] =
                                {{w[0], w[1], w[3], w[2]},
                                 {w[1], w[0], w[2], w[3]},
                                 {w[1], w[0], w[3], w[2]}};
    int min = semiReduce(hash);
    for(int i=0; i < 3; i++){
        const int curr = semiReduce(hashWorkers(alternateStarts[i]));
        if(curr < min){
            min = curr;
        }
    }
    return min;
}

U64 hashBoard (const Board &b){
    U64 key = 0;
    for(int i=0; i < 25; i++){
        key += static_cast<U64>(pow(5, i)) * b.squares[i];
    }
    key *= hashWorkers(b.workers);
    return key;
}