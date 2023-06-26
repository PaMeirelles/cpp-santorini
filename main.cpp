#include "match.h"
#include "benchmark.h"
#include "postMatch.h"
#include "hashTable.h"
#include "eval.h"

using namespace std;

int main() { 
    play(1000 * 60 * 10, 1, "Infinity", "Divine");
    int defaultTime = 1000 * 60;
    // play(defaultTime, 12, "Infinity", "Missing");
    while (true){
        system("python core.py");
        system(".\\pushToGit.bat");
        heal(defaultTime);
        playHighestConfidence(defaultTime, 60);
    }
    int w[4] = {0, 12, 1, 2};
    Board b = Board(w);
    std::cout << nh_s(b) << std::endl;
    std::cout << nh_s_1(b) << std::endl;
 }
