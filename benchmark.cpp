#include "benchmark.h"
#include "db_manager.h"
#include "search.h"
#include "eval.h"
#include "defs.h"

using namespace std;
using namespace std::chrono;

map<string, function<SearchResult(SearchInfo)>> getEngineMap(){
    return map<string, function<SearchResult(SearchInfo)>>
    {{"mvb-0", negamax},
    {"mvb-1", alphabeta},
    {"mvb-15", mvb15},
        {"mvb-127", mvb127},
        {"properMO", properMO},
        {"properMOV2", properMOV2},
{"properMOV3", properMOV3},
        {"mvb-143", mvb143},
    {"creator", creator}
     };
}

map<string, function<int(Board)>> getEvalMap(){
    return map<string, function<int(Board)>>
    {{"nhs-0", nh_s},
     {"nhc-0", nh_c},
        {"dbs-0", db_s}
    };

}

void runBenchmark(const Board &board, int matchId, const string &search_engine, const string &eval_function, const int depth) {
    int moveNumber = board.ply;

    const auto engine = getEngineMap()[search_engine];
    const auto eval = getEvalMap()[eval_function];

    HashTable hash_table{};
    allocateHashTable(&(hash_table), 1000);

    const auto start = high_resolution_clock::now();
    time_point<system_clock> end;
    SearchResult result;
    sqlite3 * db;
    sqlite3_open(DB_PATH, &db);
    for(int i=1; i <= depth; i++) {
        const auto search_info = SearchInfo(board, i, eval, INT_MAX, hash_table, start);
        result = engine(search_info);
        end = high_resolution_clock::now();
        const auto duration = duration_cast<microseconds>(end - start);
        saveBenchmark(db, matchId, moveNumber, search_engine, eval_function, i, result.score, duration.count());
    }
    sqlite3_close(db);
    freeHashTable(&hash_table);
}

void runBenchmark(const string &search_engine, const string &eval_function, const int depth) {
    int matchId, moveNumber;

    sqlite3 * db;
    sqlite3_open(DB_PATH, &db);

    const auto board = retrieveRandomBoard(db, &matchId, &moveNumber);
    runBenchmark(board, matchId, search_engine, eval_function, depth);
    sqlite3_close(db);

}

void compare_engines(const vector<string>& engines, const string &eval, int depth) {
    int matchId, moveNumber;

    sqlite3 * db;
    sqlite3_open(DB_PATH, &db);

    const auto board = retrieveRandomBoard(db, &matchId, &moveNumber);
    for(const auto& engine: engines) {
        runBenchmark(board, matchId, engine, eval, depth);
    }
    sqlite3_close(db);
}