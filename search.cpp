#include "search.h"
#include "defs.h"
#include <algorithm>
#include "eval.h"
#include "hashTable.h"
#include "timeManagement.h"
#include <chrono>

using namespace std;

string editScore(const int score, const int depth){
    if(score > 9000){
        return "#" + to_string((MATE-score + depth + 1) / 2);
    }
    if(score < -9000){
        return "#" + to_string((MATE+score - depth - 1) / 2);
    }
    return to_string(score);
}

TimeInfo::TimeInfo(int * dc, const int t, const chrono::_V2::system_clock::time_point s, bool * o){
    diveCheck = dc;
    time = t;
    start = s;
    oot = o;
}

AlphaBetaInfo::AlphaBetaInfo(const int alpha2, const int beta2){
    alpha = alpha2;
    beta = beta2;
}

SearchResult::SearchResult(const Move &m, const int s, const bool o){
    move = m;
    score = s;
    outOftime = o;
    keepResult = !o;
}

SearchResult::SearchResult(Move &m, const int s, const bool o, const bool k){
    move = m;
    score = s;
    outOftime = o;
    keepResult = k;
}

SearchInfo::SearchInfo(const Board &board, int d, function<int(Board)> e, int t, const HashTable &ht, chrono::_V2::system_clock::time_point st){
  b = board;
  depth = d;
  eval = e;
  time = t;
  hashTable = ht;
  start = st;
}

bool compareMoveHeightOnly(const Move& a, const Move& b) {
  const int aDiff = a.toHeight - a.fromHeight;
  const int bDiff = b.toHeight - b.fromHeight;
    return aDiff > bDiff;
}

SearchResult::SearchResult()= default;

SearchResult negamaxRecur(Board b, const int depth, const function<int(Board)>& eval, int * diveCheck, const int time,
  chrono::_V2::system_clock::time_point start) {
    (*diveCheck)++;
    if(*diveCheck == DIVE_CHECK){
      *diveCheck = 0;
      const auto end = chrono::high_resolution_clock::now();
      const auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
      if (duration.count() > time) {
        return {Move(), -MATE, true};
      }
    }

    if (depth == 0) {
      return {Move(), eval(b) * b.turn, false};
    }

    vector<Move> moves = b.gen_moves(b.turn);
    if (moves.empty()) {
      return {Move(), -MATE - depth, false};
    }

    int maxScore = -MATE * 100;
    Move bestMove = moves[0];
    SearchResult s = SearchResult();
    bool oot = false;

    for (Move move : moves) {
      if (move.build == WIN) {
        return {move, MATE + depth, false};
      }

      b.makeMove(move);
      s = negamaxRecur(b, depth - 1, eval, diveCheck, time, start);

      if(s.outOftime){
        oot = s.outOftime;
        break;
      }

      if (const int currScore = -s.score; currScore > maxScore) {
        maxScore = currScore;
        bestMove = move;
      }
      b.unmakeMove(move);
    }
    return {bestMove, maxScore, oot};
  }

SearchResult negamax(const SearchInfo& si){
  int dc = 0;
  return negamaxRecur(si.b, si.depth, si.eval, &dc, si.time, si.start);
}

