#include "block.h"

MineButton::MineButton(QWidget* parent) : QPushButton(parent)
{
	this->setFixedSize(50, 50);
	this->setStyleSheet(QString::fromUtf8("QPushButton:!hover\n"
		"{\n"
		"		border: 1px solid darkgray;\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 #bbb);\n"
		"	border - radius: 1px;\n"
		"}\n"
		"QPushButton:hover\n"
		"{\n"
		"	background: qradialgradient(cx : 0.4, cy : -0.1, fx : 0.4, fy : -0.1, radius : 1.35, stop : 0 #fff, stop: 1 rgb(159, 186, 214));"
		"	border: 1px solid lightgray;\n"
		"}"));
}

void UnclickedButton::mousePressEvent(QMouseEvent* event)
{
}

void ClickedButton::mousePressEvent(QMouseEvent* event)
{
}

Block::Block() : flag(0), button(new UnclickedButton) {}

Block::~Block()
{
	delete button;
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
}

bool Block::HasMine()
{
	return flag & MINE;
}
