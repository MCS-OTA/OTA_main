#include "handlerStubImpl.hpp"
#include <iomanip>  // std::hex, std::setw
using namespace v0::commonapi;

handlerStubImpl::handlerStubImpl() : downloadStarted_(false){
    std::cout << "[Server] HandlerStubImpl called.\n";
    //read undone stauts file
    if (!std::filesystem::exists(statusFilePath_)){
        std::ofstream create_file(statusFilePath_);
        create_file << "status=0"<<std::endl;
        create_file.close();
    }else{
        std::ifstream file(statusFilePath_);
        std::string line;
        while(std::getline(file, line)){
            if(line.rfind("status=",0)==0){
                status_ = std::stoi(line.substr(7));
                break;
            }
        }
        file.close();
    }
}

handlerStubImpl::~handlerStubImpl(){
    
}
void handlerStubImpl::saveStatus(){
    std::ofstream file(statusFilePath_);
    file<<"status="<<status_<<std::endl;
    file.close();
}


void handlerStubImpl::updateMsg(const std::shared_ptr<CommonAPI::ClientId> _client, const CommonAPI::ByteBuffer udsMsg, updateMsgReply_t _reply) {
    if (udsMsg.empty()) {
        std::cout << "Empty Message";
        return;
        //response = {0x7F, 0x00, 0x11};
    }
    CommonAPI::ByteBuffer response;

    uint8_t serviceID = static_cast<uint8_t>(udsMsg[0]);
    std::cout << "Received service ID: 0x" << std::hex << static_cast<int>(serviceID) << std::dec << std::endl;

    switch (serviceID) {
        case 0x33: {
            std::cout << "Start download...\n";
            setStatus(static_cast<int32_t>(HandlerStatus::INIT));
            response = {0x73};
            break;
        }

        case 0x34: {
            std::cout << "[RequestDownload] Initializing download...\n";

            // std::filesystem::path filePath = "/tmp/ota_firmware.bin";
            // firmwareFile_.open(filePath, std::ios::binary | std::ios::out);
            // if (!firmwareFile.is_open()) {
            //     std::cerr << "Failed to open file for writing.\n";
            //     response = {0x7F, serviceID, 0x11};
            //     break;
            // }


            // updateFile_.open("./handler_tcp_server/received_update", std::ios::binary | std::ios::app);
            // if (!updateFile_) {
            //     std::cerr << "Fail to create new file" << std::endl;
            //     response = {0x7F, serviceID, 0x11};
            // }

            // if (updateFile_.is_open()) {
            //     updateFile_.close();
            // }

            downloadStarted_ = true;
            setStatus(static_cast<int32_t>(HandlerStatus::WAIT));
            //change
            response = {0x74};
            break;
        }

        case 0x36: {
            // Start Installation
            setStatus(static_cast<int32_t>(HandlerStatus::PROCESSING));
            
            // Read udsRequest message
            int32_t n = udsMsg[2];
            int8_t fileNameLength = static_cast<int8_t>(udsMsg[3]);
            std::cout << "current file length:" << fileNameLength<< std::endl;
            std::string fileName = std::string(udsMsg.begin() + 4,udsMsg.begin() + fileNameLength + 4);
            std::cout << "current file name:" << fileName << std::endl;
            std::cout<<"# of chunks: "<<n<<std::endl;
            CommonAPI::ByteBuffer file(udsMsg.begin() + 4 + fileNameLength, udsMsg.end());
            fileName = "./handler_tcp_server/" + fileName;
            updateFile_.open(fileName, std::ios::binary | std::ios::app);

            // Write file
            try {
                if (updateFile_.is_open()) {
                    updateFile_.write(reinterpret_cast<const char*>(file.data()), file.size());
                }
                else {
                    std::cerr << "File is not open" << std::endl;
                    response = {0x7F, serviceID, 0x11};
                    break;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Fail to save update file" << std::endl;
                response = {0x7F, serviceID, 0x11};
                break;
            }
            if (updateFile_.is_open()) {
                updateFile_.close();
                std::cout<<"file:"<<fileName<<" chunk # "<<n<<"is closed"<<std::endl;
            }else{
                std::cout<<"nothing to close"<<std::endl;
            }
            // if (!downloadStarted_ || !firmwareFile_.is_open()) {
            //     std::cerr << "TransferData called before RequestDownload.\n";
            //     response = {0x7F, serviceID, 0x11};
            //     break;
            // }

            //firmwareFile_.write(reinterpret_cast<const char*>(&udsRequest[1]), udsRequest.size() - 1);
            //setStatus(static_cast<int32_t>(HandlerStatus::WAIT));

            // Set udsResponse message
            response = {0x76, static_cast<uint8_t>(n+1)}; //0x76, iter
            // save undone status file
            break;
        }

        case 0x37: {
            std::cout << "[TransferExit] Finalizing firmware transfer.\n";
            // if (firmwareFile_.is_open()) {
            //     firmwareFile_.close();
            // }
            // del undone undone status file

            // if (updateFile_.is_open()) {
            //     updateFile_.close();
            // }

            downloadStarted_ = false;
            setStatus(static_cast<int32_t>(HandlerStatus::VERIFY));
            
            // Process the verification

            // move file
            std::string targetDir = extractDirFromJson("/home/ota/boot_manager/status.json", 4);
            targetDir = "/home/ota/boot_manager/" + targetDir + "/my_app";
            moveFile("./handler_tcp_server/received_update", targetDir); 
            setStatus(static_cast<int32_t>(HandlerStatus::READY));
            response = {0x77};
            break;
        }

        default:
            std::cerr << "Unsupported service ID: " << static_cast<int>(serviceID) << std::endl;
            response = {0x7F, serviceID, 0x11};
            break;
    }

    _reply(response);
    if (getStatus() == static_cast<int32_t>(HandlerStatus::PROCESSING)) setStatus(static_cast<int32_t>(HandlerStatus::WAIT));
}

// void handlerStubImpl::pushUpdate(const std::shared_ptr<CommonAPI::ClientId> _client, 
//             const CommonAPI::ByteBuffer _firmware, const CommonAPI::ByteBuffer _signature, pushUpdateReply_t _reply){
//     std::ofstream firmwareFile("./handler_tcp_server/received_update", std::ios::binary | std::ios::out);
//     std::ofstream signatureFile("./handler_tcp_server/received_signature", std::ios::binary | std::ios::out);
//     if (!firmwareFile) {
//         std::cerr << "Failed to create Firmware\n";
//         _reply(-1);
//         return;
//     }
//     if (!signatureFile) {
//         std::cerr << "Failed to verify signature\n";
//         firmwareFile.close();
//         _reply(-1);
//         return;
//     }
//     // Save firmware and signature
//     try{
//         firmwareFile.write(reinterpret_cast<const char*>(_firmware.data()), _firmware.size());
//         signatureFile.write(reinterpret_cast<const char*>(_signature.data()), _signature.size());
//         firmwareFile.close();
//         signatureFile.close();
//     }
//     catch (const std::exception& e) {
//         std::cerr << "Error in saving process: " << e.what() << "\n";
//         firmwareFile.close();
//         signatureFile.close();
//         _reply(-1);
//         return;
//     }
//     std::cout << "Complete saving (" << _firmware.size() << " bytes, " << _signature.size() << " bytes)\n";
//     _reply(1);
// }

void handlerStubImpl::notifyHandlerStatus(const std::shared_ptr<CommonAPI::ClientId> _client, const int32_t& statusCode) {
    std::cout << "[Server] Broadcasting status: " << statusCode << std::endl;

    fireHandlerStatusEvent(statusCode);
}