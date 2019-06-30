#ifndef RANKING_H
#define RANKING_H

#include "minefield.h"
#include <QDate>
#include <QDialog>
#include <QLabel>
#include <fstream>
#include <set>
using namespace std;

struct Score
{
	difficulty d = easy;
	QDateTime start_time;
	int time = 0; // elapsed time in milliseconds
	int num = 0; // position of this data in file (number of line, make sure every line has equal length)
	Score() = default;
	Score(difficulty d, QDateTime, int);
	bool operator<(const Score&) const;
	friend ifstream& operator>>(ifstream&, Score&);
	friend ofstream& operator<<(ofstream&, const Score&);
};

ifstream& operator>>(ifstream&, Score&);
ofstream& operator<<(ofstream&, const Score&);

class Ranking
{
private:
	set<Score> list[3];
	ifstream in;
	ofstream out;

public:
	Ranking();
	~Ranking();
	bool isEmpty(difficulty) const;
	const Score& GetBestScore(difficulty) const;
	void NewScore(Score&);

	friend class RecordBoard;
};

class RecordBoard : public QDialog
{
Q_OBJECT

private:
	QLabel* text;
	QString record;

public:
	RecordBoard(const Ranking&);
	~RecordBoard();
};

#endif // RANKING_H
