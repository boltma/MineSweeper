#include "timer.h"

Timer::Timer(QWidget* parent) : QLCDNumber(parent), display_time(0)
{
	this->setDigitCount(4); // ÏÔÊ¾4Î»
	t = new QTimer;
	t->start(1000);
	connect(t, SIGNAL(timeout()), this, SLOT(IncTime()));
}

Timer::~Timer()
{
	delete t;
}

void Timer::IncTime()
{
	/*
	 * Todo: begin counting after laying mines, end game after 1000 seconds.
	 */
	++display_time;
	this->display(display_time);
}

int Timer::GetTime()
{
	return display_time;
}
