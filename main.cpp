#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include "game_of_life.h"

using namespace std;

int main(int argc, char* argv[]) {

    int sizeX = 10, sizeY = 10, iterations = 10;

    std::cout << thread::hardware_concurrency() << std::endl;

    if (argc > 2) {
        sizeX = stoi(argv[1]);
        sizeY = stoi(argv[2]);
        if (argc > 3)
            iterations = stoi(argv[3]);
    }

    gol_thread(sizeX, sizeY, iterations, 4);
    //gol_serial(sizeX, sizeY, iteration);

    return 0;
}
