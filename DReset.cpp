#include "DReset.h"

#include <qmessagebox.h>

#include "sipper.h"

DReset::DReset(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
}

DReset::~DReset()
{
}

void DReset::on_ResetOK_accepted() {
	if (linePassword->text() == "ResetMe") {
		Sipper::Reset();
		QMessageBox* msg = new QMessageBox;
		msg->setText("Password correct. Databases have been reset.");
		msg->exec();
	}
	else {
		QMessageBox* msg = new QMessageBox;
		msg->setText("Wrong password. Reset not performed.");
		msg->exec();
	}
}
