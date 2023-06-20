#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"

using namespace std;

int main() { 
    // play(1000 * 60 * 5, 1, "Harmony", "Pilot");
    int defaultTime = 1000 * 60;
    while (true){
        system("python core.py");
        system(".\\pushToGit.bat");
        heal(defaultTime);
        playHighestConfidence(defaultTime, 12);
    }
 }
