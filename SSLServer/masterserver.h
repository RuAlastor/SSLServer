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

        int Start() noexcept;
        int Handle() noexcept(false);


    private:
        enum MasterSocketErrors { noError, socketInitError, socketBindError, socketListenError, connectionError, slaveSocketError };

        const in_addr_t _ip;
        const int _port;
        int _masterSocket;

    };

    class SlaveSocket : public Socket {

    public:

        explicit SlaveSocket(int fd,
                             const char* filename,
                             sockaddr_in* socketInfo,
                             socklen_t* socketInfoLen) noexcept;
        ~SlaveSocket() noexcept;

        SlaveSocket() = delete;
        SlaveSocket(const SlaveSocket& other) = delete;
        SlaveSocket(SlaveSocket&& other) = delete;
        SlaveSocket& operator=(const SlaveSocket& other) = delete;
        SlaveSocket& operator=(SlaveSocket&& other) = delete;

        int Start() noexcept;
        int SignFile() noexcept(false);
        int SendBack() noexcept;

    private:
        enum SlaveSocketErrors { noError, fileWritingError, parseError, acceptError, sendError };

        int _slaveSocket;
        const char* _filename;
        sockaddr_in* _socketInfo;
        socklen_t* _socketInfoLen;


    };

}

#endif // MASTERSERVER_H
