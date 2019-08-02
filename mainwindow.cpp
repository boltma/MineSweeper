#include "mainwindow.h"
#include "custommenu.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	rank(new Ranking)
{
	ui->setupUi(this);
	connect(ui->widget, &MapPainter::Resize, this, &MainWindow::Resize);
	connect(ui->widget, &MapPainter::UpdateRanking, this, &MainWindow::NewScore);
	connect(ui->ViewRecord, &QAction::triggered, this, &MainWindow::ViewRecord);
	connect(ui->Game, &QMenu::triggered, this, &MainWindow::SendDifficulty);
	connect(this, &MainWindow::NewDifficulty, ui->widget, qOverload<difficulty>(&MapPainter::NewGameDifficulty));
	connect(this, &MainWindow::NewCustomDifficulty, ui->widget,
	        qOverload<int, int, int>(&MapPainter::NewGameDifficulty));
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
	p->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
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
	else if (action == ui->Customize)
	{
		auto m = new CustomMenu(this);
		m->setAttribute(Qt::WA_DeleteOnClose);
		connect(m, &CustomMenu::SendSize, this, &MainWindow::NewCustomDifficulty);
		m->exec();
	}
}

void MainWindow::Resize()
{
	const QSize& size = ui->widget->size();
	int h = ui->menuBar->size().height() + 2 * ui->widget->geometry().top();
	int w = 2 * ui->widget->geometry().left();
	this->setFixedSize(size.width() + w, size.height() + h);
}

void MainWindow::NewScore()
{
	ui->widget->show(); // refresh so that marks and blocks are shown correctly before message box
	difficulty d = ui->widget->field->GetDifficulty();
	if (d == NA)
		return;
	Score temp(d, ui->widget->timer->GetStartTime(), ui->widget->timer->GetTime());
	if (rank->isEmpty(d) || temp < rank->GetBestScore(d))
	{
		auto m = new QMessageBox(this);
		m->setIcon(QMessageBox::Information);
		m->setText("New Record!");
		m->setAttribute(Qt::WA_DeleteOnClose);
		m->show(); // show instead of exec to prevent painting errors(not locking layout)
	}
	rank->NewScore(temp);
}
