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
public:
	explicit MineButton(QWidget* parent = nullptr);
protected:
	virtual void mousePressEvent(QMouseEvent* event) override = 0;
};

class UnclickedButton : public MineButton
{
public:
	UnclickedButton() {}
	virtual void mousePressEvent(QMouseEvent* event) override;
};

class ClickedButton : public MineButton
{
public:
	ClickedButton() {}
	virtual void mousePressEvent(QMouseEvent* event) override;
};

// 方格信息
class Block
{
private:
	int flag;
	MineButton* button;

public:
	Block();
	~Block();
	void PlaceMine();
	void MarkMine();
	void QuestionMine();
	void OpenMine();
	bool HasMine();
	friend class MapPainter;
};

#endif // BLOCK_H