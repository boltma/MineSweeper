#ifndef COUNTER_H
#define COUNTER_H

#include <QLCDNumber>

class Counter : public QLCDNumber
{
Q_OBJECT

private:
	int cnt;

public:
    Counter(int = 0);

signals:
	void Switch(); // cnt has decreased to zero or leaves zero

public slots:
	void DecCount();
	void IncCount();
};

#endif // COUNTER_H