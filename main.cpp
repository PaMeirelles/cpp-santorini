#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"

using namespace std;

int main() {   
    // EngineInfo engineA = assemblyEngine("Infinity");
    // EngineInfo engineB = assemblyEngine("Divine");
    // allocateHashTable(&(engineA.hashTable), 1000);
    // allocateHashTable(&(engineB.hashTable), 1000);

    // Board b = Board();

    // getBestMove(b, engineA, 1000 * 60 * 25);
    // getBestMove(b, engineB, 1000 * 60 * 25);

    // play(1000 * 60 * 10, 1, "Infinity", "Divine");
    int defaultTime = 1000 * 60;
    // play(defaultTime, 12, "Infinity", "Divine");
    while (true){
        system("python core.py");
        system(".\\pushToGit.bat");
        heal(defaultTime);
        playHighestConfidence(defaultTime, 60);
    }
 }
