#pragma once

#include <QDialog>
#include "ui_DDetectors.h"

class DDetectors : public QDialog, public Ui::DDetectors
{
	Q_OBJECT

public:
	DDetectors(QWidget *parent = Q_NULLPTR);
	~DDetectors();

signals:
	void reload();

private slots:
	void on_OKButton_accepted();
};
