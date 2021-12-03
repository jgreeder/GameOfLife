#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef bool Node;

typedef vector<vector<Node>> Grid;

int live(const Grid& grid, int x, int y) {
    int li = 0;
    for (int i = max(0, y-1); i <= y+1 && i < grid.size(); i++) {
        for (int j = max(0, x-1); j <= x+1 && j < grid[0].size(); j++) {
            if (i == y && j == x) continue;
            li += (grid[i][j] == true ? 1 : 0);
        }
    }
    return li;
}

void init(Grid& grid) {
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
            grid[i][j] = rand() % 2;
}

void run(int x, int y, int runs) {
    Grid grid(y, vector<bool>(x, false));
    Grid next(y, vector<bool>(x, false));

    init(grid);

    grid[0][0] = true;
    grid[0][1] = true;
    grid[1][0] = true;
    grid[1][1] = true;

    cout << grid.size() << " * " << grid[0].size() << endl;

    for (int R = 0; R < runs; R++) {
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                int lives = live(grid, j, i);
                if (lives == 2 && grid[i][j]) next[i][j] = true;
                else if (lives == 3) next[i][j] = true;
                else next[i][j] = false;
            }
        }
        int survivors = 0;
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                grid[i][j] = next[i][j];
                survivors += grid[i][j] ? 1 : 0;
            }
        }
        cout << "Iteration: " << R << " " << "Survivors: " << survivors << endl;
    }

}


int main(int argc, char* argv[]) {

    int sizeX = 10, sizeY = 10, iterations = 10;

    if (argc > 2) {
        sizeX = stoi(argv[1]);
        sizeY = stoi(argv[2]);
        if (argc > 3)
            iterations = stoi(argv[3]);
    }

    run(sizeX, sizeY, iterations);

    return 0;
}
