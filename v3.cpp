#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

const int WIDTH = 20;  // �Թ����
const int HEIGHT = 20; // �Թ��߶�

class Maze {
public:
    Maze(int width, int height, int seed) : width(width), height(height), rng(seed) {
        maze.resize(height, vector<char>(width, '#'));  // ��ʼ���Թ���ȫ��ǽ
    }

    void generate() {
        // ��ʼ���߽�
        for (int y = 0; y < height; ++y) {
            maze[y][0] = maze[y][width - 1] = '#';
        }
        for (int x = 0; x < width; ++x) {
            maze[0][x] = maze[height - 1][x] = '#';
        }

        // ���ѡ�������յ�
        startX = 1;
        startY = 1;
        endX = width - 2;
        endY = height - 2;

        // ���������յ�
        maze[startY][startX] = '&';  // ���
        maze[endY][endX] = '��';      // �յ�

        // ʹ�� Prim �㷨�����Թ�
        primAlgorithm();

        // ȷ���յ��ǿ��ŵ�
        maze[endY][endX] = '��';
    }

    void primAlgorithm() {
        // ��ʼ��ǽ�ڼ��ϣ�ѡ��һ���������㣬ʹ�� Prim �㷨
        vector<pair<int, int>> walls;
        walls.push_back({startX, startY});

        // �ĸ������ҡ����¡���
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        maze[startY][startX] = '.';  // ���������Ϊ·��

        // ʹ�� Prim �㷨̽���Թ�
        while (!walls.empty()) {
            shuffle(walls.begin(), walls.end(), rng);  // �����ǽ�ڵ�ѡ��˳��

            // ��ǽ�ڼ�����ѡ��һ�������ǽ
            pair<int, int> wall = walls.back();
            walls.pop_back();

            int x = wall.first;
            int y = wall.second;

            // �����ĸ����򣬼����Χ��ǽ�Ƿ���Գ�Ϊ·��
            for (auto& dir : directions) {
                int nx = x + dir.first * 2;
                int ny = y + dir.second * 2;

                // �����λ�����Թ�����δ�����ʹ���������·��
                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                    maze[ny][nx] = '.';  // ���µ�λ������Ϊ·��
                    maze[y + dir.second][x + dir.first] = '.';  // �ڵ�ǰλ�ú���λ��֮���ͨǽ��

                    // ����ǽ����ӵ�ǽ�ڼ���
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
    mt19937 rng;  // ʹ��һ�����������������������
};

int main() {
    // ��ȡ�û����������
    int seed;
    cout << "������һ����������ӣ�";
    cin >> seed;

    // �����Թ���ʹ���û����������
    Maze maze(WIDTH, HEIGHT, seed);
    
    // �����Թ�
    maze.generate();
    
    // ��ӡ�Թ�
    maze.print();
    return 0;
}

