#include "DGraphs.h"
#include <QtCharts>
#include <vector>
#include <fstream>
#include "Sipper.h"

DGraphs::DGraphs(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	library lib;
	lib.readIsotopes();

	std::vector<QLineSeries*> series;
	for (size_t i = 0; i < lib.isotopes.size(); i++) {
		series.push_back(new QLineSeries());
	}

	qreal ymax = 0;
	qreal ymin = std::numeric_limits<double>::max();

	std::ifstream csv;
	csv.open("C:/Sipper/analyzed.csv");
	std::string line;
	std::getline(csv, line);
	for (size_t i = 0; i < series.size(); i++) {
		series[i]->setName(QString::fromStdString(getcell(line, 6+i*3)));
	}

	while (std::getline(csv, line)) {
		for (size_t i = 0; i < series.size(); i++) {
			series[i]->append((qreal) stoi(getcell(line, 2)), (qreal) stod(getcell(line, 6+i*3)));
			ymax = std::max(ymax, (qreal)stod(getcell(line, 6 + i * 3)));
			ymin = std::min(ymin, (qreal)stod(getcell(line, 6 + i * 3)));
		}
	}

	csv.close();

	for (size_t i = 0; i < series.size(); i++) {
		QVector<QPointF> points = series[i]->pointsVector();
		std::sort(points.begin(), points.end(), [](const QPointF& p1, const QPointF& p2) {
			return p1.x() > p2.x();
			});
		series[i]->replace(points);
		FieldGraph->chart()->addSeries(series[i]);
	}

	QValueAxis* axisX = new QValueAxis();
	QValueAxis* axisY = new QValueAxis();

	FieldGraph->chart()->addAxis(axisX, Qt::AlignBottom);
	FieldGraph->chart()->addAxis(axisY, Qt::AlignLeft);

	for (size_t i = 0; i < series.size(); i++) {
		series[i]->attachAxis(axisX);
		series[i]->attachAxis(axisY);
	}

	axisX->setTickType(QValueAxis::TicksDynamic);
	axisY->setTickType(QValueAxis::TicksFixed);

	axisY->setRange(ymin, ymax);
	axisY->setTickCount(10);

	axisX->setTickAnchor(0);
	axisX->setTickInterval(5);
}

DGraphs::~DGraphs()
{
}
