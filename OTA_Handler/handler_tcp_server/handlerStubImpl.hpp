#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/Handler_msgStubDefault.hpp>
#include <v0/commonapi/Handler_msg.hpp>
#include <fstream>

using namespace v0::commonapi;

class handlerStubImpl : public v0::commonapi::Handler_msgStubDefault {
public:
    explicit handlerStubImpl();
    virtual ~handlerStubImpl();

    virtual void pushUpdate(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        const CommonAPI::ByteBuffer _firmware,
        const CommonAPI::ByteBuffer _signature,
        // int32_t _firmware,
        // int32_t _signature,
        pushUpdateReply_t _reply);
};