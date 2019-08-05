#ifndef CLIENT_H
#define CLIENT_H

#include "SSLServer/sockets.h"

namespace Sorokin {

    class Client {

    public:
        enum err {
            undefinedError = -1,
            noError = 0
        };

    public:
        Client() noexcept : _clientSocket(nullptr) {}

        ~Client() = default;

    private:
        Socket* _clientSocket;

    private:
        Client(const Client&) = delete;
        Client(Client&&) = delete;
        Client& operator =(const Client&) = delete;
        Client& operator =(Client&&) = delete;
    };

    /*
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
    */

}

#endif // CLIENT_H
