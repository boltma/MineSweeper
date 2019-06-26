#include "timer.h"

Timer::Timer(QWidget* parent) : QLCDNumber(parent), display_time(0), t(new QTimer)
{
	this->setFixedSize(90, 40);
	connect(t, &QTimer::timeout, this, &Timer::IncTime);
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

void Timer::Reset()
{
	t->stop();
	display_time = 0;
	this->display(display_time);
}

void Timer::StartTime()
{
	t->start(1000);
}

void Timer::StopTime()
{
	// Timer would stop either win or lose, because blocks will always all open
	t->stop();
}
