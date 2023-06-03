#include <iostream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "board.h"
#include "eval.h"
#include "matchManager.h"

int main() {
    std::set<int> pos = getPositionsForMatch(nh_c, nh_c, 10);
    int w[4];
    for (int element : pos) {
        unhashWorkers(element, w);
        for(int x: w){
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
