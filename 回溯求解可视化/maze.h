#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<stack>
#include<windows.h>
#define ARRIVED 2
#define WALL 1
#define SPEED 1.5
#define EMPTY 0
#define DEATH 3
using namespace std;

const int Move[4][2] = { {-1,0},{0,1},{1,0},{0,-1} };
const enum Dir { UP, RIGHT, DOWN, LEFT };

//	输出地图需要的辅助函数

void hideConsoleCursor();
void setC(int color);
void clearScreen();

class Point
{
public:
	Point(int x, int y) :x(x), y(y), di(UP) {}
	Point() {}
	bool turn(Point& other);
	bool equal(int x, int y);
	Point forward();
	bool good(vector <vector <char>>& map);
	bool victory(Point exit);
	friend class Maze;
private:
	int x;
	int y;
	Dir di;
};
class Maze
{
public:
	Maze(Point start, Point exit);
	bool readMaze(string fileName);
	void findPath();
	void showPath();
	void showMap();
private:
	stack <Point> path;
	int height;
	int width;
	Point next;
	Point start;
	Point exit;
	bool Found;
	vector <vector <char>> map;
	void arrived(Point& p);
	void reversed(Point& p);
};

Point Point::forward()
{
	Point newPoint(this->x, this->y);
	newPoint.x += Move[di][0];
	newPoint.y += Move[di][1];
	return newPoint;
}
bool Point::turn(Point& other)
{
	other = *this;
	other.di = Dir(other.di + 1);
	if (di < 4)
		return true;
	return false;
}
bool Point::equal(int x, int y)
{
	return this->x == x && this->y == y;
}
bool Point::good(vector <vector <char>>& map)
{
	return !map[x][y];
}
bool Point::victory(Point exit)
{
	return x == exit.x && y == exit.y;
}

Maze::Maze(Point start, Point exit) :start(start), exit(exit), Found(false), height(0), width(0)
{
	this->start.x += 1;
	this->start.y += 1;
	this->exit.x += 1;
	this->exit.y += 1;
}
void Maze::reversed(Point& p)
{
	map[p.x][p.y] = DEATH;

}
void Maze::findPath()
{
	hideConsoleCursor();
	path.push(start);
	arrived(start);
	while (!path.empty() && !Found)
	{
		Point now = path.top();
		path.pop();
		while (now.turn(next) && !Found)
		{
			next = now.forward();
			showMap();
			if (next.victory(exit))
			{
				arrived(next);
				path.push(next);
				Found = true;
			}
			else
			{
				if (next.good(map))
				{
					arrived(next);
					path.push(now);
					arrived(now);
					now = next;
				}
				else
					now.turn(now);
			}
			Sleep(SPEED * 100);
			clearScreen();
		}
		if (!now.turn(next))
			reversed(now);
	}
}
void Maze::showPath()
{
	if (false == Found)
	{
		cout << "未找到路径!" << endl;
		return;
	}
	cout << "路径为:" << endl;
	stack <Point> res;
	while (!path.empty())
	{
		res.push(path.top());
		path.pop();
	}
	while (!res.empty())
	{
		Point p = res.top();
		printf("(%d,%d)\n", p.x + 1, p.y + 1);
		res.pop();
	}
}
void Maze::showMap()
{
	for (int i = 0;i < height;i++)
	{
		for (int j = 0;j < width;j++)
		{
			if (next.equal(i, j) && !Found)
			{
				setC(0x12);
				cout << int(map[i][j]);
				setC(0x07);
				cout << " ";
			}
			else if (ARRIVED == map[i][j])
			{
				setC(0xca);
				cout << int(map[i][j]);
				setC(0x07);
				cout << " ";
			}
			else if (DEATH == map[i][j])
			{
				setC(0xb3);
				cout << int(map[i][j]);
				setC(0x07);
				cout << " ";
			}
			else
				cout << int(map[i][j]) << " ";
		}
		cout << endl;
	}
	cout << endl;
}
void Maze::arrived(Point& p)
{
	map[p.x][p.y] = ARRIVED;
}
bool Maze::readMaze(string fileName)
{
	ifstream in(fileName);
	if (!in.is_open())
	{
		perror("文件打开失败!\n");
		return false;
	}
	vector <vector <char>> newMap;
	vector <char> line;
	string str;
	stringstream ss;
	while (in.good())
	{
		line.clear();
		ss.clear();
		getline(in, str);
		ss = stringstream(str);
		while (ss)
		{
			int state;
			ss >> state;
			line.push_back(bool(state));
		}
		newMap.push_back(line);
	}
	in.close();
	height = int(newMap.size() + 2);
	width = int(newMap[0].size() + 2);
	vector<char> Wall(width, 1);
	map.push_back(Wall);
	for (int i = 0;i < height - 2;i++)
	{
		newMap[i].insert(newMap[i].begin(), 1);
		newMap[i].push_back(1);
		map.push_back(newMap[i]);
	}
	map.push_back(Wall);
	return true;
}
void setC(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		color);
}
void clearScreen()	// 高效清屏
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };
	SetConsoleCursorPosition(hConsole, coordScreen);
}
void hideConsoleCursor()	// 隐藏控制台光标
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}