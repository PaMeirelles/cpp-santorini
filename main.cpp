#include <iostream>
#include "hash.h"

int main() {
    int w[4] = {0, 1, 0, 3};
    int h = hashWorkers(w);
    bool b = validateHash(h);
    std::cout << "Boolean value (without std::boolalpha): " << b << std::endl;
}
