#pragma once

#include <QDialog>
#include "ui_DReset.h"

class DReset : public QDialog, public Ui::DReset
{
	Q_OBJECT

public:
	DReset(QWidget *parent = Q_NULLPTR);
	~DReset();

private slots:
	void on_ResetOK_accepted();
};
