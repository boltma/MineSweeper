#include "timer.h"

Timer::Timer(QWidget* parent) : QLCDNumber(parent), display_time(0), elapsed_time(0), t(new QTimer)
{
	this->setFixedSize(90, 40);
	connect(t, &QTimer::timeout, this, &Timer::IncTime);
	time = QDateTime::currentDateTime();
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

const QDateTime& Timer::GetStartTime() const
{
	return time;
}

int Timer::GetTime() const
{
	return elapsed_time;
}

void Timer::Reset()
{
	display_time = elapsed_time = 0;
	t->stop();
	this->display(display_time);
}

void Timer::StartTime()
{
	t->start(1000);
	time = QDateTime::currentDateTime();
}

void Timer::StopTime()
{
	// Timer would stop either win or lose, because blocks will always all open
	t->stop();
	if (!elapsed_time)
		elapsed_time = time.msecsTo(QDateTime::currentDateTime()); // elapsed_time only change once
}
