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
        // returns 0 if everything was correct
        int Start() noexcept;
        // Waits for client to connect
        // When client connected creates SlaveSocket obj and gives client to it
        // returns 0 if everything was correct
        int Handle() noexcept(false);


    private:
        enum MasterSocketErrors { noError, socketInitError, socketBindError, socketListenError, slaveSocketError };

        const in_addr_t _ip;
        const int _port;
        int _masterSocket;
        std::string _pwd;

    };

    class SlaveSocket : public Socket {

    public:

        explicit SlaveSocket(int fd,
                             const std::string* pwd,
                             const char* filename,
                             sockaddr_in* socketInfo,
                             socklen_t* socketInfoLen) noexcept;
        ~SlaveSocket() noexcept;

        SlaveSocket() = delete;
        SlaveSocket(const SlaveSocket& other) = delete;
        SlaveSocket(SlaveSocket&& other) = delete;
        SlaveSocket& operator=(const SlaveSocket& other) = delete;
        SlaveSocket& operator=(SlaveSocket&& other) = delete;

        int RecvFile() noexcept;
        int SignFile() noexcept(false);
        int SendFile() noexcept;

    private:
        enum SlaveSocketErrors { noError, connectionError, fileWritingError, parseError, signerError, acceptError, sendError };

        int _slaveSocket;
        const std::string* _pwd;
        const char* _filename;
        sockaddr_in* _socketInfo;
        socklen_t* _socketInfoLen;

    };

}

#endif // MASTERSERVER_H
