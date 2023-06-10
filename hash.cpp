#include "hash.h"

int hashWorkers(const int (&w)[4]){
    int sum = 0;
    for(int i=0; i < 4; i++){
        sum += w[i] * pow(25, i);
    }
    return sum;
}

void unhashWorkers(int hash, int * w){
    for(int i=0; i < 4; i++){
        w[i] = hash % 25;
        hash /= 25;
    }
}

bool validateHash(int hash){
    int w[4];
    unhashWorkers(hash, w);
    std::unordered_set<int> uniqueElements;
    
    for (int i = 0; i < 4; i++) {
        // If the element is already in the set, it is a duplicate
        if (uniqueElements.find(w[i]) != uniqueElements.end()) {
            return false;
        }
        
        // Add the element to the set
        uniqueElements.insert(w[i]);
    }
    
    return true;
}

int genHash(){
    // Create a random device
    std::random_device rd;

    // Create a random engine
    std::mt19937 eng(rd());

    // Create a uniform distribution
    std::uniform_int_distribution<> dist(minHash, maxHash);

    int hash = dist(eng);
    if(validateHash(hash)){
        return reduceHash(hash);
    }
    else{
        return genHash();
    }
}

Coord cordFromTwoInt(int x, int y){
    Coord coord;
    coord.x = x;
    coord.y = y;
    return coord;
}

Coord makeCoord(int n){
    return cordFromTwoInt(n % 5, n / 5);
}

int breakCoord(Coord coord){
    return 5 * coord.y + coord.x;
}

void fillSimetryTable(Coord coord, Coord * symetryTable){
    int x = coord.x;
    int y = coord.y;
    Coord st[] = {cordFromTwoInt(x, y), cordFromTwoInt(y, 4-x),
                  cordFromTwoInt(4-x, 4-y), cordFromTwoInt(4-y, x),
                  cordFromTwoInt(4-x, y), cordFromTwoInt(4-y, 4-x),
                  cordFromTwoInt(x, 4-y), cordFromTwoInt(y, x),
                 };
    for(int i=0; i < 8; i++){
        symetryTable[i] = st[i];
    }
}

void fillAlternativeHashes(int hash, int * alternativeHashes){
    int w[4];
    Coord symetryTable[4][8];
    unhashWorkers(hash, w);
    for(int i=0; i < 4; i++){
        fillSimetryTable(makeCoord(w[i]), symetryTable[i]);
    }
    for(int i=0; i < 8; i++){
        for(int j =0; j < 4; j++){
            w[j] = breakCoord(symetryTable[j][i]);
        }
        alternativeHashes[i] = hashWorkers(w);
    }
}

int semiReduce(int hash){
    int alterativeHashes[8];
    fillAlternativeHashes(hash, alterativeHashes);
    int wah[8][4];
    for(int i=0; i < 8; i++){
        unhashWorkers(alterativeHashes[i], wah[i]);
    }
    int min = alterativeHashes[0];  // Assume the first element as the minimum

    for (int i = 1; i < 8; i++) {
        if (alterativeHashes[i] < min) {
            min = alterativeHashes[i];
        }
    }
    return min;
}
int reduceHash(int hash){
    int w[4];
    unhashWorkers(hash, w);
    int alternateStarts[3][4] = {{w[0], w[1], w[3], w[2]},
                                 {w[1], w[0], w[2], w[3]},
                                 {w[1], w[0], w[3], w[2]}};
    int min = semiReduce(hash);
    int curr;
    for(int i=0; i < 3; i++){
        curr = semiReduce(hashWorkers(alternateStarts[i]));
        if(curr < min){
            min = curr;
        }
    }
    return min;
}

U64 hashBoard (Board b){
    U64 key = 0;
    for(int i=0; i < 25; i++){
        key += pow(5, i) * b.squares[i];
    }
    key += pow(5, 25) * hashWorkers(b.workers);
    return key;
}