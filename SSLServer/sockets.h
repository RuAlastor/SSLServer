#ifndef MASTERSERVER_H
#define MASTERSERVER_H

#include "headers.h"
#include "exceptions.h"

namespace Sorokin {

    class Socket {

    public:
        Socket() noexcept;
        virtual ~Socket() noexcept = default;

        int setUpSocket(const int domain     = AF_INET,
                        const int type       = SOCK_STREAM,
                        const int protocol   = 0
                        ) noexcept;
        int setUpSocketInfo(const int domain      = AF_INET,
                       const in_addr_t ip    = INADDR_LOOPBACK,
                       const int port        = 12345
                       ) noexcept(false);


        int getSocketfd() const noexcept {
            return _socketfd;
        }
        const sockaddr_in* getSocketInfo() const noexcept {
            return _socketInfo;
        }


        void deleteSocketInfo() noexcept(false);
        void closeDescriptor() throw(SocketError);

    protected:
        int _socketfd;
        sockaddr_in* _socketInfo;

        int setNonBlock(int& fd) noexcept;        
        void printCError(std::string preErrorMsg) noexcept;
        void throwCError() noexcept(false);

    private:
        Socket(const Socket&) = delete;
        Socket(Socket&&) = delete;
        Socket& operator =(const Socket&) = delete;
        Socket& operator =(Socket&&) = delete;

    };

    // int shutdownConnection() noexcept;
    /*
    int Socket::shutdownConnection() noexcept {
        if (_socketfd != -1) {
            if (shutdown(_socketfd, SHUT_RDWR) != 0) {
                this->printCError("Can't shutdown connection: ");
                return -1;
            }
        }
        return 0;
    }
    */

    // int sendString(const std::string& strToSend) noexcept(false);
    /*
    int Socket::sendString(const std::string& strToSend) noexcept(false) {
        uint16_t strLen = static_cast<uint16_t>(strToSend.size());
        int sResult = send(_socketfd, reinterpret_cast<const char*>(&strLen), sizeof(uint16_t), 0);
        if (sResult <= 0) {
            this->printCError("Failed to send data: ");
            return -1;
        }
        // Continue here
        return 0;
    }
    */

/*
    struct SlaveSocketInfo {
        int _fd;
        sockaddr_in* _slaveSocketInfo;
        socklen_t* _slaveSocketInfoLen;
    };
*/
/*
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
*/
/*
    class MasterSocket : public Socket {

    public:

        explicit MasterSocket(int port) noexcept;
        ~MasterSocket() noexcept;

        MasterSocket() = delete;
        MasterSocket(const MasterSocket& other) = delete;
        MasterSocket(MasterSocket&& other) = delete;
        MasterSocket& operator=(const MasterSocket& other) = delete;
        MasterSocket& operator=(MasterSocket&& other) = delete;

        // Initializes, binds and sets socket to listen
        void Start() noexcept(false);        
        // Shutdowns the connection and closes the socket
        void CloseConnection() noexcept;
        // Waits for client to connect
        // When client connected creates SlaveSocket obj and  moves connection to new socket which is transfered to SlaveSocket obj
        SlaveSocketInfo Handle() noexcept(false);

    private:
        const in_addr_t _ip;
        const int _port;
        int _masterSocket;

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

        // Recieves the document and its size
        void RecvFile() noexcept(false);
        // Signs recieved document
        void SignFile() noexcept(false);
        // Sends signed document
        void SendFile() noexcept(false);
        // Shutdowns the connection and closes the socket
        void CloseConnection() noexcept;

    private:
        int _slaveSocket;
        const std::string* _pwd;
        std::string _xmlFileIn;
        sockaddr_in* _socketInfo;
        socklen_t* _socketInfoLen;

    };
*/

}

#endif // MASTERSERVER_H
