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


std::vector<int> getValidPositions(const std::string& playerA, const std::string& playerB) {
  std::ifstream inputFile("valid_hashes.txt");
  std::vector<int> validPositions;

  if (inputFile.is_open()) {
    std::unordered_set<int> playedPositions;
    std::ifstream csvFile("meta/matches.csv");
    if (csvFile.is_open()) {
      std::string csvLine;
      // Skip the first line (header)
      std::getline(csvFile, csvLine);
      while (std::getline(csvFile, csvLine)) {
        std::istringstream ss(csvLine);
        std::string id, startingPos, playerARecord, playerBRecord, timeA, timeB, result;
        std::getline(ss, id, ',');
        std::getline(ss, startingPos, ',');
        std::getline(ss, playerARecord, ',');
        std::getline(ss, playerBRecord, ',');
        std::getline(ss, timeA, ',');
        std::getline(ss, timeB, ',');
        std::getline(ss, result, ',');

        // Check if the match involves playerA and playerB
        if (playerARecord == playerA && playerBRecord == playerB) {
          int startingPosition = std::stoi(startingPos);
          playedPositions.insert(startingPosition);
        }
      }
      csvFile.close();
    } else {
      std::cout << "Failed to open the CSV file." << std::endl;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
      try {
        int hash = std::stoi(line);
        if (playedPositions.find(hash) == playedPositions.end()) {
          validPositions.push_back(hash);
        }
      } catch (const std::exception& e) {
        // Handle invalid lines if needed
        std::cout << "Invalid line: " << line << std::endl;
      }
    }
    inputFile.close();
  } else {
    std::cout << "Failed to open the file." << std::endl;
  }

  return validPositions;
}


std::set<int> getPositionsForMatch(std::string playerA,
                                   std::string playerB, int n) {
 
  std::vector<int> hashListOne = getValidPositions(playerA, playerB);                
  std::vector<int> hashListTwo(hashListOne);

  auto evalOne = assemblyEngine(playerA).eval;
  auto evalTwo = assemblyEngine(playerB).eval;

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

int playMatch(int time, int startingPos, EngineInfo engineA, EngineInfo engineB) {
  int clockA = time;
  int clockB = time;

  int result = 0;

  std::chrono::_V2::system_clock::time_point start;
  std::chrono::_V2::system_clock::time_point end;
  std::chrono::milliseconds duration;

  Board b = Board(startingPos);
  Move move = Move(-2, -2, -2);

  while (true) {
    if(DETAILED){
      printClocks(clockA, clockB);
      b.print();
    }
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
      return b.turn * 2;
    }
    if(clockA <= 0){
      return -3;
    }
    if(clockB <= 0){
      return 3;
    }
  }
}
void appendToCSV(const std::string& filename, const std::string& row) {
    // Open the CSV file in append mode
    std::ofstream file(filename, std::ios::app);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return;
    }

    // Append the row to the CSV file
    file << row << std::endl;

    // Close the file
    file.close();
}
int readCounter() {
    // Open the file
    std::ifstream file("meta/counter");

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return 0;  // or any other default value you prefer
    }

    // Read the integer value from the file
    int number;
    file >> number;

    // Close the file
    file.close();

    // Return the read integer value
    return number;
};
void updateCounter() {
    int prevCounter = readCounter();
    // Open the file in input/output mode
    std::fstream file("meta/counter", std::ios::in | std::ios::out);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return;
    }

    // Write the new integer value to the file
    file << prevCounter+1;

    // Close the file
    file.close();
}

void registerMatch(int startingPos, std::string playerA, std::string playerB, int timeA, int timeB, int result){
   std::string filename = "meta/matches.csv";

    // Example data to append
    std::string idS = std::to_string(readCounter());
    std::string startingPosS = std::to_string(startingPos);
    std::string timeAS = std::to_string(timeA);
    std::string timeBS = std::to_string(timeB);
    std::string resultS = std::to_string(result);

    // Create the row to append
    std::stringstream ss;
    ss << idS << "," << startingPosS << "," << playerA << "," << playerB << ","
       << timeAS << "," << timeBS << "," << resultS;
    std::string row = ss.str();

    // Append the row to the CSV file
    appendToCSV(filename, row);
    updateCounter();
}
void play(int time, int numMatches, std::string playerA, std::string playerB){
    EngineInfo engineA = assemblyEngine(playerA);
    EngineInfo engineB = assemblyEngine(playerB);
    std::set<int> positionsA = getPositionsForMatch(playerA, playerB, numMatches);
    std::set<int> positionsB = getPositionsForMatch(playerB, playerA, numMatches);

    int pos;
    int lastResult;
    int scoreA;
    int scoreB;

    while(positionsA.size() > 0){
      pos = *positionsA.begin();
      positionsA.erase(positionsA.begin());
      lastResult = playMatch(time, pos, engineA, engineB);
      if(lastResult > 0){
        scoreA++;
      }
      else{
        scoreB++;
      }
      registerMatch(pos, playerA, playerB, time, time, lastResult);
    }
    while(positionsB.size() > 0){
      pos = *positionsB.begin();
      positionsB.erase(positionsB.begin());
      lastResult = playMatch(time, pos, engineB, engineA);
      if(lastResult > 0){
        scoreB++;
      }
      else{
        scoreA++;
      }
      registerMatch(pos, playerB, playerA, time, time, lastResult);
    }
}