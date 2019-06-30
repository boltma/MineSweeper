#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "minefield.h"
#include "ranking.h"
#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow* ui;
	Ranking* rank;

private slots:
	void ViewRecord();
	void SendDifficulty(QAction* action);

public slots:
	void Resize();
	void NewRecord();

signals:
	void NewDifficulty(difficulty);
};

#endif // MAINWINDOW_H
