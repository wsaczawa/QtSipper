#include "DResults.h"
#include "DGraphs.h"
#include "Sipper.h"

DResults::DResults(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	Sipper::resultList list = Sipper::getList();
	if (list.yellow.size() > 0) {
		QPalette yellowPallete;
		yellowPallete.setColor(QPalette::Window, Qt::transparent);
		yellowPallete.setColor(QPalette::WindowText, Qt::darkYellow);
		summary->setAutoFillBackground(true);
		summary->setPalette(yellowPallete);
		summary->setText("There ARE suspisios samples");
		for (auto i = 0; i < list.yellow.size(); i++) {
			QListWidgetItem* newItem = new QListWidgetItem;
			newItem->setText(QString::fromStdString(list.yellow[i]));
			yellowList->insertItem(i, newItem);
		}
	}
	if (list.red.size() > 0) {
		QPalette redPallete;
		redPallete.setColor(QPalette::Window, Qt::transparent);
		redPallete.setColor(QPalette::WindowText, Qt::red);
		summary->setAutoFillBackground(true);
		summary->setPalette(redPallete);
		summary->setText("There ARE suspisios samples");
		for (auto i = 0; i < list.red.size(); i++) {
			QListWidgetItem* newItem = new QListWidgetItem;
			newItem->setText(QString::fromStdString(list.red[i]));
			redList->insertItem(i, newItem);
		}
	}
}

DResults::~DResults()
{
}

void DResults::on_seeButton_clicked() {
	DGraphs dialog(this);
	dialog.exec();
}
