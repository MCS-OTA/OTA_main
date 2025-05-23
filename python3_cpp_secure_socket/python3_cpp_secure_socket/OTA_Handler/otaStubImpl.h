#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/OTA_interStubDefault.hpp>
#include <v0/commonapi/OTA_inter.hpp>
#include <fstream>

using namespace v0::commonapi;

class otaStubImpl : public v0::commonapi::OTA_interStubDefault {
public:
    explicit otaStubImpl();
    virtual ~otaStubImpl();

    virtual void pushUpdate(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        const CommonAPI::ByteBuffer _firmware,
        const CommonAPI::ByteBuffer _signature,
        // int32_t _firmware,
        // int32_t _signature,
        pushUpdateReply_t _reply);
};
