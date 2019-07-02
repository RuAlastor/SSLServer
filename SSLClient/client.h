#ifndef CLIENT_H
#define CLIENT_H

#include "headers.h"

namespace Sorokin {

    class Client {

    public:
        enum ClientErrors { noErrors, fileOpeningError, socketInitError, connectionError, sendError };

        explicit Client(const char* filename, int port) noexcept;
        ~Client() noexcept;

        Client() = delete;
        Client(const Client& other) = delete;
        Client(Client&& other) = delete;
        Client& operator=(const Client& other) = delete;
        Client& operator=(Client&& other) = delete;

        int Start() noexcept;
        int Handle() noexcept(false);

    private:
        const char* _filename;
        const in_addr_t _ip;
        const int _port;
        int _masterSocket;

    };

}

#endif // CLIENT_H
