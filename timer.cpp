#include "timer.h"

Timer::Timer(QWidget* parent) : QLCDNumber(parent), display_time(0)
{
	this->setDigitCount(4); // ÏÔÊ¾4Î»
	t = new QTimer;
	t->start(1000);
	connect(t, SIGNAL(timeout()), this, SLOT(inc_time()));
}

Timer::~Timer()
{
	delete t;
}

void Timer::inc_time()
{
	++display_time;
	this->display(display_time);
}

int Timer::get_time()
{
	return display_time;
}
