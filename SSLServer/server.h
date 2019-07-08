#ifndef SERVER_H
#define SERVER_H

#include "headers.h"
#include "exceptions.h"
#include "sockets.h"

namespace Sorokin {

    class Server {

    public:
        Server() noexcept;
        ~Server() noexcept;

        Server(const Server& other) = delete;
        Server(Server&& other) = delete;
        Server& operator=(const Server& other) = delete;
        Server& operator=(Server&& other) = delete;

        inline void SetPwd(const char* pwd) noexcept(false){
            _pwd = new std::string;
            *_pwd = pwd;
        }
        void Clear() noexcept;
        void CreateMasterSocket(int port) noexcept(false);
        void WaitForConnection() noexcept(false);

    private:
        MasterSocket* _masterSocketPtr;
        SlaveSocket* _slaveSocektPtr;
        std::string* _pwd;

        void GetSignSend() noexcept(false);

    };

}

#endif // SERVER_H
