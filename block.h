#ifndef BLOCK_H
#define BLOCK_H

#include <QPushButton>

enum BLOCK_STATUS
{
	MINE = 0x1,
	MARK = 0x2,
	OPEN = 0x4,
	QUESTION = 0x8
};

class MineButton : public QPushButton
{
Q_OBJECT

protected:
	class Block* const block;
	QIcon* icon;

protected:
	void mousePressEvent(QMouseEvent* event) override = 0;

public:
	explicit MineButton(Block*, QWidget* parent = nullptr);
	~MineButton();
};

class UnclickedButton : public MineButton
{
protected:
	void mousePressEvent(QMouseEvent* event) override;

public:
	UnclickedButton(Block*);
};

class ClickedButton : public MineButton
{
protected:
	void mousePressEvent(QMouseEvent* event) override;

public:
	ClickedButton(Block*, int);
};

// Block Info
class Block : public QObject
{
Q_OBJECT

private:
	int row;
	int col;
	int flag;
	int cnt;	// number of Adjacent Mines
	MineButton* button;

public:
	Block();
	~Block();
	void SetLocation(int, int);
	void SetAdjacentNum(int);
	void PlaceMine();
	void MarkMine();
	void QuestionMine();
	void OpenMine();
	bool HasMine();
	bool HasMark();
	friend class MapPainter;

signals:
	void FirstClick(int, int);
	void OpenNoAdjacent(int, int);
	void Refresh(int, int);	// refresh graphics
};

#endif // BLOCK_H
