#include <iostream>
#include "board.h"
#include "hash.h"

int main() {
    int w[4] = {4, 3, 2, 1};
    int hash = hashWorkers(w);
    unhashWorkers(hash, w);
    for(int i=0; i < 4; i ++){
        std::cout << w[i] << " ";
    }
    std::cout << hash;
    return 0;
}
