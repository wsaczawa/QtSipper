#pragma once

#include <QDialog>
#include "ui_DServer.h"

class DServer : public QDialog, public Ui::DServer
{
	Q_OBJECT

public:
	DServer(QWidget *parent = Q_NULLPTR);
	~DServer();

private slots:
	void on_OKButton_accepted();
};
