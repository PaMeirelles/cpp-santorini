#include "benchmark.h"
#include "db_manager.h"
#include "search.h"
#include "eval.h"

using namespace std;
using namespace std::chrono;

map<string, function<SearchResult(SearchInfo)>> getEngineMap(){
    return map<string, function<SearchResult(SearchInfo)>>
    {{"mvb-0", negamax},
     {"mvb-1", alphabeta},
        {"mvb-15", mvb15}
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
    time_point<system_clock> end;
    SearchResult result;
    sqlite3 * db;
    sqlite3_open("santorini.db", &db);
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