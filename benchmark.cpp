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

void runBenchmark(const string &search_engine, const string &eval_function, const int depth) {
    int matchId;
    int moveNumber;
    const auto board = retrieveRandomBoard(&matchId, &moveNumber);

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
}
