#include <iostream>
#include <iomanip>
#include "hash.h"
#include "board.h"

int main() {
    int invalid = 0;
    int redundant = 0;
    int valid = 0;
    for(int hash = minHash; hash < maxHash; hash++){
        if(!validateHash(hash)){
            invalid++;
        }
        else{
            if(reduceHash(hash) == hash){
                valid++;
            }
            else{
                redundant++;
            }
        }

    }
    // Print the final results
std::cout << "Invalid: " << invalid << " (" << std::fixed << std::setprecision(2) << (invalid * 100.0 / maxHash) << "%)" << std::endl;
std::cout << "Redundant: " << redundant << " (" << std::fixed << std::setprecision(2) << (redundant * 100.0 / maxHash) << "%)" << std::endl;
std::cout << "Valid: " << valid << " (" << std::fixed << std::setprecision(2) << (valid * 100.0 / maxHash) << "%)" << std::endl;
 
    return 0;
}