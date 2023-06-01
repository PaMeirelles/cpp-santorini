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