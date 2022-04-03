#pragma once

#include <QWidget>
#include "ui_DSure.h"

class DSure : public QWidget, public Ui::DSure
{
	Q_OBJECT

public:
	DSure(QWidget *parent = Q_NULLPTR);
	~DSure();
};
