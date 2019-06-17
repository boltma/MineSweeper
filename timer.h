#ifndef TIMER_H
#define TIMER_H

#include <QLCDNumber>
#include <QTimer>

// ��ʱ��
class Timer : public QLCDNumber
{
	Q_OBJECT
private:
	int display_time;
	QTimer* t;

private slots:
	void inc_time();

public:
	explicit Timer(QWidget* parent = nullptr);
	~Timer();
	int get_time();
};

#endif // TIMER_H