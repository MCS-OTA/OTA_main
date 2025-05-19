#include "handlerStubImpl.hpp"
#include <iomanip>  // std::hex, std::setw
using namespace v0::commonapi;

handlerExterStubImpl::handlerExterStubImpl() : downloadStarted_(false){
    std::cout << "[Server] HandlerStubImpl called.\n";

}

handlerExterStubImpl::~handlerExterStubImpl(){
    
}

void handlerExterStubImpl::updateMsgExter(const std::shared_ptr<CommonAPI::ClientId> _client, const CommonAPI::ByteBuffer udsMsg, updateMsgExterReply_t _reply) {
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
            updateFile_.open("./handler_tcp_server_exter/received_update", std::ios::binary | std::ios::out);
            if (!updateFile_) {
                std::cerr << "Fail to create new file" << std::endl;
                response = {0x7F, serviceID, 0x11};
            }

            downloadStarted_ = true;
            setStatus(static_cast<int32_t>(HandlerStatus::WAIT));
            response = {0x74};
            break;
        }

        case 0x36: {
            setStatus(static_cast<int32_t>(HandlerStatus::PROCESSING));
            CommonAPI::ByteBuffer file(udsMsg.begin() + 1, udsMsg.end());
            
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

            // if (!downloadStarted_ || !firmwareFile_.is_open()) {
            //     std::cerr << "TransferData called before RequestDownload.\n";
            //     response = {0x7F, serviceID, 0x11};
            //     break;
            // }

            //firmwareFile_.write(reinterpret_cast<const char*>(&udsRequest[1]), udsRequest.size() - 1);
            setStatus(static_cast<int32_t>(HandlerStatus::WAIT));
            response = {0x76};
            break;
        }

        case 0x37: {
            std::cout << "[TransferExit] Finalizing firmware transfer.\n";
            // if (firmwareFile_.is_open()) {
            //     firmwareFile_.close();
            // }

            if (updateFile_.is_open()) {
                updateFile_.close();
            }

            downloadStarted_ = false;
            setStatus(static_cast<int32_t>(HandlerStatus::VERIFY));
            
            // Process the verification

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

void handlerExterStubImpl::notifyHandlerStatusExter(const std::shared_ptr<CommonAPI::ClientId> _client, const int32_t& statusCode) {
    std::cout << "[Server] Broadcasting status: " << statusCode << std::endl;

    fireHandlerStatusExterEvent(statusCode);
}
