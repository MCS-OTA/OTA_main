#include "handlerStubImpl.hpp"
#include <iomanip>  // std::hex, std::setw
using namespace v0::commonapi;

handlerStubImpl::handlerStubImpl(){

}

handlerStubImpl::~handlerStubImpl(){
    
}

void handlerStubImpl::pushUpdate(const std::shared_ptr<CommonAPI::ClientId> _client, 
            const CommonAPI::ByteBuffer _firmware, const CommonAPI::ByteBuffer _signature, pushUpdateReply_t _reply){
    std::ofstream firmwareFile("./handler_tcp_server/received_update", std::ios::binary | std::ios::out);
    std::ofstream signatureFile("./handler_tcp_server/received_signature", std::ios::binary | std::ios::out);
    if (!firmwareFile) {
        std::cerr << "Failed to create Firmware\n";
        _reply(-1);
        return;
    }
    if (!signatureFile) {
        std::cerr << "Failed to verify signature\n";
        firmwareFile.close();
        _reply(-1);
        return;
    }
    // Save firmware and signature
    try{
        firmwareFile.write(reinterpret_cast<const char*>(_firmware.data()), _firmware.size());
        signatureFile.write(reinterpret_cast<const char*>(_signature.data()), _signature.size());
        firmwareFile.close();
        signatureFile.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving process: " << e.what() << "\n";
        firmwareFile.close();
        signatureFile.close();
        _reply(-1);
        return;
    }
    std::cout << "Complete saving (" << _firmware.size() << " bytes, " << _signature.size() << " bytes)\n";
    _reply(1);
}
