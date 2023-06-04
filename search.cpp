#include "search.h"
#include "eval.h"

int negamax(Board b, int depth, int turn, std::function<int(Board)> eval){
    if(depth == 0){
        return eval(b) * turn;
    }
    std::vector<Move> moves = b.gen_moves(turn);
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
        currScore = -negamax(b, depth-1, -turn, eval);
        if (currScore > maxScore){
            maxScore = currScore;
        }
        b.unmakeMove(move);
    }
    return maxScore;
}

Move getBestMove(Board b, int depth, int turn, std::function<int(Board)> eval){
    if(depth == 0){
        throw std::runtime_error("Invalid Depth (0)");
    }
    std::vector<Move> moves = b.gen_moves(turn);
    if(moves.size() == 0){
        throw std::runtime_error("No moves available");
    }   
    int maxScore = -MAX_SCORE;
    Move bestMove = moves.front();
    int currScore;
    for(Move move: moves){
        if(move.build == WIN){
            return move;
        }
        b.makeMove(move);
        currScore = -negamax(b, depth-1, -turn, eval);
        if (currScore > maxScore){
            maxScore = currScore;
            bestMove = move;
        }
        b.unmakeMove(move);
    }
    return bestMove;
}
