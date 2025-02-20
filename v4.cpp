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

        // 起点和终点的位置
        startX = 1;  // 起点位置在 (1, 1)
        startY = 1;
        endX = width - 2;
        endY = height - 2;

        // 设置起点和终点
        maze[startY][startX] = '?';  // 起点
        maze[endY][endX] = '%';      // 终点

        // 确保起点和终点之间的路径
        ensurePath();

        // 使用 DFS 算法生成迷宫的其余部分
        dfsGenerate(startX, startY);

        // 确保终点是开放的
        maze[endY][endX] = '%';
    }

    void ensurePath() {
        // 确保起点和终点之间有路径
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int x = startX, y = startY;
        maze[y][x] = '.';  // 起点设置为路径

        // 随机生成从起点到终点的路径
        while (x != endX || y != endY) {
            shuffle(directions.begin(), directions.end(), rng);  // 随机化方向
            bool moved = false;

            for (auto& dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                    maze[ny][nx] = '.';  // 打通路径
                    x = nx;
                    y = ny;
                    moved = true;
                    break;
                }
            }

            if (!moved) break;  // 如果没有移动，说明卡住了，退出
        }
    }

    void dfsGenerate(int x, int y) {
        // 四个方向：右、左、下、上
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        shuffle(directions.begin(), directions.end(), rng);  // 随机化方向

        for (auto& dir : directions) {
            int nx = x + dir.first * 2;
            int ny = y + dir.second * 2;

            // 确保新位置在迷宫内，且没有被访问过
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                maze[ny][nx] = '.';  // 打通路径
                maze[y + dir.second][x + dir.first] = '.';  // 在当前位置和新位置之间打通墙壁

                // 递归继续生成迷宫
                dfsGenerate(nx, ny);
            }
        }
    }

    void print() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
            	if(x==1&&y==1) cout<<"&";
            	else if(x==17&&y==18) cout<<".";
                else cout << maze[y][x];
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

