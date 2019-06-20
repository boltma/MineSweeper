#include "mappainter.h"

MapPainter::MapPainter(QWidget* parent) : QWidget(parent), field(new MineField(easy)), layout(new QGridLayout(this))
{
	UpdateLayout();
}

void MapPainter::UpdateLayout()
{
	const QSize& button_size = field->_map->button->size();
	this->setFixedSize(field->col * button_size.width(), field->row * button_size.height());
	for (int r = 0; r < field->row; ++r)
	{
		for (int c = 0; c < field->col; ++c)
		{
			layout->addWidget((*field)[r][c].button, r, c);
		}
	}
}
