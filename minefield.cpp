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
			_map[col * i + j].SetLocation(i, j);
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
	ConnectSignals();
}

MineField::MineField(int r, int c, int n) : Map(r, c, n)
{
	ConnectSignals();
}

/**
 * \brief Connect button/block signals to MineField
 */
void MineField::ConnectSignals()
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			connect(&(*this)[i][j], &Block::FirstClick, this, &MineField::LayMine);
			connect(&(*this)[i][j], &Block::OpenNoAdjacent, this, &MineField::OpenAdjacentBlocks);
		}
	}
}

/**
 * \brief Lay mines according to first move, and set all adjacent numbers
 * \param r Row number of first click
 * \param c Column number of first click
 */
void MineField::LayMine(int r, int c)
{
	random_device rd;
	mt19937 e(rd() + time(nullptr));
	uniform_int_distribution<int> row_rand(0, row - 1);
	uniform_int_distribution<int> col_rand(0, col - 1);
	int cnt = 0;
	do
	{
		int rand_r = row_rand(e);
		int rand_c = col_rand(e);
		if (rand_r == r && rand_c == c)
			continue; // continue if random location is first click location
		if ((*this)[rand_r][rand_c].HasMine())
			continue; // continue if already has mine
		(*this)[rand_r][rand_c].PlaceMine();
		++cnt;
	}
	while (cnt < num);
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			(*this)[i][j].SetAdjacentNum(CountAdjacentMine(i, j));
		}
	}
}

/**
 * \brief Open adjacent blocks if there are no adjacent mines
 * \param r Row number of click
 * \param c Column number of click
 */
void MineField::OpenAdjacentBlocks(int r, int c)
{
	int x[] = {r - 1, r, r + 1};
	int y[] = {c - 1, c, c + 1};
	for (auto i : x)
	{
		for (auto j : y)
		{
			if (i == r && j == c)
				continue;
			try
			{
				(*this)[i][j].OpenMine();
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
