#include "benchmark.h"

std::map<std::string, std::function<SearchResult(Board, int, std::function<int(Board)>, int)>> getEngineMap(){
    return std::map<std::string, std::function<SearchResult(Board, int, std::function<int(Board)>, int)>> 
    {{"negamax", negamax},
     {"alphabeta", alphabeta},
     {"climb mo v5", alphabetaWitClimbhMo}
     };


}
std::vector<int> getRandomStartingPos(int n) {

    std::vector<Record> records;
    std::ifstream csvFile("matches.csv");
    if (!csvFile) {
        std::cout << "Error opening the CSV file." << std::endl;
        return {};
    }

    std::string line;
    std::getline(csvFile, line);  // Skip header line

    while (std::getline(csvFile, line)) {
        std::stringstream ss(line);
        std::string cell;
        Record record;

        // Parse CSV values
        std::getline(ss, cell, ',');
        record.id = std::stoi(cell);
        std::getline(ss, cell, ',');
        record.starting_pos = std::stoi(cell);
        std::getline(ss, record.player_a, ',');
        std::getline(ss, record.player_b, ',');
        std::getline(ss, cell, ',');
        record.time_a = std::stoi(cell);
        std::getline(ss, cell, ',');
        record.time_b = std::stoi(cell);
        std::getline(ss, record.result, ',');

        records.push_back(record);
    }

    std::vector<int> startingPos;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, records.size() - 1);

    for (int i = 0; i < n; ++i) {
        int index = distribution(generator);
        startingPos.push_back(records[index].starting_pos);
    }

    return startingPos;
}

std::vector<Move> runTest(std::vector<int> positions, std::string search, int depth) {
    std::vector<Move> moves;  // Vector to store the moves
    auto engineMap = getEngineMap();
    Board b;
    auto searchFunc = engineMap[search];
    std::chrono::system_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int pos : positions) {
        b = Board(pos);
        auto s = searchFunc(b, depth, nh_s, 1000 * 60 * 60);
        std::cout << "Completed test for " << search << " on depth " << depth << std::endl;
        moves.push_back(s.move);  // Add the move to the vector
    }
    std::chrono::system_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::ofstream file("benchmark.csv", std::ios::app);
    file << search << "," << depth << "," << positions.size() << "," << duration.count() / positions.size() << std::endl;
    file.close();

    return moves;  // Return the vector of moves
}

void twinTest(std::string search1, std::string search2, int n, int depth) {
    auto positions = getRandomStartingPos(n);
    std::vector<Move> moves1 = runTest(positions, search1, depth);
    std::vector<Move> moves2 = runTest(positions, search2, depth);

    // Compare move vectors
    if (moves1.size() != moves2.size()) {
        std::cout << "Move vectors have different sizes." << std::endl;
        return;
    }

    bool equal = true;
    for (size_t i = 0; i < moves1.size(); i++) {
        auto m1 = moves1[i];
        auto m2 = moves2[i];

        if (m1.from != m2.from || m1.to != m2.to || m1.build != m2.build) {
            equal = false;
            break;
        }
    }

    if (equal) {
        std::cout << "Move vectors are equal." << std::endl;
    } else {
        std::cout << "Move vectors are not equal." << std::endl;
    }
}

void testUpTo(std::string search1, std::string search2, int n, int depth){
    for(int i=1; i <= depth; i++){
        twinTest(search1, search2, n, i);
        // Print completion or other relevant information
        std::cout << "Test " << i << " completed." << std::endl;
    }
}
