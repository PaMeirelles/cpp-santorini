#include "search.h"
#include "board.h"
#include "hashTable.h"
using namespace std;

int main() {
    Board b = Board();
    int time = 1000 * 60;
    HashTable ht = HashTable();
    allocateHashTable(&ht, 1000);
    auto bestMove = getBestMove(&b, time, &ht);
}
