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
	void UpdateLayout();

public:
	explicit MapPainter(QWidget* parent = nullptr);

signals:

public slots:
};

#endif // MAPPAINTER_H