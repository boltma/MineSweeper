#include "custommenu.h"
#include "ui_custommenu.h"
#include <QValidator>
#include <QMessageBox>

CustomMenu::CustomMenu(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::CustomMenu),
	row_validator(new QIntValidator(9, 16, this)),
	col_validator(new QIntValidator(9, 30, this)),
	num_validator(new QIntValidator(10, 479, this))
{
	ui->setupUi(this);
	ui->row->setValidator(row_validator);
	ui->column->setValidator(col_validator);
	ui->number->setValidator(num_validator);
}

CustomMenu::~CustomMenu()
{
	delete num_validator;
	delete col_validator;
	delete row_validator;
	delete ui;
}

void CustomMenu::done(int r)
{
	if (Accepted == r)
	{
		int row = ui->row->text().toInt();
		int col = ui->column->text().toInt();
		int num = ui->number->text().toInt();
		if (row >= 9 && row <= 16 && col >= 9 && col <= 30 && num >= 10 && num < row * col)
		{
			QDialog::done(r);
			emit SendSize(row, col, num);
			return;
		}
		auto m = new QMessageBox(this);
		m->setIcon(QMessageBox::Critical);
		m->setText("Invalid input!");
		m->setAttribute(Qt::WA_DeleteOnClose);
		m->show();
		return;
	}
	QDialog::done(r);
}
