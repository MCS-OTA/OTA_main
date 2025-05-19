#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/Handler_msg_exterStubDefault.hpp>
#include <v0/commonapi/Handler_msg_exter.hpp>
#include "HandlerStatus.hpp"
#include <fstream>

using namespace v0::commonapi;

// enum class HandlerStatus : int32_t {
//     IDLE = 0,
//     INIT = 1,
//     WAIT = 2,
//     PROCESSING = 3,
//     VERIFY = 4,
//     READY = 5,
//     ACTIVATE = 6,
//     ERROR = 7
// };

class handlerExterStubImpl : public v0::commonapi::Handler_msg_exterStubDefault {
public:
    explicit handlerExterStubImpl();
    virtual ~handlerExterStubImpl();
    virtual int32_t getStatus() {
        return status_;
    }
    virtual void setStatus(int32_t status) {
        status_ = status;
    }

    virtual void updateMsgExter(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        const CommonAPI::ByteBuffer udsRequest,
        // int32_t _firmware,
        // int32_t _signature,
        updateMsgExterReply_t _reply);

    virtual void notifyHandlerStatusExter(const std::shared_ptr<CommonAPI::ClientId> _client, const int32_t& statusCode);

private:
    int32_t status_;
    std::ofstream firmwareFile_;
    CommonAPI::ByteBuffer payload_;
    bool downloadStarted_;
    std::ofstream updateFile_;
};
