#ifndef MASTERSERVER_H
#define MASTERSERVER_H

#include "parser.h"
#include "signer.h"

namespace Sorokin {

    class Socket {

    public:

        Socket() noexcept = default;
        virtual ~Socket() noexcept = default;

        Socket(const Socket& other) = delete;
        Socket(Socket&& other) = delete;
        Socket& operator=(const Socket& other) = delete;
        Socket& operator=(Socket&& other) = delete;

    protected:

        // Sets given socket to nonblock state
        // return 0 if everything was correct
        inline int setNonBlock(int& fd) noexcept {
            int flags;
        #ifdef O_NONBLOCK
            if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
                flags = 0;
            }
            return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        #else
            flags = 1;
            return ioctl(fd, FIOBIO, &flags);
        #endif
        }

    };

    class MasterSocket : public Socket {

    public:

        explicit MasterSocket(int port) noexcept;
        ~MasterSocket() noexcept;

        MasterSocket() = delete;
        MasterSocket(const MasterSocket& other) = delete;
        MasterSocket(MasterSocket&& other) = delete;
        MasterSocket& operator=(const MasterSocket& other) = delete;
        MasterSocket& operator=(MasterSocket&& other) = delete;

        // Asks user to write password for private key file
        void AskPwd() noexcept(false);
        // Initializes, binds and sets socket to listen
        void Start() noexcept(false);
        // Shutdowns the connection and closes the socket
        void End() noexcept;
        // Waits for client to connect
        // When client connected creates SlaveSocket obj and gives connection to it
        void Handle() noexcept(false);


    private:
        const in_addr_t _ip;
        const int _port;
        int _masterSocket;
        std::string _pwd;

    };

    class SlaveSocket : public Socket {

    public:

        explicit SlaveSocket(int fd,
                             const std::string* pwd,
                             sockaddr_in* socketInfo,
                             socklen_t* socketInfoLen) noexcept;
        ~SlaveSocket() noexcept;

        SlaveSocket() = delete;
        SlaveSocket(const SlaveSocket& other) = delete;
        SlaveSocket(SlaveSocket&& other) = delete;
        SlaveSocket& operator=(const SlaveSocket& other) = delete;
        SlaveSocket& operator=(SlaveSocket&& other) = delete;

        void CloseConnection() noexcept;
        void RecvFile() noexcept(false);
        void SignFile() noexcept(false);
        void SendFile() noexcept(false);

    private:
        int _slaveSocket;
        const std::string* _pwd;
        std::string _xmlFileIn;
        sockaddr_in* _socketInfo;
        socklen_t* _socketInfoLen;

    };

}

#endif // MASTERSERVER_H
