#include<iostream>
#include"maze.h"
using namespace std;

int main()
{
	
	Point start(1, 1), exit(12, 12);
	Maze maze(start, exit);
	if (!maze.readMaze("map.txt"))
		return 1;
	maze.findPath();
	maze.showMap();
	maze.showPath();
	system("pause");
	return 0;
}