#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSipper.h"

class QtSipper : public QMainWindow
{
    Q_OBJECT

public:
    QtSipper(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtSipperClass ui;

public slots:
    void reloadDet();

private slots:
    void on_measureButton_clicked();
    void on_reevButton_clicked();
    void on_resetButton_clicked();
    void on_resultButton_clicked();
    void on_actionServer_triggered();
    void on_actionDetectors_triggered();
    void on_actionFtp_triggered();
};
