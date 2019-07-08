#include "sockets.h"

using namespace Sorokin;


// MASTER SOCKET

MasterSocket::MasterSocket(int port) noexcept : _ip(INADDR_LOOPBACK),
                                                _port(port),
                                                _masterSocket(-1) {}
MasterSocket::~MasterSocket() noexcept {
    shutdown(_masterSocket, SHUT_RDWR);
    close(_masterSocket);
}

void MasterSocket::Start() noexcept(false) {
    // Initialize server
    _masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_masterSocket == -1) {
        throw MasterSocketInitError();
    }
    std::cout << "Socket was initialized!\n";

    // Set socket info
    sockaddr_in socketInfo;
    memset(&socketInfo.sin_zero, 0, sizeof(socketInfo.sin_zero));
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(_ip);
    socketInfo.sin_port = htons(_port);

    // Bind socket
    if (bind(_masterSocket, reinterpret_cast<sockaddr*>(&socketInfo), sizeof(socketInfo)) == -1) {
        throw MasterSocketBindError();
    }
    std::cout << "Socket was binded!\n";

    // Set to listen
    if (listen(_masterSocket, 1) == -1) {
        throw MasterSocketListenError();
    }
    std::cout << "Socket was set to listen!\n";
}

void MasterSocket::CloseConnection() noexcept {
    shutdown(_masterSocket, SHUT_RDWR);
    close(_masterSocket);
}

SlaveSocketInfo MasterSocket::Handle() noexcept(false) {
    std::cout << "Waiting for connection...\n";

    SlaveSocketInfo tmp;
    tmp._slaveSocketInfo = nullptr;
    tmp._slaveSocketInfoLen = nullptr;
    tmp._fd = accept(_masterSocket, reinterpret_cast<sockaddr*>(tmp._slaveSocketInfo), tmp._slaveSocketInfoLen);
    return tmp;
}

// SLAVE SOCKET

SlaveSocket::SlaveSocket(int fd,
                         const std::string* pwd,
                         sockaddr_in* socketInfo,
                         socklen_t* socketInfoLen) noexcept : _slaveSocket(fd),
                                                              _pwd(pwd),
                                                              _socketInfo(socketInfo),
                                                              _socketInfoLen(socketInfoLen) {}

SlaveSocket::~SlaveSocket() {
    shutdown(_slaveSocket, SHUT_RDWR);
    close(_slaveSocket);
}

void SlaveSocket::CloseConnection() noexcept {
    shutdown(_slaveSocket, SHUT_RDWR);
    close(_slaveSocket);
}

void SlaveSocket::RecvFile() noexcept(false) {
    if (_slaveSocket < 0) {
        throw SlaveSocketConnectionError();
    }
    std::cout << "Got a connection!\n";

    // Get size of the doc
    char* buffer = new char[sizeof(int)];
    int rResult = recv(_slaveSocket, buffer, sizeof(int), 0);
    if (rResult <0) {
        throw SlaveSocketRecievingError();
    }
    int fileSize = 0;
    memcpy(&fileSize, buffer, sizeof(int));
    delete[] buffer;
    buffer = nullptr;

    // Get the doc
    buffer = new char[fileSize];
    rResult = recv(_slaveSocket, buffer, fileSize, 0);
    if (rResult <0) {
        throw SlaveSocketRecievingError();
    }

    // Copy the doc to the string
    _xmlFileIn.clear();
    for (int i = 0; i < fileSize; i++) {
        _xmlFileIn += buffer[i];
    }
    std::cout << "Got the document!\n";
    delete[] buffer;
    buffer = nullptr;
}

void SlaveSocket::SignFile() noexcept(false) {
    std::list<std::string> strsToSign;

    Parser xmlParser(&_xmlFileIn, &strsToSign);
    try {
        xmlParser.loadDocument();
        xmlParser.parseDocument();
    }
    catch (std::exception& error) {
        xmlParser.Clear();
        throw error;
    }

    Signer signer(*_pwd);

    std::list<std::string> signedStrs;
    for (const auto iter : strsToSign) {
        signedStrs.push_back(signer.SignString(iter));
    }

    std::string publicKey = signer.GetPublicKey();

    xmlParser.rebuildDocument(signedStrs, publicKey);
}

void SlaveSocket::SendFile() noexcept(false) {
    // Send size of the doc
    int fileSize = _xmlFileIn.size();
    int sResult = send(_slaveSocket, reinterpret_cast<const char*>(&fileSize), sizeof(int), 0);
    if (sResult <= 0) {
        throw SlaveSocketSendingError();
    }

    // Send doc
    sResult = send(_slaveSocket, _xmlFileIn.c_str(), fileSize, 0);
    if (sResult <= 0) {
        throw SlaveSocketSendingError();
    }
}



