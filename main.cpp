#include <iostream>
#include "hash.h"
#include "board.h"

int main() {
    int w[4] = {0, 4, 20, 24};
    int hash = hashWorkers(w);
    int alterativeHashes[8];
    fillAlternativeHashes(hash, alterativeHashes);
    int wah[8][4];
    for(int i=0; i < 8; i++){
        unhashWorkers(alterativeHashes[i], wah[i]);
        for (int j = 0; j < 4; j++) {
        std::cout << wah[i][j] << " ";
    }
    std::cout << std::endl;
    }
    return 0;
}
