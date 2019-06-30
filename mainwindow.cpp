#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	rank(new Ranking)
{
	ui->setupUi(this);
	connect(ui->widget, &MapPainter::Resize, this, &MainWindow::Resize);
	connect(ui->widget, &MapPainter::UpdateRanking, this, &MainWindow::NewRecord);
	connect(ui->ViewRecord, &QAction::triggered, this, &MainWindow::ViewRecord);
	connect(ui->Game, &QMenu::triggered, this, &MainWindow::SendDifficulty);
	connect(this, &MainWindow::NewDifficulty, ui->widget, &MapPainter::NewGameDifficulty);
	this->setWindowIcon(QIcon(":/minesweeper"));
	this->setWindowTitle(tr("MineSweeper"));
	Resize();
}

MainWindow::~MainWindow()
{
	delete rank;
	delete ui;
}

void MainWindow::ViewRecord()
{
	QDialog* p = new RecordBoard(*rank);
	p->setAttribute(Qt::WA_DeleteOnClose);
	p->setWindowModality(Qt::ApplicationModal);
	p->exec();
}

void MainWindow::SendDifficulty(QAction* action)
{
	if (action == ui->Easy)
		emit NewDifficulty(easy);
	else if (action == ui->Medium)
		emit NewDifficulty(medium);
	else if (action == ui->Hard)
		emit NewDifficulty(hard);
}

void MainWindow::Resize()
{
	const QSize& size = ui->widget->size();
	int h = ui->menuBar->size().height() + 2 * ui->widget->geometry().top();
	int w = 2 * ui->widget->geometry().left();
	this->setFixedSize(size.width() + w, size.height() + h);
}

void MainWindow::NewRecord()
{
	difficulty d = ui->widget->field->GetDifficulty();
	if (d == NA)
		return;
	Score temp(d, ui->widget->timer->GetStartTime(), ui->widget->timer->GetTime());
	rank->NewScore(temp);
}
