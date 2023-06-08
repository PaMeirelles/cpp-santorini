#include "search.h"
#include "eval.h"
#include <chrono>

AlphaBetaInfo::AlphaBetaInfo(int alpha2, int beta2){
    alpha = alpha2;
    beta = beta2;
}

SearchResult::SearchResult(Move m, int s, bool o){
    move = m;
    score = s;
    outOftime = o;
}
SearchResult::SearchResult(){};
SearchResult negamaxRecur(Board b, int depth, std::function<int(Board)> eval, int * diveCheck, int time, std::chrono::_V2::system_clock::time_point start) {
    if(*diveCheck % 1000 == 0){
      auto end = std::chrono::high_resolution_clock::now();  
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      if (duration.count() > time) {
        return SearchResult(Move(), -MAX_SCORE, true);
      }
    }
    if (depth == 0) {
      return SearchResult(Move(), eval(b) * b.turn, false);
    }
    std::vector<Move> moves = b.gen_moves(b.turn);
    if (moves.size() == 0) {
      return SearchResult(Move(), -MAX_SCORE - depth, false);
    }
    int maxScore = -MAX_SCORE;
    int currScore;
    Move bestMove = moves[0];
    SearchResult s = SearchResult();
    bool oot;

    for (Move move : moves) {
      if (move.build == WIN) {
        return SearchResult(move, MAX_SCORE + depth - 1, false);
      }
      b.makeMove(move);
      *diveCheck ++;
      s = negamaxRecur(b, depth - 1, eval, diveCheck, time, start);
      if(s.outOftime){
        oot = s.outOftime;
        break;
      }
      currScore = -s.score;
      if (currScore > maxScore) {
        maxScore = currScore;
        bestMove = move;
      }
      b.unmakeMove(move);
    }
    return SearchResult(bestMove, maxScore, oot);
  }
SearchResult negamax(Board b, int depth, std::function<int(Board)> eval, int time){
  auto start = std::chrono::high_resolution_clock::now();
  int diveCheck = 0;
  return negamaxRecur(b, depth, eval, &diveCheck, time, start);
}
SearchResult alphabetaRecur(Board b, int depth, std::function<int(Board)> eval, 
AlphaBetaInfo alphaBeta, int * diveCheck, int time, std::chrono::_V2::system_clock::time_point start) {
    if(*diveCheck % 1000 == 0){
      auto end = std::chrono::high_resolution_clock::now();  
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      if (duration.count() > time) {
        return SearchResult(Move(), -MAX_SCORE, true);
      }
    }
    if (depth == 0) {
      return SearchResult(Move(), eval(b) * b.turn, false);
    }
    std::vector<Move> moves = b.gen_moves(b.turn);
    if (moves.size() == 0) {
      return SearchResult(Move(), -MAX_SCORE - depth, false);
    }
    int maxScore = -MAX_SCORE;
    int currScore;
    Move bestMove = Move();
    SearchResult s = SearchResult();
    bool oot;
    for (Move move : moves) {
        if (move.build == WIN) {
          return SearchResult(move, MAX_SCORE + depth - 1, false);
        }
        b.makeMove(move);
        *diveCheck ++;
        s = alphabetaRecur(b, depth - 1, eval, AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), diveCheck, time, start);
        if(s.outOftime){
          oot = s.outOftime;
          break;
      }
        currScore = -s.score;
        if (currScore > maxScore) {
          maxScore = currScore;
          bestMove = move;
        }
        b.unmakeMove(move);
        if(maxScore > alphaBeta.alpha){
          alphaBeta.alpha = maxScore;
        }
        if(alphaBeta.alpha >= alphaBeta.beta){
          break;
        }
    }
    return SearchResult(bestMove, maxScore, oot);
  }
