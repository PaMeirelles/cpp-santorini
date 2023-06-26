#include "board.h"
#include "hash.h"
#include "move.h"
#include <iostream>

Board::Board(const int (&w)[4]) {
  // Fill the squares array with zeros
  for (int i = 0; i < 25; i++) {
    squares[i] = 0;
  }
  // Copy the worker array to the workers array
  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

Board::Board(int startingPos) {
  int w[4];
  unhashWorkers(startingPos, w);
  // Fill the squares array with zeros
  for (int i = 0; i < 25; i++) {
    squares[i] = 0;
  }
  // Copy the worker array to the workers array
  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

Board::Board(const int (&w)[4], const int (&s)[25]) {
  // Copy the squares array to the squares member variable
  for (int i = 0; i < 25; i++) {
    squares[i] = s[i];
  }

  // Copy the worker array to the workers member variable
  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

void Board::print() const {
  std::cout << "Squares:\n";
  for (int i = 0; i < 25; i++) {
    std::cout << squares[i] << " ";
    if ((i + 1) % 5 == 0) {
      std::cout << "\n";
    }
  }

  std::cout << "\nWorkers:\n";
  for (int i = 0; i < 4; i++) {
    std::cout << workers[i] << " ";
  }
  std::cout << std::endl;
}
Board::Board() {
  int w[4];
  int hash = genHash();

  unhashWorkers(hash, w);

  // Fill the squares array with zeros
  for (int i = 0; i < 25; i++) {
    squares[i] = 0;
  }

  // Copy the worker array to the workers array
  for (int i = 0; i < 4; i++) {
    workers[i] = w[i];
  }
  turn = 1;
  ply = 0;
}

int Board::getWorkerHeight(int workerId) { return squares[workers[workerId]]; }
int Board::getWorkerPos(int workerId) { return workers[workerId]; }
void Board::makeMove(Move move) {
  bool workerFound = false;

  for (int i = 0; i < 4; i++) {
    if (workers[i] == move.from) {
      workerFound = true;

      if (workers[i] == move.to) {
        throw std::runtime_error(
            "Invalid move: 'from' and 'to' positions cannot be the same.");
      }

      workers[i] = move.to;
      break;
    } else {
      if (workers[i] == move.build) {
        throw std::runtime_error(
            "Invalid move: a worker cannot be at the 'build' position.");
      }
    }
  }

  if (!workerFound) {
    throw std::runtime_error("Worker not found at the specified position.");
  }

  if (move.from > 24 || move.to > 24 || move.build > 24) {
    throw std::runtime_error("Invalid position specified in the move.");
  }

  if (squares[move.build] == 4) {
    throw std::runtime_error(
        "Maximum number of squares already built at the specified position.");
  }
  if(move.build != HALF_MOVE){
    squares[move.build]++;
  }
  turn *= -1;
  ply++;
}

std::vector<Move> Board::gen_moves(int player) {
  int w[2];
  int wId[2];
  int currW;
  int currH;
  std::vector<int> neighbors;
  std::vector<int> toBuild;
  std::vector<Move> moves;
  if (player == 1) {
    w[0] = workers[0];
    w[1] = workers[1];
  } else {
    w[0] = workers[2];
    w[1] = workers[3];
  }
  for (int i = 0; i < 2; i++) {
    currW = w[i];
    neighbors = getNeighbors(currW);
    currH = getHeight(currW);
    int neighborH;
    for (int nb : neighbors) {
      neighborH = getHeight(nb);
      bool isClimb = neighborH > currH;
      if (!isFree(nb) || getHeight(nb) - currH > 1) {
        continue;
      }
      if (squares[nb] == 3) {
        moves.push_back(Move(currW, nb, WIN, isClimb, neighborH));
      } else {
        toBuild = getNeighbors(nb);
        for (int b : toBuild) {
          if (!isFree(b) && b != currW) {
            continue;
          }
          moves.push_back(Move(currW, nb, b, isClimb, neighborH));
        }
      }
    }
  }
  return moves;
}
bool Board::isFree(int square) {
  for (int i = 0; i < 4; i++) {
    if (workers[i] == square) {
      return false;
    }
  }
  if (squares[square] == 4) {
    return false;
  }
  return true;
}
int Board::getHeight(int square) { return squares[square]; }
void Board::unmakeMove(Move move) {
  bool workerFound = false;

  for (int i = 0; i < 4; i++) {
    if (workers[i] == move.to) {
      workerFound = true;

      if (workers[i] == move.from) {
        throw std::runtime_error(
            "Invalid move: 'from' and 'to' positions cannot be the same.");
      }

      workers[i] = move.from;
      break;
    } else {
      if (workers[i] == move.build) {
        throw std::runtime_error(
            "Invalid move: a worker cannot be at the 'build' position.");
      }
    }
  }

  if (!workerFound) {
    throw std::runtime_error("Worker not found at the specified position.");
  }

  if (move.from > 24 || move.to > 24 || move.build > 24) {
    throw std::runtime_error("Invalid position specified in the move.");
  }

  if (squares[move.build] == 0) {
    throw std::runtime_error(
        "Minimum number of squares already built at the specified position.");
  }
  turn *= -1;
  if(move.build != HALF_MOVE){
      squares[move.build]--;
  }
  ply--;
}
std::vector<Move> Board::gen_half_moves(int player) {
  int w[2];
  int wId[2];
  int currW;
  int currH;
  std::vector<int> neighbors;
  std::vector<int> toBuild;
  std::vector<Move> moves;
  if (player == 1) {
    w[0] = workers[0];
    w[1] = workers[1];
  } else {
    w[0] = workers[2];
    w[1] = workers[3];
  }
  for (int i = 0; i < 2; i++) {
    currW = w[i];
    neighbors = getNeighbors(currW);
    currH = getHeight(currW);
    int neighborH;
    for (int nb : neighbors) {
      neighborH = getHeight(nb);
      bool isClimb = neighborH > currH;
      if (!isFree(nb) || getHeight(nb) - currH > 1) {
        continue;
      }
      if (squares[nb] == 3) {
        moves.push_back(Move(currW, nb, WIN, isClimb, neighborH));
      } else {
        moves.push_back(Move(currW, nb, HALF_MOVE, isClimb, neighborH));
      }
    }
  }
  return moves;
}