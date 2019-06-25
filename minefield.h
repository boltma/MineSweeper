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
	const int row;
	const int col;
	const int num; // number of mines

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
		Block* const start_block; // pointer to first block of the row
		const int c; // column number
	public:
		MineField1D(Block*, int);
		Block& operator[](int);
	};

	int cnt; // #blocks - #mines - #clicked

private slots:
	void LayMine(int, int);
	void OpenAdjacentBlocks(int, int);
	void DualOpen(int, int);
	void DualRestore(int, int);
	void DecCnt(); // decrement cnt, emit win when cnt decreased to zero
	void ClickMine();

public:
	explicit MineField(difficulty = easy);
	MineField(int, int, int);
	void ConnectSignals();
	int CountAdjacentMine(int, int);
	int CountAdjacentMark(int, int);
	MineField1D operator[](int);

signals:
	void StartTimer();
	void DecCounter(); // Mine marked, counter decrement
	void IncCounter(); // Block questioned, counter increment
	void Win();
	void Lose();

	friend class MapPainter;
};

#endif // MINEMAP_H
