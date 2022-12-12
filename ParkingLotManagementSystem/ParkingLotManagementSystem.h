#pragma once

#include <vector>
#include <QtWidgets/QWidget>
#include "ui_ParkingLotManagementSystem.h"

class QLabel;
class QLineEdit;
class QPushButton;

struct CarInfo
{
    int carID = 0;
    std::string startTime;
    std::string endTime;
};

class ParkingLotManagementSystem : public QWidget
{
    Q_OBJECT

public:
    ParkingLotManagementSystem(QWidget *parent = nullptr);
    ~ParkingLotManagementSystem();

    void initUI();
    void lineEditReturnPressed();
    void setParkingWidgetUI();
    void onClickedReleaseCarFunc(int it);
    
private slots:
    void onClickedAddCarFunc();

private:
    std::vector<CarInfo*> m_vectorCar;
    std::vector<CarInfo*> m_vectorTempCar;
    std::list<CarInfo*> m_QueueCar;

    std::vector<QPushButton*> m_vectorParking;
    std::vector<QLabel*> m_vectorTempParking;

    QWidget * m_bg{nullptr};
    QWidget * m_parkingWidget{nullptr};
    QWidget * m_parkingQueueWidget{nullptr};
    QLineEdit * m_lineEdit{nullptr};
    int m_Max{0};
    int m_carID{1};
    Ui::ParkingLotManagementSystemClass ui;
};
