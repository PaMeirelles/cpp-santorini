#include <iostream>
#include "hash.h"
#include "board.h"

int main() {
    int w[4] = {0, 4, 20, 24};
    int hash = hashWorkers(w);
    std::cout << hash << " ";
    std::cout << reduceHash(hash) << " ";
    unhashWorkers(reduceHash(hash), w);
    for(int i=0; i < 4; i++){
        std::cout << w[i] << " ";
    }
    return 0;
}
