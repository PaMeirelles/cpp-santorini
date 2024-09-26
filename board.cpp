#include "board.h"
#include "hash.h"
#include "move.h"
#include <iostream>

#include "defs.h"

using namespace  std;

Board::Board(const int (&w)[4]) {
  for (int & square : squares) {
    square = 0;
  }
  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

Board::Board(const int startingPos) {
  int w[4];
  unhashWorkers(startingPos, w);
  for (int & square : squares) {
    square = 0;
  }
  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

Board::Board(const int (&w)[4], const int (&s)[25]) {
  for (int i = 0; i < 25; i++) {
    squares[i] = s[i];
  }

  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

void Board::print() const {
  cout << "Squares:\n";
  for (int i = 0; i < 25; i++) {
    cout << squares[i] << " ";
    if ((i + 1) % 5 == 0) {
      cout << "\n";
    }
  }

  cout << "\nWorkers:\n";
  for (const int worker : workers) {
    cout << worker << " ";
  }
  cout << "\nPly: " << ply << endl;
  cout << endl;
}

Board::Board() {
  int w[4];
  const int hash = genHash();

  unhashWorkers(hash, w);

  for (int & square : squares) {
    square = 0;
  }

  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

int Board::getWorkerHeight(const int workerId) const { return squares[workers[workerId]]; }
int Board::getWorkerPos(const int workerId) const { return workers[workerId]; }
void Board::makeMove(const Move &move) {
  bool workerFound = false;

  for (int & worker : workers) {
    if (worker == move.from) {
      workerFound = true;

      if (worker == move.to) {
        throw runtime_error(
            "Invalid move: 'from' and 'to' positions cannot be the same.");
      }

      worker = move.to;
    } else {
      if (worker == move.build) {
        throw runtime_error(
            "Invalid move: a worker cannot be at the 'build' position.");
      }
    }
  }

  if (!workerFound) {
    throw runtime_error("Worker not found at the specified position.");
  }

  if (move.from > 24 || move.to > 24 || move.build > 24) {
    throw runtime_error("Invalid position specified in the move.");
  }

  if (squares[move.build] == 4) {
    throw runtime_error(
        "Maximum number of squares already built at the specified position.");
  }
  if(move.build != HALF_MOVE){
    squares[move.build]++;
  }
  turn *= -1;
  ply++;
}

vector<Move> Board::gen_moves(const int player) const {
  int w[2];
  vector<Move> moves;
  if (player == 1) {
    w[0] = workers[0];
    w[1] = workers[1];
  } else {
    w[0] = workers[2];
    w[1] = workers[3];
  }
  for (const int currW : w) {
    vector<int> neighbors = getNeighbors(currW);
    const int currH = getHeight(currW);
    for (const int nb : neighbors) {
      const int neighborH = getHeight(nb);
      if (!isFree(nb) || getHeight(nb) - currH > 1) {
        continue;
      }
      if (squares[nb] == 3) {
        moves.emplace_back(currW, nb, WIN, currH, neighborH);
      } else {
        for (vector<int> toBuild = getNeighbors(nb); const int b : toBuild) {
          if (!isFree(b) && b != currW) {
            continue;
          }
          moves.emplace_back(currW, nb, b, currH, neighborH);
        }
      }
    }
  }
  return moves;
}

bool Board::isFree(const int square) const {
  for (const int worker : workers) {
    if (worker == square) {
      return false;
    }
  }
  if (squares[square] == 4) {
    return false;
  }
  return true;
}
int Board::getHeight(const int square) const { return squares[square]; }
void Board::unmakeMove(const Move &move) {
  bool workerFound = false;

  for (int & worker : workers) {
    if (worker == move.to) {
      workerFound = true;

      if (worker == move.from) {
        throw runtime_error(
            "Invalid move: 'from' and 'to' positions cannot be the same.");
      }

      worker = move.from;
    } else {
      if (worker == move.build) {
        throw runtime_error(
            "Invalid move: a worker cannot be at the 'build' position.");
      }
    }
  }

  if (!workerFound) {
    throw runtime_error("Worker not found at the specified position.");
  }

  if (move.from > 24 || move.to > 24 || move.build > 24) {
    throw runtime_error("Invalid position specified in the move.");
  }

  if (squares[move.build] == 0) {
    throw runtime_error(
        "Minimum number of squares already built at the specified position.");
  }
  turn *= -1;
  if(move.build != HALF_MOVE){
      squares[move.build]--;
  }
  ply--;
}
vector<Move> Board::gen_half_moves(const int player) const {
  int w[2];
  vector<int> toBuild;
  vector<Move> moves;
  if (player == 1) {
    w[0] = workers[0];
    w[1] = workers[1];
  } else {
    w[0] = workers[2];
    w[1] = workers[3];
  }
  for (const int currW : w) {
    vector<int> neighbors = getNeighbors(currW);
    const int currH = getHeight(currW);
    for (const int nb : neighbors) {
      const int neighborH = getHeight(nb);
      const bool isClimb = neighborH > currH;
      if (!isFree(nb) || getHeight(nb) - currH > 1) {
        continue;
      }
      if (squares[nb] == 3) {
        moves.emplace_back(currW, nb, WIN, isClimb, neighborH);
      } else {
        moves.emplace_back(currW, nb, currW, isClimb, neighborH);
      }
    }
  }
  return moves;
}
