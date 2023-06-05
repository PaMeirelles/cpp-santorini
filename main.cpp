#include "board.h"
#include "eval.h"
#include "hash.h"
#include "matchManager.h"
#include "search.h"
#include "timeManagement.h"
#include <fstream>
#include <iomanip>
#include <iostream>

int main() { std::cout << playMatch(1000 * 60, 680, "Titan", "Ruler"); }
