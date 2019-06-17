#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include "block.h"
#include "minefield.h"
#include <QPushButton>
#include <QWidget>

class MineButton : public QPushButton
{
	Q_OBJECT
public:
	explicit MineButton(QWidget* parent = nullptr);
protected:
	virtual void mousePressEvent(QMouseEvent* event) = 0;
};

class MapPainter : public QWidget
{
	Q_OBJECT
public:
	explicit MapPainter(QWidget* parent = nullptr);

signals:

public slots:
};

#endif // MAPPAINTER_H