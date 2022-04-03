#pragma once

#include <QDialog>
#include "ui_DGraphs.h"

class DGraphs : public QDialog, public Ui::DGraphs
{
	Q_OBJECT

public:
	DGraphs(QWidget *parent = Q_NULLPTR);
	~DGraphs();
};
