#include "ranking.h"
#include <string>
#include <QFile>

Score::Score(difficulty d, QDateTime t, int time) : d(d), start_time(std::move(t)), time(time), num(0) {}

bool Score::operator<(const Score& s) const
{
	if (time != s.time)
		return time < s.time;
	return start_time < s.start_time;
}

ifstream& operator>>(ifstream& in, Score& s)
{
	int d;
	string temp1, temp2; // DateTime has two parts
	in >> d >> temp1 >> temp2 >> s.time >> s.num;
	s.d = static_cast<difficulty>(d);
	string temp = temp1 + ' ' + temp2;
	s.start_time = QDateTime::fromString(QString(temp.c_str()), Qt::SystemLocaleShortDate);
	return in;
}

ofstream& operator<<(ofstream& out, const Score& s)
{
	QString temp = QString(std::to_string(s.d).c_str()) + ' ' + s.start_time.toString(Qt::SystemLocaleShortDate) + ' '
		+ QString(std::to_string(s.time).c_str()) + ' ' + QString(std::to_string(s.num).c_str());
	temp.append(QString(35 - temp.size(), ' ')); // every line has equal length
	temp += '\n';
	out << temp.toStdString();
	return out;
}

Ranking::Ranking()
{
	QFile file("record.dat");
	if (!file.exists())
		file.open(QIODevice::WriteOnly); // create file if doesn't exist
	if (file.isOpen())
		file.close();
	in.open("record.dat");
	while (in.good())
	{
		Score temp;
		in >> temp;
		if (in.eof())
			break;
		list[temp.d].insert(temp);
	}
	in.close();
	out.open("record.dat", ios_base::out | ios_base::in); // no cleaning file
}

Ranking::~Ranking()
{
	try
	{
		out.close();
	}
	catch (std::exception&)
	{
	}
}

bool Ranking::isEmpty(difficulty d) const
{
	return list[d].empty();
}

const Score& Ranking::GetBestScore(difficulty d) const
{
	return *list[d].begin();
}

void Ranking::NewScore(Score& s)
{
	const int list_length = 10;
	if (list[s.d].size() < list_length || s < *list[s.d].rbegin())
	{
		int n = 0;
		if (!list[s.d].empty())
			n = list[s.d].rbegin()->num; // num of largest time
		if (list[s.d].size() > list_length)
		{
			list[s.d].erase(--list[s.d].end());
			out.seekp(n, ios_base::beg);
			s.num = n;
		}
		else
		{
			out.seekp(0, ios_base::end);
			s.num = out.tellp();
		}
		list[s.d].insert(s);
		out << s;
		out.flush();
	}
}

RecordBoard::RecordBoard(const Ranking& r) : text(new QLabel(this))
{
	// Todo
	for (const auto& s : r.list)
	{
		for (const auto& i : s)
		{
			QString temp(std::to_string(i.time / 1000.0).c_str());
			temp += ' ';
			temp += i.start_time.toString(Qt::SystemLocaleDate);
			temp += '\n';
			record += temp;
		}
		record += '\n';
	}
	text->setText(record);
	text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->setWindowTitle("Records");
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

RecordBoard::~RecordBoard()
{
	delete text;
}
