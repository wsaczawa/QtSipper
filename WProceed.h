#pragma once

#include <QWidget>
#include "ui_WProceed.h"

class WProceed : public QWidget, public Ui::WProceed
{
	Q_OBJECT

public:
	WProceed(QWidget *parent = Q_NULLPTR);
	~WProceed();
};
