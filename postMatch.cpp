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
void saveMoves(const std::vector<Move>& moves) {
    const std::string& filename = "games/"+std::to_string(readCounter());
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

    // Example data to append
    std::string idS = std::to_string(readCounter());
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
    saveMoves(result.moves);
    updateCounter();
}
