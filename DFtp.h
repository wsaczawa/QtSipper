#pragma once

#include <QDialog>
#include "ui_DFtp.h"

class DFtp : public QDialog, public Ui::DFtp
{
	Q_OBJECT

public:
	DFtp(QWidget *parent = Q_NULLPTR);
	~DFtp();

private slots:
	void on_OKButton_accepted();
};
