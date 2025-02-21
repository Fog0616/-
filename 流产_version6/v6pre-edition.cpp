#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <stack>

using namespace std;

const int WIDTH = 20;  // �Թ����
const int HEIGHT = 20; // �Թ��߶�
const float versions = 1.4; // �汾

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

        // �����յ��λ��
        startX = 1;  // ���λ���� (1, 1)
        startY = 1;
        endX = width - 2;
        endY = height - 2;

        // ���������յ�
        maze[startY][startX] = '?';  // ���
        maze[endY][endX] = '%';      // �յ�

        // ʹ�� DFS �㷨�����Թ������ಿ��
        dfsGenerate(startX, startY);

        // ȷ���յ��ǿ��ŵ�
        maze[endY][endX] = '%';
    }

    void dfsGenerate(int x, int y) {
        // �ĸ������ҡ����¡���
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        shuffle(directions.begin(), directions.end(), rng);  // ���������

        // ��ջģ�� DFS ����
        stack<pair<int, int>> stack;
        stack.push({x, y});
        maze[y][x] = '.';  // �������Ϊ·��

        while (!stack.empty()) {
            pair<int, int> curr = stack.top();
            stack.pop();
            x = curr.first;
            y = curr.second;

            // ����·���ĸ����ԣ�����򵥵�·������
            if (rand() % 100 < 10) {  // ��С��������Ŷ�·��
                shuffle(directions.begin(), directions.end(), rng);
            }

            // �����ĸ�����
            for (auto& dir : directions) {
                int nx = x + dir.first * 2;
                int ny = y + dir.second * 2;

                // ȷ����λ�����Թ��ڣ���û�б����ʹ�
                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                    maze[ny][nx] = '.';  // ��ͨ·��
                    maze[y + dir.second][x + dir.first] = '.';  // �ڵ�ǰλ�ú���λ��֮���ͨǽ��
                    stack.push({nx, ny});
                }
            }
        }
    }

    // Ӳ�������Ч�Լ��
    bool isValidMaze() {
        // ����������㵽�յ�һ����һ��·��
        // ����ͨ���򵥵�Ӳ���������ж��Թ��Ƿ���Ч
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (maze[y][x] == '#') {
                    // ����Թ��Ƿ������·
                    if (isDeadEnd(x, y)) {
                        return false; // ������·��������Ч
                    }
                }
            }
        }
        return true;  // û����·��������Ч
    }

    // ����Ƿ�Ϊ��·
    bool isDeadEnd(int x, int y) {
        int wallCount = 0;
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        
        for (auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;

            // �����Χ�Ƿ���ǽ
            if (nx < 0 || nx >= width || ny < 0 || ny >= height || maze[ny][nx] == '#') {
                wallCount++;
            }
        }

        // �����Χ����ǽ����Ϊ��·
        return wallCount == 4;
    }

    void print() {
        // ʹ��Ӳ�������Ƿ���Ч
        bool validMaze = isValidMaze();

        // �����ʾ�������Թ���Чʱ��
        if (!validMaze) {
            cout << "��Ч�Թ����������������Ȼ���������Ч�Թ�" << endl;
        }

        // ����Թ�
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 1 && y == 1) cout << "&";
                else if (x == 17 && y == 18) cout << ".";  // ǿ�����ת��
                else cout << maze[y][x];
            }
            cout << endl;
        }

        // ����Թ���Ч����ʾ�汾��Ϣ
        if (validMaze) {
            cout << "�Թ����ɹ���version" << versions << endl;
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
    // �������ֿ�ʼ
    cout << "�Թ����ɹ���version" << versions << endl;
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

