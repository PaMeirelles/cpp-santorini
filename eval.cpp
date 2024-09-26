#include "eval.h"
#include <algorithm>
#include <cmath>

using namespace std;

constexpr int posScore[] =   {-50, -30, -10, -30, -50,
                                  -30,  10,  30,  10, -30,
                                  -10,  30,  50,  30,  10,
                                  -30,  10,  30,  10, -30,
                                  -50, -30, -10, -30, -50,};

constexpr int heightScore[] = {0, 100, 400};

constexpr int sameHeightSupport[] = {-30, 0, 55};
constexpr int nextHeightSupport[] = {0, 35, 120};

int eval(const Board * b) {

    auto scoreWorker = [&](const int worker_pos) {
        const int square = b->workers[worker_pos];
        const int height = b->squares[square];

        const int pScore = posScore[square];
        const int hScore = heightScore[height];

        int support = 0;

        if (height > 0) {
            int sameH = 0;
            int nextH = 0;
            for(const auto n: neighbors[square]) {
                if(b->isFree(n)) {
                    if(b->squares[n] == height) sameH++;
                    else if (b->squares[n] == height + 1) nextH++;
                }
            }
            sameH = min(sameH, 2);
            nextH = min(nextH, 2);
            support = sameHeightSupport[sameH] + nextHeightSupport[nextH];
        }
        return pScore + hScore + support;
    };
    return scoreWorker(0) + scoreWorker(1) - scoreWorker(2) - scoreWorker(3);
}
