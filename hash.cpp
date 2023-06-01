#include <cmath>


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