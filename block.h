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

protected:
	void mousePressEvent(QMouseEvent* event) override = 0;

public:
	explicit MineButton(Block*, QWidget* parent = nullptr);
	virtual void SetStyle() = 0;
	virtual void SetHover() = 0;
};

class UnclickedButton : public MineButton
{
Q_OBJECT

private:
	static QIcon& flag_icon();
	static bool MarkAvailable; // false if zero marks left;

protected:
	void mousePressEvent(QMouseEvent* event) override;

public:
	UnclickedButton(Block*);
	void SetStyle() override;
	void SetHover() override;

public slots:
	static void SwitchStatus();
};

class ClickedButton : public MineButton
{
Q_OBJECT

private:
	static QIcon mine_icon;
	bool dual_flag = false;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

public:
	ClickedButton(Block*, int);
	void SetStyle() override {}
	void SetHover() override {}

signals:
	void DualClick();
	void DualRelease();
};

// Block Info
class Block : public QObject
{
Q_OBJECT

private:
	int row;
	int col;
	int flag;
	int cnt; // number of Adjacent Mines

public:
	MineButton* button;
	Block();
	~Block();
	void SetLocation(int, int);
	void SetAdjacentNum(int);
	void PlaceMine();
	void MarkMine();
	void QuestionMine();
	void OpenMine();
	void ClearFlag();
	bool HasMine();
	bool HasMark();
	bool HasQuestion();

public slots:
	void Dual(); // Dual click on clicked button, open adjacent blocks
	void DualR(); // Dual release

signals:
	void FirstClick(int, int);
	void OpenAdjacent(int, int);
	void DualClick(int, int);
	void DualRelease(int, int);
	void Refresh(int, int); // refresh graphics
	void Mark(); // Mark mine, counter decrement, slot in minefield, then in counter
	void Question(); // Question block, counter increment
};

#endif // BLOCK_H
