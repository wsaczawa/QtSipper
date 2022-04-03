#include "DMeasure.h"
#include "WProceed.h"
#include "DProceed.h"
#include "DResults.h"
#include "DOverwrite.h"
#include "QtSipper.h"

#include <thread>
#include <time.h>

DMeasure::DMeasure(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
}

void DMeasure::setData()
{
	LabName->setText(qname);
	LabTime->setText(QString::number(time));
	LabVol->setText(QString::number(volume));
	LabDet->setText(det);
}

DMeasure::~DMeasure()
{
}

void DMeasure::on_MeasureOK_accepted() {
	std::string name = qname.toStdString();
	std::string detector = det.toStdString();
	if (Sipper::Check(name)) {
		DOverwrite dialog(this);
		dialog.name = name;
		dialog.time = time;
		dialog.volume = volume;
		dialog.detector = detector;
		dialog.exec();
	}
	else {
		WProceed msg;
		msg.show();
		QCoreApplication::processEvents();

		sample;
		sample.input(name, time, volume, detector);
		sample.analyze();

		std::this_thread::sleep_for(std::chrono::seconds(3));

		msg.close();

		DProceed dialog(this);
		dialog.currentSample = sample;
		dialog.check = false;
		dialog.setData();
		dialog.exec();
	};
}
