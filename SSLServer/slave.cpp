#include "slave.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Slave::err Slave::sendString(const std::string& strToSend) noexcept(false) {
    uint32_t strLen = static_cast<uint32_t>(strToSend.size());
    int sResult = send(_slaveSocket->getSocketfd(), reinterpret_cast<const char*>(&strLen), sizeof(uint32_t), 0);
    if (sResult <= 0) {
        std::cout << "Failed to send data!\n";
        return undefinedError;
    }
#ifdef DEBUG
    std::cout << "Bytes sent: " << sResult << '\n';
#endif
    sResult = send(_slaveSocket->getSocketfd(), strToSend.c_str(), strToSend.size(), 0);
    if (sResult <= 0) {
        std::cout << "Failed to send data!\n";
        return undefinedError;
    }
#ifdef DEBUG
    std::cout << "Bytes sent: " << sResult << '\n'
              << "Message sent: " << strToSend.c_str() << '\n';
#endif
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Slave::err Slave::recvString(std::string& strToRecv) noexcept(false) {
    uint32_t strLen = 0;
    int rResult = recv(_slaveSocket->getSocketfd(), reinterpret_cast<char*>(&strLen), sizeof(uint32_t), 0);
    if (rResult <= 0) {
        std::cout << "Failed to recieve data!\n";
        return undefinedError;
    }
#ifdef DEBUG
    std::cout << "Bytes recieved: " << rResult << '\n';
#endif

    char* buffer = new char[strLen];
    rResult = recv(_slaveSocket->getSocketfd(), buffer, strLen, 0);
    if (rResult <= 0) {
        std::cout << "Failed to recieve data!\n";
        delete[] buffer;
        buffer = nullptr;
        return undefinedError;
    }

    strToRecv.clear();
    for (uint32_t i = 0; i < strLen; ++i) {
        strToRecv += buffer[i];
    }
#ifdef DEBUG
    std::cout << "Bytes recieved: " << rResult << '\n'
              << "Message: " << strToRecv << '\n';
#endif

    delete[] buffer;
    buffer = nullptr;
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Slave::err Slave::closeConnection() noexcept(false) {
    if (_slaveSocket->getSocketfd() != -1) {
        if (shutdown(_slaveSocket->getSocketfd(), SHUT_RDWR) != 0) {
            this->printCError("Can't shutdown connection: ");
            return undefinedError;
        }
    }
    return noError;
}

void Slave::deleteSocket() noexcept(false) {
    try {
        _slaveSocket->deleteSocketInfo();
        _slaveSocket->closeDescriptor();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
    }

    delete _slaveSocket;
    _slaveSocket = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Slave::printCError(std::string preErrorMsg) noexcept(false) {
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
