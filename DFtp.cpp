#include "DFtp.h"
#include<fstream>
#include<string>

DFtp::DFtp(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	std::string s;

	std::ifstream ch;
	ch.open("../ftp.txt");
	std::getline(ch, s);
	ch.close();
	Check->setChecked(stoi(s));

	std::ifstream g;
	g.open("../getFtp.txt");
	while (std::getline(g, s)) {
		get->appendPlainText(QString::fromStdString(s));
	}
	g.close();

	std::ifstream p;
	p.open("../putFtp.txt");
	while (std::getline(p, s)) {
		put->appendPlainText(QString::fromStdString(s));
	}
	p.close();
}

void DFtp::on_OKButton_accepted() {
	std::stringstream s;

	std::ofstream ch;
	ch.open("../ftp.txt");
	ch << Check->isChecked();
	ch.close();

	std::ofstream g;
	g.open("../getFtp.txt");
	g << get->toPlainText().toStdString();
	g.close();

	std::ofstream p;
	p.open("../putFtp.txt");
	p << put->toPlainText().toStdString();
	p.close();
}

DFtp::~DFtp()
{
}
