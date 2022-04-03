#include "DDetectors.h"
#include "QtSipper.h"
#include <fstream>
#include <string>

DDetectors::DDetectors(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

    std::ifstream det;
    std::string d;
    det.open("../detectors.txt");

    while (std::getline(det, d)) {
        size_t split;
        split = d.find('/');
        listDet->appendPlainText(QString::fromStdString(d.substr(0, split)));
        listAsf->appendPlainText(QString::fromStdString(d.substr(split+1, d.length())));
    }
    det.close();
}

void DDetectors::on_OKButton_accepted() {
    std::ofstream det;
    std::string d;
    std::stringstream detList;
    std::stringstream asfList;
    detList << listDet->toPlainText().toStdString();
    asfList << listAsf->toPlainText().toStdString();
    det.open("../detectors.txt");
    while (getline(detList, d)) {
        det << d << "/";
        getline(asfList, d);
        det << d << std::endl;
    }
    det.close();
}

DDetectors::~DDetectors()
{
    emit reload();
}
