#include "preMatch.h"

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
std::set<int> getPositionsForMatch(std::string playerA,std::string playerB, int n) {
 
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
