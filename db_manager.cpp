#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sqlite3.h>
#include <stdexcept>
#include <filesystem>

#include "board.h"

using namespace std;
namespace fs = std::filesystem;

// Function to insert match data into the database
int insertMatchData(sqlite3* db, const int id, const int starting_pos, const string& player_1, const string& player_2,
    const int time_1, const int time_2, const int result) {
    const int legacy = id != -1;
    string sql;
    sqlite3_stmt* stmt;

    // If id is -1, don't include it in the INSERT statement
    if (id == -1) {
        sql = "INSERT INTO TB_MATCHES (starting_pos, player_1, player_2, time_1, time_2, result, legacy) "
              "VALUES (?, ?, ?, ?, ?, ?, ?);";
    } else {
        sql = "INSERT INTO TB_MATCHES (id, starting_pos, player_1, player_2, time_1, time_2, result, legacy) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    }

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    int bind_index = 1;

    // Bind the id if provided (not -1)
    if (id != -1) {
        sqlite3_bind_int(stmt, bind_index++, id);
    }

    // Binding the rest of the values
    sqlite3_bind_int(stmt, bind_index++, starting_pos);
    sqlite3_bind_text(stmt, bind_index++, player_1.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, bind_index++, player_2.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, bind_index++, time_1);
    sqlite3_bind_int(stmt, bind_index++, time_2);
    sqlite3_bind_int(stmt, bind_index++, result);
    sqlite3_bind_int(stmt, bind_index++, legacy);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return -1;
    }

    // Retrieve the last inserted id if auto-increment was used
    int return_id;
    if (id == -1) {
        return_id = static_cast<int>(sqlite3_last_insert_rowid(db));
    } else {
        return_id = -1;
    }

    sqlite3_finalize(stmt);
    return return_id;
}


// Function to process all CSV rows and insert them into the database
void add_all() {
    sqlite3* db;
    if (sqlite3_open("santorini.db", &db)) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    }
    ifstream file("matches.csv");
    if (!file.is_open()) {
        cerr << "Could not open CSV file!" << endl;
        sqlite3_close(db);
        return;
    }

    string line, header;
    getline(file, header); // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        int id, starting_pos, time_a, time_b, result;
        string player_a, player_b;

        try {
            // Parse each value from the CSV
            getline(ss, token, ','); id = stoi(token);
            getline(ss, token, ','); starting_pos = stoi(token);
            getline(ss, token, ','); player_a = token;  // Text field
            getline(ss, token, ','); player_b = token;  // Text field
            getline(ss, token, ','); time_a = stoi(token);
            getline(ss, token, ','); time_b = stoi(token);
            getline(ss, token, ','); result = stoi(token);

        } catch (const std::invalid_argument& e) {
            cerr << "Error: Invalid argument while parsing row: " << line << "\n";
            cerr << "Reason: " << e.what() << "\n";
            continue;  // Skip this row and continue with the next
        } catch (const std::out_of_range& e) {
            cerr << "Error: Out of range value while parsing row: " << line << "\n";
            cerr << "Reason: " << e.what() << "\n";
            continue;  // Skip this row and continue with the next
        }

        // Insert the parsed data into the database
        if (!insertMatchData(db, id, starting_pos, player_a, player_b, time_a, time_b, result)) {
            cerr << "Failed to insert data for ID: " << id << endl;
        }
    }

    // Close the file and the database connection
    file.close();
    sqlite3_close(db);
}


