#pragma once

#include <QWidget>
#include "ui_WMeasure.h"

class WMeasure : public QWidget, public Ui::WMeasure
{
	Q_OBJECT

public:
	WMeasure(QWidget *parent = Q_NULLPTR);
	~WMeasure();
};
