#include "gamebutton.h"

GameButton::GameButton()
{
	this->setFixedSize(50, 50);
}

void GameButton::mousePressEvent(QMouseEvent* event)
{
	// todo:
	emit Restart();
}

void GameButton::Win()
{
	// todo:
}

void GameButton::Lose()
{
}
