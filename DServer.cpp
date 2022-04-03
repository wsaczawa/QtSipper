#include "DServer.h"
#include <fstream>
#include <chrono>
#include <thread>

DServer::DServer(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	std::ifstream file;
	file.open("../server.txt");
	while (!file.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		file.open("../server.txt");
	}
	std::string server;
	std::getline(file, server);
	file.close();
	serverName->setText(QString::fromStdString(server));
}

DServer::~DServer()
{
}

void DServer::on_OKButton_accepted() {
	std::ofstream file;
	file.open("../server.txt");
	while (!file.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		file.open("../server.txt");
	}
	QString server;
	server = serverName->text();
	file << server.toStdString();
	file.close();
}
