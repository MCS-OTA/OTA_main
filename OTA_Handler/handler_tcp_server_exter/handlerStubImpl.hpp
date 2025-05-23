#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/Handler_msg_exterStubDefault.hpp>
#include <v0/commonapi/Handler_msg_exter.hpp>
#include "HandlerStatus.hpp"
#include <fstream>
#include <filesystem>
#include <json.hpp>

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
        std::ifstream file(statusFilePath_);
        std::string line;
        while(std::getline(file, line)){
            if(line.rfind("status=",0)==0){
                status_ = std::stoi(line.substr(7));
            }
        }
        file.close();
        return status_;
    }
    virtual void setStatus(int32_t status) {
        status_ = status;
        saveStatus();
    }

    virtual void updateMsgExter(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        const CommonAPI::ByteBuffer udsRequest,
        // int32_t _firmware,
        // int32_t _signature,
        updateMsgExterReply_t _reply);

    virtual void notifyHandlerStatusExter(const std::shared_ptr<CommonAPI::ClientId> _client, const int32_t& statusCode);

    std::string extractDirFromJson(const std::string& filename, int32_t lineNum){
        std::ifstream file(filename);
        std::string line;
        int current = 1;
        while(std::getline(file, line)){
            if(current == lineNum){
                line.erase(std::remove_if(line.begin(),line.end(), [](char c){
                    return c == '"' || c == ',' || std::isspace(c);
                }),line.end());
                return line;
            }
            ++current;
        }
        return "";
    }

    void moveFile(const std::string& from, const std::string& to){
        try{
            std::filesystem::rename(from, to);
            std::cout << "file move done: "<< from <<" -> " << to << std::endl;
        }catch(const std::filesystem::filesystem_error& e){
            std::cerr << "file move fail: " << e.what() << std::endl;
        }
    }
    void saveStatus();
    CommonAPI::ByteBuffer calculate_cmac(const uint8_t* data, size_t data_len);
    bool verify_message(const uint8_t* data, size_t data_len, const CommonAPI::ByteBuffer& expected_mac);

private:
    int32_t status_;
    std::ofstream firmwareFile_;
    CommonAPI::ByteBuffer payload_;
    bool downloadStarted_;
    std::ofstream updateFile_;
    std::string statusFilePath_ = "./status.txt";
};
