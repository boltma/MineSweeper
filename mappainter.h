#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include "counter.h"
#include "gamebutton.h"
#include "minefield.h"
#include "timer.h"
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
	void Initialize();

private slots:
	void UpdateLayout(int, int);
	void RestartGame();
	void NewGameDifficulty(difficulty);
	void NewGame(int, int, int);

public:
	explicit MapPainter(QWidget* parent = nullptr);
	~MapPainter();

signals:
	void Resize();
	void UpdateRanking();

	friend class MainWindow;
	friend class Ranking;
};

#endif // MAPPAINTER_H
