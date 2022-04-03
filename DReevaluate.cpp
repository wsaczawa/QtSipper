#include "DReevaluate.h"
#include "DResults.h"

#include "Sipper.h"

DReevaluate::DReevaluate(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
}

DReevaluate::~DReevaluate()
{
}

void DReevaluate::on_ReevOK_accepted() {
	Sipper::Reevaluate();
	DResults dialog(this);
	dialog.exec();
}