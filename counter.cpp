#include "counter.h"

Counter::Counter(int n) : cnt(n)
{
	this->setFixedSize(90, 40);
	this->display(cnt);
}

void Counter::DecCount()
{
	this->display(--cnt);
	if(!cnt)
		emit Switch();
}

void Counter::IncCount()
{
	if (!cnt)
		emit Switch();
	this->display(++cnt);
}
