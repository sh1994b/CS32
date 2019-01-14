
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	if (sr == er && sc == ec)       //if starting an dending coordinates are the same
		return true;                //maze is solved

	maze[sr][sc] = 'a';             //starting coordinate is visited

									//call pathExists for 4 positions around the start point
	if (maze[sr][sc + 1] != 'a' && maze[sr][sc + 1] != 'X' && pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
		return true;
	if (maze[sr][sc - 1] != 'a' && maze[sr][sc - 1] != 'X' && pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
		return true;
	if (maze[sr + 1][sc] != 'a' && maze[sr + 1][sc] != 'X' && pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
		return true;
	if (maze[sr - 1][sc] != 'a' && maze[sr - 1][sc] != 'X' && pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
		return true;

	return false;
}
