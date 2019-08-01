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
    delete _socketInfo; // Might throw an exception
    _socketInfo = nullptr;
    std::cout << "Socket info was succesfully deleted!\n";
}

//-----------------------------------------------------------------------------------------------------------------------------

Socket::err Socket::closeDescriptor() noexcept(false) {
    if (_socketfd != -1) {
        if (close(_socketfd) != 0) {
            __printCError("Failed to close socket: ");
            return undefinedError;
        }
    }
    std::cout << "Descriptor was succesfully destroyed!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

int Socket::__setNonBlock() noexcept {
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

void Socket::__throwCError() noexcept(false) {
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = new char[bufMsgSize]; // Might throw an exception
    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(errorNum, bufMsg, bufMsgSize);
    SocketError error(errorMsg);

    delete[] bufMsg; // Might throw an exception
    bufMsg = nullptr;
    throw error;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Socket::__printCError(std::string preErrorMsg) noexcept(false) {
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = nullptr;
    try {
        bufMsg = new char[bufMsgSize];
    }
    catch (...) {
        std::cout << "Unable to allocate memory to print error!\n";
    }

    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(errorNum, bufMsg, bufMsgSize);
    std::cout << preErrorMsg << errorMsg << '\n';

    delete[] bufMsg; // Might throw an exception
    bufMsg = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------

