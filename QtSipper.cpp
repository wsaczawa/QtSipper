#include "QtSipper.h"

#include "DReevaluate.h"
#include "DMeasure.h"
#include "DReset.h"
#include "DResults.h"
#include "DServer.h"
#include "DDetectors.h"
#include "DFtp.h"
#include "Sipper.h"

#include <qmessagebox.h>

#include <fstream>

QtSipper::QtSipper(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    reloadDet();
}

void QtSipper::reloadDet() {
    ui.detectorBox->clear();
    std::ifstream det;
    std::string d;
    det.open("../detectors.txt");

    while (std::getline(det, d)) {
        ui.detectorBox->addItem(QString::fromStdString(d));
    }

    Sipper::SippNBL();
}

void QtSipper::on_reevButton_clicked() {
    ftpComm ftp;
    if (ftp.check) {
        QMessageBox* msg = new QMessageBox;
        msg->setText("This computer is connected with the server via FTP. Reevaluation can be performed only on the same machine as the database is located.");
        msg->exec();
    }
    else {
        DReevaluate dialog(this);
        dialog.exec();
    }

}

void QtSipper::on_measureButton_clicked() {
    DMeasure dialog(this);
    dialog.qname = ui.mainInput->text();
    dialog.time = ui.timeInput->value();
    dialog.volume = ui.volumeInput->value();
    dialog.det = ui.detectorBox->currentText();
    dialog.setData();
    dialog.exec();
}

void QtSipper::on_resetButton_clicked() {
    ftpComm ftp;
    if (ftp.check) {
        QMessageBox* msg = new QMessageBox;
        msg->setText("This computer is connected with the server via FTP. Reset can be performed only on the same machine as the database is located.");
        msg->exec();
    }
    else {
        DReset dialog(this);
        dialog.exec();
    }
}

void QtSipper::on_resultButton_clicked() {
    DResults dialog(this);
    dialog.exec();
}

void QtSipper::on_actionServer_triggered() {
    DServer dialog(this);
    dialog.exec();
}

void QtSipper::on_actionDetectors_triggered() {
    DDetectors dialog(this);
    dialog.exec();
    connect(&dialog, &DDetectors::reload,
        this, &QtSipper::reloadDet);
}

void QtSipper::on_actionFtp_triggered() {
    DFtp dialog(this);
    dialog.exec();
}

