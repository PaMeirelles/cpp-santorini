#include "search.h"
#include "board.h"
#include "hashTable.h"
#include "interface.h"

using namespace std;

int main() {
    string position =
        "0N0N0N0N0N"
        "0N0N0G0N0N"
        "0N0G0B0B0N"
        "0N0N0N0N0N"
        "0N0N0N0N0N";
    Board b = parse_position(position);
    int time = 1000 * 60;
    HashTable ht = HashTable();
    allocateHashTable(&ht, 1000);
    auto bestMove = getBestMove(&b, time, &ht);
}
