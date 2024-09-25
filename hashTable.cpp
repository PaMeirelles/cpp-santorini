#include "hashTable.h"
#include "defs.h"

using namespace std;

vector<Move> getPvLine(const int depth, Board & b, const HashTable * ht) {
    int score;
    auto move = probePvMove(b, ht, &score);
    int count = 0;
    vector<Move> pvLine;

    while(move != NO_MOVE && count < depth) {
        try {
            b.makeMove(move);
        }
        catch (const runtime_error& e) {
            break;
        }
        pvLine.push_back(move);

        move = probePvMove(b, ht, &score);
        count++;
    }

    for (int i = pvLine.size() - 1; i >= 0; --i) {
        auto mv = pvLine[i];
        b.unmakeMove(mv);
    }

    return pvLine;
}

string pvLineToString(const vector<Move> &pvLine) {
    string stringPvLine;
    for(int i=1; i <= pvLine.size(); i++) {
        stringPvLine += (to_string(i) + ". " + pvLine[i-1].toString() + " ");
    }
    return stringPvLine;
}


void clearHashTable(HashTable *table) {
    table->cut = 0;
    table->hit = 0;
    table->newWrite = 0;
    table->overWrite = 0;
    for (HashEntry *tableEntry = table->pTable; tableEntry < table->pTable + table->numEntries; tableEntry++) {
        tableEntry->hashKey = 0ULL;
        tableEntry->move = NO_MOVE;
        tableEntry->depth = 0;
        tableEntry->score = 0;
        tableEntry->flag = ' ';
    }
}

void allocateHashTable(HashTable *hashTable, const int MB) {
    const unsigned long long hashSize = 0x100000 * MB;
    hashTable->numEntries = hashSize / sizeof(HashEntry);
    hashTable->numEntries -= 2;
    hashTable->pTable = static_cast<HashEntry *>(malloc(hashTable->numEntries * sizeof(HashEntry)));

    if (!hashTable->pTable) {
        if(DEBUG){
            cout << "Hash Allocation Failed, trying " << MB / 2 << "MB...\n";
        }
        allocateHashTable(hashTable, MB / 2);
    } else {
        clearHashTable(hashTable);
        if(DEBUG){
            std::cout << "HashTable init complete with " << hashTable->numEntries << " entries\n";
        }
    }
}


void storeHashEntry(const Board &b, const Move &m, const int score, const int depth, const char flag, HashTable * hashTable){
    if(m.from < 0){
        throw runtime_error("Invalid move");
    }
    const U64 key = hashBoard(b);
    const U64 index = key % hashTable->numEntries;
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

bool probeHashEntryOld(const Board &b, HashTable * hashTable, Move * move, int * score, const int alpha, const int beta, const int depth){
    const U64 key = hashBoard(b);
    const U64 index = key % hashTable->numEntries;
    const HashEntry he = hashTable->pTable[index];
    if(he.hashKey != key){
        return false;
    }
    (hashTable->hit)++;
    *move = he.move;
    if(he.depth < depth){
        return false;
    }
    const char flag = he.flag;
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

bool probeHashEntry(const Board &b, HashTable * hashTable, Move * move, int * score, const int alpha, const int beta, const int depth){
    const U64 key = hashBoard(b);
    const U64 index = key % hashTable->numEntries;
    const HashEntry he = hashTable->pTable[index];
    if(he.hashKey != key) {
        return false;
    }
    *move = he.move;
    if(he.depth < depth){
        return false;
    }
    hashTable->hit++;
    *score = he.score;
    const char flag = he.flag;
    if(flag == 'A' && *score <= alpha){
        *score = alpha;
        return true;
    }
    if(flag == 'B' && *score >= beta){
        *score = beta;
        return true;
    }
    if (flag == 'E'){
        return true;
    }
    return false;
}

void freeHashTable(const HashTable * hashTable){
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
    const auto s = m.toString();
    std::cout << "Hash Key: " << entry.hashKey << std::endl;
    std::cout << "Move: " << s << std::endl;
    std::cout << "Depth: " << entry.depth << std::endl;
    std::cout << "Score: " << entry.score << std::endl;
    std::cout << "Flag: " << entry.flag << std::endl;
}

Move probePvMove(const Board &b, const HashTable * hashTable, int * score){
    const U64 key = hashBoard(b);
    const U64 index = key % hashTable->numEntries;
    const HashEntry he = hashTable->pTable[index];
    if(he.hashKey == key){
        *score = he.score;
        return he.move;
    }
    return NO_MOVE;
}