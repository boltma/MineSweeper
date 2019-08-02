#ifndef TIMER_H
#define TIMER_H

#include <QDateTime>
#include <QLCDNumber>
#include <QTimer>

class Timer : public QLCDNumber
{
Q_OBJECT

private:
	int display_time;
	int elapsed_time;
	QDateTime time;
	QTimer* t;

private slots:
	void IncTime();

public:
	explicit Timer(QWidget* parent = nullptr);
	~Timer();
	const QDateTime& GetStartTime() const;
	int GetTime() const;
	void Reset();

public slots:
	void StartTime();
	void StopTime();

signals:
	void TimeOut();
};

#endif // TIMER_H
