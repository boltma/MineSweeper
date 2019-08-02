#ifndef CUSTOMMENU_H
#define CUSTOMMENU_H

#include <QDialog>
#include <QValidator>

namespace Ui {
class CustomMenu;
}

class CustomMenu : public QDialog
{
    Q_OBJECT

public:
    explicit CustomMenu(QWidget *parent = nullptr);
    ~CustomMenu();

protected:
	void done(int) override; // override to validate data

private:
    Ui::CustomMenu *ui;
	QIntValidator* row_validator;
	QIntValidator* col_validator;
	QIntValidator* num_validator;

signals:
	void SendSize(int, int, int);
};

#endif // CUSTOMMENU_H