SearchResult alphabeta(Board b, int depth, std::function<int(Board)> eval, int time){  
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MAX_SCORE, MAX_SCORE);
      auto start = std::chrono::high_resolution_clock::now();
      int diveCheck = 0;
    return alphabetaRecur(b, depth, eval, alphaBeta, &diveCheck, time, start);
    }


SearchResult alphabetaRecurWithMo(Board b, int depth, std::function<int(Board)> eval,
 AlphaBetaInfo alphaBeta, std::function<bool(const Move& a, const Move& b)> compareMoves, int * diveCheck, int time, std::chrono::_V2::system_clock::time_point start) {
    if(*diveCheck % 1000 == 0){
      auto end = std::chrono::high_resolution_clock::now();  
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      if (duration.count() > time) {
        return SearchResult(Move(), -MAX_SCORE, true);
      }
    }
    if (depth == 0) {
      return SearchResult(Move(), eval(b) * b.turn, false);
    }
    std::vector<Move> moves = b.gen_moves(b.turn);
    if (moves.size() == 0) {
      return SearchResult(Move(), -MAX_SCORE - depth, false);
    }
    int maxScore = -MAX_SCORE;
    int currScore;
    bool oot;
    Move bestMove = Move();
    SearchResult s = SearchResult();
    std::sort(moves.begin(), moves.end(), compareMoves);
    for (Move move : moves) {
        if (move.build == WIN) {
          return SearchResult(move, MAX_SCORE + depth - 1, false);
        }
        b.makeMove(move);
        *diveCheck ++;
        s = alphabetaRecur(b, depth - 1, eval, AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), diveCheck, time, start);
        if(s.outOftime){
          oot = s.outOftime;
          break;
      }
        currScore = -s.score;
        if (currScore > maxScore) {
          maxScore = currScore;
          bestMove = move;
        }
        b.unmakeMove(move);
        if(maxScore > alphaBeta.alpha){
          alphaBeta.alpha = maxScore;
        }
        if(alphaBeta.alpha >= alphaBeta.beta){
          break;
        }
    }
    return SearchResult(bestMove, maxScore, oot);
  }

bool compareMoves(const Move& a, const Move& b) {
    int aDiff = a.toHeight - a.fromHeight;
    int bDiff = b.toHeight - b.fromHeight;
    return aDiff > bDiff;
}


SearchResult alphabetaWitClimbhMo(Board b, int depth, std::function<int(Board)> eval, int time){
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MAX_SCORE, MAX_SCORE);
    auto start = std::chrono::high_resolution_clock::now();
    int diveCheck = 0;
    return alphabetaRecurWithMo(b, depth, eval, alphaBeta, compareMoves, &diveCheck, time, start);
  }

Move getBestMove(
    Board b, std::function<SearchResult(Board, int, std::function<int(Board)>, int)> search,
    std::function<int(Board)> eval, std::function<int(int)> timeManager,
    int time) {
  int thinkingTime = timeManager(time);
  int depth = 1;
  bool running = true;
  int maxScore;
  Move bestMove = Move(-2, -2, -2);
  Move gbestMove = Move(-2, -2, -2);
  SearchResult s = SearchResult();
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::milli> duration;
  while (running) {
    s = search(b, depth, eval, thinkingTime);
    bestMove = s.move;
    maxScore = s.score;
    running = !s.outOftime || bestMove.build == WIN;
    duration = std::chrono::steady_clock::now() - start;
    // Print the maximum score at each depth
    if (VERBOSE && running) {
      std::cout << "Depth: " << depth << std::endl;
      std::cout << "Best move at depth " << depth << ": " << bestMove.toString()
                << std::endl;
      std::cout << "Time spent at depth " << depth << ": " << duration.count()
                << "ms" << std::endl;
      std::cout << "Maximum score at depth " << depth << ": " << maxScore
                << std::endl;
      std::cout << "------------------------------" << std::endl;
    }
    depth++;
    if (running) {
      gbestMove = bestMove;
    }
  }
  return gbestMove;
}
