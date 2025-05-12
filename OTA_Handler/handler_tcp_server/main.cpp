#include "handlerStubImpl.hpp"
#include <iostream>
#include <thread>

int main(){
    std::cout << "Hello";
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "handler_tcp_server");
    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();


    std::string domain = "local";
    std::string instance = "commonapi.Handler_msg";

    std::shared_ptr<handlerStubImpl> Service = std::make_shared<handlerStubImpl>();
    std::cout << "Registering service..." << std::endl;
    bool successfullyRegistered = runtime->registerService(domain, instance, Service);

    while (!successfullyRegistered) {
        std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        successfullyRegistered = runtime->registerService(domain, instance, Service);
    }

    std::cout << "Successfully Registered Service!" << std::endl;

    int32_t status = 100;
    while (true) {
        Service->fireHandlerStatusEvent(status++);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}