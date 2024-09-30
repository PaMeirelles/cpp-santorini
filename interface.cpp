//
// Created by Rafael on 9/26/2024.
//

#include "interface.h"

#include <sstream>

#include "board.h"
#include "hashTable.h"
#include "search.h"

using namespace std;

Board parse_position(const string &position) {
    if (position.size() != 51) {
        throw runtime_error("Invalid position: Expected length 50, but got " + to_string(position.size()));
    }

    int blocks[25];
    int workers[4];

    int numGrayWorkers = 0;
    int numBlueWorkers = 0;

    for (int i = 0; i < 25; i++) {
        char buffer = position[2 * i];
        const int height = buffer - '0';
        if (height < 0 || height > 4) {
            throw runtime_error("Invalid position: Block height at index " + to_string(i) + " is " + buffer + ", expected between 0 and 4");
        }
        blocks[i] = height;

        buffer = position[2 * i + 1];
        if (buffer == 'G') {
            if (numGrayWorkers == 2) {
                throw runtime_error("Invalid position: More than 2 gray workers found");
            }
            workers[numGrayWorkers] = i;
            numGrayWorkers++;
        } else if (buffer == 'B') {
            if (numBlueWorkers == 2) {
                throw runtime_error("Invalid position: More than 2 blue workers found");
            }
            workers[2 + numBlueWorkers] = i;
            numBlueWorkers++;
        } else if (buffer != 'N') {
            throw runtime_error("Invalid position: Unexpected character '" + string(1, buffer) + "' at position " + to_string(2 * i + 1) + ", expected 'G', 'B', or 'N'");
        }
    }

    if (numGrayWorkers != 2) {
        throw runtime_error("Invalid position: Expected 2 gray workers, but found " + to_string(numGrayWorkers));
    }
    if (numBlueWorkers != 2) {
        throw runtime_error("Invalid position: Expected 2 blue workers, but found " + to_string(numBlueWorkers));
    }

    int turn;

    if(position[50] == '0') turn = 1;
    else if (position[50] == '1') turn = -1;
    else throw runtime_error("Invalid turn: '" + string(1, position[50]) + "' expected '0' or '1'");

    return {workers, blocks, turn};
}

string buildMove(const Move &move) {
    return numToSquare[move.from] + numToSquare[move.to] + numToSquare[move.build];
}
// Updated mainLoop to handle time management
void mainLoop(Board * b, HashTable * ht) {
    string command;
    int gtime = 0, btime = 0;

    while (true) {
        getline(cin, command);
        if (command == "quit") {
            break;
        }
        if (command == "isready") {
            cout << "readyok" << endl;
        } else if (command.substr(0, 9) == "position ") {
            string position = command.substr(9);
            try {
                *b = parse_position(position);
                cout << "Position set." << endl; // Optionally confirm the position
            } catch (const runtime_error &e) {
                cerr << "Error parsing position: " << e.what() << endl;
            }
        } else if (command.substr(0, 2) == "go") {
            // Extract the time controls
            istringstream iss(command.substr(3));
            string token;
            while (iss >> token) {
                if (token == "gtime") iss >> gtime;
                else if (token == "btime") iss >> btime;
            }

            try {
                // Adjust the best move calculation logic based on available time
                int time;
                if(b->turn == 1) time = gtime;
                else time = btime;
                Move bestMove = getBestMove(b, time, ht);
                cout << "bestmove " << buildMove(bestMove) << endl;
            } catch (const exception &e) {
                cerr << "Error during move calculation: " << e.what() << endl;
            }
        } else {
            cerr << "Unknown command: " << command << endl;
        }
    }
}
