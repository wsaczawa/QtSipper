#pragma once

#include <QDialog>
#include "ui_DResults.h"

class DResults : public QDialog, public Ui::DResults
{
	Q_OBJECT

public:
	DResults(QWidget *parent = Q_NULLPTR);
	~DResults();

private slots:
	void on_seeButton_clicked();
};
