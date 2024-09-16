#include "postMatch.h"
#include "db_manager.h"
#include <fstream>
#include <sqlite3.h>

using namespace std;

void saveMoves(const vector<Move>& moves, const int id) {
    sqlite3* db;
    if (sqlite3_open("santorini.db", &db)) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    }
    for(int i=0; i < moves.size(); i++) {
        const Move& move = moves[i];
        insertMoveData(db, id, i, move.from, move.to, move.build);
    }
}
void registerMatch(const int startingPos, const string& playerA, const string& playerB, const int timeA, const int timeB, const MatchResult& result){
    sqlite3* db;
    if (sqlite3_open("santorini.db", &db)) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    }
    const int id = insertMatchData(db, -1, startingPos, playerA, playerB, timeA, timeB, result.result);
    saveMoves(result.moves, id);
}
