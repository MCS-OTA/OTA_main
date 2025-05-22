#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/Handler_msgStubDefault.hpp>
#include <v0/commonapi/Handler_msg.hpp>
#include "HandlerStatus.hpp"
#include <fstream>
#include <filesystem>

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

class handlerStubImpl : public v0::commonapi::Handler_msgStubDefault {
public:
    explicit handlerStubImpl();
    virtual ~handlerStubImpl();
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

    virtual void updateMsg(
        const std::shared_ptr<CommonAPI::ClientId> _client,
        const CommonAPI::ByteBuffer udsRequest,
        // int32_t _firmware,
        // int32_t _signature,
        updateMsgReply_t _reply);

    virtual void notifyHandlerStatus(const std::shared_ptr<CommonAPI::ClientId> _client, const int32_t& statusCode);
    
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
    // int32_t getStatus(){
    //     std::ifstream file(statusFilePath_);
    //     std::string line;
    //     while(std::getline(file, line)){
    //         if(line.rfind("status=",0)==0){
    //             status_ = std::stoi(line.substr(7));
    //         }
    //     }
    //     file.close();
    //     return status_;
    // }


    private:
    int32_t status_;
    std::ofstream firmwareFile_;
    CommonAPI::ByteBuffer payload_;
    bool downloadStarted_;
    std::ofstream updateFile_;
    std::string statusFilePath_ = "./status.txt";
};
