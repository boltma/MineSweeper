#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QPushButton>

class GameButton : public QPushButton
{
Q_OBJECT

private:
	static QIcon& button_icon();
	static QIcon& win_icon();
	static QIcon& lose_icon();

protected:
	void mousePressEvent(QMouseEvent* event) override;

public:
	GameButton();
	void RefreshButton();

public slots:
	void Win();
	void Lose();

signals:
	void Restart();
};

#endif // GAMEBUTTON_H
