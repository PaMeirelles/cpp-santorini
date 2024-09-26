#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"
#include "db_manager.h"

using namespace std;

int main() {
    play(1000 * 60 * 1, 500, "Zephyr", "Dark");
    // for(int i=0; i < 99; i++) compare_engines({"probe"}, {"dbs-0"}, 7);
}
