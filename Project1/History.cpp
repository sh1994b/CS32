#include "History.h"
#include "City.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_row = nRows;
	m_col = nCols;

	for (int r=0; r < m_row; r++)
		for (int c=0; c < m_col; c++)
			grid[r][c]='@'; //because in ASCII '@' comes before 'A'
}

bool History::record(int r, int c)
{
	cout << "record is ran" << endl;
	if (r < 1 || r > m_row|| c < 1 || c > m_col)
		return false;
	if (grid[r-1][c-1] <= 'Z')
		grid[r-1][c-1]++;
	else 
		grid[r-1][c-1] = 'Z';
	return true;
}

void History::display() const
{
	clearScreen();
	for (int r = 0; r < m_row; r++)
	{
		for (int c = 0; c < m_col; c++)
		{
			if (grid[r][c] == '@')
				cout << '.';
			else
				cout << grid[r][c];
		}
		cout << endl;
	}
	cout << endl;
}