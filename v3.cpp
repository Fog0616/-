#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

const int WIDTH = 20;  // 迷宫宽度
const int HEIGHT = 20; // 迷宫高度

class Maze {
public:
    Maze(int width, int height, int seed) : width(width), height(height), rng(seed) {
        maze.resize(height, vector<char>(width, '#'));  // 初始化迷宫，全是墙
    }

    void generate() {
        // 初始化边界
        for (int y = 0; y < height; ++y) {
            maze[y][0] = maze[y][width - 1] = '#';
        }
        for (int x = 0; x < width; ++x) {
            maze[0][x] = maze[height - 1][x] = '#';
        }

        // 随机选择起点和终点
        startX = 1;
        startY = 1;
        endX = width - 2;
        endY = height - 2;

        // 设置起点和终点
        maze[startY][startX] = '&';  // 起点
        maze[endY][endX] = '▲';      // 终点

        // 使用 Prim 算法生成迷宫
        primAlgorithm();

        // 确保终点是开放的
        maze[endY][endX] = '▲';
    }

    void primAlgorithm() {
        // 初始化墙壁集合，选择一个随机的起点，使用 Prim 算法
        vector<pair<int, int>> walls;
        walls.push_back({startX, startY});

        // 四个方向：右、左、下、上
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        maze[startY][startX] = '.';  // 将起点设置为路径

        // 使用 Prim 算法探索迷宫
        while (!walls.empty()) {
            shuffle(walls.begin(), walls.end(), rng);  // 随机化墙壁的选择顺序

            // 从墙壁集合中选择一个随机的墙
            pair<int, int> wall = walls.back();
            walls.pop_back();

            int x = wall.first;
            int y = wall.second;

            // 遍历四个方向，检查周围的墙是否可以成为路径
            for (auto& dir : directions) {
                int nx = x + dir.first * 2;
                int ny = y + dir.second * 2;

                // 如果新位置在迷宫内且未被访问过，则生成路径
                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                    maze[ny][nx] = '.';  // 将新的位置设置为路径
                    maze[y + dir.second][x + dir.first] = '.';  // 在当前位置和新位置之间打通墙壁

                    // 将新墙壁添加到墙壁集合
                    walls.push_back({nx, ny});
                }
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
    mt19937 rng;  // 使用一个随机数生成器，基于种子
};

int main() {
    // 获取用户输入的种子
    int seed;
    cout << "请输入一个随机数种子：";
    cin >> seed;

    // 创建迷宫并使用用户输入的种子
    Maze maze(WIDTH, HEIGHT, seed);
    
    // 生成迷宫
    maze.generate();
    
    // 打印迷宫
    maze.print();
    return 0;
}

