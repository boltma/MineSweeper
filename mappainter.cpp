#include "mappainter.h"

const int spacing_height = 30;

MapPainter::MapPainter(QWidget* parent) :
	QWidget(parent),
	field(new MineField(easy)),
	timer(new Timer),
	counter(new Counter(field->num)),
	spacer(new QSpacerItem(0, spacing_height)),
	layout(new QGridLayout(this))
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
	this->setFixedSize(field->col * button_size.width(), field->row * button_size.height() + timer->size().height() + spacing_height);
	for (int r = 0; r < field->row; ++r)
	{
		for (int c = 0; c < field->col; ++c)
		{
			layout->addWidget((*field)[r][c].button, r + 2, c);
		}
	}
	layout->addWidget(timer, 0, 0, 1, 3);
	layout->addWidget(counter, 0, field->col - 3, 1, 3);
	layout->addItem(spacer, 1, 0);

	connect(counter, &Counter::Switch, &UnclickedButton::SwitchStatus);
	connect(field, &MineField::StartTimer, timer, &Timer::StartTime);
	connect(field, &MineField::Win, timer, &Timer::StopTime);
	connect(field, &MineField::Lose, timer, &Timer::StopTime);
	connect(field, &MineField::DecCounter, counter, &Counter::DecCount);
	connect(field, &MineField::IncCounter, counter, &Counter::IncCount);
}

MapPainter::~MapPainter()
{
	// No need to delete spacer because layout takes care of it
	layout->deleteLater();
	delete counter;
	delete timer;
	delete field;
}

void MapPainter::UpdateLayout(int r, int c)
{
	layout->addWidget((*field)[r][c].button, r + 2, c);
}
