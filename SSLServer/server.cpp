#include "server.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::setSocket(const int domain,
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
        return undefinedError;
    }
    if (_masterSocket->setSocket(domain, type, protocol) ||
        _masterSocket->setSocketInfo(domain, ip, port)) {
        return undefinedError;
    }
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::setUpListener() noexcept(false) {
    if (bind(_masterSocket->getSocketfd(),
             reinterpret_cast<const sockaddr*>(_masterSocket->getSocketInfo()), // Might throw
             sizeof(*(_masterSocket->getSocketInfo()))
             )) {
        __printCError("Can't bind the socket: "); // Might throw
        return undefinedError;
    }
    std::cout << "Socket was succesfully binded!\n";
    if (listen(_masterSocket->getSocketfd(), 1)) {
        __printCError("Can't set socket to listen: "); // Might throw
        return undefinedError;
    }
    std::cout << "Socket was succesfully set to listen!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::getConnection(Socket*& slaveSocket) noexcept(false) {
    std::cout << "Waiting for connection...\n";
    socklen_t slaveSocketSize = 0;
    try {
        slaveSocket = new Socket;
    }
    catch (...) {
        std::cout << "Can't allocate memory for new socket!\n";
        return undefinedError;
    }
    slaveSocket->accessSocket() = accept(_masterSocket->getSocketfd(),
                                      reinterpret_cast<sockaddr*>(slaveSocket->accessSocketInfo()), // Might throw
                                      &slaveSocketSize);
    if (slaveSocket->getSocketfd() == -1) {
        __printCError("Failed to accept connection: "); // Might throw
        return undefinedError;
    }

    std::cout << "Connection accepted!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::closeConnection() noexcept(false) {
    if (_masterSocket->getSocketfd() != -1) {
        if (shutdown(_masterSocket->getSocketfd(), SHUT_RDWR) != 0) {
            __printCError("Can't shutdown connection: "); // Might throw error
            return undefinedError;
        }
    }
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Server::deleteSocket() noexcept(false) {
    try {
        _masterSocket->deleteSocketInfo();
        _masterSocket->closeDescriptor();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
    }

    delete _masterSocket; // Might throw
    _masterSocket = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Server::__printCError(std::string preErrorMsg) noexcept(false) {
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

//-----------------------------------------------------------------------------------------------------------------------------
