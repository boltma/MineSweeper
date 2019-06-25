#include "block.h"
#include <QMouseEvent>

MineButton::MineButton(Block* block, QWidget* parent) : QPushButton(parent), block(block)
{
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedSize(30, 30);
}

QIcon& UnclickedButton::flag_icon()
{
	static QIcon icon(":/flag");
	return icon;
}

bool UnclickedButton::MarkAvailable = true;

UnclickedButton::UnclickedButton(Block* block) : MineButton(block)
{
	this->setStyleSheet(QString::fromUtf8("QPushButton:!hover\n"
		"{\n"
		"	border: 1px solid darkgray;\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);\n"
		"}\n"
		"QPushButton:hover\n"
		"{\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 rgb(159, 186, 214));\n"
		"	border: 1px solid lightgray;\n"
		"}"));
}

void UnclickedButton::SetStyle()
{
	this->setStyleSheet(QString::fromUtf8("QPushButton:!hover\n"
		"{\n"
		"	border: 1px solid darkgray;\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);\n"
		"}\n"
		"QPushButton:hover\n"
		"{\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 rgb(159, 186, 214));\n"
		"	border: 1px solid lightgray;\n"
		"}"));
}

void UnclickedButton::SetHover()
{
	if (block->HasMark() || block->HasQuestion())
		return;
	this->setStyleSheet(QString::fromUtf8("QPushButton\n"
		"{\n"
		"	background: rgba(171, 255, 255, 50);\n"
		"	border: 1px solid lightgray;\n"
		"}"));
}

void UnclickedButton::SwitchStatus()
{
	MarkAvailable = !MarkAvailable;
}

void UnclickedButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		block->OpenMine();
	}
	else if (event->button() == Qt::RightButton)
	{
		if (Block::Lost())
			return;
		if (block->HasMark())
		{
			// Question
			this->setText(QString(1, '?'));
			this->setIcon(QIcon());
			this->setStyleSheet(QString::fromUtf8("QPushButton\n"
				"{\n"
				"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);\n"
				"	font-weight: bold;\n"
				"	border: 1px solid darkgray;\n"
				"}\n"));
			block->QuestionMine();
		}
		else if (block->HasQuestion())
		{
			// Clear
			this->setText("");
			this->setStyleSheet(QString::fromUtf8("QPushButton:!hover\n"
				"{\n"
				"	border: 1px solid darkgray;\n"
				"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);\n"
				"}\n"
				"QPushButton:hover\n"
				"{\n"
				"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 rgb(159, 186, 214));\n"
				"	border: 1px solid lightgray;\n"
				"}"));
			block->ClearFlag();
		}
		else
		{
			// Mark
			if (!MarkAvailable)
				return;
			this->setIcon(flag_icon());
			this->setStyleSheet(QString::fromUtf8("QPushButton\n"
				"{\n"
				"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);\n"
				"	border: 1px solid darkgray;\n"
				"}\n"));
			block->MarkMine();
		}
	}
}

ClickedButton::ClickedButton(Block* block, int cnt) : MineButton(block)
{
	if (this->block->HasMine())
	{
		this->setIcon(explosion_flag ? explosion_icon() : mine_icon());
		this->setStyleSheet(QString::fromUtf8("QPushButton\n"
			"{\n"
			"	border: 1px solid darkgray;\n"
			"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #fff);\n"
			"}\n"));
		return;
	}
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

void ClickedButton::SetFlag(bool flag)
{
	explosion_flag = flag;
}

QIcon& ClickedButton::mine_icon()
{
	static QIcon icon(":/mine");
	return icon;
}

QIcon& ClickedButton::explosion_icon()
{
	static QIcon icon(":/explosion");
	return icon;
}

bool ClickedButton::explosion_flag = true;

void ClickedButton::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton && event->buttons() & Qt::RightButton)
	{
		emit DualClick();
		dual_flag = true;
	}
}

void ClickedButton::mouseReleaseEvent(QMouseEvent* event)
{
	if(dual_flag)
	{
		// Todo
		emit DualRelease();
		dual_flag = false;
	}
}

bool Block::lost_flag = false;

Block::Block() : row(0), col(0), flag(0), cnt(-1), button(new UnclickedButton(this))
{
	lost_flag = false;
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

void Block::SetAdjacentNum(int n)
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
	emit Mark();
}

void Block::QuestionMine()
{
	flag |= QUESTION;
	flag &= ~MARK;
	emit Question();
}

bool Block::Lost()
{
	return lost_flag;
}

void Block::OpenMine()
{
	if (HasMine())
	{
		// Todo: Lose
	}
	if (flag & ~MINE) // Open/Question/Mark
		return;
	flag |= OPEN;
	flag &= ~MARK;
	flag &= ~QUESTION;
	if (cnt == -1)
	{
		// Lay mine if first click
		emit FirstClick(row, col);
	}
	if (cnt == 0)
	{
		// Open adjacent blocks recursively if no adjacent mines
		emit OpenAdjacent(row, col);
	}
	button->deleteLater();
	button = new ClickedButton(this, cnt);
	connect(reinterpret_cast<ClickedButton*>(button), &ClickedButton::DualClick, this, &Block::Dual);
	connect(reinterpret_cast<ClickedButton*>(button), &ClickedButton::DualRelease, this, &Block::DualR);
	if (this->HasMine())
		emit ClickMine();
	emit Refresh(row, col); // update layout
	emit DecCnt();
}

void Block::ClearFlag()
{
	flag &= MINE | OPEN;
}

bool Block::HasMine()
{
	return flag & MINE;
}

bool Block::HasMark()
{
	return flag & MARK;
}

bool Block::HasQuestion()
{
	return flag & QUESTION;
}

void Block::Dual()
{
	emit DualClick(row, col);
}

void Block::DualR()
{
	emit DualRelease(row, col);
}

void Block::LoseGame()
{
	lost_flag = true;
}
