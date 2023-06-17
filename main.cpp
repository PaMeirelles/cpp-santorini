#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"

int main() { 
    while (true){
        system(".\\pushToGit.bat");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        play(1000 * 60 * 1, 12, "Ascendant", "Cosmic");
    }
 }
