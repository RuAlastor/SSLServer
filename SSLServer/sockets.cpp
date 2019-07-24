#include "sockets.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Socket::Socket() noexcept : _socketfd(-1), _socketInfo(nullptr) {}

Socket::err Socket::setSocket(const int domain    /* = AF_INET */,
                        const int type      /* = SOCK_STREAM */,
                        const int protocol  /* = 0 */
                        ) noexcept {
    _socketfd = socket(domain, type, protocol);
    if (_socketfd == -1) {
        std::cout << "Failed to create socket!\n";
        return undefinedError;
    }
    std::cout << "Socket created!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Socket::err Socket::setSocketInfo(const int domain     /* = AF_INET */,
                       const in_addr_t ip   /* = INADDR_LOOPBACK */,
                       const int port       /* = 12345 */
                       ) noexcept {
    try {
        _socketInfo = new sockaddr_in;
    }
    catch (...) {
        std::cout << "Failed to set socket info!\n";
        return undefinedError;
    }

    memset(_socketInfo, 0, sizeof(*_socketInfo));

    _socketInfo->sin_family = domain;
    _socketInfo->sin_port = htons(port);
    _socketInfo->sin_addr.s_addr = htonl(ip);

    std::cout << "Socket info is set!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------


void Socket::deleteSocketInfo() noexcept(false) {
    delete _socketInfo;
    _socketInfo = nullptr;
    std::cout << "Socket info was succesfully deleted!\n";
}

//-----------------------------------------------------------------------------------------------------------------------------

void Socket::closeDescriptor() noexcept(false) {
    if (_socketfd != -1) {
        if (close(_socketfd) != 0) {
            this->throwCError();
        }
    }
    std::cout << "Descriptor was succesfully destroyed!\n";
}

//-----------------------------------------------------------------------------------------------------------------------------

int Socket::setNonBlock() noexcept {
    int flags;
#ifdef O_NONBLOCK
    if ((flags = fcntl(_socketfd, F_GETFL, 0)) == -1) {
        flags = 0;
    }
    return fcntl(_socketfd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(_socketfd, FIOBIO, &flags);
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------

void Socket::throwCError() noexcept(false) {
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = new char[bufMsgSize];
    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(errorNum, bufMsg, bufMsgSize);
    SocketError error(errorMsg);

    delete[] bufMsg;
    bufMsg = nullptr;
    throw error;
}

//-----------------------------------------------------------------------------------------------------------------------------
