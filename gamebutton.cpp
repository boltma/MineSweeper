#include "gamebutton.h"

GameButton::GameButton()
{
	this->setFixedSize(50, 50);
	this->setIconSize(QSize(30, 30));
	this->setIcon(button_icon());
}

QIcon& GameButton::button_icon()
{
	static QIcon icon(":/button");
	return icon;
}

QIcon& GameButton::win_icon()
{
	static QIcon icon(":/win");
	return icon;
}

QIcon& GameButton::lose_icon()
{
	static QIcon icon(":/lose");
	return icon;
}

void GameButton::mousePressEvent(QMouseEvent* event)
{
	this->setIcon(button_icon());
	emit Restart();
}

void GameButton::RefreshButton()
{
	this->setIcon(button_icon());
}

void GameButton::Win()
{
	this->setIcon(win_icon());
}

void GameButton::Lose()
{
	this->setIcon(lose_icon());
}
