#include <thread>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include "game_of_life.h"

#define MAX_THREADS 8

typedef bool Node;
typedef std::vector<std::vector<Node>> Grid;


static int live(const Grid& grid, int x, int y) {
    int li = 0;
    for (int i = std::max(0, y-1); i <= y+1 && i < (int)grid.size(); i++) {
        for (int j = std::max(0, x-1); j <= x+1 && j < (int)grid[0].size(); j++) {
            if (i == y && j == x) continue;
            li += (grid[i][j] == true ? 1 : 0);
        }
    }
    return li;
}

Grid init(int x, int y){
    return Grid(y, std::vector<Node>(x, false));
}

void randomize(Grid& g) {
    for (int i = 0; i < (int)g.size(); i++) 
        for (int j = 0; j < (int)g[i].size(); j++) g[i][j] = rand() % 2;
}

bool time_step(const Grid &grid, Grid& next) {
    for (int row = 0; row < (int)grid.size(); row++) {
        for (int col = 0; col < (int)grid[row].size(); col++) {
            int lives = live(grid, col, row);
            if (lives == 2 && grid[row][col]) next[row][col] = true;
            else if (lives == 3)          next[row][col] = true;
            else                          next[row][col] = false;
        }
    }

    return true;
}

bool update(Grid& grid, Grid& next) {
    grid = next;
    return true;
}

int survivors(const Grid& grid) {
    return std::accumulate(grid.cbegin(), grid.cend(), 0,
                [](int b, const auto& i) { 
                    return std::count(i.cbegin(), i.cend(), true) + b; 
            }
        );
}

void print(const Grid& grid) {
  printf("\x1b[H");
  for (int j = 0; j < (int)grid.size(); j++) {
    for (int i = 0; i < (int)grid[j].size(); i++) {
      putchar(grid[j][i] ? 'X' : ' ');
    }
    putchar('\n');
  }
}

void gol_serial(int x, int y, int runs) {
    Grid grid, next;
    grid = next = init(x, y);

    randomize(grid);

    std::cout << x << " * " << y << std::endl;

    for (int R = 0; R < runs; R++) {
        time_step(grid, next);
        update(grid, next);
        print(grid);
        printf("Survivors: %d\n", survivors(grid));
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}


struct WorkerArgs {
    Grid& grid;
    Grid& next;
    int iters;
    int threadId;
    int numThreads;
};

void thread_work(WorkerArgs& w) {
    for (int i = w.threadId*w.iters ; i < w.threadId*w.iters+w.iters && i < (int)w.grid.size(); i++) {
        for (int j = 0; j < (int)w.grid[i].size(); j++) {
            int lives = live(w.grid, j, i);
            if (lives == 2 && w.grid[i][j]) w.next[i][j] = true;
            else if (lives == 3) w.next[i][j] = true;
            else w.next[i][j] = false;
        }
    }
}

void gol_thread(int x, int y, int runs, int threads) {
    Grid grid, next;
    grid = init(x, y);
    next = init(x, y);
    randomize(grid);

    std::cout << x << " * " << y << std::endl;

    if (threads > MAX_THREADS) threads = MAX_THREADS;
    int iters = y / threads;

    for (int R = 0; R < runs; R++) {
        std::vector<std::thread> aThreads;
        for (int t = 0; t < threads; t++) {
            WorkerArgs c = {grid, next, iters, t, threads};
            aThreads.push_back(std::thread(thread_work, std::ref(c)));
        }
        for (std::thread & t : aThreads) {
            if (t.joinable())
                t.join();
        }

        update(grid, next);
        print(grid);
        printf("Survivors: %d\n", survivors(grid));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
