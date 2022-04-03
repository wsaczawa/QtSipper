#pragma once

#include <QDialog>
#include <string>
#include "ui_DOverwrite.h"
#include "Sipper.h"

class DOverwrite : public QDialog, public Ui::DOverwrite
{
	Q_OBJECT

public:
	DOverwrite(QWidget *parent = Q_NULLPTR);
	~DOverwrite();
	std::string name;
	double time;
	double volume;
	std::string detector;
	dataset sample;

private slots:
	void on_overOK_accepted();
};
