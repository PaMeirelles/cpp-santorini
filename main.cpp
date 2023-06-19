#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"

using namespace std;

int main() { 
    // play(1000 * 60 * 5, 1, "Harmony", "Pilot");

    while (true){
        system("python core.py");
        system(".\\pushToGit.bat");
        heal(1000 * 60);
        play(1000 * 60 * 1, 24, "Pilot", "Harmony");
    }
 }
