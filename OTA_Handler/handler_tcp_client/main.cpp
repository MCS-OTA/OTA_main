#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/Handler_msgProxy.hpp>
#include <v0/commonapi/Handler_msg_exterProxy.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <condition_variable>
#include <mutex>
#include <filesystem>
#include <vector>

using namespace v0::commonapi;

std::mutex mtx;
std::condition_variable cv;
bool exitFlag = false;
std::vector<CommonAPI::ByteBuffer> splitIntoChunk(const CommonAPI::ByteBuffer& data, size_t chunkSize);

std::string chunkStatusFilePath_ = "/home/ota/Documents/handler_tcp_client/chunkStatusFile.txt";
int main(){
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "handler_tcp_client");
    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();
    int counter = 0;
    int chunkN = 0;
    int chunkTotal = 0;

    std::string domain = "local";
    std::string instance = "commonapi.Handler_msg";
    std::string instance_exter = "commonapi.Handler_msg_exter";
    std::shared_ptr<Handler_msgProxy<>> myProxy_inter = runtime->buildProxy<Handler_msgProxy>(domain, instance);
    std::cout << "Checking inter availability!" << std::endl;

    while (!myProxy_inter->isAvailable()){
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        if(counter % 100 == 0)
            std::cout << "inter not available..."<< counter << std::endl;
        counter++;
        if (counter > 10000) {
            std::cout << "Proxy is not available, exiting..." << std::endl;
            return -1;
        }
    }

    if (!std::filesystem::exists(chunkStatusFilePath_)){
        std::ofstream create_file(chunkStatusFilePath_);
        create_file << "0/0"<<std::endl;
        create_file.close();
    }else{
        std::ifstream file(chunkStatusFilePath_);
        std::string line;
        if(std::getline(file, line)){
            size_t slash = line.find('/');
            if (slash != std::string::npos){
                chunkN = std::stoi(line.substr(0,slash));
                chunkTotal = std::stoi(line.substr(slash + 1));
            }
        }
        file.close();
    }
    

    // std::shared_ptr<Handler_msg_exterProxy<>> myProxy_exter = runtime->buildProxy<Handler_msg_exterProxy>(domain, instance_exter);
    // std::cout << "Checking exter availability!" << std::endl;
    
    // while (!myProxy_exter->isAvailable()){
    //     std::this_thread::sleep_for(std::chrono::microseconds(1000));    std::string instance_exter = "commonapi.Handler_msg_exter";

    //     if(counter % 100 == 0)
    //         std::cout << "exter not available..."<< counter << std::endl;
    //     counter++;
    //     if (counter > 10000) {
    //         std::cout << "Proxy is not available, exiting..." << std::endl;
    //         return -1;
    //     }
    // }



    std::cout << "Available..." << std::endl;
    std::string updatePath = "./handler_tcp_client/update";

    // myProxy_exter->getHandlerStatusExterEvent().subscribe([&](int32_t statusCode) {
    //     std::cout << "[Client] Received ServerStatusBroadcast: " << statusCode << std::endl;

    //     CommonAPI::ByteBuffer udsRequest;
    //     switch (statusCode) {
    //         case 0: //IDLE
    //             // 0x33, 
    //             // If the Master ECU downloads the update file     
    //             if (!std::filesystem::is_empty(updatePath)){
    //                 udsRequest = {0x33};
    //                 std::cout << "Status: 0\n\n" << std::endl;

    //                 myProxy_exter->updateMsgExterAsync(udsRequest, 
    //                     [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result) {
    //                     std::cout << "Send Request1\n\n" << std::endl;
    //                     if (result[0] == 0x7F) {
    //                         std::cout << "Negative Response\n" << std::endl;
    //                     }
    //                     else {
    //                         std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
    //                     }
    //                 });
    //                 break;
    //             }
    //             else {
    //                 std::cout << "\nNO UPDATE\n" << std:: endl;
    //                 break;
    //             }
    //         case 1: // INIT
    //             // 0x34, sub, data size, dir
    //             // RequestDownload(0x34)
    //             //udsRequest.push_back(0x34);
    //             udsRequest = {0x34};
    //             std::cout << "Status: 1\n\n" << std::endl;

    //             myProxy_exter->updateMsgExterAsync(udsRequest, 
    //                 [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result) {
    //                 std::cout << "Send Request1\n\n" << std::endl;
    //                 if (result[0] == 0x7F) {
    //                     std::cout << "Negative Response1\n" << std::endl;
    //                 }
    //                 else {
    //                     std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
    //                 }
    //             });
    //             break;
    //         case 2: {// WAIT
    //             // 0x36, sub, size, iter, data
    //             // TransferData(0x36)
    //             udsRequest = {0x36};
    //             std::ifstream file("./handler_tcp_client/update/gui_Test", std::ios::binary);
    //             // read iter file, send next iter chunk
    //             CommonAPI::ByteBuffer chunk((std::istreambuf_iterator<char>(file)), {});
    //             udsRequest.insert(udsRequest.end(), chunk.begin(), chunk.end());
    //             // while (file.read(reinterpret_cast<char*>(block.data()), block.size()) || file.gcount()) {
    //             //     size_t size = file.gcount();
    //             //     block.resize(size);
    //             // }
    //             myProxy_exter->updateMsgExterAsync(udsRequest,
    //                 [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result){
    //                 std::cout << "Send Request2\n\n" << std::endl;
    //                 if (result[0] == 0x7F) {
    //                     std::cout << "Negative Response\n" << std::endl;
    //                 }
    //                 else { // save iter as file
    //                     std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
    //                 }
    //             });
    //         }

    //             // If transferring process end, TransferExit(0x37)
    //             udsRequest = {0x37};
    //             myProxy_exter->updateMsgExterAsync(udsRequest, 
    //                 [](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result){
    //                 std::cout << "Send Request3\n\n" << std::endl;
    //                 if (result[0] == 0x7F) {
    //                     std::cout << "Negative Response\n" << std::endl;
    //                 }
    //                 else {
    //                     std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
    //                 }
    //             });
    //             break;
    //         case 3: // PROCESSING
    //             // TrnasferData(0x36)
    //             break;
    //         case 4: // VERIFY
    //             break;
    //         case 5: // READY
    //             break;
    //         case 6: // ACTIVATE
    //             break;
    //         case 7: // ERROR
    //             break;
    //         default: 
    //             break;
    //     }
    // });

    int32_t n = 0;

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
                int32_t chunkSize = 100;
                //int32_t n = 0; //read
                udsRequest = {0x36};
                std::ifstream file("./handler_tcp_client/update/gui_Test", std::ios::binary);
                CommonAPI::ByteBuffer chunk((std::istreambuf_iterator<char>(file)), {});

                std::vector<CommonAPI::ByteBuffer> chunks = splitIntoChunk(chunk, chunkSize);
                int32_t total_n = chunks.size();
                if(chunkN < total_n) {
                    udsRequest.push_back(total_n);
                    udsRequest.push_back(chunkN);
                    udsRequest.insert(udsRequest.end(), chunks[chunkN].begin(),chunks[chunkN].end());
                    udsRequest.insert(udsRequest.end(), chunks[chunkN].begin(),chunks[chunkN].end());


                    // while (file.read(reinterpret_cast<char*>(block.data()), block.size()) || file.gcount()) {
                    //     size_t size = file.gcount();
                    //     block.resize(size);
                    // }



                    myProxy_inter->updateMsgAsync(udsRequest,
                        [&](const CommonAPI::CallStatus& status, CommonAPI::ByteBuffer result){
                        std::cout << "Send Request2 ## chunks number "<<chunkN<<"out of "<<total_n << std::endl;
                        if (result[0] == 0x7F) {
                            std::cout << "Negative Response\n" << std::endl;
                        }
                        else {
                            std::cout << "Positive Response 0x" << std::hex << static_cast<int>(result[0]) << std::dec << std::endl;
                            chunkN = static_cast<int>(result[1]);
                            std::cout << "chunkN is " <<chunkN<<std::endl;
                        }
                    });

                    std::ofstream file(chunkStatusFilePath_);
                    file << std::to_string(chunkN+1) << "/" << std::to_string(total_n)<<std::endl;
                    file.close();

                
                }
                else {
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
                    std::ofstream file(chunkStatusFilePath_);
                    file << "0/0" <<std::endl;
                    file.close();
                    break;
                    // verify and change chunkN to 0/0
                }
            }
            case 3: // PROCESSING
                break;
            case 4: // VERIFY
         
                break;
            case 5: // READY
                break;
            case 6: // ACTIVATE
                break;
            case 7: // ERROR
                std::cout<<"%%%%% Activate Error %%%%%" <<std::endl;
                break;
            default:
                break;
        }
    });

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return exitFlag; });
    }

    return 0;
}

std::vector<CommonAPI::ByteBuffer> splitIntoChunk(const CommonAPI::ByteBuffer& data, size_t chunkSize){
    std::vector<CommonAPI::ByteBuffer> chunks;
    size_t offset = 0;
    size_t totalSize = data.size();

    while(offset < totalSize){
        size_t currentSize = std::min(chunkSize, totalSize-offset);
        chunks.emplace_back(data.begin()+offset, data.begin()+offset+currentSize);
        offset += currentSize;
    }
    std::cout<<"Total Number of Chunks: "<<chunks.size()<<std::endl;
    return chunks;
}