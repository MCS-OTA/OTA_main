#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/Handler_msgProxy.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <condition_variable>
#include <mutex>
#include <filesystem>

using namespace v0::commonapi;

std::mutex mtx;
std::condition_variable cv;
bool exitFlag = false;

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
    std::string updatePath = "./handler_tcp_client/update";

    myProxy_inter->getHandlerStatusEvent().subscribe([&](int32_t statusCode) {
        std::cout << "[Client] Received ServerStatusBroadcast: " << statusCode << std::endl;

        CommonAPI::ByteBuffer udsRequest;
        switch (statusCode) {
            case 0: //IDLE
                // If the Master ECU downloads the update file     
                if (!std::filesystem::is_empty(updatePath)){
                    udsRequest = {0x33};
                    std::cout << "Status: 0\n\n" << std::endl;

                    myProxy_inter->updateMsgAsync(udsRequest, 
                        [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result) {
                        std::cout << "Send Request1\n\n" << std::endl;
                        if (result[0] == 0x7F) {
                            std::cout << "Negative Response\n" << std::endl;
                        }
                        else {
                            std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
                        }
                    });
                    break;
                }
                else {
                    std::cout << "\nNO UPDATE\n" << std:: endl;
                    break;
                }
            case 1: // INIT
                // RequestDownload(0x34)
                //udsRequest.push_back(0x34);
                udsRequest = {0x34};
                std::cout << "Status: 1\n\n" << std::endl;

                myProxy_inter->updateMsgAsync(udsRequest, 
                    [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result) {
                    std::cout << "Send Request1\n\n" << std::endl;
                    if (result[0] == 0x7F) {
                        std::cout << "Negative Response1\n" << std::endl;
                    }
                    else {
                        std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
                    }
                });
                break;
            case 2: {// WAIT
                // TransferData(0x36)
                udsRequest = {0x36};
                std::ifstream file("./handler_tcp_client/update/gui_Test", std::ios::binary);
                CommonAPI::ByteBuffer chunk((std::istreambuf_iterator<char>(file)), {});
                udsRequest.insert(udsRequest.end(), chunk.begin(), chunk.end());
                // while (file.read(reinterpret_cast<char*>(block.data()), block.size()) || file.gcount()) {
                //     size_t size = file.gcount();
                //     block.resize(size);
                // }
                myProxy_inter->updateMsgAsync(udsRequest,
                    [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result){
                    std::cout << "Send Request2\n\n" << std::endl;
                    if (result[0] == 0x7F) {
                        std::cout << "Negative Response\n" << std::endl;
                    }
                    else {
                        std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
                    }
                });
            }

                // If transferring process end, TransferExit(0x37)
                udsRequest = {0x37};
                myProxy_inter->updateMsgAsync(udsRequest, 
                    [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result){
                    std::cout << "Send Request3\n\n" << std::endl;
                    if (result[0] == 0x7F) {
                        std::cout << "Negative Response\n" << std::endl;
                    }
                    else {
                        std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
                    }
                });
                break;
            case 3: // PROCESSING
                // TrnasferData(0x36)
                break;
            case 4: // VERIFY
                break;
            case 5: // READY
                break;
            case 6: // ACTIVATE
                break;
            case 7: // ERROR
                break;
            default:
                break;
        }
        
        // if (statusCode % 10 == 0) {
        //     CommonAPI::CallStatus status;
        //     int32_t result;
        //     std::ifstream file("./handler_tcp_client/gui_Test", std::ios::binary);
        //     CommonAPI::ByteBuffer firmware((std::istreambuf_iterator<char>(file)), {});
        //     CommonAPI::ByteBuffer signature = {0x05, 0x06, 0x07, 0x08};

        //     std::cout << "Sending pushUpdate request..." << std::endl;

        //     // Firmware & signature 예시 데이터
        //     // std::vector<uint8_t> firmware = {0x01, 0x02, 0x03, 0x04};
        //     // std::vector<uint8_t> signature = {0x05, 0x06, 0x07, 0x08};

        //     myProxy_inter->pushUpdateAsync(firmware, signature,
        //         [](const CommonAPI::CallStatus& status, const int32_t& result) {
        //             if (status == CommonAPI::CallStatus::SUCCESS) {
        //                 std::cout << "[Async] Success inter pushUpdate, result = " << result << std::endl;
        //             } else {
        //                 std::cout << "[Async] Fail pushUpdate" << std::endl;
        //             }
        //         });
        //}

        // if (statusCode == 500) {
        //     std::unique_lock<std::mutex> lock(mtx);
        //     exitFlag = true;
        //     cv.notify_one();
        // }
    });

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return exitFlag; });
    }

    return 0;
}
