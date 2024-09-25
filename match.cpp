#include "match.h"

#include "defs.h"

using namespace std;

void printClocks(const int clockA, const int clockB){
    // Format clockA and clockB as minutes:seconds:milliseconds
    const int clockASeconds = clockA / 1000;           // Convert milliseconds to seconds
    const int clockAMinutes = clockASeconds / 60;      // Convert seconds to minutes
    const int clockAMilliseconds = clockA % 1000;      // Extract milliseconds

    const int clockBSeconds = clockB / 1000;           
    const int clockBMinutes = clockBSeconds / 60;      
    const int clockBMilliseconds = clockB % 1000;      

  cout << "clockA: " << setfill('0') << setw(2) << clockAMinutes << ":"
            << setfill('0') << setw(2) << clockASeconds % 60 << ":"
            << setfill('0') << setw(3) << clockAMilliseconds << "   ";
  cout << "clockB: " << setfill('0') << setw(2) << clockBMinutes << ":"
            << setfill('0') << setw(2) << clockBSeconds % 60 << ":"
            << setfill('0') << setw(3) << clockBMilliseconds << endl;

}

MatchResult getResult(const int resultValue, const vector<Move>& moves, const HashTable& hashTableA, const HashTable& hashTableB) {
    MatchResult result;
    result.result = resultValue;
    result.moves = moves;
    freeHashTable(&hashTableA);
    freeHashTable(&hashTableB);
    return result;
}

MatchResult playMatch(int time, int startingPos, EngineInfo engineA, EngineInfo engineB) {
    int clockA = time;
    int clockB = time;

    vector<Move> moves;

    chrono::system_clock::time_point start;
    chrono::system_clock::time_point end;
    chrono::milliseconds duration;

    auto b = Board(startingPos);
    Move move;

    allocateHashTable(&(engineA.hashTable), 1000);
    allocateHashTable(&(engineB.hashTable), 1000);

    while (true) {
        if (CLOCK) {
            printClocks(clockA, clockB);
        }
        if (BOARD){
           b.print();
        }
        start = chrono::high_resolution_clock::now();
        if (b.turn == 1) {
            move = getBestMove(b, engineA, clockA);
        } else {
            move = getBestMove(b, engineB, clockB);
        }
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (b.turn == 1) {
            clockA -= static_cast<int>(duration.count());
        } else {
            clockB -= static_cast<int>(duration.count());
        }
        moves.push_back(move); // Store the move in the vector

        if (move.build == WIN) {
            return getResult(b.turn, moves, engineA.hashTable, engineB.hashTable);
        }

        try {
            b.makeMove(move);
        } catch (const runtime_error& _) {
            return getResult(b.turn * -4, moves, engineA.hashTable, engineB.hashTable);
        }

        if (b.gen_moves(b.turn).empty()) {
            return getResult(b.turn * 2, moves, engineA.hashTable, engineB.hashTable);
        }

        if (clockA <= 0) {
            return getResult(-3, moves, engineA.hashTable, engineB.hashTable);
        }

        if (clockB <= 0) {
            return getResult(3, moves, engineA.hashTable, engineB.hashTable);
        }

    }

}
void playSingleMatch(int time, const string& playerA, const string& playerB, int pos){
    EngineInfo engineA = assemblyEngine(playerA);
    EngineInfo engineB = assemblyEngine(playerB);
    MatchResult lastResult = playMatch(time, pos, engineA, engineB);
    registerMatch(pos, playerA, playerB, time, time, lastResult);
    string resultA = to_string((lastResult.result > 0) ? 1:0);
    string resultB = to_string((lastResult.result < 0) ? 1:0);
    cout << playerA << " " << resultA << " X " << resultB << " " << playerB << endl;
}

void play(int time, int numMatches, const string& playerA, const string& playerB) {
    EngineInfo engineA = assemblyEngine(playerA);
    EngineInfo engineB = assemblyEngine(playerB);
    set<int> positionsA = getPositionsForMatch(playerA, playerB, numMatches);
    set<int> positionsB = getPositionsForMatch(playerB, playerA, numMatches);

    map<string, int> scores;
    scores[playerA] = 0;
    scores[playerB] = 0;

    int matchesPlayed = 0;

    cout << "===== MATCHES START =====" << endl;

    auto playMatchAndUpdateScores = [&](set<int>& positions, const EngineInfo& engine1, const EngineInfo& engine2, const string& player1, const string& player2) {
        const int pos = *positions.begin();
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
        cout << "Match " << matchesPlayed << " | " << playerA << " vs " << playerB << " | ";
        cout << "Score: " << scores[playerA] << " - " << scores[playerB] << " | ";
        cout << "Matches played: " << matchesPlayed << "/" << numMatches * 2 << endl;
        // Player B vs. Player A
        MatchResult resultB = playMatchAndUpdateScores(positionsB, engineB, engineA, playerB, playerA);

        // Print the score and matches played after each match
        cout << "Match " << matchesPlayed << " | " << playerA << " vs " << playerB << " | ";
        cout << "Score: " << scores[playerA] << " - " << scores[playerB] << " | ";
        cout << "Matches played: " << matchesPlayed << "/" << numMatches * 2 << endl;
    }

    cout << "===== MATCHES END =====" << endl;
}

void heal(const int time){
  auto entries = findEntriesWithNoPair(time);
  const U64 totalPositions = entries.size(); // Total number of positions
  int positionsPlayed = 0; // Number of positions played
  cout << "Starting healing " << totalPositions << " pos" <<endl;

  while (!entries.empty()) {
    auto entry = *entries.begin();
    entries.erase(entries.begin());
    
    cout << "Playing match: " << entry.player_b << " vs " << entry.player_a << " (Starting position: " << entry.starting_pos << ")" << endl;
    
    playSingleMatch(time, entry.player_b, entry.player_a, stoi(entry.starting_pos));
    
    positionsPlayed++; // Increment the number of positions played

    const double fractionPlayed = static_cast<double>(positionsPlayed) / static_cast<double>(totalPositions);
    cout << fractionPlayed * 100 << "\% played" << endl;

    entries = findEntriesWithNoPair(time);
  }
}
