#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon(":/minesweeper"));
}

MainWindow::~MainWindow()
{
	delete ui;
}
