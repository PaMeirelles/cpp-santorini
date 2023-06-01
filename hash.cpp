#include <cmath>
#include <unordered_set>
#include <random>
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
        return hash;
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