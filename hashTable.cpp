#include "hashTable.h"
#include "defs.h"
#include <string>

using namespace std;

vector<Move> getPvLine(const int depth, Board * b, const HashTable * ht) {
    int score;
    auto move = probePvMove(b, ht, &score);
    int count = 0;
    vector<Move> pvLine;

    while(move != NO_MOVE && count < depth) {
        try {
            b->makeMove(move);
        }
        catch ([[maybe_unused]] const runtime_error& e) {
            break;
        }
        pvLine.push_back(move);

        move = probePvMove(b, ht, &score);
        count++;
    }

    for (int i = static_cast<int>(pvLine.size() - 1); i >= 0; --i) {
        auto mv = pvLine[i];
        b->unmakeMove(mv);
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
        if constexpr (DEBUG){
            cout << "Hash Allocation Failed, trying " << MB / 2 << "MB...\n";
        }
        allocateHashTable(hashTable, MB / 2);
    } else {
        clearHashTable(hashTable);
        if constexpr (DEBUG){
            cout << "HashTable init complete with " << hashTable->numEntries << " entries\n";
        }
    }
}


void storeHashEntry(const Board * b, const Move &m, const int score, const int depth, const char flag, HashTable * hashTable){
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

bool probeHashEntry(const Board * b, HashTable * hashTable, Move * move, int * score, const int alpha, const int beta, const int depth){
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
    cout << "Number of entries: " << table.numEntries << endl;
    cout << "New writes: " << table.newWrite << endl;
    cout << "Overwrites: " << table.overWrite << endl;
    cout << "Hits: " << table.hit << endl;
    cout << "Cuts: " << table.cut << endl;
}

void printHashEntry(const HashEntry& entry) {
    auto m = entry.move;
    const auto s = m.toString();
    cout << "Hash Key: " << entry.hashKey << endl;
    cout << "Move: " << s << endl;
    cout << "Depth: " << entry.depth << endl;
    cout << "Score: " << entry.score << endl;
    cout << "Flag: " << entry.flag << endl;
}

Move probePvMove(const Board * b, const HashTable * hashTable, int * score){
    const U64 key = hashBoard(b);
    const U64 index = key % hashTable->numEntries;
    const HashEntry he = hashTable->pTable[index];
    if(he.hashKey == key){
        *score = he.score;
        return he.move;
    }
    return NO_MOVE;
}
