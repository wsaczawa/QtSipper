#include "QtSipper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSipper w;
    w.show();
    return a.exec();
}
