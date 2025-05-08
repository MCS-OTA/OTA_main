#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/Handler_msgProxy.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

using namespace v0::commonapi;

int main(){
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "handler_tcp_client");
    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();
    int counter = 0;
    std::string domain = "local";
    std::string instance = "commonapi.Handler_msg";
    
    //std::shared_ptr<Handler_msgProxy<>> myProxy_exter = runtime->buildProxy<OTA_exterProxy>(domain, instance_exter);
    // std::cout << "Checking exter availability!" << std::endl;
    // while (!myProxy_exter->isAvailable()){
    //     std::this_thread::sleep_for(std::chrono::microseconds(1000));    std::string instance_exter = "commonapi.OTA_service_exter";

    //     if(counter % 100 == 0)
    //         std::cout << "exter not available..."<<counter << std::endl;
    //     counter++;
    //     if (counter > 10000) {
    //         std::cout << "Proxy is not available, exiting..." << std::endl;
    //         return -1;
    //     }
    // }
    counter = 0;

    std::shared_ptr<Handler_msgProxy<>> myProxy_inter = runtime->buildProxy<Handler_msgProxy>(domain, instance);
    std::cout << "Checking inter availability!" << std::endl;

    while (!myProxy_inter->isAvailable()){
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        if(counter % 100 == 0)
            std::cout << "inter not available..."<<counter << std::endl;
        counter++;
        if (counter > 10000) {
            std::cout << "Proxy is not available, exiting..." << std::endl;
            return -1;
        }
    }


    std::cout << "Available..." << std::endl;
    CommonAPI::CallStatus status;
    int32_t result;

    std::ifstream file("./handler_tcp_client/gui_Test", std::ios::binary);
    CommonAPI::ByteBuffer firmware((std::istreambuf_iterator<char>(file)), {});
    CommonAPI::ByteBuffer signature = {0x05, 0x06, 0x07, 0x08};

    std::cout << "Sending pushUpdate request..." << std::endl;

    // Firmware & signature 예시 데이터
    // std::vector<uint8_t> firmware = {0x01, 0x02, 0x03, 0x04};
    // std::vector<uint8_t> signature = {0x05, 0x06, 0x07, 0x08};

    myProxy_inter->pushUpdateAsync(firmware, signature,
        [](const CommonAPI::CallStatus& status, const int32_t& result) {
            if (status == CommonAPI::CallStatus::SUCCESS) {
                std::cout << "[Async] Success inter pushUpdate, result = " << result << std::endl;
            } else {
                std::cout << "[Async] Fail pushUpdate" << std::endl;
            }
        });
    // myProxy_exter->pushUpdate_exterAsync(firmware, signature,
    //     [](const CommonAPI::CallStatus& status, const int32_t& result) {
    //         if (status == CommonAPI::CallStatus::SUCCESS) {
    //             std::cout << "[Async] exter pushUpdate 성공, result = " << result << std::endl;
    //         } else {
    //             std::cout << "[Async] pushUpdate 실패!" << std::endl;
    //         }
    //     });
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}