#include "search.h"
#include "eval.h"
#include "hashTable.h"
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
TimeInfo::TimeInfo(int * dc, int t, std::chrono::_V2::system_clock::time_point s, bool * o){
    diveCheck = dc;
    time = t;
    start = s;
    oot = o;
}

AlphaBetaInfo::AlphaBetaInfo(int alpha2, int beta2){
    alpha = alpha2;
    beta = beta2;
}

SearchResult::SearchResult(Move m, int s, bool o){
    move = m;
    score = s;
    outOftime = o;
    keepResult = !o;
}

SearchResult::SearchResult(Move m, int s, bool o, bool k){
    move = m;
    score = s;
    outOftime = o;
    keepResult = k;
}

SearchInfo::SearchInfo(Board board, int d, std::function<int(Board)> e, int t, HashTable ht, std::chrono::_V2::system_clock::time_point st){
  b = board;
  depth = d;
  eval = e;
  time = t;
  hashTable = ht;
  start = st;
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
SearchResult negamax(SearchInfo si){
  int dc = 0;
  return negamaxRecur(si.b, si.depth, si.eval, &dc, si.time, si.start);
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
SearchResult alphabeta(SearchInfo si){  
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MAX_SCORE, MAX_SCORE);
      int dc = 0;
    return alphabetaRecur(si.b, si.depth, si.eval, alphaBeta, &dc, si.time, si.start);
    }
template<typename T>
void moveElementToFront(std::vector<T>& vec, const T& element) {
    auto it = std::find(vec.begin(), vec.end(), element);
    if (it != vec.end()) {
        std::rotate(vec.begin(), it, it + 1);
    }
}
int mvb3Recur(AlphaBetaInfo alphaBeta, int depth, Board b, TimeInfo timeInfo, std::function<int(Board)> eval, HashTable * hashTable){
    (*(timeInfo.diveCheck))++;
      if((*(timeInfo.diveCheck)) % 1000){
        auto end = std::chrono::high_resolution_clock::now();  
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *(timeInfo.oot) = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }
    std::vector<Move> moves = b.gen_moves(b.turn);
    if (moves.size() == 0) {
      return -MAX_SCORE - depth;
    }

    int maxScore = -MAX_SCORE * 100;
    int currScore;
    int oldAlpha = alphaBeta.alpha;
    Move bestMove = moves[0];

    if(probeHashEntry(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
      hashTable->cut++;
      return maxScore;
    }
    auto pvMove = probePvMove(b, hashTable, &currScore);
    if(pvMove.from > 0){
      moveElementToFront(moves, pvMove);
    }
    for (Move move : moves) {
        if (move.build == WIN) {
          currScore = MAX_SCORE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -mvb3Recur(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
        }

        if(*(timeInfo.oot)){
          break;
        }

        if (currScore > maxScore) {
          maxScore = currScore;
          bestMove = move;
            if(maxScore > alphaBeta.alpha){
                if(maxScore >= alphaBeta.beta){
                    storeHashEntry(b, bestMove, alphaBeta.beta, depth, 'B', hashTable);
                    return alphaBeta.beta;
                }
                alphaBeta.alpha = maxScore;
            }
        }       
    }
    if(alphaBeta.alpha != oldAlpha){
      storeHashEntry(b, bestMove, maxScore, depth, 'E', hashTable);
    }
    else{
      storeHashEntry(b, bestMove, alphaBeta.alpha, depth, 'A', hashTable);
    }
    return alphaBeta.alpha;
}

SearchResult mvb3(SearchInfo si){  
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MAX_SCORE, MAX_SCORE);
    int dc = 0;
    bool oot = false;
    auto ti = TimeInfo(&dc, si.time, si.start, &oot);
    int score = mvb3Recur(alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
    Move m = probePvMove(si.b, &(si.hashTable), &score);
    return SearchResult(m, score, oot, true);
    }

Move getBestMove(
    Board b, std::function<SearchResult(SearchInfo)> search,
    std::function<int(Board)> eval, std::function<int(int)> timeManager,
    int time) {
  int thinkingTime = timeManager(time);
  int depth = 1;
  int maxScore;
  Move bestMove = Move();
  Move gbestMove = Move();
  SearchResult s = SearchResult();
  std::chrono::_V2::system_clock::time_point start = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> duration;
  HashTable ht;
  allocateHashTable(&ht, 2000);
  while (true) {
    s = search(SearchInfo(b, depth, eval, thinkingTime, ht, start));
    bestMove = s.move;
    maxScore = s.score;
    if(s.keepResult){
      gbestMove = bestMove;
    }
    duration = std::chrono::system_clock::now() - start;
    // Print the maximum score at each depth
    if (VERBOSE && s.keepResult) {
      std::cout << "Depth: " << depth << std::endl;
      std::cout << "Best move up to depth " << depth << ": " << bestMove.toString()
                << std::endl;
      std::cout << "Time spent up to depth " << depth << ": " << duration.count()
                << "ms" << std::endl;
      std::cout << "Maximum score up to depth " << depth << ": " << editScore(maxScore, depth)
                << std::endl;
      std::cout << "------------------------------" << std::endl;
    }
    if(s.outOftime){
      break;
    }
    if(bestMove.build == WIN || maxScore > 9000 || maxScore < -9000){
      gbestMove = bestMove;
      break;
    }
    depth++;
  }
  freeHashTable(&ht);
  return gbestMove;
}
