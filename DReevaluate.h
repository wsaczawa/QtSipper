#pragma once

#include <QDialog>
#include "ui_DReevaluate.h"

class DReevaluate : public QDialog, public Ui::DReevaluate
{
	Q_OBJECT

public:
	DReevaluate(QWidget *parent = Q_NULLPTR);
	~DReevaluate();

private slots:
	void on_ReevOK_accepted();
};
