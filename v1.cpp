#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>  // 添加此行以包含 floor 和其他数学函数
#include <queue>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 50;
const double THRESHOLD = 0.06; // 阈值，决定路径和墙壁

class PerlinNoise {
public:
    PerlinNoise() {
        permutation = {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,214,53,135,97,98,12,4,85,24,179,29,210,118,50,240,122,90,255,123,81,9,161,133,50,85,112,113,129,9,4,50,0,128,32,81,90,50,122,199,232,123,79,40,32,122,73,14,183,195,71,47,50,92,14,113,114,89,91,162,174,45,45,222,97,159,50,0,132,30,3,128,123,111,42,127,18,10};
        for (int i = 0; i < 256; i++) {
            p[256 + i] = p[i] = permutation[i % 256];
        }
    }

    double noise(double x, double y) {
        int X = int(floor(x)) & 255;
        int Y = int(floor(y)) & 255;

        x -= floor(x);
        y -= floor(y);
        
        double u = fade(x);
        double v = fade(y);
        
        int A = p[X] + Y;
        int B = p[X + 1] + Y;

        return lerp(v, lerp(u, grad(p[A], x, y), grad(p[B], x - 1, y)),
                        lerp(u, grad(p[A + 1], x, y - 1), grad(p[B + 1], x - 1, y - 1)));
    }

private:
    std::vector<int> p = std::vector<int>(512);
    std::vector<int> permutation;

    double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    double grad(int hash, double x, double y) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
        return (h & 8 ? -1 : 1) * (u + v);
    }
};

class Maze {
public:
    Maze(int width, int height) : width(width), height(height) {
        maze.resize(height, vector<char>(width, '#'));
    }

    void generate(PerlinNoise& perlin) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double nx = x / double(width), ny = y / double(height);
                double value = perlin.noise(nx * 10, ny * 10);  // 缩放噪声以调整细节
                if (value < THRESHOLD) {
                    maze[y][x] = '.';  // 路径
                }
            }
        }
    }

    void ensurePath() {
        // 起点是左下角(1, height-2)
        int startX = 1, startY = height - 2;
        // 终点是右上角(width-2, 1)
        int endX = width - 2, endY = 1;

        // 用广度优先搜索生成从起点到终点的路径
        vector<vector<bool>> visited(height, vector<bool>(width, false));
        queue<pair<int, int>> q;

        // 从(1, height-2)开始，确保是路径
        maze[startY][startX] = '.';
        visited[startY][startX] = true;
        q.push({startX, startY});

        // 用广度优先搜索生成通路
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        bool found = false;

        while (!q.empty() && !found) {
            // 修改此行以使用结构化绑定
            auto front = q.front();
            int x = front.first;
            int y = front.second;
            q.pop();

            // 检查四个方向
            for (auto& dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                // 如果是合法的位置并且未访问过
                if (nx >= 1 && nx < width - 1 && ny >= 1 && ny < height - 1 && !visited[ny][nx]) {
                    maze[ny][nx] = '.';
                    visited[ny][nx] = true;
                    q.push({nx, ny});

                    // 如果到达终点
                    if (nx == endX && ny == endY) {
                        found = true;
                        break;
                    }
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
    vector<vector<char>> maze;
};

int main() {
    srand(time(0));
    PerlinNoise perlin;
    Maze maze(WIDTH, HEIGHT);
    
    // 生成迷宫
    maze.generate(perlin);
    
    // 确保有一条从左下角到右上角的通路
    maze.ensurePath();
    
    // 打印迷宫
    maze.print();
    return 0;
}

