#include "ParkingLotManagementSystem.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QDateTime>
#include <QMessageBox>

ParkingLotManagementSystem::ParkingLotManagementSystem(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(1000, 900);
    initUI();
    ui.setupUi(this);
}

ParkingLotManagementSystem::~ParkingLotManagementSystem()
{}

void ParkingLotManagementSystem::initUI()
{
    QIntValidator* IntValidator = new QIntValidator();
    IntValidator->setRange(1,10); //设置范围
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setGeometry((this->width() - 300) / 2,(this->height() - 20) / 2,300,20);
    m_lineEdit->setPlaceholderText(u8"请输入开放多少车位! 最大数量为10, 输入回车确定");
    m_lineEdit->setClearButtonEnabled(true);
    m_lineEdit->setValidator(IntValidator);
    connect(m_lineEdit,&QLineEdit::returnPressed,[=]() {
        m_lineEdit->clearFocus();
    this->lineEditReturnPressed();
    });

    m_bg = new QWidget(this);
    m_bg->setObjectName("bg");
    m_bg->setStyleSheet("QWidget#bg{background-image:url(:/new/prefix1/img/bg.png)}");
    m_bg->setFixedSize(this->size());
    m_bg->hide();

    auto texLabel = new QLabel(m_bg);
    texLabel->setText(u8"点击停车场的车辆出站,并且打印出站信息!");
    texLabel->setGeometry(350, 0, 400, 40);

    auto * addCar = new QPushButton(m_bg);
    addCar->setText(u8"添加车辆");
    addCar->setFixedSize(130, 40);
    addCar->move(m_bg->width() - addCar->width(), addCar->height());
    connect(addCar, SIGNAL(clicked()), this, SLOT(onClickedAddCarFunc()));

    m_parkingWidget = new QWidget(m_bg);
    m_parkingWidget->setGeometry(142, 0, 162, 700);

    m_parkingQueueWidget = new QWidget(m_bg);
    m_parkingQueueWidget->setGeometry(0, 710, 1000, 190);
}

void ParkingLotManagementSystem::lineEditReturnPressed()
{
    m_Max = m_lineEdit->text().toInt();
    if(m_Max)
    {
        m_lineEdit->hide();
        m_bg->show();
    }

    m_vectorParking.resize(m_Max);
    auto * vhlay = new  QVBoxLayout;
    for(int i = 0;i < m_Max; i++)
    {
        m_vectorParking[i] = new QPushButton(m_parkingWidget);
        m_vectorParking[i]->setFixedSize(75, 54);
        m_vectorParking[i]->setText(u8"空车位");
        vhlay->addWidget(m_vectorParking[i]);
    }
    m_parkingWidget->setLayout(vhlay);

    m_vectorTempParking.resize(m_Max + 1);
    auto * hlay = new  QHBoxLayout;
    hlay->addStretch();
    for(int i = 0;i < m_Max + 1; i++)
    {
        m_vectorTempParking[i] = new QLabel(m_parkingQueueWidget);
        m_vectorTempParking[i]->setFixedSize(75,54);
        m_vectorTempParking[i]->hide();
        hlay->addWidget(m_vectorTempParking[i]);
    }
    m_parkingQueueWidget->setLayout(hlay);
}

void ParkingLotManagementSystem::setParkingWidgetUI()
{
    for(int i = 0; i < m_vectorParking.size(); i++)
    {
        m_vectorParking[i]->disconnect();
        if (i < m_vectorCar.size())
        {
            m_vectorParking[i]->setStyleSheet("QPushButton{background-image:url(:/new/prefix1/img/car.png)}");
            m_vectorParking[i]->setText(QString(u8"%1号车辆").arg(m_vectorCar[i]->carID));
            connect(m_vectorParking[i],&QPushButton::clicked,this,[=](){onClickedReleaseCarFunc(m_vectorCar[i]->carID);});
        }
        else
        {
            m_vectorParking[i]->setStyleSheet("");
            m_vectorParking[i]->setText(u8"空车位");
        }
    }

    for(int i = 0; i < m_vectorTempParking.size();i++)
    {
        m_vectorTempParking[i]->hide();
    }

    int num = 0;
    for(auto it = m_QueueCar.begin(); it != m_QueueCar.end(); it++,num++)
    {
        auto car = *it;
        m_vectorTempParking[num]->setStyleSheet("QLabel{background-image:url(:/new/prefix1/img/car.png)}");
        m_vectorTempParking[num]->setText(QString(u8"%1号车辆").arg(car->carID));
        m_vectorTempParking[num]->show();
    }
}

void ParkingLotManagementSystem::onClickedReleaseCarFunc(int it)
{
    while(!m_vectorCar.empty())
    {
        auto val = m_vectorCar[m_vectorCar.size() - 1];
        if (val->carID == it)
        {
            val->endTime = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
            QMessageBox::warning(this, u8"出站信息", QString(u8"车辆id: %1\n进站时间为:%2\n出站时间为%3\n").arg(val->carID).arg(val->startTime.c_str()).arg(val->endTime.c_str()));
            m_vectorCar.pop_back();
            delete val;
            break;
        }
        else
        {
            m_vectorTempCar.push_back(val);
            m_vectorCar.pop_back();
        }
    }
    for (int i = m_vectorTempCar.size() - 1; i >= 0;i--)
    {
        m_vectorCar.push_back(m_vectorTempCar[i]);
    }
    m_vectorTempCar.clear();

    if (!m_QueueCar.empty())
    {
        auto it = m_QueueCar.front();
        m_QueueCar.pop_front();
        it->startTime = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
        m_vectorCar.push_back(it);
    }

    setParkingWidgetUI();
}

void ParkingLotManagementSystem::onClickedAddCarFunc()
{
    if(m_vectorCar.size() < m_vectorParking.size())
    {
        auto carInfo = new CarInfo;
        carInfo->carID = m_carID;
        carInfo->startTime = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
        m_vectorCar.push_back(carInfo);
    }
    else if (m_QueueCar.size() < m_Max)
    {
        auto carInfo = new CarInfo;
        carInfo->carID = m_carID;
        m_QueueCar.push_back(carInfo);
    }
    else
    {
        QMessageBox::critical(this, u8"提示", u8"车辆已经满了哦~");
        return;
    }
    
    m_carID++;
    setParkingWidgetUI();
}
