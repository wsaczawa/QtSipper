#include "DCharts.h"
#include <QtCharts/QScatterSeries>

DCharts::DCharts(QWidget *parent)
	: QChartView(new QChart(), parent)
{

	QScatterSeries* serie0 = new QScatterSeries();
	serie0->setName("test1");
	serie0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	serie0->setMarkerSize(5.0);

	serie0->append(0, 1);
	serie0->append(2, 5);
	serie0->append(3, 8);

	chart()->addSeries(serie0);

}
