#include "search.h"
#include "defs.h"
#include <algorithm>
#include "eval.h"
#include "hashTable.h"
#include "timeManagement.h"
#include <chrono>

using namespace std;

SearchInfo::SearchInfo(Board *b, const int depth, unsigned long long *nodes, const bool quit, HashTable *hashTable, const chrono::_V2::system_clock::time_point end) {
  this->b=b, this->depth=depth, this->nodes=nodes, this->quit=quit, this->hashTable=hashTable, this->end = end;
}


string editScore(const int score, const int depth){
    if(score > 9000){
        return "#" + to_string((MATE-score + depth + 1) / 2);
    }
    if(score < -9000){
        return "#" + to_string((MATE+score - depth - 1) / 2);
    }
    return to_string(score);
}


void score_moves(vector<Move>& moves, const HashTable * ht, const Board * board) {
  int numDoubleNeighbors[] = {9,  12, 15, 12, 9,
                              12, 16, 20, 16, 12,
                              15, 20, 25, 20, 15,
                              12, 16, 20, 16, 12,
                              9,  12, 15, 12, 9};
  int currScore;
  const auto pvMove = probePvMove(board, ht, &currScore);
  for(auto& move : moves) {
    if (move == pvMove) move.moveOrderingScore = 100;
    else move.moveOrderingScore = (move.toHeight - move.fromHeight) * 10 + (numDoubleNeighbors[move.to] - numDoubleNeighbors[move.from]);
  }
}

void pickMove(vector<Move>& moves, const int start) {
  for(int i=start+1; i < moves.size(); i++) {
    if(moves[i].moveOrderingScore > moves[start].moveOrderingScore) {
      swap(moves[i], moves[start]);
    }
  }
}

int search(SearchInfo * search_info, const int depth, int alpha, int beta){
      (*search_info->nodes)++;
      if(*search_info->nodes % CHECK_EVERY == 0){
        if (const auto now = chrono::high_resolution_clock::now(); now > search_info->end) {
          search_info->quit = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(search_info->b) * search_info->b->turn;
    }

  auto bestMove = NO_MOVE;
  int currScore;

  if(probeHashEntry(search_info->b, search_info->hashTable, &bestMove, &currScore, alpha, beta, depth)){
    search_info->hashTable->cut++;
    return currScore;
  }

    vector<Move> moves = search_info->b->gen_moves();
    if (moves.empty()) {
      return -MATE - depth;
    }

    int maxScore = -MATE * 100;

    const int oldAlpha = alpha;

    score_moves(moves, search_info->hashTable, search_info->b);

    for (int i=0; i < moves.size(); i++) {
        pickMove(moves, i);
        auto move = moves[i];
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          search_info->b->makeMove(move);
          currScore = -search(search_info, depth-1, -beta, -alpha);
          search_info->b->unmakeMove(move);
        }
        if (currScore > maxScore) {
          maxScore = currScore;
          bestMove = move;
            if(maxScore > alpha){
                if(maxScore >= beta){
                    storeHashEntry(search_info->b, bestMove, beta, depth, 'B', search_info->hashTable);
                    return beta;
                }
                alpha = maxScore;
            }
        }

        if(search_info->quit){
          return 0;
        }

    }
    if(alpha != oldAlpha){
      storeHashEntry(search_info->b, bestMove, maxScore, depth, 'E', search_info->hashTable);
    }
    else{
      storeHashEntry(search_info->b, bestMove, alpha, depth, 'A', search_info->hashTable);
    }
    return alpha;
}


Move getBestMove(Board * b, int remaining_time, HashTable * hashTable) {
  int thinkingTime = getTime(remaining_time);
  int depth = 1;
  U64 nodes = 0;
  const chrono::_V2::system_clock::time_point start = chrono::high_resolution_clock::now();
  auto end = start + chrono::milliseconds(thinkingTime);
  chrono::_V2::system_clock::time_point now;
  chrono::duration<double, milli> duration;
  Move bestMove = NO_MOVE;
  int maxScore;
  while (true) {
    SearchInfo search_info = SearchInfo(b, depth, &nodes, false, hashTable, end);
    auto result = search(&search_info, depth, -MATE, MATE);
    now = chrono::high_resolution_clock::now();
    duration = (now - start);
    bestMove = probePvMove(b, hashTable, &maxScore);
    auto pvLine = getPvLine(depth, b, hashTable);
    if (VERBOSE) {
      cout <<  "Depth: " << depth
      << " - Best move: " << bestMove.toString()
      << " - Time: " << duration.count() << "ms"
      << setw(10 - to_string(duration.count()).length() - 2) << " "  // Calculate padding after "ms"
      << " - Best score: " << editScore(maxScore, depth) << endl;
      cout << "PV Line: " << pvLineToString(pvLine) << endl;
    }

    if(search_info.quit){
      break;
    }
    if(bestMove.build == WIN || maxScore > 9000 || maxScore < -9000){
      break;
    }
    depth++;
  }
  return bestMove;
}
