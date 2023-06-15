#include "match.h"


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
MatchResult playMatch(int time, int startingPos, EngineInfo engineA, EngineInfo engineB) {
    int clockA = time;
    int clockB = time;

    std::vector<Move> moves;

    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    std::chrono::milliseconds duration;

    Board b = Board(startingPos);
    Move move = Move(-2, -2, -2);

    allocateHashTable(&(engineA.hashTable), 1000);
    allocateHashTable(&(engineB.hashTable), 1000);

    while (true) {
        if (CLOCK) {
            printClocks(clockA, clockB);
        }
        if (BOARD){
           b.print();
        }
        start = std::chrono::high_resolution_clock::now();
        if (b.turn == 1) {
            move = getBestMove(b, engineA, clockA);
        } else {
            move = getBestMove(b, engineB, clockB);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (b.turn == 1) {
            clockA -= duration.count();
        } else {
            clockB -= duration.count();
        }
        moves.push_back(move); // Store the move in the vector
        if (move.build == WIN) {
            MatchResult result;
            result.result = b.turn;
            result.moves = moves;
            freeHashTable(&(engineA.hashTable));
            freeHashTable(&(engineB.hashTable));
            return result; // Return the MatchResult struct with the result and moves played
        }
        b.makeMove(move);
        if (b.gen_moves(b.turn).empty()) {
            MatchResult result;
            result.result = b.turn * 2;
            result.moves = moves;
            freeHashTable(&(engineA.hashTable));
            freeHashTable(&(engineB.hashTable));            
            return result; // Return the MatchResult struct with the result and moves played
        }
        if (clockA <= 0) {
            MatchResult result;
            result.result = -3;
            result.moves = moves;
            freeHashTable(&(engineA.hashTable));
            freeHashTable(&(engineB.hashTable));            
            return result; // Return the MatchResult struct with the result and moves played
        }
        if (clockB <= 0) {
            MatchResult result;
            result.result = 3;
            result.moves = moves;
            freeHashTable(&(engineA.hashTable));
            freeHashTable(&(engineB.hashTable));            
            return result; // Return the MatchResult struct with the result and moves played
        }
    }

}
void playSingleMatch(int time, std::string playerA, std::string playerB, int pos){
    EngineInfo engineA = assemblyEngine(playerA);
    EngineInfo engineB = assemblyEngine(playerB);
    MatchResult lastResult = playMatch(time, pos, engineA, engineB);
    registerMatch(pos, playerA, playerB, time, time, lastResult);
    std::string resultA = std::to_string((lastResult.result > 0) ? 1:0);
    std::string resultB = std::to_string((lastResult.result < 0) ? 1:0);
    std::cout << playerA << " " << resultA << " X " << resultB << " " << playerB << std::endl;
}

void play(int time, int numMatches, std::string playerA, std::string playerB) {
    EngineInfo engineA = assemblyEngine(playerA);
    EngineInfo engineB = assemblyEngine(playerB);
    std::set<int> positionsA = getPositionsForMatch(playerA, playerB, numMatches);
    std::set<int> positionsB = getPositionsForMatch(playerB, playerA, numMatches);

    std::map<std::string, int> scores;
    scores[playerA] = 0;
    scores[playerB] = 0;

    int matchesPlayed = 0;

    std::cout << "===== MATCHES START =====" << std::endl;

    auto playMatchAndUpdateScores = [&](std::set<int>& positions, EngineInfo& engine1, EngineInfo& engine2, const std::string& player1, const std::string& player2) {
        int pos = *positions.begin();
        positions.erase(positions.begin());
        MatchResult lastResult = playMatch(time, pos, engine1, engine2);
        scores[player1] += (lastResult.result > 0) ? 1 : 0;
        scores[player2] += (lastResult.result > 0) ? 0 : 1;
        registerMatch(pos, player1, player2, time, time, lastResult);
        matchesPlayed++;
        return lastResult;
    };

    for (int i = 0; i < numMatches; i++) {
        // Player A vs. Player B
        MatchResult resultA = playMatchAndUpdateScores(positionsA, engineA, engineB, playerA, playerB);

        // Print the score and matches played after each match
        std::cout << "Match " << matchesPlayed << " | " << playerA << " vs " << playerB << " | ";
        std::cout << "Score: " << scores[playerA] << " - " << scores[playerB] << " | ";
        std::cout << "Matches played: " << matchesPlayed << "/" << numMatches * 2 << std::endl;
        // Player B vs. Player A
        MatchResult resultB = playMatchAndUpdateScores(positionsB, engineB, engineA, playerB, playerA);

        // Print the score and matches played after each match
        std::cout << "Match " << matchesPlayed << " | " << playerA << " vs " << playerB << " | ";
        std::cout << "Score: " << scores[playerA] << " - " << scores[playerB] << " | ";
        std::cout << "Matches played: " << matchesPlayed << "/" << numMatches * 2 << std::endl;
    }

    std::cout << "===== MATCHES END =====" << std::endl;
}
void heal(int time){
  auto entries = findEntriesWithNoPair(time);
  int totalPositions = entries.size(); // Total number of positions
  int positionsPlayed = 0; // Number of positions played
  std::cout << "Starting healing " << totalPositions << " pos" <<std::endl;

  while (!entries.empty()) {
    auto entry = *entries.begin();
    entries.erase(entries.begin());
    
    std::cout << "Playing match: " << entry.player_b << " vs " << entry.player_a << " (Starting position: " << entry.starting_pos << ")" << std::endl;
    
    playSingleMatch(time, entry.player_b, entry.player_a, std::stoi(entry.starting_pos));
    
    positionsPlayed++; // Increment the number of positions played
    
    double fractionPlayed = static_cast<double>(positionsPlayed) / totalPositions;
    std::cout << fractionPlayed * 100 << "\% played" << std::endl;

    entries = findEntriesWithNoPair(time);
  }
}
