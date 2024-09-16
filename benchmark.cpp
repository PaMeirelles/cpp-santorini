#include "benchmark.h"
#include "db_manager.h"

using namespace std;
using namespace std::chrono;

map<string, function<SearchResult(SearchInfo)>> getEngineMap(){
    return map<string, function<SearchResult(SearchInfo)>>
    {{"mvb-0", negamax},
     {"mvb-1", alphabeta}
     };
}



void runBenchmark(const SearchInfo& info, const function<SearchResult(SearchInfo)>& searchFunction) {
    auto result = searchFunction(info);

}
