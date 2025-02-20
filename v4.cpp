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

        // �����յ��λ��
        startX = 1;  // ���λ���� (1, 1)
        startY = 1;
        endX = width - 2;
        endY = height - 2;

        // ���������յ�
        maze[startY][startX] = '?';  // ���
        maze[endY][endX] = '%';      // �յ�

        // ȷ�������յ�֮���·��
        ensurePath();

        // ʹ�� DFS �㷨�����Թ������ಿ��
        dfsGenerate(startX, startY);

        // ȷ���յ��ǿ��ŵ�
        maze[endY][endX] = '%';
    }

    void ensurePath() {
        // ȷ�������յ�֮����·��
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int x = startX, y = startY;
        maze[y][x] = '.';  // �������Ϊ·��

        // ������ɴ���㵽�յ��·��
        while (x != endX || y != endY) {
            shuffle(directions.begin(), directions.end(), rng);  // ���������
            bool moved = false;

            for (auto& dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                    maze[ny][nx] = '.';  // ��ͨ·��
                    x = nx;
                    y = ny;
                    moved = true;
                    break;
                }
            }

            if (!moved) break;  // ���û���ƶ���˵����ס�ˣ��˳�
        }
    }

    void dfsGenerate(int x, int y) {
        // �ĸ������ҡ����¡���
        vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        shuffle(directions.begin(), directions.end(), rng);  // ���������

        for (auto& dir : directions) {
            int nx = x + dir.first * 2;
            int ny = y + dir.second * 2;

            // ȷ����λ�����Թ��ڣ���û�б����ʹ�
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                maze[ny][nx] = '.';  // ��ͨ·��
                maze[y + dir.second][x + dir.first] = '.';  // �ڵ�ǰλ�ú���λ��֮���ͨǽ��

                // �ݹ���������Թ�
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

