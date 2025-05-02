#include "otaExter_StubImpl.h"
#include <iomanip>  // std::hex, std::set
using namespace v0::commonapi;

otaExter_StubImpl::otaExter_StubImpl(){

}

otaExter_StubImpl::~otaExter_StubImpl(){
    
}

void otaExter_StubImpl::pushUpdate(const std::shared_ptr<CommonAPI::ClientId> _client, 
            const CommonAPI::ByteBuffer _firmware, const CommonAPI::ByteBuffer _signature, pushUpdateReply_t _reply){
    std::ofstream firmwareFile("./OTA_Handler/received_update", std::ios::binary | std::ios::out);
    std::ofstream signatureFile("./OTA_Handler/received_signature", std::ios::binary | std::ios::out);
    if (!firmwareFile) {
        std::cerr << "펌웨어 파일 생성 실패!\n";
        _reply(-1);
        return;
    }
    if (!signatureFile) {
        std::cerr << "서명 파일 생성 실패!\n";
        firmwareFile.close();
        _reply(-1);
        return;
    }
    // 펌웨어와 서명 데이터를 파일에 저장
    try{
        firmwareFile.write(reinterpret_cast<const char*>(_firmware.data()), _firmware.size());
        signatureFile.write(reinterpret_cast<const char*>(_signature.data()), _signature.size());
        firmwareFile.close();
        signatureFile.close();
    }
    catch (const std::exception& e) {
        std::cerr << "파일 저장 중 오류 발생: " << e.what() << "\n";
        firmwareFile.close();
        signatureFile.close();
        _reply(-1);
        return;
    }
    std::cout << "펌웨어와 서명 저장 완료 (" << _firmware.size() << " bytes, " << _signature.size() << " bytes)\n";
    _reply(1);
}

