#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"

int main() { 
    while (true){
        system("python core.py");
        system(".\\pushToGit.bat");
        play(1000 * 60 * 1, 12, "Ascendant", "Cosmic");
    }
 }
