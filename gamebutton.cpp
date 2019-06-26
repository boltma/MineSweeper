#include "gamebutton.h"

void GameButton::mousePressEvent(QMouseEvent* event)
{
	// todo:
	emit Restart();
}

GameButton::GameButton()
{

}

void GameButton::Win()
{
}

void GameButton::Lose()
{
}
