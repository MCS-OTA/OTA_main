// "v0.0.0"
#include "ICStubImpl.h"
#include "speed.h"
#include "battery.h"
#include <string>
ICStubImpl::ICStubImpl(Gear* gear, Battery * battery, QObject *parent)
    : QObject(parent), gear_(gear),battery_(battery) {

    //    CommonAPI::Runtime::get()->registerStateChangeListener([this](CommonAPI::AvailabilityState state) {
//        if (state == CommonAPI::AvailabilityState::AVAILABLE) {
//            onClientConnected();
//        } else if (state == CommonAPI::AvailabilityState::NOT_AVAILABLE) {
//            onClientDisconnected();
//        }
//    });

}
ICStubImpl::~ICStubImpl(){

}

void ICStubImpl::setGear(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _gear, setGearReply_t _reply){
    std::cout << "gear :" << _gear << "\n";
    if(_gear == "Start"){
        emit signalStart();
        _reply(0);
    }else if(_gear == "P" || _gear == "D" || _gear == "N" || _gear == "R"){
        emit signalGear(_gear);
        _reply(0);
    }else{
        _reply(-1);
    }
}
void ICStubImpl::getBattery(const std::shared_ptr<CommonAPI::ClientId> _client, getBatteryReply_t _reply) {
    int32_t batteryValue = battery_->batteryValue();
    if(batteryValue >= 0 || batteryValue <= 100){
        _reply(batteryValue, 0);
    }else{
        _reply(batteryValue, -1);
    }
}
void ICStubImpl::setMode(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _mode, setModeReply_t _reply){
    std::cout << "mode :" << _mode << "\n";
    emit signalMode(_mode);
    if(_mode == 0 || _mode == 1 || _mode == 2 || _mode == 42){
        _reply(0);
    }else{
        _reply(-1);
    }
}
void ICStubImpl::notifyBatteryStatusChanged(int32_t batteryValue){
    if(batteryValue >=0 && batteryValue <= 100){
        qDebug()<<"bat has changed to"<<batteryValue;
        fireBatteryStatusChangedEvent(batteryValue);
    }
}

void ICStubImpl::notifyGearStatusChanged(std::string gearValue){
    fireGearStatusChangedEvent(gearValue);
}
void ICStubImpl::notifyLRSignStatusChanged(int32_t signValue){
    fireLrSignStatusChangedEvent(signValue);
}
void ICStubImpl::notifyDistStatusChanged(int32_t distValue){
    firePdcStatusChangedEvent(distValue);
}
