#include <iostream>
#include <iomanip>
#include <fstream>
#include "hash.h"
#include "board.h"

int main() {
    int invalid = 0;
    int redundant = 0;
    int valid = 0;

    std::ofstream outputFile("valid_hashes.txt"); // Open the file for writing

    for (int hash = minHash; hash < maxHash; hash++) {
        if (!validateHash(hash)) {
            invalid++;
        } else {
            if (reduceHash(hash) == hash) {
                valid++;
                outputFile << hash << std::endl; // Write valid hash to the file
            } else {
                redundant++;
            }
        }
    }

    // Close the file after writing
    outputFile.close();

    // Print the final results
    std::cout << "Invalid: " << invalid << " (" << std::fixed << std::setprecision(2) << (invalid * 100.0 / maxHash) << "%)" << std::endl;
    std::cout << "Redundant: " << redundant << " (" << std::fixed << std::setprecision(2) << (redundant * 100.0 / maxHash) << "%)" << std::endl;
    std::cout << "Valid: " << valid << " (" << std::fixed << std::setprecision(2) << (valid * 100.0 / maxHash) << "%)" << std::endl;

    return 0;
}
