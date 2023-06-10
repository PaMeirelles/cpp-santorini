#include "search.h"
#include "eval.h"
#include <chrono>

std::string editScore(int score, int depth){
    if(score > 9000){
        return "#" + std::to_string((MAX_SCORE-score + depth + 1) / 2);
    }
    if(score < -9000){
        return "#" + std::to_string((MAX_SCORE+score - depth - 1) / 2);
    }
    return std::to_string(score);
}

AlphaBetaInfo::AlphaBetaInfo(int alpha2, int beta2){
    alpha = alpha2;
    beta = beta2;
}

SearchResult::SearchResult(Move m, int s, bool o){
    move = m;
    score = s;
    outOftime = o;
}

bool compareMoves(const Move& a, const Move& b) {
    int aDiff = a.toHeight - a.fromHeight;
    int bDiff = b.toHeight - b.fromHeight;
    return aDiff > bDiff;
}

SearchResult::SearchResult(){};
SearchResult negamaxRecur(Board b, int depth, std::function<int(Board)> eval, int * diveCheck, int time, std::chrono::_V2::system_clock::time_point start) {
    (*diveCheck)++;
    if(*diveCheck == 1000){
      *diveCheck = 0;
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
    int maxScore = -MAX_SCORE * 100;
    int currScore;
    Move bestMove = moves[0];
    SearchResult s = SearchResult();
    bool oot = false;

    for (Move move : moves) {
      if (move.build == WIN) {
        return SearchResult(move, MAX_SCORE + depth, false);
      }
      b.makeMove(move);
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
  int dc = 0;
  return negamaxRecur(b, depth, eval, &dc, time, start);
}
SearchResult alphabetaRecur(Board b, int depth, std::function<int(Board)> eval, 
AlphaBetaInfo alphaBeta, int * diveCheck, int time, std::chrono::_V2::system_clock::time_point start) {
    (*diveCheck)++;
    if(*diveCheck == 1000){
      *diveCheck = 0;
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
    int maxScore = -MAX_SCORE * 100;
    int currScore;
    Move bestMove = moves[0];
    SearchResult s = SearchResult();
    bool oot = false;
    for (Move move : moves) {
        if (move.build == WIN) {
          return SearchResult(move, MAX_SCORE + depth - 1, false);
        }
        b.makeMove(move);
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
      int dc = 0;
    return alphabetaRecur(b, depth, eval, alphaBeta, &dc, time, start);
    }
Move getBestMove(
    Board b, std::function<SearchResult(Board, int, std::function<int(Board)>, int)> search,
    std::function<int(Board)> eval, std::function<int(int)> timeManager,
    int time) {
  int thinkingTime = timeManager(time);
  int depth = 1;
  int maxScore;
  Move bestMove = Move();
  Move gbestMove = Move();
  SearchResult s = SearchResult();
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::milli> duration;
  while (true) {
    s = search(b, depth, eval, thinkingTime);
    bestMove = s.move;
    maxScore = s.score;
    if(s.outOftime){
      break;
    }
    duration = std::chrono::steady_clock::now() - start;
    // Print the maximum score at each depth
    if (VERBOSE) {
      std::cout << "Depth: " << depth << std::endl;
      std::cout << "Best move at depth " << depth << ": " << bestMove.toString()
                << std::endl;
      std::cout << "Time spent at depth " << depth << ": " << duration.count()
                << "ms" << std::endl;
      std::cout << "Maximum score at depth " << depth << ": " << editScore(maxScore, depth)
                << std::endl;
      std::cout << "------------------------------" << std::endl;
    }
    if(bestMove.build == WIN || maxScore > 9000 || maxScore < -9000){
      gbestMove = bestMove;
      break;
    }
    depth++;
    gbestMove = bestMove;
  }
  return gbestMove;
}
