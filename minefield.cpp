#include "minefield.h"
#include <ctime>
#include <random>
#include <stdexcept>
using namespace std;

Map::Map(int r, int c, int n) : row(r), col(c), num(n)
{
	_map = new Block[row * col];
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			_map[row * i + j].SetLocation(i, j);
		}
	}
}

Map::~Map()
{
	delete[] _map;
}

MineField::MineField1D::MineField1D(Block* s, int c) : start_block(s), c(c)
{
}

Block& MineField::MineField1D::operator[](int c)
{
	if (c >= 0 && c < this->c)
		return start_block[c];
	throw std::invalid_argument("c");
}

int _size[][3] = {{9, 9, 10}, {16, 16, 40}, {16, 30, 99}};

MineField::MineField(difficulty d) : Map(_size[d][0], _size[d][1], _size[d][2])
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			connect(&(*this)[i][j], &Block::FirstClick, this, &MineField::LayMine);
		}
	}
}

MineField::MineField(int r, int c, int n) : Map(r, c, n)
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			connect(&(*this)[i][j], &Block::FirstClick, this, &MineField::LayMine);
		}
	}
}

/**
 * \brief Lay mines according to first move, and set all adjacent numbers
 * \param x Row number of first click
 * \param y Column number of first click
 */
void MineField::LayMine(int x, int y)
{
	random_device rd;
	mt19937 e(rd() + time(nullptr));
	uniform_int_distribution<int> row_rand(0, row - 1);
	uniform_int_distribution<int> col_rand(0, col - 1);
	int cnt = 0;
	do
	{
		int rand_x = row_rand(e);
		int rand_y = col_rand(e);
		if (rand_x == x && rand_y == y)
			continue; // continue if random location is first click location
		if ((*this)[rand_x][rand_y].HasMine())
			continue; // continue if already has mine
		(*this)[rand_x][rand_y].PlaceMine();
		++cnt;
	}
	while (cnt < num);
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			(*this)[i][j].SetAdjacentMine(CountAdjacentMine(i, j));
		}
	}
}

/**
 * \brief Count adjacent mines
 * \param r Row number
 * \param c Column number
 * \return Number of adjacent mines
 */
int MineField::CountAdjacentMine(int r, int c)
{
	int x[] = {r - 1, r, r + 1};
	int y[] = {c - 1, c, c + 1};
	int cnt = 0;
	for (auto i : x)
	{
		for (auto j : y)
		{
			if (i == r && j == c)
				continue;
			try
			{
				if ((*this)[i][j].HasMine())
					++cnt;
			}
			catch (std::invalid_argument& e)
			{
				// jump through invalid row numbers (no need to jump column numbers)
				if (strcmp(e.what(), "r") == 0)
				{
					break;
				}
			}
		}
	}
	return cnt;
}

MineField::MineField1D MineField::operator[](int r)
{
	if (r >= 0 && r < row)
		return {_map + r * col, col};
	throw std::invalid_argument("r");
}
