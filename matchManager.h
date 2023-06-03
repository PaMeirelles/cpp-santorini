#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <functional>
#include "board.h"
#include "hash.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

int evalPosition(std::function<int(Board)> eval, int hash);

std::set<int> getPositionsForMatch(std::function<int(Board)> evalOne, std::function<int(Board)> evalTwo, int n);

#endif /* MATCH_MANAGER_H */
