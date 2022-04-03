#pragma once

#include <QDialog>
#include "ui_DMeasure.h"
#include "Sipper.h"

class DMeasure : public QDialog, public Ui::DMeasure
{
	Q_OBJECT

public:
	DMeasure(QWidget *parent = Q_NULLPTR);
	~DMeasure();
	QString qname;
	QString det;
	double time;
	double volume;
	void setData();
	dataset sample;

private slots:
	void on_MeasureOK_accepted();
};
