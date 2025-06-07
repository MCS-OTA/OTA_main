#include "carInfoController.h"
#include <QTimer>

CarInfoController::CarInfoController(QObject *parent)
    : QObject(parent), m_batteryLevel(80), m_modeColor("#A2F2D9"), m_direction("non"), qt_battery(0), m_distance(0.0f) {
    runtime = CommonAPI::Runtime::get();

    myProxy = runtime->buildProxy<ICProxy>("local","commonapi.IC_service");
    std::cout<<"Pailable"<<std::endl;


    myProxy->getProxyStatusEvent().subscribe([this](CommonAPI::AvailabilityStatus status){
        if(status == CommonAPI::AvailabilityStatus::AVAILABLE){
            std::cout<<"Proxy is Avbbbbbailable"<<std::endl;
        }else{
            std::cout<<"Proxy is NOT bbbbbbbbbAvailable"<<std::endl;
        }
    });

    myProxy->getBatteryStatusChangedEvent().subscribe([this](int32_t batValue){
        qDebug()<<"battery change" << batValue;
        this->qt_battery = batValue;
        emit batteryValueChanged();
    });
    myProxy->getPdcStatusChangedEvent().subscribe([this](float distValue){
        qDebug()<<"pdc change" << distValue;
        this->qt_dist = distValue;
        emit distValueChanged();
    });

    myProxy->getLrSignStatusChangedEvent().subscribe([this](int32_t signValue){
        if(signValue == 0){
            this->m_direction = "non";
        }else if(signValue == 1){
            this->m_direction = "emergency";
        }else if(signValue == 2){
            this->m_direction = "left";
        }else if(signValue == 3){
            this->m_direction = "right";
        }
        qDebug()<<"battery change" << this->m_direction;

        emit directionChanged();

    });

    connect(&m_timer, &QTimer::timeout, this, &CarInfoController::generateRandomDistance);
    m_timer.start(1000);
}

int CarInfoController::batteryLevel() const {
    return m_batteryLevel;
}

void CarInfoController::setBatteryLevel(int level) {
    if (m_batteryLevel != level) {
        m_batteryLevel = level;
        emit batteryLevelChanged();
    }
}

QString CarInfoController::modeColor() const {
    return m_modeColor;
}

void CarInfoController::setModeColor(const QString& color) {
    if (m_modeColor != color) {
        m_modeColor = color;
        if (m_modeColor == "#A2F2D9"){
            int_mode = 0;
            emit modeColorChanged();
        }else if(m_modeColor == "red"){
            int_mode = 1;
            emit modeColorChanged();
        }else if(m_modeColor == "white"){
            int_mode = 2;
            emit modeColorChanged();
        }
        myProxy->setModeAsync(int_mode, [this](const CommonAPI::CallStatus& callStatus, const int32_t& result){
            if(callStatus == CommonAPI::CallStatus::SUCCESS){
                std::cout<<"mode set ok"<<std::endl;
            }else{
                std::cout<<"mode set failed"<<std::endl;
            }
        });
    }
}

QString CarInfoController::blinkDirection() const {
    return m_direction;
}

void CarInfoController::setDirection(const QString& direction) {
    if (m_direction != direction) {
        m_direction = direction;
        emit directionChanged();
    }
}

float CarInfoController::distanceCM() const {
    return m_distance;
}

void CarInfoController::setDistance(float distance) {
    if (m_distance != distance) {
        m_distance = std::round(distance * 10) / 10.0f;
        emit distanceChanged();
    }
}

float CarInfoController::distanceValue() const {
    return qt_dist;
}

int CarInfoController::batteryValue() const {
    return qt_battery;
}

void CarInfoController::getBattery() {
    myProxy->getBatteryAsync([this](const CommonAPI::CallStatus &callStatus, int bat, int result){
        if (callStatus == CommonAPI::CallStatus::SUCCESS){
            qt_battery = bat;
            emit batteryValueChanged();
        }
        else{
            qDebug()<<"get battery failed";
        }
    });
}
