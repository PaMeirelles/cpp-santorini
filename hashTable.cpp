#include "hashTable.h"

void clearHashTable(HashTable *table) {

  HashEntry *tableEntry;
  
  for (tableEntry = table->pTable; tableEntry < table->pTable + table->numEntries; tableEntry++) {
    tableEntry->hashKey = 0ULL;
    tableEntry->move = NO_MOVE;
    tableEntry->depth = 0;
    tableEntry->score = 0;
    tableEntry->flag = ' ';
  }
  table->newWrite=0;
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
    U64 key = hashBoard(b);
    int index = key % hashTable->numEntries;
    if(hashTable->pTable[index].hashKey == 0){
        hashTable->newWrite++;
    }
    else{
        hashTable->overWrite++;
    }
    hashTable->pTable->depth = depth;
    hashTable->pTable->score = score;
    hashTable->pTable->flag = flag;
    hashTable->pTable->move = m;
    hashTable->pTable->hashKey = key;
}

bool probeHashEntry(HashEntry * hashEntry, Board b, HashTable * hashTable){
    U64 key = hashBoard(b);
    int index = key % hashTable->numEntries;
    if(hashTable->pTable[index].hashKey != key){
        return false;
    }
    *hashEntry = hashTable->pTable[index];
    (hashTable->hit)++;
    return true;
}

void freeHashTable(HashTable * hashTable){
    free(hashTable->pTable);
}