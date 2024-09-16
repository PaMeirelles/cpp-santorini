//
// Created by Rafael on 9/16/2024.
//

#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <sqlite3.h>

void add_all();
void add_moves();
bool insertMoveData(sqlite3* db, int match_id, int move_num, int from, int to, int build);
int insertMatchData(sqlite3* db, int id, int starting_pos, const string& player_1, const string& player_2, int time_1, int time_2, int result);
#endif //DB_MANAGER_H
