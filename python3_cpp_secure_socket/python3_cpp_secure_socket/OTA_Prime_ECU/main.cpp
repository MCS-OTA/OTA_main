#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/OTA_interProxy.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

using namespace v0::commonapi;

int main(){
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "OTA_ecu");
    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.OTA_service_inter";
    std::string instance_exter = "commonapi.OTA_service_exter";
    std::shared_ptr<OTA_interProxy<>> myProxy = runtime->buildProxy<OTA_interProxy>(domain, instance);
    std::cout << "Checking availability!" << std::endl;
    while (!myProxy->isAvailable()){
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        std::cout << "not available..." << std::endl;
    }
    std::cout << "Available..." << std::endl;
    CommonAPI::CallStatus status;
    int32_t result;

    std::ifstream file("./OTA_Prime_ECU/gui_Test", std::ios::binary);
    CommonAPI::ByteBuffer firmware((std::istreambuf_iterator<char>(file)), {});
    CommonAPI::ByteBuffer signature = {0x05, 0x06, 0x07, 0x08};

    std::cout << "Sending pushUpdate request..." << std::endl;

    // Firmware & signature 예시 데이터
    // std::vector<uint8_t> firmware = {0x01, 0x02, 0x03, 0x04};
    // std::vector<uint8_t> signature = {0x05, 0x06, 0x07, 0x08};
    myProxy->pushUpdateAsync(firmware, signature,
        [](const CommonAPI::CallStatus& status, const int32_t& result) {
            if (status == CommonAPI::CallStatus::SUCCESS) {
                std::cout << "[Async] pushUpdate 성공, result = " << result << std::endl;
            } else {
                std::cout << "[Async] pushUpdate 실패!" << std::endl;
            }
        });
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}