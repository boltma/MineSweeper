#include "mappainter.h"
#include <stdexcept>
#include <QMessageBox>

const int spacing_height = 30;

MapPainter::MapPainter(QWidget* parent) :
	QWidget(parent),
	field(new MineField(easy)),
	timer(new Timer),
	counter(new Counter(field->num)),
	button(new GameButton),
	spacer(new QSpacerItem(0, spacing_height)),
	layout(new QGridLayout(this))
{
	layout->setSpacing(0);
	layout->setMargin(0);
	connect(counter, &Counter::Switch, &UnclickedButton::SwitchStatus);
	connect(button, &GameButton::Restart, this, &MapPainter::RestartGame);
	layout->addItem(spacer, 1, 0);
	Initialize();
}

MapPainter::~MapPainter()
{
	// No need to delete spacer because layout takes care of it
	delete layout;
	delete button;
	delete counter;
	delete timer;
	delete field; // wouldn't matter if delete nullptr
}

// Only cares about buttons, not timer or counter
void MapPainter::Initialize()
{
	// Set size
	const QSize& button_size = field->_map->button->size();
	this->setFixedSize(field->col * button_size.width(),
	                   field->row * button_size.height() + timer->size().height() + spacing_height);

	// Connect signals and add widgets to layout
	for (int i = 0; i < field->row; ++i)
	{
		for (int j = 0; j < field->col; ++j)
		{
			connect(&(*field)[i][j], &Block::Refresh, this, &MapPainter::UpdateLayout);
		}
	}
	connect(this, &MapPainter::DecCnt, field, &MineField::DecCnt);
	connect(field, &MineField::StartTimer, timer, &Timer::StartTime);
	connect(field, &MineField::Win, timer, &Timer::StopTime);
	connect(field, &MineField::Lose, timer, &Timer::StopTime);
	connect(field, &MineField::Win, button, &GameButton::Win);
	connect(field, &MineField::Lose, button, &GameButton::Lose);
	connect(field, &MineField::DecCounter, counter, &Counter::DecCount);
	connect(field, &MineField::IncCounter, counter, &Counter::IncCount);
	connect(field, &MineField::Win, this, &MapPainter::UpdateRanking);
	connect(timer, &Timer::TimeOut, field, &MineField::Lose);
	connect(timer, &Timer::TimeOut, this, &MapPainter::TimeOut);
	for (int r = 0; r < field->row; ++r)
	{
		for (int c = 0; c < field->col; ++c)
		{
			layout->addWidget((*field)[r][c].button, r + 2, c);
		}
	}
	layout->addWidget(button, 0, 3, 1, field->col - 6, Qt::AlignCenter);
	layout->addWidget(timer, 0, 0, 1, 3);
	layout->addWidget(counter, 0, field->col - 3, 1, 3);
	button->RefreshButton();
}

void MapPainter::UpdateLayout(int r, int c)
{
	layout->addWidget((*field)[r][c].button, r + 2, c);
	(*field)[r][c].button->show();
	emit DecCnt();
}

void MapPainter::RestartGame()
{
	NewGame(field->row, field->col, field->num); // same size as before
}

void MapPainter::NewGameDifficulty(difficulty d)
{
	layout->removeWidget(timer);
	layout->removeWidget(button);
	layout->removeWidget(counter);
	delete field;
	field = new MineField(d);
	timer->Reset();
	counter->SetCnt(field->num);
	Initialize();
	emit Resize();
}

void MapPainter::NewGameDifficulty(int r, int c, int n)
{
	layout->removeWidget(timer);
	layout->removeWidget(button);
	layout->removeWidget(counter);
	delete field;
	try
	{
		field = new MineField(r, c, n);
	}
	catch (std::invalid_argument&)
	{
		auto m = new QMessageBox(this);
		m->setIcon(QMessageBox::Critical);
		m->setText("Invalid input!");
		m->setAttribute(Qt::WA_DeleteOnClose);
		m->show();
	}
	timer->Reset();
	counter->SetCnt(field->num);
	Initialize();
	emit Resize();
}

void MapPainter::NewGame(int r, int c, int n)
{
	// no need to remove buttons because destructor of button takes care
	layout->removeWidget(timer);
	layout->removeWidget(button);
	layout->removeWidget(counter);
	delete field;
	field = new MineField(r, c, n);
	timer->Reset();
	counter->SetCnt(field->num);
	Initialize();
	emit Resize();
}

void MapPainter::TimeOut()
{
	field->OpenAll();
}
