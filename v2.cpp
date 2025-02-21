#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>
#include <algorithm>  // 添加此头文件以使用 std::shuffle
#include <random>     // 添加此头文件以使用随机数生成器

using namespace std;

const int WIDTH = 20;  // 将宽度调整为20
const int HEIGHT = 20; // 将高度调整为20

class Maze {
public:
    Maze(int width, int height) : width(width), height(height) {
        maze.resize(height, vector<char>(width, '#'));  // Initialize maze with walls
    }

    void generate() {
        // Add boundary walls (already initialized with walls)
        for (int y = 0; y < height; ++y) {
            maze[y][0] = maze[y][width - 1] = '#';
        }
        for (int x = 0; x < width; ++x) {
            maze[0][x] = maze[height - 1][x] = '#';
        }

        // Choose random start and end points
        startX = 1;
        startY = 1;
        endX = width - 2;
        endY = height - 2;

        // Set start and end points
        maze[startY][startX] = '&';  // Start point
        maze[endY][endX] = '&';      // End point

        // Start DFS from a random point
        dfs(startX, startY);

        // Ensure that the end is open and marked as ▲
        maze[endY][endX] = '▲';  // End point
    }

    void dfs(int x, int y) {
        // Directions: Right, Left, Down, Up
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        // Randomize directions to add complexity
        random_device rd;                // Random number generator
        mt19937 g(rd());                 // Mersenne Twister engine
        shuffle(directions.begin(), directions.end(), g);  // Randomize directions to create different maze paths

        for (auto& dir : directions) {
            int nx = x + dir.first * 2;
            int ny = y + dir.second * 2;

            // If within bounds and the cell has not been visited (i.e., is still a wall)
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                maze[ny][nx] = '.';  // Carve a path
                maze[y + dir.second][x + dir.first] = '.';  // Carve the wall between current and new cell

                // Recursively visit the next cell
                dfs(nx, ny);
            }
        }
    }

    void print() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << maze[y][x];
            }
            cout << endl;
        }
    }

private:
    int width, height;
    int startX, startY, endX, endY;
    vector<vector<char>> maze;
};

int main() {
    srand(time(0));
    Maze maze(WIDTH, HEIGHT);
    
    // 生成迷宫
    maze.generate();
    
    // 打印迷宫
    maze.print();
    return 0;
}

