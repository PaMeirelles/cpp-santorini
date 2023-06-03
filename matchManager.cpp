#include "matchManager.h"

int evalPosition(std::function<int(Board)> eval, int hash){
    int w[4];
    unhashWorkers(hash, w);
    Board board = Board(w);
    return eval(board);
}


std::set<int> getPositionsForMatch(std::function<int(Board)> evalOne, std::function<int(Board)> evalTwo, int n){
    std::ifstream inputFile("valid_hashes.txt");
    std::vector<int> hashListOne;

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            try {
                int hash = std::stoi(line);
                hashListOne.push_back(hash);
            } catch (const std::exception& e) {
                // Handle invalid lines if needed
                std::cout << "Invalid line: " << line << std::endl;
            }
        }
        inputFile.close();

    } else {
        std::cout << "Failed to open the file." << std::endl;
    }

    std::vector<int> hashListTwo(hashListOne);

    std::sort(hashListOne.begin(), hashListOne.end(), [&](int a, int b) {
        return evalPosition(evalOne, a) < evalPosition(evalOne, b);
    });
    std::sort(hashListTwo.begin(), hashListTwo.end(), [&](int a, int b) {
        return evalPosition(evalTwo, a) < evalPosition(evalTwo, b);
    });


    std::set<int> setHash(hashListOne.begin(), hashListOne.end());

    int indexOne = 0;
    int indexTwo = hashListTwo.size() - 1;
    int current = 1;
    std::set<int>::iterator it;
    while(setHash.size() > n){
        if(current == 1){
            it = setHash.find(hashListOne[indexOne]);
            indexOne++;
            if(it != setHash.end()){
                setHash.erase(it);
                current = 2;            
            }
        }
        else{
            it = setHash.find(hashListTwo[indexTwo]);
            indexTwo--;
            if(it != setHash.end()){
                setHash.erase(it);
                current = 1;
            }
        }
    }
    return setHash;
}