#ifndef TIMER_H
#define TIMER_H

#include <QLCDNumber>
#include <QTimer>

class Timer : public QLCDNumber
{
Q_OBJECT

private:
	int display_time;
	QTimer* t;

private slots:
	void IncTime();

public:
	explicit Timer(QWidget* parent = nullptr);
	~Timer();
	int GetTime();
	void Reset();

public slots:
	void StartTime();
	void StopTime();
};

#endif // TIMER_H
