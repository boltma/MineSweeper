#include "mappainter.h"

MapPainter::MapPainter(QWidget* parent) : QWidget(parent), field(new MineField(hard)), layout(new QGridLayout(this))
{
	for (int i = 0; i < field->row; ++i)
	{
		for (int j = 0; j < field->col; ++j)
		{
			connect(&(*field)[i][j], &Block::Refresh, this, &MapPainter::UpdateLayout);
		}
	}
	layout->setSpacing(0);
	layout->setMargin(0);
	this->setLayout(layout);
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

MapPainter::~MapPainter()
{
	delete layout;
	delete field;
}

void MapPainter::UpdateLayout(int r, int c)
{
	layout->addWidget((*field)[r][c].button, r, c);
}
