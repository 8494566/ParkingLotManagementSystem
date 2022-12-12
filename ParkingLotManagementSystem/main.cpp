#include "ParkingLotManagementSystem.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParkingLotManagementSystem w;
    w.show();
    return a.exec();
}
