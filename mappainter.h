#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include "minefield.h"
#include <QButtonGroup>
#include <QGridLayout>
#include <QWidget>

class MapPainter : public QWidget
{
Q_OBJECT

private:
	MineField* field;
	QGridLayout* layout;

private slots:
	void UpdateLayout(int, int);

public:
	explicit MapPainter(QWidget* parent = nullptr);
	~MapPainter();
};

#endif // MAPPAINTER_H
