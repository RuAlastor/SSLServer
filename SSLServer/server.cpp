#include "server.h"

using namespace Sorokin;

int Server::setSocket(const int domain,
                     const int type,
                     const int protocol,
                     const in_addr_t ip,
                     const int port
                     ) noexcept {
    try {
        _masterSocket = new Socket;
    }
    catch (...) {
        std::cout << "Unable to allocate memory for socket object!\n";
        return -1;
    }
    if (_masterSocket->setSocket(domain, type, protocol) ||
        _masterSocket->setSocketInfo(domain, ip, port)
        ) {
        return -1;
    }
    return 0;
}

int Server::setUpListener() noexcept {
    if (bind(_masterSocket->getSocketfd(),
             reinterpret_cast<const sockaddr*>(_masterSocket->getSocketInfo()),
             sizeof(*(_masterSocket->getSocketInfo()))
             )) {
        this->printCError("Can't bind the socket: ");
        return -1;
    }
    std::cout << "Socket was succesfully binded!\n";
    if (listen(_masterSocket->getSocketfd(), 1)) {
        this->printCError("Can't set socket to listen: ");
        return -1;
    }
    std::cout << "Socket was succesfully set to listen!\n";
    return 0;
}

int Server::getConnection(Socket*& slaveSocket) noexcept(false) {
    std::cout << "Waiting for connection...\n";
    socklen_t slaveSocketSize = 0;
    try {
        slaveSocket = new Socket;
    }
    catch (...) {
        std::cout << "Can't allocate memory for new socket!\n";
        return -1;
    }
    slaveSocket->accessSocket() = accept(_masterSocket->getSocketfd(),
                                      reinterpret_cast<sockaddr*>(slaveSocket->accessSocketInfo()),
                                      &slaveSocketSize);
    if (slaveSocket->getSocketfd() == -1) {
        this->printCError("Failed to accept connection: ");
        return -1;
    }

    std::cout << "Connection accepted!\n";
    return 0;
}

int Server::closeConnection() noexcept(false) {
    if (_masterSocket->getSocketfd() != -1) {
        if (shutdown(_masterSocket->getSocketfd(), SHUT_RDWR) != 0) {
            this->printCError("Can't shutdown connection: ");
            return -1;
        }
    }
    return 0;
}

void Server::deleteSocket() noexcept(false) {
    try {
        _masterSocket->deleteSocketInfo();
        _masterSocket->closeDescriptor();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
    }

    delete _masterSocket;
    _masterSocket = nullptr;
}

void Server::printCError(std::string preErrorMsg) noexcept(false) {
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = new char[bufMsgSize];
    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(errorNum, bufMsg, bufMsgSize);
    std::cout << preErrorMsg << errorMsg << '\n';

    delete[] bufMsg;
    bufMsg = nullptr;
}
