#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"

using namespace std;

int main() { 
    // play(1000 * 60 * 30, 1, "Cosmic", "Vortex");
    int defaultTime = 1000 * 60;
    play(defaultTime, 12, "Cosmic", "Vortex");
    while (true){
        system("python core.py");
        system(".\\pushToGit.bat");
        heal(defaultTime);
        playHighestConfidence(defaultTime, 12);
    }
    // int w[4] = {0, 12, 1, 2};
    // Board b = Board(w);
    // std::cout << nh_s(b) << std::endl;
    // std::cout << nh_s_1(b) << std::endl;
 }
