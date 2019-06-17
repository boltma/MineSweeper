#ifndef MINEMAP_H
#define MINEMAP_H

#include "block.h"

enum difficulty
{
	easy,
	medium,
	hard
};

// 地图
class Map
{
protected:
	Block* _map;
	int row;
	int col;
	int num; // 布雷数量

public:
	Map(int, int, int);
	~Map();
};

class MineField : public Map
{
private:
	/**
	 * \brief 一维的游戏地图代理类，仅用以重载[]运算符
	 */
	class MineField1D
	{
	private:
		Block* start_block; // 该行初始位置指针
		const int c; // 列数（即column）
	public:
		MineField1D(Block*, int);
		Block& operator[](int);
	};

	void LayMine(int, int);

public:
	MineField(difficulty = easy);
	MineField(int, int, int);
	MineField1D operator[](int);
};

#endif // MINEMAP_H
