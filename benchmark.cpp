#include "benchmark.h"
#include "db_manager.h"
#include "search.h"
#include "eval.h"

using namespace std;
using namespace std::chrono;

map<string, function<SearchResult(SearchInfo)>> getEngineMap(){
    return map<string, function<SearchResult(SearchInfo)>>
    {{"mvb-0", negamax},
     {"mvb-1", alphabeta}
     };
}

map<string, function<int(Board)>> getEvalMap(){
    return map<string, function<int(Board)>>
    {{"nhs-0", nh_s},
     {"nhc-0", nh_c}
    };

}

void runBenchmark(const Board &board, int matchId, const string &search_engine, const string &eval_function, const int depth) {
    int moveNumber = board.ply;

    const auto engine = getEngineMap()[search_engine];
    const auto eval = getEvalMap()[eval_function];

    HashTable hash_table{};
    allocateHashTable(&(hash_table), 1000);

    const auto start = high_resolution_clock::now();
    const auto search_info = SearchInfo(board, depth, eval, INT_MAX, hash_table, start);
    const auto result = engine(search_info);

    const auto end = high_resolution_clock::now();

    const auto duration = duration_cast<microseconds>(end - start);
    sqlite3 * db;
    sqlite3_open("santorini.db", &db);
    saveBenchmark(db, matchId, moveNumber, search_engine, eval_function, depth, result.score, duration.count());
    sqlite3_close(db);
    freeHashTable(&hash_table);
}

void runBenchmark(const string &search_engine, const string &eval_function, const int depth) {
    int matchId, moveNumber;

    sqlite3 * db;
    sqlite3_open("santorini.db", &db);

    const auto board = retrieveRandomBoard(db, &matchId, &moveNumber);
    runBenchmark(board, matchId, search_engine, eval_function, depth);
    sqlite3_close(db);

}

void compare_engines(const string &engine_1, const string &engine_2, const string &eval, int depth) {
    int matchId, moveNumber;

    sqlite3 * db;
    sqlite3_open("santorini.db", &db);

    const auto board = retrieveRandomBoard(db, &matchId, &moveNumber);
    runBenchmark(board, matchId, engine_1, eval, depth);
    runBenchmark(board, matchId, engine_2, eval, depth);

    sqlite3_close(db);
}