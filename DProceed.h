#pragma once

#include <QDialog>
#include "ui_DProceed.h"
#include "Sipper.h"

#include <string>

class DProceed : public QDialog, public Ui::DProceed
{
	Q_OBJECT

public:
	DProceed(QWidget *parent = Q_NULLPTR);
	~DProceed();
	void setData();
	dataset currentSample;
	bool check{ true };

private slots:
	void on_ProceedOK_accepted();
};
