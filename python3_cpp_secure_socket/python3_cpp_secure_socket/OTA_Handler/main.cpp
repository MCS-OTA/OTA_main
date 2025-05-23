#include "otaStubImpl.h"
#include <iostream>
int main(){
    std::cout << "hello";
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "OTA_inter");
    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();


    std::string domain = "local";
    std::string instance = "commonapi.OTA_service_inter";

    std::shared_ptr<otaStubImpl> Service = std::make_shared<otaStubImpl>();
    std::cout << "Registering service..." << std::endl;
    bool successfullyRegistered = runtime->registerService(domain, instance, Service);

    while (!successfullyRegistered) {
        std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        successfullyRegistered = runtime->registerService(domain, instance, Service);
    }

    std::cout << "Successfully Registered Service!" << std::endl;
    for(int i = 0; i < 20; i++) {
        std::cout << "Waiting for calls... (Abort with CTRL+C)" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}