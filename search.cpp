#include "search.h"
#include "eval.h"
#include <chrono>

int negamax(Board b, int depth, std::function<int(Board)> eval){
    if(depth == 0){
        return eval(b) * b.turn;
    }
    std::vector<Move> moves = b.gen_moves(b.turn);
    if(moves.size() == 0){
        return -MAX_SCORE - depth;
    }   
    int maxScore = -MAX_SCORE;
    int currScore;
    for(Move move: moves){
        if(move.build == WIN){
            return MAX_SCORE + depth - 1;
        }
        b.makeMove(move);
        currScore = -negamax(b, depth-1, eval);
        if (currScore > maxScore){
            maxScore = currScore;
        }
        b.unmakeMove(move);
    }
    return maxScore;
}

Move getBestMove(Board b, std::function<int(Board)> eval, std::function<int(int)> timeManager, int time) {
    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::_V2::system_clock::time_point end;
    std::chrono::milliseconds duration;
    int thinkingTime = timeManager(time);

    int depth = 1;
    bool running = true;
    int maxScore;
    Move bestMove = Move(-2, -2, -2);
    Move gbestMove = Move(-2, -2, -2);
    int currScore;
    
    std::vector<Move> moves = b.gen_moves(b.turn);
    if (moves.size() == 0) {
        throw std::runtime_error("No moves available");
    }
    
    int numMoves = moves.size(); // Store the number of moves
    
    while (running) {
        maxScore = -MAX_SCORE;
        bestMove = moves.front();

        for (Move move : moves) {
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            if (duration.count() > thinkingTime) {
                running = false;
                break;
            }

            if (move.build == WIN) {
                return move;
            }
            b.makeMove(move);
            currScore = -negamax(b, depth - 1, eval);
            if (currScore > maxScore) {
                maxScore = currScore;
                bestMove = move;
            }
            b.unmakeMove(move);
        }
        
        // Print the maximum score at each depth
        if (VERBOSE) {
            std::cout << "Depth: " << depth << std::endl;
            std::cout << "Best move at depth " << depth << ": " << bestMove.toString() << std::endl;
            std::cout << "Time spent at depth " << depth << ": " << duration.count() << "ms" << std::endl;
            std::cout << "Maximum score at depth " << depth << ": " << maxScore << std::endl;
            std::cout << "------------------------------" << std::endl;
        }
        
        depth++;
    }
    
    // Print the number of moves and the final maximum score
    if (VERBOSE) {
        std::cout << "Number of moves: " << numMoves << std::endl;
        std::cout << "Final Maximum Score: " << maxScore << std::endl;
    }

    return gbestMove;
}