bool insertMoveData(sqlite3* db, const int match_id, const int move_num, const int from, const int to, const int build) {
    const string sql = "INSERT INTO TB_MOVES (ID_match, move_num, from_square, to_square, build_square) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, match_id) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, move_num) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, from) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 4, to) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 5, build) != SQLITE_OK) {
        cerr << "Failed to bind values: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

void add_moves() {
    sqlite3* db;
    if (sqlite3_open("santorini.db", &db)) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    }

    int file_count = 0;
    for (const auto& entry : fs::directory_iterator("games")) {
        ifstream file(entry.path());
        if (!file.is_open()) {
            cerr << "Could not open file: " << entry.path() << endl;
            continue;
        }
        string match_id_str = entry.path().filename().stem().string();
        int match_id = stoi(match_id_str);

        string line;
        int move_num = 1;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            int from, to, build;

            getline(ss, token, ','); from = stoi(token.substr(token.find(": ") + 2));
            getline(ss, token, ','); to = stoi(token.substr(token.find(": ") + 2));
            getline(ss, token, ','); build = stoi(token.substr(token.find(": ") + 2));

            if (!insertMoveData(db, match_id, move_num++, from, to, build)) {
                cerr << "Failed to insert move for match ID: " << match_id << endl;
            }
        }

        file.close();
        cout << "Parsed file: " << entry.path().filename().string() << endl;
        file_count++;
    }

    sqlite3_close(db);
    cout << "Total files parsed: " << file_count << endl;
}

bool saveBenchmark(sqlite3* db, int match_id, int move_number, string search_engine, string eval_func, int depth, int eval, int time) {
    string sql = "INSERT INTO TB_BENCHMARK (match_id, move_num, search_engine, eval_func, depth, eval, time) "
                 "VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    // Bind the values to the SQL statement
    if (sqlite3_bind_int(stmt, 1, match_id) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, move_number) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, search_engine.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 4, eval_func.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 5, depth) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 6, eval) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 7, time) != SQLITE_OK) {
        cerr << "Failed to bind values: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
        }

    // Execute the SQL statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    return true;
}

// Function to retrieve a random match ID and its starting position
int retrieveRandomMatchId(sqlite3 *db, int *starting_pos) {
    string sql = "SELECT ID, starting_pos FROM TB_MATCHES ORDER BY RANDOM() LIMIT 1;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return -1;
    }

    int match_id = -1;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        match_id = sqlite3_column_int(stmt, 0);
        *starting_pos = sqlite3_column_int(stmt, 1);
    }

    sqlite3_finalize(stmt);
    return match_id;
}

// Function to retrieve moves for a given match ID
#include <cstdlib>   // For rand()
#include <ctime>     // For seeding rand()
#include <vector>
#include <iostream>
#include <sqlite3.h>

using namespace std;

vector<Move> retrieveRandomMatch(sqlite3 *db, int mid) {
    vector<Move> moves;
    vector<int> move_numbers;
    string sql = "SELECT from_square, to_square, build_square, move_num FROM TB_MOVES WHERE ID_match = ? ORDER BY move_num;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return moves;
    }

    // Bind the match ID
    sqlite3_bind_int(stmt, 1, mid);

    // Retrieve all moves and store them in the vector, along with their move numbers
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int from_square = sqlite3_column_int(stmt, 0);
        int to_square = sqlite3_column_int(stmt, 1);
        int build_square = sqlite3_column_int(stmt, 2);
        int move_num = sqlite3_column_int(stmt, 3);
        moves.emplace_back(from_square, to_square, build_square);
        move_numbers.push_back(move_num);
    }

    sqlite3_finalize(stmt);

    if (move_numbers.empty()) {
        cerr << "No moves found for the match." << endl;
        return {};
    }

    // Seed the random number generator
    srand(time(nullptr));

    // Pick a random move number
    int random_index = rand() % move_numbers.size();
    int random_move_num = move_numbers[random_index];

    // Collect moves that have a smaller move number than the random move
    vector<Move> filtered_moves;
    for (int i = 0; i < random_index; ++i) {
        filtered_moves.push_back(moves[i]);
    }

    return filtered_moves;
}


Board retrieveRandomBoard(sqlite3* db, int * matchId, int * moveNumber) {
    int starting_pos;
    *matchId = retrieveRandomMatchId(db, &starting_pos);
    auto moves = retrieveRandomMatch(db, *matchId);
    *moveNumber = static_cast<int>(moves.size());
    auto board = Board(starting_pos);
    for (const auto& move : moves) {
        board.makeMove(move);
    }
    return board;
}
