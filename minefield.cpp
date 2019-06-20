#include "minefield.h"
#include <ctime>
#include <random>
#include <stdexcept>
using namespace std;

Map::Map(int r, int c, int n) : row(r), col(c), num(n)
{
	_map = new Block[row * col];
}

Map::~Map()
{
	delete[] _map;
}

MineField::MineField1D::MineField1D(Block* s, int c) : start_block(s), c(c) {}

Block& MineField::MineField1D::operator[](int c)
{
	if (c >= 0 && c < this->c)
		return start_block[c];
	else
		throw std::invalid_argument("c");
}

int _size[][3] = { {9, 9, 10}, {16, 16, 40}, {16, 30, 99} };

MineField::MineField(difficulty d) : Map(_size[d][0], _size[d][1], _size[d][2]) {}
MineField::MineField(int r, int c, int n) : Map(r, c, n) {}

/**
 * \brief 根据第一步点击的位置布雷
 * \param x 第一步点击的行号
 * \param y 第一步点击的列号
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
			continue; // 若随机处为第一步点击处，跳过
		else if ((*this)[rand_x][rand_y].HasMine())
			continue; // 若已布雷，跳过
		else
			(*this)[rand_x][rand_y].PlaceMine();
	} while (cnt < num);
}

MineField::MineField1D MineField::operator[](int r)
{
	if (r >= 0 && r < row)
		return { _map + r * col, col };
	else
		throw std::invalid_argument("r");
}
