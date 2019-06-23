#ifndef MINEMAP_H
#define MINEMAP_H

#include "block.h"
#include <QObject>

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
	int num; // number of mines

public:
	Map(int, int, int);
	~Map();
};

class MineField : public QObject, public Map
{
Q_OBJECT

private:
	/**
	 * \brief 1D proxy class for map, only used for overload []operator
	 */
	class MineField1D
	{
	private:
		Block* start_block; // pointer to first block of the row
		const int c; // column number
	public:
		MineField1D(Block*, int);
		Block& operator[](int);
	};

private slots:
	void LayMine(int, int);
	void OpenAdjacentBlocks(int, int);

public:
	MineField(difficulty = easy);
	MineField(int, int, int);
	void ConnectSignals();
	int CountAdjacentMine(int, int);
	MineField1D operator[](int);
	friend class MapPainter;
};

#endif // MINEMAP_H
