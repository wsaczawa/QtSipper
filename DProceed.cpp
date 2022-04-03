#include "DProceed.h"
#include "DResults.h"

DProceed::DProceed(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
}

DProceed::~DProceed()
{
}

void DProceed::setData() {
	LabName->setText(QString::fromStdString(currentSample.name));
	LabTime->setText(QString::number(currentSample.time));
	LabVol->setText(QString::number(currentSample.volume));
	LabCheck->setVisible(check);

	QListWidgetItem* newItem = new QListWidgetItem;
	newItem->setText(QString::fromStdString("Nuclides \t Activities [MBq/m3]"));
	dataList->insertItem(0, newItem);
	for (int i = 0; i < currentSample.nuclides.size(); i++) {
		QListWidgetItem* newItem = new QListWidgetItem;
		newItem->setText(QString::fromStdString(currentSample.nuclides[i].name + "\t" + std::to_string(currentSample.nuclides[i].raw)));
		dataList->insertItem(i+1, newItem);
	}


}

void DProceed::on_ProceedOK_accepted() {
	if (Sipper::Check(currentSample.name)) {
		Sipper::SippOverwrite(currentSample);
	}
	else {
		Sipper::SippNormal(currentSample);
	}
	DResults dialog(this);
	dialog.exec();
}
