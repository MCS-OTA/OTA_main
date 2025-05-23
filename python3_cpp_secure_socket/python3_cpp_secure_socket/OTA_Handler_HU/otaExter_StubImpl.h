#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/OTA_exterStubDefault.hpp>
#include <v0/commonapi/OTA_exter.hpp>
#include <fstream>

using namespace v0::commonapi;

class otaExter_StubImpl : public v0::commonapi::OTA_exterStubDefault {
public:
    explicit otaExter_StubImpl();
    virtual ~otaExter_StubImpl();

    virtual void pushUpdate_exter(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        const CommonAPI::ByteBuffer _firmware,
        const CommonAPI::ByteBuffer _signature,
        pushUpdateReply_t _reply);
private:
    std::ofstream firmwareFile;
    std::ofstream signatureFile;
    std::string firmwareFilePath = "./OTA_Handler/received_update";
    std::string signatureFilePath = "./OTA_Handler/received_signature";
};
