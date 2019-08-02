#ifndef MINEMAP_H
#define MINEMAP_H

#include "block.h"
#include <QObject>

enum difficulty
{
	easy,
	medium,
	hard,
	NA // null difficulty
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
		Block& operator[](int) const;
	};

	int cnt; // #blocks - #mines - #clicked
	void MarkAll() const; // Mark all unclicked buttons after winning
	void OpenAll() const; // Open all unclicked buttons after losing

private slots:
	void LayMine(int, int);
	void OpenAdjacentBlocks(int, int) const;
	void DualOpen(int, int) const;
	void DualRestore(int, int) const;
	void DecCnt(); // decrement cnt, emit win when cnt decreased to zero
	void ClickMine();

public:
	explicit MineField(difficulty = easy);
	MineField(int, int, int);
	void Initialize() const;
	int CountAdjacentMine(int, int) const;
	int CountAdjacentMark(int, int) const;
	difficulty GetDifficulty() const;
	MineField1D operator[](int) const;

signals:
	void StartTimer();
	void DecCounter(); // Mine marked, counter decrement
	void IncCounter(); // Block questioned, counter increment
	void Win();
	void Lose();

	friend class MapPainter;
};

#endif // MINEMAP_H
