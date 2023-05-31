#include <cmath>


int hashWorkers(const int (&w)[4]){
    int sum = 0;
    for(int i=0; i < 4; i++){
        sum += w[i] * pow(25, i);
    }
    return sum;
}

void unhashWorkers(int hash, int * w){
    int buffer;
    for(int i=0; i < 4; i++){
        buffer = hash % 25;
        w[i] = buffer;
        hash /= 25;
    }
}