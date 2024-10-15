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
int eval(const Board* b) {
    auto scoreWorker = [&](const int worker_pos) {
        const int square = b->workers[worker_pos];
        const int height = b->squares[square];

        const int pScore = posScore[square];
        const int hScore = heightScore[height];

        int support = 0;

        if (height > 0) {
            int sameH = 0;
            int nextH = 0;

            const auto& square_neighbors = neighbors[square];
            for (const auto n : square_neighbors) {
                if (b->isFree(n)) {
                    const int neighbor_height = b->squares[n]; // Cache neighbor height
                    if (neighbor_height == height) {
                        sameH++;
                    }
                    else if (neighbor_height == height + 1) {
                        nextH++;
                    }
                }
            }

            // Use bitwise AND to cap values at 2
            sameH &= 0b11; // Cap sameH to 2
            nextH &= 0b11; // Cap nextH to 2

            // Directly access support arrays
            support = sameHeightSupport[sameH] + nextHeightSupport[nextH];
        }
        return pScore + hScore + support;
        };

    return scoreWorker(0) + scoreWorker(1) - scoreWorker(2) - scoreWorker(3);
}
