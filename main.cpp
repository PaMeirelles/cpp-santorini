#include "search.h"
#include "board.h"
#include "hashTable.h"
#include "interface.h"

using namespace std;

int main() {
    auto ht = HashTable();
    Board b;
    allocateHashTable(&ht, 1000);
    mainLoop(&b, &ht);
}
