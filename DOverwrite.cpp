#include "DOverwrite.h"
#include "DResults.h"
#include "WProceed.h"
#include "DProceed.h"
#include "Sipper.h"

#include <thread>
#include <time.h>

DOverwrite::DOverwrite(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
}

DOverwrite::~DOverwrite()
{
}

void DOverwrite::on_overOK_accepted() {
	sample;
	sample.input(name, time, volume, detector);
	sample.analyze();

	WProceed msg;
	msg.show();
	QCoreApplication::processEvents();
	std::this_thread::sleep_for(std::chrono::seconds(3));
	msg.close();

	DProceed dialog(this);
	dialog.currentSample = sample;
	dialog.setData();
	dialog.exec();
}
