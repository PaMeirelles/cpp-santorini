#include "matchManager.h"
#include "eval.h"
#include "search.h"
#include "timeManagement.h"
#include <chrono>
#include <iomanip>

int evalPosition(std::function<int(Board)> eval, int hash) {
  int w[4];
  unhashWorkers(hash, w);
  Board board = Board(w);
  return eval(board);
}

std::set<int> getPositionsForMatch(std::function<int(Board)> evalOne,
                                   std::function<int(Board)> evalTwo, int n) {
  std::ifstream inputFile("valid_hashes.txt");
  std::vector<int> hashListOne;

  if (inputFile.is_open()) {
    std::string line;
    while (std::getline(inputFile, line)) {
      try {
        int hash = std::stoi(line);
        hashListOne.push_back(hash);
      } catch (const std::exception &e) {
        // Handle invalid lines if needed
        std::cout << "Invalid line: " << line << std::endl;
      }
    }
    inputFile.close();

  } else {
    std::cout << "Failed to open the file." << std::endl;
  }

  std::vector<int> hashListTwo(hashListOne);

  std::sort(hashListOne.begin(), hashListOne.end(), [&](int a, int b) {
    return evalPosition(evalOne, a) < evalPosition(evalOne, b);
  });
  std::sort(hashListTwo.begin(), hashListTwo.end(), [&](int a, int b) {
    return evalPosition(evalTwo, a) < evalPosition(evalTwo, b);
  });

  std::set<int> setHash(hashListOne.begin(), hashListOne.end());

  int indexOne = 0;
  int indexTwo = hashListTwo.size() - 1;
  int current = 1;
  std::set<int>::iterator it;
  while (setHash.size() > n) {
    if (current == 1) {
      it = setHash.find(hashListOne[indexOne]);
      indexOne++;
      if (it != setHash.end()) {
        setHash.erase(it);
        current = 2;
      }
    } else {
      it = setHash.find(hashListTwo[indexTwo]);
      indexTwo--;
      if (it != setHash.end()) {
        setHash.erase(it);
        current = 1;
      }
    }
  }
  return setHash;
}
EngineInfo assemblyEngine(std::string name) {
  EngineInfo eInfo;
  if (name == "Titan") {
    eInfo.search = negamax;
    eInfo.eval = nh_s;
    eInfo.timeManager = et_s;
  } else if (name == "Ruler") {
    eInfo.search = negamax;
    eInfo.eval = nh_c;
    eInfo.timeManager = et_s;
  } else {
    throw std::runtime_error("Invalid engine");
  }
  return eInfo;
}

void printClocks(int clockA, int clockB){
    // Format clockA and clockB as minutes:seconds:milliseconds
  int clockASeconds = clockA / 1000;           // Convert milliseconds to seconds
  int clockAMinutes = clockASeconds / 60;      // Convert seconds to minutes
  int clockAMilliseconds = clockA % 1000;      // Extract milliseconds

  int clockBSeconds = clockB / 1000;           // Convert milliseconds to seconds
  int clockBMinutes = clockBSeconds / 60;      // Convert seconds to minutes
  int clockBMilliseconds = clockB % 1000;      // Extract milliseconds

  // Print clockA and clockB in minutes:seconds:milliseconds format
  std::cout << "clockA: " << std::setfill('0') << std::setw(2) << clockAMinutes << ":"
            << std::setfill('0') << std::setw(2) << clockASeconds % 60 << ":"
            << std::setfill('0') << std::setw(3) << clockAMilliseconds << "   ";
  std::cout << "clockB: " << std::setfill('0') << std::setw(2) << clockBMinutes << ":"
            << std::setfill('0') << std::setw(2) << clockBSeconds % 60 << ":"
            << std::setfill('0') << std::setw(3) << clockBMilliseconds << std::endl;

}

int playMatch(int time, int startingPos, std::string playerA,
              std::string playerB) {
  EngineInfo engineA = assemblyEngine(playerA);
  EngineInfo engineB = assemblyEngine(playerB);

  int clockA = time;
  int clockB = time;

  int result = 0;

  std::chrono::_V2::system_clock::time_point start;
  std::chrono::_V2::system_clock::time_point end;
  std::chrono::milliseconds duration;

  Board b = Board(startingPos);
  Move move = Move(-2, -2, -2);

  while (true) {
    printClocks(clockA, clockB);
    b.print();
    start = std::chrono::high_resolution_clock::now();
    if (b.turn == 1) {
      move = getBestMove(b, engineA.search, engineA.eval, engineA.timeManager,
                         clockA);
    } else {
      move = getBestMove(b, engineB.search, engineB.eval, engineB.timeManager,
                         clockB);
    }
    end = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if (b.turn == 1) {
      clockA -= duration.count();
    } else {
      clockB -= duration.count();
    }
    if (move.build == WIN) {
      return b.turn;
    }
    b.makeMove(move);
    if (b.gen_moves(b.turn).size() == 0) {
      return b.turn;
    }
  }
}