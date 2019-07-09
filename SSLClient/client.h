#ifndef CLIENT_H
#define CLIENT_H

#include "headers.h"
#include "exceptions.h"
#include "parser.h"

namespace Sorokin {

    class Client {

    public:
        explicit Client(const char* filenameOut, const char* filenameIn, int port) noexcept;
        ~Client() noexcept;

        Client() = delete;
        Client(const Client& other) = delete;
        Client(Client&& other) = delete;
        Client& operator=(const Client& other) = delete;
        Client& operator=(Client&& other) = delete;

        void CloseConnect() noexcept;
        void Start() noexcept(false);
        void Handle() noexcept(false);

    private:
        const char* _filenameOut;
        const char* _filenameIn;
        const in_addr_t _ip;
        const int _port;
        int _masterSocket;

        void Send() noexcept(false);
        void Get() noexcept(false);

    };

}

#endif // CLIENT_H