SearchResult alphabetaRecur(Board b, int depth, function<int(Board)> eval,
AlphaBetaInfo alphaBeta, int * diveCheck, int time, chrono::_V2::system_clock::time_point start) {
    (*diveCheck)++;
    if(*diveCheck == 1000){
      *diveCheck = 0;
      auto end = chrono::high_resolution_clock::now();
      auto duration =
          chrono::duration_cast<chrono::milliseconds>(end - start);
      if (duration.count() > time) {
        return SearchResult(Move(), -MATE, true);
      }
    }
    if (depth == 0) {
      return SearchResult(Move(), eval(b) * b.turn, false);
    }
    vector<Move> moves = b.gen_moves(b.turn);
    if (moves.size() == 0) {
      return SearchResult(Move(), -MATE - depth, false);
    }
    int maxScore = -MATE * 100;
    int currScore;
    Move bestMove = moves[0];
    SearchResult s = SearchResult();
    bool oot = false;
    for (Move move : moves) {
        if (move.build == WIN) {
          return SearchResult(move, MATE + depth - 1, false);
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
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MATE, MATE);
      int dc = 0;
    return alphabetaRecur(si.b, si.depth, si.eval, alphaBeta, &dc, si.time, si.start);
    }
template<typename T>
void moveElementToFront(vector<T>& vec, const T& element) {
    auto it = find(vec.begin(), vec.end(), element);
    if (it != vec.end()) {
        rotate(vec.begin(), it, it + 1);
    }
}
int mvb15Recur(AlphaBetaInfo alphaBeta, int depth, Board b, TimeInfo timeInfo, function<int(Board)> eval, HashTable * hashTable){
    (*(timeInfo.diveCheck))++;
      if((*(timeInfo.diveCheck)) % 1000 == 0){
        auto end = chrono::high_resolution_clock::now();
        auto duration =
            chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *(timeInfo.oot) = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }
    vector<Move> moves = b.gen_moves(b.turn);
    if (moves.size() == 0) {
      return -MATE - depth;
    }

    int maxScore = -MATE * 100;
    int currScore;
    int oldAlpha = alphaBeta.alpha;
    Move bestMove = moves[0];

    if(probeHashEntryOld(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
      hashTable->cut++;
      return maxScore;
    }
    auto pvMove = probePvMove(b, hashTable, &currScore);
    if(pvMove.from >= 0){
      moveElementToFront(moves, pvMove);
    }
    for (Move move : moves) {
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -mvb15Recur(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
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

        if(*(timeInfo.oot)){
          return 0;
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

SearchResult mvb15(SearchInfo si){
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MATE, MATE);
    int dc = 0;
    bool oot = false;
    auto ti = TimeInfo(&dc, si.time, si.start, &oot);
    int score = mvb15Recur(alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
    Move m = probePvMove(si.b, &(si.hashTable), &score);
    bool keep = m.from >= 0;
    return SearchResult(m, score, oot, keep);
    }

vector<Move> bucketOrder(const vector<Move>& moves, HashTable * hashTable, Board b, const int depth){
    vector<Move> buckets[6];
    vector<Move> orderedMoves;
    int fakeScore;
    Move fakeMove;
    for(Move move: moves){
      if(move.build == WIN){
        buckets[0].push_back(move);
        continue;
      }
      if(probePvMove(b, hashTable, &fakeScore) == move){
        buckets[1].push_back(move);
        continue;
      }
      b.makeMove(move);
      if(probeHashEntryOld(b, hashTable, &fakeMove, &fakeScore, 0, 0, depth)){
        buckets[2].push_back(move);
        b.unmakeMove(move);
        continue;
      }
      b.unmakeMove(move);
      if(move.toHeight > move.fromHeight){
        buckets[3].push_back(move);
      }
      else if(move.toHeight == move.fromHeight){
        buckets[4].push_back(move);
      }
      else{
        buckets[5].push_back(move);
      }
    }

    for (auto & bucket : buckets) {
      orderedMoves.insert(orderedMoves.end(), bucket.begin(), bucket.end());
    }
    return orderedMoves;
}
int mvb127Recur(const int currDepth, AlphaBetaInfo alphaBeta, const int depth, Board b, const TimeInfo &timeInfo, const function<int(Board)>& eval, HashTable * hashTable){
  (*(timeInfo.diveCheck))++;
  if((*(timeInfo.diveCheck)) % 1000 == 0){
    auto end = chrono::high_resolution_clock::now();
    auto duration =
        chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
    if (duration.count() > timeInfo.time) {
      *(timeInfo.oot) = true;
      return 0;
    }
  }

  if (depth == 0) {
    return eval(b) * b.turn;
  }
  vector<Move> moves = b.gen_moves(b.turn);
  if (moves.size() == 0) {
    return -MATE - depth;
  }

  int maxScore = -MATE * 100;
  int currScore;
  int oldAlpha = alphaBeta.alpha;
  Move bestMove = moves[0];

  if(probeHashEntryOld(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
    hashTable->cut++;
    return maxScore;
  }
  auto pvMove = probePvMove(b, hashTable, &currScore);

  if(currScore == 0) moves = bucketOrder(moves, hashTable, b, depth);

  if(pvMove.from >= 0){
    moveElementToFront(moves, pvMove);
  }
  for (Move move : moves) {
    if (move.build == WIN) {
      currScore = MATE+depth-1;
    }
    else{
      b.makeMove(move);
      currScore = -mvb127Recur(currDepth+1, AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
      b.unmakeMove(move);
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

    if(*(timeInfo.oot)){
      return 0;
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

SearchResult mvb127(SearchInfo si){
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MATE, MATE);
    int dc = 0;
    bool oot = false;
    auto ti = TimeInfo(&dc, si.time, si.start, &oot);
    int score = mvb127Recur(0, alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
    Move m = probePvMove(si.b, &(si.hashTable), &score);
    bool keep = m.from >= 0;
    return SearchResult(m, score, oot, keep);
    }
Move getBestMove(Board b, EngineInfo engineInfo, int time) {
  int thinkingTime = engineInfo.timeManager(TMInfo(time, b.ply));
  int depth = 1;
  int maxScore;
  Move bestMove = Move();
  Move gbestMove = Move();
  SearchResult s = SearchResult();
  chrono::_V2::system_clock::time_point start = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> duration;
  while (true) {
    s = engineInfo.search(SearchInfo(b, depth, engineInfo.eval, thinkingTime, engineInfo.hashTable, start));
    bestMove = s.move;
    maxScore = s.score;
    if(s.keepResult){
      gbestMove = bestMove;
    }
    duration = chrono::system_clock::now() - start;
    // Print the maximum score at each depth
    if (VERBOSE && s.keepResult) {
      cout << "Depth: " << depth
     << " - Best move: " << bestMove.toString()
     << " - Time: " << duration.count() << "ms"
     << setw(10 - to_string(duration.count()).length() - 2) << " "  // Calculate padding after "ms"
     << " - Best score: " << editScore(maxScore, depth);
     if(s.pvLine.size() > 0)
       cout << endl << "PV Line: " << pvLineToString(s.pvLine) << endl;
     }
    else{
      cout << endl;
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
  return gbestMove;
}
int mvb143Recur(AlphaBetaInfo alphaBeta, int depth, Board b, TimeInfo timeInfo, function<int(Board)> eval, HashTable * hashTable){
    (*(timeInfo.diveCheck))++;
      if((*(timeInfo.diveCheck)) % 1000 == 0){
        auto end = chrono::high_resolution_clock::now();
        auto duration =
            chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *(timeInfo.oot) = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }
    vector<Move> moves;
    if(depth > 1){
      moves = b.gen_moves(b.turn);
    }
    else{
      moves = b.gen_half_moves(b.turn);
    }
    if (moves.size() == 0) {
      return -MATE - depth;
    }

    int maxScore = -MATE * 100;
    int currScore;
    int oldAlpha = alphaBeta.alpha;
    Move bestMove = moves[0];

    if(probeHashEntryOld(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
      hashTable->cut++;
      return maxScore;
    }
    auto pvMove = probePvMove(b, hashTable, &currScore);
    if(!(pvMove == NO_MOVE)){
      moveElementToFront(moves, pvMove);
    }
    for (Move move : moves) {
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -mvb143Recur(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
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

        if(*(timeInfo.oot)){
          return 0;
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

SearchResult mvb143(SearchInfo si){
    AlphaBetaInfo alphaBeta = AlphaBetaInfo(-MATE, MATE);
    int dc = 0;
    bool oot = false;
    auto ti = TimeInfo(&dc, si.time, si.start, &oot);
    int score = mvb143Recur(alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
    Move m = probePvMove(si.b, &(si.hashTable), &score);
    bool keep = !(m == NO_MOVE);
    return SearchResult(m, score, oot, keep);
    }


void scoreMoves(vector<Move>& moves, const HashTable * ht, const Board * board) {
  int currScore;
  const auto pvMove = probePvMove(*board, ht, &currScore);
  for(auto& move : moves) {
    if (move == pvMove) move.moveOrderingScore = 2;
    else move.moveOrderingScore = move.toHeight - move.fromHeight;
  }
}

void scoreMovesV2(vector<Move>& moves, const HashTable * ht, const Board * board) {
  int currScore;
  const auto pvMove = probePvMove(*board, ht, &currScore);
  for(auto& move : moves) {
    if (move == pvMove) move.moveOrderingScore = 100;
    else move.moveOrderingScore = (move.toHeight - move.fromHeight) * 7 + (move.toN - move.fromN);
  }
}

void scoreMovesV3(vector<Move>& moves, const HashTable * ht, const Board * board) {
  int numDoubleNeighbors[] = {9,  12, 15, 12, 9,
                              12, 16, 20, 16, 12,
                              15, 20, 25, 20, 15,
                              12, 16, 20, 16, 12,
                              9,  12, 15, 12, 9};
  int currScore;
  const auto pvMove = probePvMove(*board, ht, &currScore);
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

int properMORecur(AlphaBetaInfo alphaBeta, const int depth, Board b, const TimeInfo &timeInfo,
  const function<int(Board)>& eval, HashTable * hashTable){
    (*timeInfo.diveCheck)++;
      if(*timeInfo.diveCheck % 1000 == 0){
        const auto end = chrono::high_resolution_clock::now();
        const auto duration =
            chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *timeInfo.oot = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }
    vector<Move> moves = b.gen_moves(b.turn);
    if (moves.empty()) {
      return -MATE - depth;
    }

    int maxScore = -MATE * 100;
    int currScore;
    const int oldAlpha = alphaBeta.alpha;
    Move bestMove = moves[0];

    if(probeHashEntryOld(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
      hashTable->cut++;
      return maxScore;
    }
    scoreMoves(moves, hashTable, &b);

    for (int i=0; i < moves.size(); i++) {
        pickMove(moves, i);
        auto move = moves[i];
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -properMORecur(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
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

        if(*(timeInfo.oot)){
          return 0;
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
SearchResult properMO(SearchInfo si){
  const auto alphaBeta = AlphaBetaInfo(-MATE, MATE);
  int dc = 0;
  bool oot = false;
  const auto ti = TimeInfo(&dc, si.time, si.start, &oot);
  int score = properMORecur(alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
  Move m = probePvMove(si.b, &(si.hashTable), &score);
  const bool keep = m.from >= 0;
  return {m, score, oot, keep};
}


int properMORecur2(AlphaBetaInfo alphaBeta, const int depth, Board b, const TimeInfo &timeInfo,
  const function<int(Board)>& eval, HashTable * hashTable){
    (*timeInfo.diveCheck)++;
      if(*timeInfo.diveCheck % 1000 == 0){
        const auto end = chrono::high_resolution_clock::now();
        const auto duration =
            chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *timeInfo.oot = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }
    vector<Move> moves = b.gen_moves(b.turn);
    if (moves.empty()) {
      return -MATE - depth;
    }

    int maxScore = -MATE * 100;
    int currScore;
    const int oldAlpha = alphaBeta.alpha;
    Move bestMove = moves[0];

    if(probeHashEntryOld(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
      hashTable->cut++;
      return maxScore;
    }
    scoreMovesV2(moves, hashTable, &b);

    for (int i=0; i < moves.size(); i++) {
        pickMove(moves, i);
        auto move = moves[i];
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -properMORecur2(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
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

        if(*(timeInfo.oot)){
          return 0;
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
SearchResult properMOV2(SearchInfo si){
  const auto alphaBeta = AlphaBetaInfo(-MATE, MATE);
  int dc = 0;
  bool oot = false;
  const auto ti = TimeInfo(&dc, si.time, si.start, &oot);
  int score = properMORecur2(alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
  Move m = probePvMove(si.b, &(si.hashTable), &score);
  const bool keep = m.from >= 0;
  return {m, score, oot, keep};
}

int properMORecur3(AlphaBetaInfo alphaBeta, const int depth, Board b, const TimeInfo &timeInfo,
  const function<int(Board)>& eval, HashTable * hashTable){
    (*timeInfo.diveCheck)++;
      if(*timeInfo.diveCheck % 1000 == 0){
        const auto end = chrono::high_resolution_clock::now();
        const auto duration =
            chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *timeInfo.oot = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }
    vector<Move> moves = b.gen_moves(b.turn);
    if (moves.empty()) {
      return -MATE - depth;
    }

    int maxScore = -MATE * 100;
    int currScore;
    const int oldAlpha = alphaBeta.alpha;
    Move bestMove = moves[0];

    if(probeHashEntryOld(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
      hashTable->cut++;
      return maxScore;
    }
    scoreMovesV3(moves, hashTable, &b);

    for (int i=0; i < moves.size(); i++) {
        pickMove(moves, i);
        auto move = moves[i];
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -properMORecur3(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
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

        if(*(timeInfo.oot)){
          return 0;
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
SearchResult properMOV3(SearchInfo si){
  const auto alphaBeta = AlphaBetaInfo(-MATE, MATE);
  int dc = 0;
  bool oot = false;
  const auto ti = TimeInfo(&dc, si.time, si.start, &oot);
  int score = properMORecur3(alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
  Move m = probePvMove(si.b, &(si.hashTable), &score);
  const bool keep = m.from >= 0;
  return {m, score, oot, keep};
}

int creatorRecur(AlphaBetaInfo alphaBeta, const int depth, Board b, const TimeInfo &timeInfo,
  const function<int(Board)>& eval, HashTable * hashTable){
    (*timeInfo.diveCheck)++;
      if(*timeInfo.diveCheck % 1000 == 0){
        const auto end = chrono::high_resolution_clock::now();
        const auto duration =
            chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *timeInfo.oot = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }
    vector<Move> moves;
    if(depth > 1) moves = b.gen_moves(b.turn);
    else moves = b.gen_half_moves(b.turn);

    if (moves.empty()) {
      return -MATE - depth;
    }

    int maxScore = -MATE * 100;
    int currScore;
    const int oldAlpha = alphaBeta.alpha;
    Move bestMove = moves[0];

    if(probeHashEntryOld(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
      hashTable->cut++;
      return maxScore;
    }
    scoreMovesV3(moves, hashTable, &b);

    for (int i=0; i < moves.size(); i++) {
        pickMove(moves, i);
        auto move = moves[i];
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -creatorRecur(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
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

        if(*(timeInfo.oot)){
          return 0;
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
SearchResult creator(SearchInfo si){
  const auto alphaBeta = AlphaBetaInfo(-MATE, MATE);
  int dc = 0;
  bool oot = false;
  const auto ti = TimeInfo(&dc, si.time, si.start, &oot);
  int score = creatorRecur(alphaBeta, si.depth, si.b, ti, si.eval, &(si.hashTable));
  Move m = probePvMove(si.b, &(si.hashTable), &score);
  const bool keep = m.from >= 0;
  return {m, score, oot, keep};
}

int voidRecur(AlphaBetaInfo alphaBeta, const int depth, Board b, const TimeInfo &timeInfo,
  const function<int(Board)>& eval, HashTable * hashTable){
    (*timeInfo.diveCheck)++;
      if(*timeInfo.diveCheck % 1000 == 0){
        const auto end = chrono::high_resolution_clock::now();
        const auto duration =
            chrono::duration_cast<chrono::milliseconds>(end - timeInfo.start);
        if (duration.count() > timeInfo.time) {
          *timeInfo.oot = true;
          return 0;
        }
      }

    if (depth == 0) {
      return eval(b) * b.turn;
    }

  int maxScore = -MATE * 100;
  auto bestMove = NO_MOVE;

  if(probeHashEntry(b, hashTable, &bestMove, &maxScore, alphaBeta.alpha, alphaBeta.beta, depth)){
    hashTable->cut++;
    return maxScore;
  }

    vector<Move> moves = b.gen_moves(b.turn);
    if (moves.empty()) {
      return -MATE - depth;
    }

    int currScore;
    const int oldAlpha = alphaBeta.alpha;

    scoreMovesV3(moves, hashTable, &b);

    for (int i=0; i < moves.size(); i++) {
        pickMove(moves, i);
        auto move = moves[i];
        if (move.build == WIN) {
          currScore = MATE+depth-1;
        }
        else{
          b.makeMove(move);
          currScore = -voidRecur(AlphaBetaInfo(-alphaBeta.beta, -alphaBeta.alpha), depth-1, b, timeInfo, eval, hashTable);
          b.unmakeMove(move);
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

        if(*(timeInfo.oot)){
          return 0;
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

SearchResult voidS(SearchInfo si){
  const auto alphaBeta = AlphaBetaInfo(-MATE, MATE);
  int dc = 0;
  bool oot = false;
  const auto ti = TimeInfo(&dc, si.time, si.start, &oot);
  int score = voidRecur(alphaBeta, si.depth, si.b, ti, si.eval, &si.hashTable);
  Move m = probePvMove(si.b, &(si.hashTable), &score);
  const bool keep = m.from >= 0;
  auto sr = SearchResult(m, score, oot, keep);
  sr.pvLine = getPvLine(si.depth, si.b, &si.hashTable);
  return sr;
}