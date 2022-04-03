#pragma once

#include <QtCharts/QChartView>

QT_USE_NAMESPACE

class DCharts : public QChartView
{
	Q_OBJECT

public:
	explicit DCharts(QWidget *parent = Q_NULLPTR);
};