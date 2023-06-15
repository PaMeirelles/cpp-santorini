#include "hashTable.h"

void clearHashTable(HashTable *table) {

    HashEntry *tableEntry;
    table->cut = 0;
    table->hit = 0;
    table->newWrite = 0;
    table->overWrite = 0;
    for (tableEntry = table->pTable; tableEntry < table->pTable + table->numEntries; tableEntry++) {
        tableEntry->hashKey = 0ULL;
        tableEntry->move = NO_MOVE;
        tableEntry->depth = 0;
        tableEntry->score = 0;
        tableEntry->flag = ' ';
    }
}

void allocateHashTable(HashTable *hashTable, const int MB) {
    int hashSize = 0x100000 * MB;
    hashTable->numEntries = hashSize / sizeof(HashEntry);
    hashTable->numEntries -= 2;
    hashTable->pTable = (HashEntry *)malloc(hashTable->numEntries * sizeof(HashEntry));

    if (!hashTable->pTable) {
        if(DEBUG){
            std::cout << "Hash Allocation Failed, trying " << MB / 2 << "MB...\n";
        }
        allocateHashTable(hashTable, MB / 2);
    } else {
        clearHashTable(hashTable);
        if(DEBUG){
            std::cout << "HashTable init complete with " << hashTable->numEntries << " entries\n";
        }
    }
}


void storeHashEntry(Board b, Move m, int score, int depth, char flag, HashTable * hashTable){
    if(m.from < 0){
        throw std::runtime_error("Invalid move");
    }
    U64 key = hashBoard(b);
    int index = key % hashTable->numEntries;
    if(hashTable->pTable[index].hashKey == 0){
        hashTable->newWrite++;
    }
    else{
        hashTable->overWrite++;
    }
    hashTable->pTable[index].depth = depth;
    hashTable->pTable[index].score = score;
    hashTable->pTable[index].flag = flag;
    hashTable->pTable[index].move = m;
    hashTable->pTable[index].hashKey = key;
}

bool probeHashEntry(Board b, HashTable * hashTable, Move * move, int * score, int alpha, int beta, int depth){
    U64 key = hashBoard(b);
    int index = key % hashTable->numEntries;
    char flag;
    HashEntry he = hashTable->pTable[index];
    if(he.hashKey != key){
        return false;
    }
    (hashTable->hit)++;
    *move = he.move;
    if(he.depth < depth){
        return false;
    }
    flag = he.flag;
    if(flag == 'A'){
        *score = alpha;
    }
    else if(flag == 'B'){
        *score = beta;
    }
    else{
        *score = he.score;
    }
    return true;
}

void freeHashTable(HashTable * hashTable){
    free(hashTable->pTable);
}
void printHashTable(const HashTable& table) {
    std::cout << "Number of entries: " << table.numEntries << std::endl;
    std::cout << "New writes: " << table.newWrite << std::endl;
    std::cout << "Overwrites: " << table.overWrite << std::endl;
    std::cout << "Hits: " << table.hit << std::endl;
    std::cout << "Cuts: " << table.cut << std::endl;
}

void printHashEntry(const HashEntry& entry) {
    auto m = entry.move;
    auto s = m.toString();
    std::cout << "Hash Key: " << entry.hashKey << std::endl;
    std::cout << "Move: " << s << std::endl;
    std::cout << "Depth: " << entry.depth << std::endl;
    std::cout << "Score: " << entry.score << std::endl;
    std::cout << "Flag: " << entry.flag << std::endl;
}

Move probePvMove(Board b, HashTable * hashTable, int * score){
    U64 key = hashBoard(b);
    int index = key % hashTable->numEntries;
    HashEntry he = hashTable->pTable[index];
    if(he.hashKey == key){
        *score = he.score;
        return he.move;
    }
    return Move(-2, -2, -2);
}