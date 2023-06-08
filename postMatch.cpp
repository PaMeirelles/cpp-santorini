#include "postMatch.h"

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
int findLowestUnusedID() {
    auto fileName = "matches.csv";
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return -1;
    }

    std::string line;
    std::getline(file, line);  // Read the header line and discard it

    std::vector<int> usedIDs;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string idStr;
        std::getline(iss, idStr, ',');
        int id = std::stoi(idStr);
        usedIDs.push_back(id);
    }

    file.close();

    // Find the lowest unused ID
    for (int i = 0; i < usedIDs.size(); i++) {
        if (usedIDs[i] != i) {
            return i;
        }
    }

    return usedIDs.size();
}

void saveMoves(const std::vector<Move>& moves, int id) {
    const std::string& filename = "games/"+std::to_string(id);
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    for (Move move : moves) {
        file << move.toString() << std::endl;
    }

    file.close();

    std::cout << "Moves written to file: " << filename << std::endl;
}
void registerMatch(int startingPos, std::string playerA, std::string playerB, int timeA, int timeB, MatchResult result){
   std::string filename = "meta/matches.csv";
    int id = findLowestUnusedID();
    // Example data to append
    std::string idS = std::to_string(id);
    std::string startingPosS = std::to_string(startingPos);
    std::string timeAS = std::to_string(timeA);
    std::string timeBS = std::to_string(timeB);
    std::string resultS = std::to_string(result.result);

    // Create the row to append
    std::stringstream ss;
    ss << idS << "," << startingPosS << "," << playerA << "," << playerB << ","
       << timeAS << "," << timeBS << "," << resultS;
    std::string row = ss.str();

    // Append the row to the CSV file
    appendToCSV(filename, row);
    saveMoves(result.moves, id);
}
