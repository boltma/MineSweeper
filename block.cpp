#include "block.h"
#include <QMouseEvent>

MineButton::MineButton(Block* block, QWidget* parent) : QPushButton(parent), block(block)
{
	this->setFixedSize(30, 30);
}

UnclickedButton::UnclickedButton(Block* block) : MineButton(block)
{
	this->setStyleSheet(QString::fromUtf8("QPushButton:!hover\n"
		"{\n"
		"	border: 1px solid darkgray;\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);\n"
		"}\n"
		"QPushButton:hover\n"
		"{\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 rgb(159, 186, 214));"
		"	border: 1px solid lightgray;\n"
		"}"));
}

void UnclickedButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		block->OpenMine();
	}
	else if (event->button() == Qt::RightButton)
	{
	}
}

ClickedButton::ClickedButton(Block* block, int cnt) : MineButton(block), icon(new QIcon)
{
	// Todo: add support for showing adjacent blocks while click with both buttons
	if (this->block->HasMine())
	{
		this->setStyleSheet(QString::fromUtf8("QPushButton:!hover\n"
			"{\n"
			"	border: 1px solid darkgray;\n"
			"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #fff);\n"
			"}\n"
			"QPushButton:hover\n"
			"{\n"
			"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #fff);"
			"	border: 1px solid lightgray;\n"
			"}"));
		return;
	}
	this->setCheckable(false);
	if (cnt)
	{
		this->setText(QString(1, '0' + cnt));
	}
	// Different colors for different numbers
	QString color[] = {"gray", "blue", "green", "red", "purple", "black", "maroon", "gray", "turquoise"};
	this->setStyleSheet(QString::fromUtf8("QPushButton:!hover, QPushButton:hover\n"
			"{\n"
			"	color: ") + color[cnt] +
		QString::fromUtf8(";\n"
			"	font-weight: bold;\n"
			"	border: 1px solid lightgray;\n"
			"}\n"));
}

ClickedButton::~ClickedButton()
{
	delete icon;
}

void ClickedButton::mousePressEvent(QMouseEvent* event)
{
}

Block::Block() : row(0), col(0), flag(0), cnt(-1), button(new UnclickedButton(this))
{
}

Block::~Block()
{
	delete button;
}

void Block::SetLocation(int r, int c)
{
	row = r;
	col = c;
}

void Block::SetAdjacentMine(int n)
{
	cnt = n;
}

void Block::PlaceMine()
{
	flag |= MINE;
}

void Block::MarkMine()
{
	flag |= MARK;
}

void Block::QuestionMine()
{
	flag |= QUESTION;
	flag &= ~MARK;
}

void Block::OpenMine()
{
	flag |= OPEN;
	flag &= ~MARK;
	flag &= ~QUESTION;
	if (cnt == -1)
	{
		// Lay mine if first click
		emit FirstClick(row, col);
	}
	delete button;
	button = new ClickedButton(this, cnt);
	emit Refresh(); // update layout
}

bool Block::HasMine()
{
	return flag & MINE;
}
