#include <iostream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "board.h"
#include "eval.h"

int main() {
    Board b = Board();
    b.print();
    std::cout << nh_c(b);
}
