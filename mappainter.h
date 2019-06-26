#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include "minefield.h"
#include "timer.h"
#include "counter.h"
#include "gamebutton.h"
#include <QGridLayout>
#include <QWidget>

class MapPainter : public QWidget
{
Q_OBJECT

private:
	MineField* field;
	Timer* timer;
	Counter* counter;
	GameButton* button;
	QSpacerItem* spacer;
	QGridLayout* layout;
	void InitiateField();

private slots:
	void UpdateLayout(int, int);
	void RestartGame();

public:
	explicit MapPainter(QWidget* parent = nullptr);
	~MapPainter();
};

#endif // MAPPAINTER_H
