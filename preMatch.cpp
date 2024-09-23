#include "preMatch.h"
#include "sqlite3.h"
using namespace std;

int evalPosition(function<int(Board)> eval, const int hash) {
  int w[4];
  unhashWorkers(hash, w);
  const auto board = Board(w);
  return eval(board);
}

vector<int> getValidPositions(const string& playerA, const string& playerB, sqlite3* db) {
  vector<int> validPositions;
  sqlite3_stmt* stmt;
  unordered_set<int> playedPositions;

  // Query to get positions of played matches between playerA and playerB
  string sql = R"(
        SELECT starting_pos
        FROM TB_MATCHES
        WHERE (player_1 = ? AND player_2 = ?)
           OR (player_1 = ? AND player_2 = ?);
    )";

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
    return validPositions;
  }

  // Bind player names
  sqlite3_bind_text(stmt, 1, playerA.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, playerB.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, playerB.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, playerA.c_str(), -1, SQLITE_STATIC);

  // Execute query and add starting positions to the played set
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int startingPos = sqlite3_column_int(stmt, 0);
    playedPositions.insert(startingPos);
  }

  sqlite3_finalize(stmt);

  // Now retrieve valid positions from TB_VALID_HASHES and check against played positions
  sql = "SELECT hash FROM TB_VALID_HASHES;";
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
    return validPositions;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int hash = sqlite3_column_int(stmt, 0);
    if (!playedPositions.contains(hash)) {
      validPositions.push_back(hash);
    }
  }

  sqlite3_finalize(stmt);

  return validPositions;
}

set<int> getPositionsForMatch(const string& playerA,const string& playerB, const int n) {
 
  vector<int> hashListOne = getValidPositions(playerA, playerB);                
  vector<int> hashListTwo(hashListOne);

  const auto evalOne = assemblyEngine(playerA).eval;
  const auto evalTwo = assemblyEngine(playerB).eval;

  ranges::sort(hashListOne, [&](const int a, const int b) {
    return evalPosition(evalOne, a) < evalPosition(evalOne, b);
  });
  ranges::sort(hashListTwo, [&](const int a, const int b) {
    return evalPosition(evalTwo, a) < evalPosition(evalTwo, b);
  });

  set<int> setHash(hashListOne.begin(), hashListOne.end());

  int indexOne = 0;
  int indexTwo = static_cast<int>(hashListTwo.size()) - 1;
  int current = 1;
  set<int>::iterator it;
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

EngineInfo assemblyEngine(const string& name) {
  EngineInfo eInfo;
  if (name == "Titan") {
    eInfo.search = negamax;
    eInfo.eval = nh_s;
    eInfo.timeManager = et_s;
  } else if (name == "Ruler") {
    eInfo.search = negamax;
    eInfo.eval = nh_c;
    eInfo.timeManager = et_s;
  } else if(name == "Fisher"){
    eInfo.search = negamax;
    eInfo.eval = nh_a;
    eInfo.timeManager = et_s;
  } else if(name == "Ascendant"){
    eInfo.search = alphabeta;
    eInfo.eval = nh_s;
    eInfo.timeManager = et_s;
  } else if(name == "Ranger"){
    eInfo.search = alphabeta;
    eInfo.eval = nh_s;
    eInfo.timeManager = et_p;
  } else if(name == "Aegis"){
    eInfo.search = alphabeta;
    eInfo.eval = nh_s;
    eInfo.timeManager = et_f;
  } else if(name == "Cosmic"){
    eInfo.search = mvb15;
    eInfo.eval = nh_s;
    eInfo.timeManager = et_s;
  } else if(name == "Dream"){
    eInfo.search = mvb127;
    eInfo.eval = nh_s;
    eInfo.timeManager = et_s;
  } else if(name == "Eclipse"){
    eInfo.search = mvb15;
    eInfo.eval = ss_h;
    eInfo.timeManager = et_s;
  } else if(name == "Pilot"){
    eInfo.search = mvb15;
    eInfo.eval = nh_s;
    eInfo.timeManager = eg_c;
  } else if(name == "Harmony"){
    eInfo.search = mvb15;
    eInfo.eval = nh_s;
    eInfo.timeManager = withEmerg;
  } else if(name == "Angel"){
    eInfo.search = mvb15;
    eInfo.eval = nh_s_1;
    eInfo.timeManager = et_s;
  } else if(name == "Vortex"){
    eInfo.search = mvb15;
    eInfo.eval = db_s;
    eInfo.timeManager = et_s;
  } else if(name == "Infinity"){
    eInfo.search = mvb15;
    eInfo.eval = db_s;
    eInfo.timeManager = et_p; 
  } else if(name == "Missing"){
    eInfo.search = mvb15;
    eInfo.eval = db_s;
    eInfo.timeManager = withEmerg;
  } else if(name == "Divine"){
    eInfo.search = mvb143;
    eInfo.eval = db_s;
    eInfo.timeManager = et_p;
  } else if(name == "Canyon"){
      eInfo.search = properMOV2;
      eInfo.eval = nh_s;
      eInfo.timeManager = et_s;
  } else if(name == "Zephyr"){
      eInfo.search = properMOV3;
      eInfo.eval = db_s;
      eInfo.timeManager = et_s;
  } else if(name == "Creator"){
      eInfo.search = creator;
      eInfo.eval = db_s;
      eInfo.timeManager = et_s;
  } else if(name == "Radiant"){
      eInfo.search = creator;
      eInfo.eval = db_s;
      eInfo.timeManager = et_p;
    }
    // else if(name == "Cyan"){
    //   eInfo.search = properMOV3;
    //   eInfo.eval = cyan;
    //   eInfo.timeManager = et_s;
    // }
  else {
    throw runtime_error("Invalid engine: " + name);
  }
  return eInfo;
}
unordered_set<Entry, EntryHash, EntryEqual> findEntriesWithNoPair(int targetTime) {
  string db_filename = "santorini.db";  // SQLite database file
  sqlite3* db;
  unordered_set<Entry, EntryHash, EntryEqual> pairs;

  // Open the SQLite database
  if (sqlite3_open(db_filename.c_str(), &db) != SQLITE_OK) {
    cerr << "Failed to open database: " << db_filename << endl;
    return pairs;
  }

  string sql = "SELECT starting_pos, player_a, player_b FROM matches WHERE time = ?";

  sqlite3_stmt* stmt;
  // Prepare the SQL statement
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return pairs;
  }

  // Bind the targetTime parameter
  if (sqlite3_bind_int(stmt, 1, targetTime) != SQLITE_OK) {
    cerr << "Failed to bind time parameter: " << sqlite3_errmsg(db) << endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return pairs;
  }

  // Loop through the query results
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Entry entry;
    entry.starting_pos = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    entry.player_a = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    entry.player_b = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

    // Check if entry has a pair
    Entry reverseEntry;
    reverseEntry.player_a = entry.player_b;
    reverseEntry.player_b = entry.player_a;
    reverseEntry.starting_pos = entry.starting_pos;

    auto it = pairs.find(reverseEntry);
    if (it == pairs.end()) {
      pairs.insert(entry);
    } else {
      // Pair found, remove from set
      pairs.erase(it);
    }
  }

  // Finalize and clean up
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return pairs;
}
