#include "minefield.h"
#include <ctime>
#include <random>
#include <stdexcept>
using namespace std;

Map::Map(int r, int c, int n) : row(r), col(c), num(n)
{
	if (r <= 0 || c <= 0 || n <= 0) // Todo: invalid argument exception
		throw std::invalid_argument("Invalid argument!");
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

MineField::MineField1D::MineField1D(Block* s, int c) : start_block(s), c(c) {}

Block& MineField::MineField1D::operator[](int c) const
{
	if (c >= 0 && c < this->c)
		return start_block[c];
	throw std::invalid_argument("c");
}

int _size[][3] = {{9, 9, 10}, {16, 16, 40}, {16, 30, 99}};

MineField::MineField(difficulty d) : Map(_size[d][0], _size[d][1], _size[d][2]), cnt(row * col - num)
{
	Initialize();
	ClickedButton::SetFlag(true);
}

MineField::MineField(int r, int c, int n) : Map(r, c, n), cnt(row * col - num)
{
	Initialize();
	ClickedButton::SetFlag(true);
}

/**
 * \brief Connect button/block signals to MineField
 */
void MineField::Initialize() const
{
	UnclickedButton::SetMarkAvailable();
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			connect(&(*this)[i][j], &Block::FirstClick, this, &MineField::LayMine);
			connect(&(*this)[i][j], &Block::OpenAdjacent, this, &MineField::OpenAdjacentBlocks);
			connect(&(*this)[i][j], &Block::DualClick, this, &MineField::DualOpen);
			connect(&(*this)[i][j], &Block::DualRelease, this, &MineField::DualRestore);
			connect(&(*this)[i][j], &Block::Mark, this, &MineField::DecCounter);
			connect(&(*this)[i][j], &Block::Question, this, &MineField::IncCounter);
			connect(&(*this)[i][j], &Block::DecCnt, this, &MineField::DecCnt);
			connect(&(*this)[i][j], &Block::ClickMine, this, &MineField::ClickMine);
			connect(this, &MineField::Lose, &Block::LoseGame);
		}
	}
}

void MineField::MarkAll() const
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			(*this)[i][j].button->SetMark();
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
	emit StartTimer();
}

/**
 * \brief Open adjacent blocks if there are no adjacent mines
 * \param r Row number of click
 * \param c Column number of click
 */
void MineField::OpenAdjacentBlocks(int r, int c) const
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

void MineField::DualOpen(int r, int c) const
{
	if (CountAdjacentMark(r, c) == CountAdjacentMine(r, c))
		OpenAdjacentBlocks(r, c);
	else
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
					(*this)[i][j].button->SetHover();
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
}

// Restore status after release
void MineField::DualRestore(int r, int c) const
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
				(*this)[i][j].button->SetStyle();
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

void MineField::DecCnt()
{
	if (Block::Lost())
		return; // No decrement if already lost
	--cnt;
	if (!cnt)
	{
		MarkAll(); // First mark all, then emit Win signal
		emit Win();
		// Todo
	}
}

void MineField::ClickMine()
{
	emit Lose(); // First emit Lose signal, then open all
	// todo
	ClickedButton::SetFlag(false);
	// All open, timer would stop
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			Block& block = (*this)[i][j];
			block.OpenMine();
			if (!block.HasMine() && (block.HasMark() || block.HasQuestion()))
				block.button->setDisabled(true);
		}
	}
}

/**
 * \brief Count adjacent mines
 * \param r Row number
 * \param c Column number
 * \return Number of adjacent mines
 */
int MineField::CountAdjacentMine(int r, int c) const
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

/**
 * \brief Count adjacent marks
 * \param r Row number
 * \param c Column number
 * \return Number of adjacent marks
 */
int MineField::CountAdjacentMark(int r, int c) const
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
				if ((*this)[i][j].HasMark())
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

difficulty MineField::GetDifficulty() const
{
	for (int d = 0; d < 3; ++d)
	{
		if (row == _size[d][0] && col == _size[d][1] && num == _size[d][2])
			return static_cast<difficulty>(d);
	}
	return NA;
}

MineField::MineField1D MineField::operator[](int r) const
{
	if (r >= 0 && r < row)
		return {_map + r * col, col};
	throw std::invalid_argument("r");
}
