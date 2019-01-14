#include <queue>
#include <iostream>
using namespace std;


class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise



int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X...X..X.X",
		"X..XX....X",
		"X.X.XXXX.X",
		"XXX......X",
		"X...X.XX.X",
		"X.X.X..X.X",
		"X.XXXX.X.X",
		"X..X...X.X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 4, 3, 1, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;		//declare a stack of coordinates
	coordQueue.push(Coord(sr, sc)); //Push the starting coordinate (sr,sc)

									//update maze[sr][sc] to indicate that the algorithm has encountered it: 
	maze[sr][sc] = 'a';

	while (!coordQueue.empty())
	{
		Coord topCoord = coordQueue.front();
		coordQueue.pop();
		if (topCoord.r() == er && topCoord.c() == ec)
			return true;

		//if you can move east and haven't encountered yet:
		if (maze[topCoord.r()][topCoord.c() + 1] != 'X' && maze[topCoord.r()][topCoord.c() + 1] != 'a')
		{
			coordQueue.push(Coord(topCoord.r(), topCoord.c() + 1));
			maze[topCoord.r()][topCoord.c() + 1] = 'a';
		}

		//if you can move south and haven't encountered yet:
		if (maze[topCoord.r() + 1][topCoord.c()] != 'X' && maze[topCoord.r() + 1][topCoord.c()] != 'a')
		{
			coordQueue.push(Coord(topCoord.r() + 1, topCoord.c()));
			maze[topCoord.r() + 1][topCoord.c()] = 'a';
		}

		//if you can move west and haven't encountered yet:
		if (maze[topCoord.r()][topCoord.c() - 1] != 'X' && maze[topCoord.r()][topCoord.c() - 1] != 'a')
		{
			coordQueue.push(Coord(topCoord.r(), topCoord.c() - 1));
			maze[topCoord.r()][topCoord.c() - 1] = 'a';
		}

		//if you can move north and haven't encountered yet:
		if (maze[topCoord.r() - 1][topCoord.c()] != 'X' && maze[topCoord.r() - 1][topCoord.c()] != 'a')
		{
			coordQueue.push(Coord(topCoord.r() - 1, topCoord.c()));
			maze[topCoord.r() - 1][topCoord.c()] = 'a';
		}
	}

	return false;
}