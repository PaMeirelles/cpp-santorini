#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"
#include "db_manager.h"

using namespace std;

int main() {
    vector<string> pot1 = {"Cyan", "Dark", "Creator", "Radiant", "Zephyr", "Void"};
    for(int i=0; i < pot1.size(); i++) {
        for(int j=i+1; j < pot1.size(); j++) {
            play(1000 * 60, 6, pot1[i], pot1[j]);
        }
    }
}
